import re
from os.path import splitext, basename, join, isfile
from os import listdir
from fnmatch import fnmatch
from pandas import to_timedelta

identifier_regex = r"([A-Za-z])+(\d{2})(\d{2})"
decimal_separator = ','
value_separator = ";"

def get_files_with_suffix(search_path, suffix):
    return [join(search_path,file) for file in listdir(search_path) if isfile(file) and fnmatch(file,f"*.{suffix}")]

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
    regex_result = re.search(identifier_regex, identifier)

    dictionary["file"] = identifier
    items = list(dictionary.items())
    items.insert(1, ("subject", regex_result.group(2)))
    items.insert(2, ("audio_track", regex_result.group(3)))
    items.insert(3, ("duration", delta.total_seconds()))
    return dict(items)


def dictionary_to_csv_string(dictionary):
    csv_string = ""
    values = [str(v) for _, v in dictionary.items()]
    keys = [key for key in dictionary.keys()]
    csv_string += value_separator.join(keys)
    csv_string += "\n"
    csv_string += value_separator.join(values).replace(".",",")
    return csv_string


def process_csv_file(filepath):
    processed_csv = ""
    with open(filepath, "r") as input_csv:
        result = csv_string_to_dict(input_csv.read().strip())
        result = process_dictionary(result)
        processed_csv = dictionary_to_csv_string(result)
    with open(filepath, "w") as out:
        out.write(processed_csv)


# CHANGE THIS PATH TO REFLECT YOURS
for path in get_files_with_suffix("./", "csv"):
    process_csv_file(path)
