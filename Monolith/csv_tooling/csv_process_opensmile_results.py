from os.path import splitext, basename
from pandas import to_timedelta
from common import get_file_paths_with_extension
from os.path import join, basename

decimal_separator = ','
value_separator = ";"


def csv_string_to_dict(csv_string):
    [fields_string, data_string, *_] = csv_string.split("\n")
    [fields, data] = [fields_string.split(","), data_string.split(",")]
    return dict(zip(fields, data))

# Filenames of Audio Files referenced in the csv file being processed have to follow this pattern:
# (x+)(y)(y).wav where x and y are Digits from 0..9 and x+ means one or more Digits (parenthesis only used to show separation, they are NOT ALLOWED in the Filename)
# ########
# Example:
#     xyy.wav
#     |||
#     vvv
#     103.wav
def extract_subject_track_from_identifier_legacy(identifier):
    return identifier[:-2], identifier[-2:]


# This function extracts the FIRST TWO parts of an identifier string having the following format:
# SUBJECT_TRACKNO
# Subsequent additional information (eg. timestamps etc.) provided after subsequent underscores will be ignored!
# This means identifiers of the form SUBJECT_TRACKNO_SOMETHING_ENTIRELY_DIFFERENT will not break this as long as the order of parts is maintained
def extract_subject_track_from_identifier_future_proofed(identifier):
    parts = identifier.split("_")
    return parts[0], parts[1]

def process_dictionary(dictionary):
    filename = basename(dictionary["file"])
    identifier = splitext(filename)[0]

    # calculate track length, remove start and end timestamps
    start = dictionary.pop("start")
    end = dictionary.pop("end")
    delta = to_timedelta(end) - to_timedelta(start)

    # get subject and track number from wav filename
    #subject = identifier[:-2]
    #audio_track = identifier[-2:]

    # REPLACE THIS WITH THE "FUTURE PROOFED" VERSION IF NEEDED!
    subject, audio_track = extract_subject_track_from_identifier_legacy(identifier)

    # add subject, audio_track and duration columns to csv
    dictionary["file"] = identifier
    items = list(dictionary.items())
    items.insert(1, ("subject", subject))
    items.insert(2, ("audio_track", audio_track))
    items.insert(3, ("duration", delta.total_seconds()))
    return dict(items)


def dictionary_to_csv_string(dictionary):
    csv_string = ""
    values = [str(v) for _, v in dictionary.items()]
    keys = [key for key in dictionary.keys()]
    csv_string += value_separator.join(keys)
    csv_string += "\n"
    csv_string += value_separator.join(values).replace(".", ",")
    return csv_string


def process_csv_file(input_file_path, output_file_path):
    processed_csv = ""
    with open(input_file_path, "r") as input_csv:
        result = csv_string_to_dict(input_csv.read().strip())
        result = process_dictionary(result)
        processed_csv = dictionary_to_csv_string(result)
    with open(output_file_path, "w") as out:
        out.write(processed_csv)


input_directory = "./"  # Path to directory with csv files
output_directory = "./"  # Path to output directory, if this is the same as input_directory files will be replaced!

for input_file_path in get_file_paths_with_extension(input_directory, "csv"):
    process_csv_file(input_file_path, join(output_directory, basename(input_file_path)))
