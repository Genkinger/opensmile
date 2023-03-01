from subprocess import run
from os import chdir
from os.path import expanduser

# Path to wav file to be inferred
# INFO: This needs to either be relative to the path "./models/research/audioset/yamnet" or absolute!
wav_file_path = ""

chdir("./models/research/audioset/yamnet")
output = run([f"{expanduser('~')}\\scoop\\apps\\python310\\current\\python.exe", "inference.py", wav_file_path], capture_output=True)
print(output.stdout.decode("utf-8"))
