/**
 * recover.c
 *
 * Computer Science 50
 * Problem Set 4
 *
 * Daniel Emergente Loiola
 *
 * Recovers JPEGs from a forensic image.
 */
#include <stdio.h>
#include <stdint.h>
#include <cs50.h>

//typedef to replce the struct
typedef uint8_t DATA;

//prototypes
bool newjpg (DATA a, DATA b, DATA c, DATA d);

// variables to control pictures
int nofpictures = 0;
bool firstjpg = true;

int main(int argc, char* argv[])
{

    // Check for raw file. Fails if no file is found
    FILE* cardfile = fopen("card.raw", "r");
    if (cardfile == NULL)
    {
        printf("Could not open %s.\n", "card.raw");
        return 2;
    }

    //declaring the data buffer to store the data and the img file
    DATA data[512];
    FILE* img;

    // reads a 512 buffer from the file
    while (fread(&data, 1, 512, cardfile) == 512)
    {
        // check for a new jpeg using the function
        if (newjpg(data[0], data[1], data[2], data[3]) == true)
        {
            // hops the file close part for the very first jpg
            if (firstjpg == true)
            {
                firstjpg = false;
            }
            else
            {
                fclose(img);
            }

            //open a new jpeg and names it
            char title[3];
            sprintf(title, "%03d.jpg", nofpictures);
            img = fopen(title, "a");
            nofpictures++;
        }

        // if we already found a image, then copy data to the new file
        if(firstjpg == false)
        {
            fwrite(&data, 512, 1, img);
        }

    }
}

// Function to find a new JPG Image. Will return true if bytes
// passed to function corresponds to the begining of a new JPG file
bool newjpg (DATA a, DATA b, DATA c, DATA d)
{
    if(a == 0xff && b == 0xd8 && c == 0xff)
    {
        if(d == 0xe0 || d == 0xe1 || d == 0xe2 || d == 0xe3 || d == 0xe4 || d == 0xe5 || d == 0xe6 || d == 0xe7 || d == 0xe8 || d == 0xe9 || d == 0xea || d == 0xeb || d == 0xec || d == 0xed || d == 0xee || d == 0xef)
        {
            return true;
        }
    }
    return false;
}
