/**
 * Resizes a 24-bit BMP image.  User gives "small" file 
 * (small.bmp) and program gives the enlarged file (large.bmp) 
 * to the degree given by the user, n.
 * 
 * notes: inptr is the small, outptr is the large / resized
 */
       
#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        fprintf(stderr, "Usage: ./resize factor infile.bmp outfile.bmp\n");
        return 1;
    }

    // resize factor
    int n = atoi(argv[1]);
    
    if (n < 0 || n > 100)
    {
        printf("Number must be between 0 and 100 for resize factor");
        return 1;
    }

    // remember filenames
    char *infile = argv[2];
    char *outfile = argv[3];
    

    // OPEN INFILE 
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
    

    //UPDATE OUTFILE HEADER INFO
    //old (before scaled by n)
    int oldWidth = bi.biWidth;
    int oldPadding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    int oldHeight = bi.biHeight;
    //int oldSizeImage = (bi.biWidth * sizeof(RGBTRIPLE) + oldPadding) * abs(bi.biHeight);
    //int oldFileSize = bi.biSizeImage + sizeof(BITMAPINFOHEADER) + sizeof(BITMAPFILEHEADER);

    //new (scaled by n)
    bi.biWidth = bi.biWidth * n;
    int padding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    bi.biHeight = bi.biHeight * n;
    bi.biSizeImage = ((sizeof(RGBTRIPLE) * bi.biWidth) + padding) * abs(bi.biHeight);
    bf.bfSize = bi.biSizeImage + sizeof(BITMAPINFOHEADER) + sizeof(BITMAPFILEHEADER);

    // write to outfile's BITMAPFILEHEADER and BITMAPINFOHEADER
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);
    
    int oldWidth2 = abs(oldWidth);
    
    //READ INFILE'S SCANLINE PIXEL BY PIXEL (done with header, now lets do image)
    for (int i = 0, oldHeight2 = abs(oldHeight); i < oldHeight2; i++)
    {
        //this loop RESIZEs VERTICALLY
        for (int j = 0; j < n; j++)
        {
            //place file position indicator in right place
            fseek(inptr, (sizeof(BITMAPINFOHEADER) + sizeof(BITMAPFILEHEADER) + ((oldWidth * sizeof(RGBTRIPLE) + oldPadding)* i)), SEEK_SET);
            
            // iterate over pixels in scanline (for each px in row)
            for (int k = 0; k < oldWidth2; k++)
            {
                // temporary storage
                RGBTRIPLE triple;
    
                //read into infile
                fread(&triple, sizeof(RGBTRIPLE), 1, inptr);
                
                //write to outfile n times (RESIZE HORIZONTALLY)
                for (int l = 0; l < n; l++)
                {
                    fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
                }
            }
            
            //write outfile's PADDING
            for (int p = 0; p < padding; p++)
            {
                fputc(0x00, outptr);
            }
        }
    }
        

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // success
    return 0;
}
