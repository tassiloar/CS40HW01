#include <stdio.h>
#include <stdlib.h>
#include "readaline.h"
#include "assert.h"

#include "mem.h"

size_t readaline(FILE *inputfd, char **datapp)
{

    assert(inputfd != NULL);
    assert(datapp != NULL);

    char byte;

    size_t char_count = 0;

    size_t buffer_size = 128;

    *datapp = ALLOC(buffer_size);

    assert(datapp != NULL);

    while ((byte = fgetc(inputfd)) != EOF && byte != '\n')
    {
        (*datapp)[char_count++] = byte;

        if (char_count >= buffer_size)
        {
            buffer_size *= 2;

            RESIZE(*datapp, buffer_size);

            assert(datapp != NULL);
        }
    }

    if (byte == = EOF)
    {
        inputfd = NULL;
        return 0;
    }

    return char_count - 1;
}
