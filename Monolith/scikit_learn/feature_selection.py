import csv
from sklearn.feature_selection import VarianceThreshold


def convert_to_float_comma(string_representation):
    return float(string_representation.replace(",", "."))


def get_data_from_file(path, feature_indices, score_index):
    file = open(path)
    csv_content = csv.reader(file, delimiter=";")
    features = []
    scores = []
    for row in csv_content:
        scores.append(row[score_index])
        queried_features = []
        for index in feature_indices:
            queried_features.append(row[index])
        features.append(queried_features[:])
    file.close()
    feature_names = features[0]
    scores = list(map(convert_to_float_comma, scores[1:]))
    features = list(map(lambda e: list(map(convert_to_float_comma, e)), features[1:]))
    return feature_names, features, scores


def apply_variance_threshold(feature_names, features, scores, threshold):
    selector = VarianceThreshold(threshold=threshold)
    selector.fit(features)
    selected_features = selector.transform(features)
    selected_indices = list(map(lambda e: int(e[1:]), selector.get_feature_names_out()))
    selected_feature_names = [name for i, name in enumerate(feature_names) if i in selected_indices]
    # print(f"[INFO]: Selected {len(selected_indices)} of {len(feature_names)} Features based on Variance...")
    return selected_feature_names, selected_features, scores
