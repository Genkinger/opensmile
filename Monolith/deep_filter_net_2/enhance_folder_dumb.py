from subprocess import run
from common import get_file_paths_with_extension
from os.path import expanduser

input_directory = "./"  # Path to the audio files
output_directory = "./"  # Path to output directory, files will NOT be replaced if same as input_directory!

wav_files = get_file_paths_with_extension(input_directory, "wav")
for wav_file in wav_files:
    run([f"{expanduser('~')}\\scoop\\apps\\python310\\current\\python.exe", f"{expanduser('~')}\\scoop\\apps\\python\\current\\Lib\\site-packages\\df\\enhance.py", "-m", "DeepFilterNet2",
         "--output-dir", output_directory, wav_file])
