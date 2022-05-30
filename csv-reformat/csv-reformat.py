from re import search
from os import listdir
from os.path import isfile, join, basename
from fnmatch import fnmatch


def get_file_paths_with_extension(path, extension):
    return [join(path, file_name) for file_name in listdir(path) if
            (isfile(join(path, file_name)) and fnmatch(file_name, f"*.{extension}"))]


def is_not_empty(string):
    return len(string) != 0


def process_line(line):
    identifier_regex = r"([A-Za-z])+(\d{2})(\d{2})"
    # get filename without extension from path
    identifier = basename(line[0]).split(".")[0]

    # extract subject / audio_track from identifier
    regex_result = search(identifier_regex, identifier)
    subject = regex_result.group(2)
    audio_track = regex_result.group(3)

    # transform decimals
    transformed_line = map(lambda x: x.replace(".", ","), line[1:])

    # return array of the transformed data
    return [identifier, subject, audio_track, *transformed_line]


def process_file_inplace(path):
    csv_output = ""
    with open(path, "rt") as csv_file:
        # get non-empty lines from file and split them into cells
        lines = list(map(lambda x: x.split(","), filter(is_not_empty, csv_file.read().split("\n"))))

        # get headings and append subject and audio_track columns
        headings = lines[0]
        headings.insert(1, "subject")
        headings.insert(2, "audio_track")

        # process the data
        intermediate = [headings, *map(process_line, lines[1:])]

        # concatenate previously split lines into one again
        transformed_lines = map(lambda x: ";".join(x), intermediate)

        # concatenate all lines with \n's into a new csv string
        csv_output = "\n".join(transformed_lines)

        # overwrite the file
    with open(path, "wt") as csv_file:
        csv_file.write(csv_output)


def main():
    directory = "./" # search path for files
    extension = "csv" # extensions to filter for
    for path in get_file_paths_with_extension(directory, extension):
        process_file_inplace(path)


main()
