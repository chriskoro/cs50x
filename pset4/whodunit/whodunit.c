/**
 * This program reveals a hidden message in a BMP, with the clue 
 * and verdict files provided at command line by the user (must be 2
 * 4-bit uncompressed Bitmap 4.0). The crux of the problem is taking 
 * the clue file and modifying the colors as needed so we can visually see "whodunit".
 * 
 * Usage: whodunit infile outfile
 */
       
#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 3)
    {
        fprintf(stderr, "Usage: ./whodunit infile outfile\n");
        return 1;
    }

    // remember filenames
    char *infile = argv[1];
    char *outfile = argv[2];

    //Step 1: Open files
    // open input (clue) file 
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }

    // open output file
    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
    }

    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 || 
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }

    //Step 2: update header's info for outfile
    // write outfile's BITMAPFILEHEADER (from clue to verdict)
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER (from clue to verdict)
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);

    // determine padding for scanlines
    int padding =  (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    // iterate over infile's (clue's) scanlines
    for (int i = 0, biHeight = abs(bi.biHeight); i < biHeight; i++)
    {
        // iterate over pixels in scanline
        for (int j = 0; j < bi.biWidth; j++)
        {
            // temporary storage, note each pixel is an rgb triple
            RGBTRIPLE triple;

            // read RGB triple from infile (Step 3: read clue's scanline px by px)
            fread(&triple, sizeof(RGBTRIPLE), 1, inptr);
            
            //Step 4: Crux- change pixel's color as necessary to reveal hidden clue here if{}
            
            if (triple.rgbtRed == 0xff)
            {
                triple.rgbtRed = 0x00;
                triple.rgbtBlue = 0x00;
                triple.rgbtGreen = 0x00;
            }
      

            // write RGB triple to outfile (Step 5: Write pixels to verdict's scanline, px by px)
            fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
        }

        // skip over padding, if any (moves file position indicator / cursor) in the clue file
        fseek(inptr, padding, SEEK_CUR);

        // then add it back (to demonstrate how) in the verdict file
        for (int k = 0; k < padding; k++)
        {
            //writes black (padding) to end of scan line as many times as needed
            fputc(0x00, outptr);
        }
    }

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // success
    return 0;
}
