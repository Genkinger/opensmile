from os import listdir
from os.path import isfile, join
from fnmatch import fnmatch


def get_file_paths_with_extension(path, extension):
    return [join(path, file_name) for file_name in listdir(path) if
            (isfile(join(path, file_name)) and fnmatch(file_name, f"*.{extension}"))]
