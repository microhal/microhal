ffmpeg -framerate 4 -i img%00d.bmp -c:v libx264 -profile:v high -crf 20 -pix_fmt yuv420p video.mp4
