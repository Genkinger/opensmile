from os.path import splitext, basename
from pandas import to_timedelta
from common import get_file_paths_with_extension

decimal_separator = ','
value_separator = ";"


def csv_string_to_dict(csv_string):
    [fields_string, data_string, *_] = csv_string.split("\n")
    [fields, data] = [fields_string.split(","), data_string.split(",")]
    return dict(zip(fields, data))


def process_dictionary(dictionary):
    filename = basename(dictionary["file"])
    identifier = splitext(filename)[0]

    start = dictionary.pop("start")
    end = dictionary.pop("end")
    delta = to_timedelta(end) - to_timedelta(start)

    subject = identifier[:-2]
    audio_track = identifier[-2:]

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


def process_csv_file(filepath):
    processed_csv = ""
    with open(filepath, "r") as input_csv:
        result = csv_string_to_dict(input_csv.read().strip())
        result = process_dictionary(result)
        processed_csv = dictionary_to_csv_string(result)
    with open(filepath, "w") as out:
        out.write(processed_csv)


input_directory = "./"  # Path to directory with csv files

for path in get_file_paths_with_extension(input_directory, "csv"):
    process_csv_file(path)
