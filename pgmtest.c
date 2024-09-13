#include <stdio.h>
#include <stdlib.h>
#include "pnmrdr.h" // Include the Pnmrdr header

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <input.pgm>\n", argv[0]);
        return 1;
    }

    // Open the PGM file in binary mode
    FILE *input_file = fopen(argv[1], "rb");
    if (input_file == NULL)
    {
        perror("Error opening file");
        return 1;
    }

    // Initialize the Pnmrdr reader
    Pnmrdr_T reader;
    TRY
    {
        reader = Pnmrdr_new(input_file);
    }
    EXCEPT(Pnmrdr_Badformat)
    {
        fprintf(stderr, "Error: Input is not a valid PGM file.\n");
        fclose(input_file);
        return 1;
    }
    END_TRY;

    printf("Success 1 \n");

    // Get image metadata
    Pnmrdr_mapdata data = Pnmrdr_data(reader);
    printf("Image width: %u, height: %u, max gray value: %u\n",
           data.width, data.height, data.denominator);

    // Check if the image is a graymap
    if (data.type != Pnmrdr_gray)
    {
        fprintf(stderr, "Error: Input file is not a PGM (graymap) file.\n");
        Pnmrdr_free(&reader);
        fclose(input_file);
        return 1;
    }
    else
    {
        printf("Success 2 \n");
    }

    // Clean up
    Pnmrdr_free(&reader);
    fclose(input_file);

    return 0;
}
