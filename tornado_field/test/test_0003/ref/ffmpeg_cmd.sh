ffmpeg -r 30 -i "/home/user0/maya/projects/default/images/test.%04d.png" -c:v libx264 -qscale 1 -pix_fmt yuv420p /home/user0/dev/mymagicbox/tornado_field/test/test_0003/ref/test.mp4

