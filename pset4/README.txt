Further Instructions provided atop each file

Main files / programs: whodunit.c, resize.c, recover.c


whodunit.c - Figure out whodunit.  working with bmps, use one clue 
to figure out verdict (outfile bmp) with 24-bit (3 bytes per pixel bgr) bitmap
Usage: ./whodunit infile outfile

resize.c - resize an image – resize user-given bmp by amount provided by user (n)
Usage: ./resize factor infile.bmp outfile.bmp

recover.c - recover some photos – recovered jpg from raw file (card.raw), just 1s and 0s. 
Usage: ./recover