from subprocess import run
from os import chdir
from os.path import expanduser, join, basename
from common import get_file_paths_with_extension
import csv

input_directory = ""
output_directory = ""
generate_summary_file = True
generate_file_per_wav = True
wav_file_paths = get_file_paths_with_extension(input_directory, "wav")


chdir("./models/research/audioset/yamnet")
outputs = []
for wav_file_path in wav_file_paths:
    command = run([f"{expanduser('~')}\\scoop\\apps\\python310\\current\\python.exe", "inference.py", wav_file_path], capture_output=True)
    output_lines = command.stdout.decode("ascii").strip().split("\r\n")
    filename = output_lines[0][:-1].strip()
    output = [filename]
    for line in output_lines[1:]:
        parts = [x.strip() for x in line.split(":")]
        output.extend(parts)
    outputs.append(output[:-1])

header = ["file", *(["predicted class", "score"] * int((len(outputs[0])-1)/2))]
if generate_summary_file:
    with open(join(output_directory, "summary_classification.csv"), "w+", newline='') as summary_file:
        writer = csv.writer(summary_file, dialect="excel")
        writer.writerow(header)
        writer.writerows(outputs)

if generate_file_per_wav:
    for row in outputs:
        with open(join(output_directory, f"{basename(row[0]).split('.')[0]}_classification.csv"), "w+", newline='') as file:
            writer = csv.writer(file, dialect="excel")
            writer.writerow(header)
            writer.writerow(row)
