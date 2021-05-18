all:
   gcc -o game pow.c -O3 -Wall -Wextra -lm -lallegro -lallegro_font -lallegro_color -lallegro_image -lallegro_primitives -lallegro_audio -lallegro_acodec
