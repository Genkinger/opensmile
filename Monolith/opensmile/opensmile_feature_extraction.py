from opensmile import Smile, FeatureLevel, FeatureSet
from common import get_file_paths_with_extension
from os.path import basename, join, exists
from os import mkdir

input_directory = "./"      # Path to folder with wav files
output_directory = "./out"  # Path to output folder

feature_set = FeatureSet.ComParE_2016
feature_level = FeatureLevel.Functionals

wav_files = get_file_paths_with_extension(input_directory, "wav")
extractor = Smile(feature_set, feature_level)

if not exists(output_directory):
    mkdir(output_directory)

for wav_file in wav_files:
    result = extractor.process_file(wav_file)
    result.to_csv(join(output_directory, f"{basename(wav_file).split('.')[0]}.csv"))
