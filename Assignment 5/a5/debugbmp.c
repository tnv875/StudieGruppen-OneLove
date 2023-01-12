// Adapted BMP writer from: https://en.wikipedia.org/wiki/User:Evercat/Buddhabrot.c
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "debugbmp.h"
#include "colormap.h"

// Helper function to keep values in range
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define CLAMP(x, min, max) (MIN(MAX(x, min), max))

// file: the open file pointer
// width: the image width
// height: the image height
size_t debugbmp_writeheader(FILE* file, size_t width, size_t height) {

    unsigned int headers[13];

    size_t extrabytes = 4 - ((width * 3) % 4);
    if (extrabytes == 4)
        extrabytes = 0;

    size_t paddedsize = ((width * 3) + extrabytes) * height;

    headers[0]  = paddedsize + 54;      // bfSize (whole file size)
    headers[1]  = 0;                    // bfReserved (both)
    headers[2]  = 54;                   // bfOffbits
    headers[3]  = 40;                   // biSize
    headers[4]  = width;                // biWidth
    headers[5]  = height;               // biHeight
    headers[7]  = 0;                    // biCompression
    headers[8]  = paddedsize;           // biSizeImage
    headers[9]  = 0;                    // biXPelsPerMeter
    headers[10] = 0;                    // biYPelsPerMeter
    headers[11] = 0;                    // biClrUsed
    headers[12] = 0;                    // biClrImportant

    fprintf(file, "BM");

    for (size_t n = 0; n <= 5; n++)
    {
        fprintf(file, "%c", headers[n] & 0x000000FF);
        fprintf(file, "%c", (headers[n] & 0x0000FF00) >> 8);
        fprintf(file, "%c", (headers[n] & 0x00FF0000) >> 16);
        fprintf(file, "%c", (headers[n] & (unsigned int) 0xFF000000) >> 24);
    }

    fprintf(file, "%c", 1);
    fprintf(file, "%c", 0);
    fprintf(file, "%c", 24);
    fprintf(file, "%c", 0);

    for (size_t n = 7; n <= 12; n++)
    {
        fprintf(file, "%c", headers[n] & 0x000000FF);
        fprintf(file, "%c", (headers[n] & 0x0000FF00) >> 8);
        fprintf(file, "%c", (headers[n] & 0x00FF0000) >> 16);
        fprintf(file, "%c", (headers[n] & (unsigned int) 0xFF000000) >> 24);
    }

    return extrabytes;
}

// filename: the target filename on disk
// width: the width of the array
// height: the height of the array
// data: the data array
void debugbmp_writebmp (const char * filename, int width, int height, const float data[]) {

    if (data == NULL)
    {
        printf("Null data pointer?\n");
        return;
    }

    FILE* outfile = fopen(filename, "wb");
    if (outfile == NULL)
    {
        printf("Failed to open file for writing\n");
        return;
    }

    // Write BMP header
    size_t extrabytes = debugbmp_writeheader(outfile, width, height);


    // Figure out the value range
    float min = data[0];
    float max = data[0];
    for (int y = height - 1; y >= 0; y--) {
        for (int x = 0; x <= width - 1; x++)
        {
            min = MIN(min, data[y*width + x]);
            max = MAX(max, data[y*width + x]);
        }
    }

    float range = max - min;

    // BMP image format is written from bottom to top...
    for (int y = height - 1; y >= 0; y--)
    {
        for (int x = 0; x <= width - 1; x++)
        {
            float ratio = (data[y*width + x] - min) / range;
            
            // Using red-to-blue mapping
            // uint32_t rgb = (0xff*3) * ratio;
            // uint8_t r = 0, g = 0, b = 0;
            // if (rgb > 2*0xff)
            //     r = rgb - 2*0xff;
            // else if (rgb > 0xff)
            //     g = rgb - 0xff;
            // else
            //     b = rgb;

            uint8_t* color = colormap[(size_t)CLAMP(0xff * ratio, 0, 255)];

            // Also, it's written in (b,g,r) format...
            fprintf(outfile, "%c", color[2]);
            fprintf(outfile, "%c", color[1]);
            fprintf(outfile, "%c", color[0]);
        }

        // BMP lines must be of lengths divisible by 4.
        if (extrabytes)      
        {
            for (size_t n = 1; n <= extrabytes; n++)
            {
                fprintf(outfile, "%c", 0);
            }
        }
    }

    fclose(outfile);
    return;
}
