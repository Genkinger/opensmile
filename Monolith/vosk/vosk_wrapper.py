from subprocess import call
from common import get_file_paths_with_extension
from os.path import join, basename
from os import remove
import csv

language = "de"
input_directory = ""
output_directory = ""
generate_summary_file = True
generate_file_per_wav = True
wav_file_paths = get_file_paths_with_extension(input_directory, "wav")

for wav_file_path in wav_file_paths:
    call(["vosk-transcriber.exe", "-l", language, "-i", wav_file_path, "-o", join(output_directory, f"{basename(wav_file_path).split('.')[0]}_vosk_transcript.txt")])

transcript_file_paths = [x for x in get_file_paths_with_extension(output_directory, "txt") if "vosk_transcript" in x]
transcripts = []
for transcript_file_path in transcript_file_paths:
    with open(transcript_file_path, encoding="utf-8") as transcript_file:
        transcripts.append([basename(transcript_file_path).replace("_vosk_transcript", "").replace(".txt", ".wav"), transcript_file.read().replace("\r\n", " ")])
    remove(transcript_file_path)

if generate_summary_file:
    with open(join(output_directory, "summary_transcript.csv"), "w+", newline='', encoding="utf-8") as summary_file:
        writer = csv.writer(summary_file, dialect="excel")
        writer.writerow(["file", "transcript"])
        writer.writerows(transcripts)

if generate_file_per_wav:
    for transcript in transcripts:
        with open(join(output_directory, f"{transcript[0].split('.')[0]}_transcript.csv"), "w+", newline='', encoding="utf-8") as file:
            writer = csv.writer(file, dialect="excel")
            writer.writerow(["file", "transcript"])
            writer.writerow(transcript)