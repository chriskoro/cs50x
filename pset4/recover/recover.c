/* Recovers a jpeg image from the forensic image (card.raw, 1s and 0s)
usage: recover
*/

#include <stdio.h>
#include <stdint.h>

//for use in buffer
typedef uint8_t BYTE;

int main(void)
{
    //ensure proper usage else quit
    FILE* rawptr = fopen("card.raw", "r");

    //OPEN FILE. rawptr holds address to raw file, make sure it's not null
    if (rawptr == NULL)
    {
        fprintf(stderr, "Could not open cardfile");
        return 1;
    }
    
    //FIND BEGINNING OF JPEG
    int jpgCount = 0;
    BYTE buffer[512];
    FILE* recoveredJpeg;
    char filename[7];
    
    
    //DETECT END OF FILE. While not end of file (meaning block size is less than 512, not == 512), do this. 
    //note fread returns number of items successfully read, which here should be 512 (512 bytes)
    while (fread(buffer, 1, 512, rawptr) == 512)
    {
        //while statement condition read the 512 blocks (1 byte each) into the buffer at a time & checked that it is 512 elements.
        
        //FIND BEGINNING OF JPEG See if first 4 bytes indicate JPEG header
        if (buffer[0] == 0xff &&
        buffer[1] == 0xd8 &&
        buffer[2] == 0xff &&
        (buffer[3] & 0xf0) == 0xe0)
        {
            //if we found jpeg before, close it! else, it's the first time.
            if (jpgCount > 0)
            {
                fclose(recoveredJpeg); 
            }
            
            //name jpg file
            sprintf(filename, "%03i.jpg", jpgCount);
                
            //found jpg since we're in this statment, so increment jpgCount
            jpgCount++;
            
            //OPEN NEW JPEG FILE
            recoveredJpeg = fopen(filename, "w");
            
            //write to new file
            fwrite(buffer, 512, 1, recoveredJpeg);
        }
        else
        {
            //WRITE 512 BYTES UNTIL NEW JPEG IS FOUND
            if (jpgCount > 0)
            {
                //if we're in the jpegs, then write this iteration's buffer to the new file
                fwrite(buffer, 512, 1, recoveredJpeg);
            }
            //else, discard 512 bytes and go back to start of loop
        }
    }
    
    //close any remaining files
    if (jpgCount > 0)
    {
        fclose(recoveredJpeg);
    }
    
    //close raw file
    fclose(rawptr);
    
    //success code
    return 0;
}