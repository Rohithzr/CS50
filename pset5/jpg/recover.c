/**
 * recover.c
 *
 * Ankit Gupta
 * ankitgupta@college.harvard.edu
 *
 * Recovers JPEGs from a forensic image.
 */

#include <stdio.h>
#include <stdlib.h>

// declare prototype for the tool that will make an image.
void makeimage(int i, FILE* file, char* buffer, char* title);

int main(int argc, char* argv[])
{
    // make sure the user did not enter any command line arguments.
    if (argc != 1)
    {
        return 0;
    }
    
    // open the file from which we are going to recover jpgs. 
    // check for null pointer if file did not exist.
    FILE* file = fopen("card.raw", "r");
    if (file == NULL)
    {
        printf("Could not open card.raw");
        return 1;
    }
    
    // declare a buffer of size 512 and allocate space for the title.
    char buffer[512];
    char* title = malloc(sizeof(char) * 7);
    
    // declare and initialize integers that will be used later.
    int elements = 0;
    int i = -1;
    
    do
    {
        // start reading until program reaches the start of a jpg.
        // the numbers correspond to the starting values of a jpg.
        elements = fread(&buffer, sizeof(char), 512, file);
        if (buffer[0] == (char) 255 && buffer[1] == (char) 216 && buffer[2] 
                == (char) 255 && (buffer[3] == (char) 224 || buffer[3] 
                == (char) 225))
        {
            // i will be used for the fielename, and fseek moves back to 
            // the start of the jpg so that the function makeimage can 
            // process the whole jpg.
            i++;
            fseek(file, -512, SEEK_CUR);
            makeimage(i, file, buffer, title);	
        }
    }
    while (elements == 512);
    
    // close the file and free memory.
    fclose(file);
    free(title);
}   

// This function makes the new image when main discovers the start of a jpg.
void makeimage(int i, FILE* file, char* buffer, char* title)
{
    // initialize integers; counter makes sure that the loop doesn't break
    // too early.
    
    int a = 0;
    int counter = 0;
    
    // title the new image and open it, then check for null pointers.
    sprintf(title, "%03d.jpg", i);
    FILE* newimg = fopen(title, "a");
    if (newimg == NULL)
    {
        printf("Could not open newimg");
        return;
    }
    
    // if the start of a new image is not found, then continue to copy 
    // 512 byte at a time.
    do
    {  
        counter++;
        a = fread(buffer, sizeof(char), 512, file);
        if ((counter == 1) || ( (a == 512) && (buffer[0] != (char) 255 || 
                buffer[1] != (char) 216 || buffer[2] != (char) 255 || 
                (buffer[3] != (char) 224 && buffer[3] != (char) 225))))
        {
            fwrite(buffer, sizeof(char), 512, newimg); 
        }
        
        // if the start of a new image is found, break out of the loop.
        else
        {
            break;
        }    
    }
    while (1 == 1);
    
    // move back 512 bytes so that main goes to the start of the image
    fseek(file, -512, SEEK_CUR);
    
    // close the image file.
    fclose(newimg);
}
    
