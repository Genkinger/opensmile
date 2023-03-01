from common import get_file_paths_with_extension
from os.path import join, basename, exists
from os import mkdir

de2std = {
    "delimiter":
        {
            "from": ";",
            "to": ","
        },
    "decimal":
        {
            "from": ",",
            "to": "."
        }
}
std2de = {
    "delimiter":
        {
            "from": ",",
            "to": ";"
        },
    "decimal":
        {
            "from": ".",
            "to": ","
        }
}


def csv_string_change_format(input_csv_string, format_configuration, ignore_first_row=True):
    output_csv_string = ""
    lines = input_csv_string.split("\n")
    for index, line in enumerate(lines):
        cells = line.split(format_configuration["delimiter"]["from"])
        if index != 0 or not ignore_first_row:
            cells = list(map(lambda cell: cell.replace(format_configuration["decimal"]["from"],
                                                       format_configuration["decimal"]["to"]), cells))
        output_csv_string += format_configuration["delimiter"]["to"].join(cells) + "\n"
    return output_csv_string.strip()


def csv_file_change_format(input_csv_file_path, output_csv_file_path, format_configuration, ignore_first_row=True):
    input_csv = open(input_csv_file_path)
    input_csv_string = input_csv.read().strip()
    input_csv.close()
    output_csv = open(output_csv_file_path, "w+")
    output_csv_string = csv_string_change_format(input_csv_string, format_configuration, ignore_first_row)
    output_csv.write(output_csv_string)
    output_csv.close()


def csv_file_change_format_bulk(input_directory, output_directory, format_configuration, ignore_first_row=True):
    input_csv_file_paths = get_file_paths_with_extension(input_directory, "csv")
    if not exists(output_directory):
        mkdir(output_directory)
    for input_csv_file_path in input_csv_file_paths:
        output_csv_file_path = join(output_directory, basename(input_csv_file_path))
        csv_file_change_format(input_csv_file_path, output_csv_file_path,
                               format_configuration, ignore_first_row)


input_directory = "./"  # Path to directory of csv files
output_directory = "./"  # Path to output directory. WARNING: IF THIS IS THE SAME AS input_directory, IT WILL REPLACE THE INPUT FILES!

# The third parameter describes the conversion that should be applied:
#   NAME | SEPARATOR | DECIMAL |
# =======|===========|=========|
# std2de |   , -> ;  | . -> ,  |
# de2std |   ; -> ,  | , -> .  |

csv_file_change_format_bulk(input_directory, output_directory, std2de)
