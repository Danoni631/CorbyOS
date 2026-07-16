ffmpeg -i boot.wav -ar 8000 -ac 1 -c:a pcm_u8 boot.wav
ffmpeg -i crash.wav -ar 8000 -ac 1 -c:a pcm_u8 crash.wav