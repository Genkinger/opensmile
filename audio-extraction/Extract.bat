@echo off
if not exist Output\ mkdir Output\

for %%f in (*.mp4) do (
    ffmpeg -y -i "%%f" -q:a 0 -map a "./Output/%%~nf.wav"
)

@echo on
