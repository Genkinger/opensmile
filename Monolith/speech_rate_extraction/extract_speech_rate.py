from os.path import join
import subprocess


def get_file_length_in_seconds(directory, file_name):
    file_path = join(directory, file_name)
    # NOTE(L): Hier ggf. den kompletten pfad zu ffprobe angeben wenn ffprobe nicht im PATH ist!
    result = subprocess.Popen(['ffprobe', '-i', file_path], stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
    for line in result.stdout.readlines():
        if "Duration" in line.decode('ascii'):
            hh, mm, ss = map(float, line[12:23].decode('ascii').split(':'))
            return hh * 60 * 60 + mm * 60 + ss
    return float('inf')


def count_words_per_file(transcript_path, audio_directory):
    result = {}
    with open(transcript_path) as transcript:
        lines = filter(lambda x: len(x) > 0, transcript.read().split('\n'))
        for line in lines:
            file, *words = line.split(' ')
            words = list(filter(lambda x: len(x) > 0, words))
            word_count = len(words)
            file_length_in_seconds = get_file_length_in_seconds(audio_directory, file)
            result[file] = {
                "word_count": word_count,
                "file_length_in_seconds": file_length_in_seconds,
                "words_per_second": word_count / file_length_in_seconds,
                "words_per_minute": word_count * float(60) / file_length_in_seconds
            }
    return result


def result_to_csv(result):
    output_csv = "file,file_length_in_seconds,word_count,words_per_second,words_per_minute\n"
    for k, v in result.items():
        output_csv = output_csv + k + ',' + str(v["file_length_in_seconds"]) + ',' + str(v["word_count"]) + ',' + str(
            v["words_per_second"]) + ',' + str(v["words_per_minute"]) + '\n'
    return output_csv


# NOTE(L): WICHTIG - Damit das Skript funktioniert muss das programm ffprobe im PATH sein! ()
# NOTE(L): Die CSV muss noch processed werden !

def main():
    res = count_words_per_file("pfad/zur/transcript.txt",
                               "./pfad/zu/den/audio/dateien")  # NOTE(L): Hier muessen noch die richtigen Werte eingetragen werden!
    csv = result_to_csv(res)
    with open("transcript_output.csv", "w+") as output_file:
        output_file.write(csv)
