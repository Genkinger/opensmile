from sklearn import svm
import numpy as np
from feature_selection import *
from sklearn.model_selection import train_test_split


def get_training_and_test_feature_set_random(features, scores, test_size=0.33):
    training_features, test_features, training_scores, test_scores = train_test_split(features, scores,
                                                                                      test_size=test_size)
    return np.array(training_scores), np.array(training_features), np.array(test_scores), np.array(test_features)


def get_training_and_test_feature_set_interleaved(features, scores):
    training_scores = np.array(scores[0::2])
    training_features = np.array(features[0::2])
    test_scores = np.array(scores[1::2])
    test_features = np.array(features[1::2])
    return training_scores, training_features, test_scores, test_features


def svr_kernel_comparison(path, feature_indices, score_index, threshold, random_feature_selection, test_size):
    feature_names, features, scores = get_data_from_file(path, feature_indices, score_index)
    selected_feature_names, selected_features, selected_scores = apply_variance_threshold(feature_names, features,
                                                                                          scores,
                                                                                          threshold)  # TODO(L.): Find good threshold
    print(len(feature_names))
    print(len(selected_feature_names))
    # Build set of training and test data from selected features TODO(L.): maybe more training data, less test data

    # Create Training / Testing set
    training_scores, training_features, test_scores, test_features = get_training_and_test_feature_set_random(
        selected_features, selected_scores,
        test_size) if random_feature_selection else get_training_and_test_feature_set_interleaved(selected_features,
                                                                                                  selected_scores)

    # Initialize SVRs with different kernels
    poly = svm.SVR(kernel="poly")
    linear = svm.SVR(kernel="linear")
    rbf = svm.SVR(kernel="rbf")
    sigmoid = svm.SVR(kernel="sigmoid")

    regressors = [("poly", poly), ("linear", linear), ("rbf", rbf), ("sigmoid", sigmoid)]

    # Train the SVRs
    for name, regressor in regressors:
        regressor.fit(training_features, training_scores)

    # Evaluate the SVRs
    regressor_evaluation = {}
    for name, regressor in regressors:
        predictions = regressor.predict(test_features)
        mse = ((predictions - test_scores) ** 2).mean()
        regressor_evaluation[name] = {"mse": mse}
        print(f"[{name}]:")
        for prediction, score in zip(predictions, test_scores):
            print(f"\tPrediction: {prediction}, Actual Score: {score}")
        print(f"\tMSE: {mse}")

    # Sort regressors from best to worst and print them
    regressor_evaluation = {key: val for key, val in sorted(regressor_evaluation.items(), key=lambda e: e[1]['mse'])}
    return regressor_evaluation


def main():
    csv_file_path = "hier/kommt/der/pfad/hin.csv"  # Pfad zur CSV
    feature_indices = [*range(21, 108), 113]  # Liste der Indizes der zu betrachtenden features (angefangen bei 0)
    score_index = 8  # Index der Scorespalte
    threshold = 0  # Threshold erstmal einfach so lassen
    random_feature_selection = True  # Wenn True -> Zufaellige auswahl der Test-/Trainingsdaten
    test_size = 0.33  # Wenn random_feature_selection == True -> Prozentualer Anteil des Test-Sets (hier z.B.: 1/3 aller features sind im Test-Set)
    regressor_evaluation = svr_kernel_comparison(csv_file_path, feature_indices, score_index, threshold, random_feature_selection, test_size)
    print(regressor_evaluation)


main()
