/*
 *     restoration.c
 *     Tassilo, Arshia
 *     HW01
 *     This accepts a corrupted pgm as input and outputs it as an uncorrupted
 * raw pgm
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "seq.h"
#include "mem.h"
#include "atom.h"
#include "assert.h"
#include "stdbool.h"
#include "readaline.h"
#include "pnmrdr.h"

struct line
{
    char *digits;
    const char *non_digits;
};

int isDigit(char x)
{
    if (x >= 48 && x <= 57)
    {
        return 1;
    }
    return 0;
}

void sortCharacters(struct line *new_line, char *curr_line, size_t length)
{
    new_line->digits = ALLOC(length);
    char *non_digits = ALLOC(length);

    int digit_count = 0;
    int non_digit_count = 0;

    for (size_t i = 0; i < length;)
    {

        if (isDigit(curr_line[i]))
        {
            char a;
            char b;
            char c;

            if (i + 1 < length && isDigit(curr_line[i + 1]))
            {

                if (i + 2 < length && isDigit(curr_line[i + 2]))
                {
                    a = curr_line[i];
                    b = curr_line[i + 1];
                    c = curr_line[i + 2];
                    i += 3;
                }
                else
                {
                    a = 0;
                    b = curr_line[i];
                    c = curr_line[i + 1];
                    i += 2;
                }
            }
            else
            {
                a = 0;
                b = 0;
                c = curr_line[i];
                i += 1;
            }

            char res = (100 * a) + (10 * b) + c;

            new_line->digits[digit_count++] = res;
        }
        else
        {
            non_digits[non_digit_count++] = curr_line[i];
            i++;
        }
    }
    new_line->digits[digit_count++] = '\0';
    non_digits[non_digit_count++] = '\0';
    new_line->non_digits = Atom_string(non_digits);

    FREE(non_digits);
}

void readlines(Seq_T lines, FILE *inputfd)
{
    char *curr_line = NULL;

    size_t length;

    while ((length = readaline(inputfd, &curr_line)) > 0)
    {

        struct line *new_line = ALLOC(sizeof(struct line));

        assert(new_line != NULL);

        sortCharacters(new_line, curr_line, length);

        Seq_addhi(lines, new_line);

        FREE(curr_line);
    }
}

size_t findDuplicates(Seq_T lines)
{
    bool found = false;
    int width_found = 0;
    size_t width;

    for (int i = 0; i < Seq_length(lines); i++)
    {
        for (int k = 0; k < Seq_length(lines); k++)
        {
            if (found)
            {
                break;
            }
            if ((((struct line *)Seq_get(lines, i))->non_digits == ((struct line *)Seq_get(lines, k))->non_digits) && k != i)
            {
                found = 1;

                const char *corrputed = ((struct line *)Seq_get(lines, i))->non_digits;

                for (int x = 0; x < Seq_length(lines); x++)
                {

                    if (((struct line *)Seq_get(lines, x))->non_digits != corrputed)
                    {

                        ((struct line *)Seq_get(lines, x))->digits = NULL;
                    }
                    else if (!width_found)
                    {
                        width = strlen(((struct line *)Seq_get(lines, x))->digits);
                        width_found = 1;
                    }
                }
            }
        }
        if (found)
        {
            break;
        }
    }
    return width;
}

void outputBinary(Seq_T lines, size_t width)
{
    printf("P5 \n");
    printf("%lu \n", width);
    printf("%i \n", Seq_length(lines));
    printf("%i \n", 255);

    for (int i = 0; i < Seq_length(lines); i++)
    {
        struct line *current_line = (struct line *)Seq_get(lines, i);
        if (current_line->digits)
        {
            for (int k = 0; current_line->digits[k] != '\0'; k++)
            {

                fwrite(&(current_line->digits[k]), sizeof(char), 1, stdout);
            }
            printf("\n");
        }
    }
}

void freeMemory(Seq_T lines)
{
    for (int i = 0; i < Seq_length(lines); i++)
    {
        struct line *current_line = (struct line *)Seq_get(lines, i);

        FREE(current_line->digits);
        assert(current_line->digits == NULL);

        FREE(current_line);
        assert(current_line == NULL);
    }
    FREE(lines);
    assert(lines == NULL);
}

int main(int argc, char *argv[])
{
    FILE *inputfd;

    assert(argc <= 2);

    if (argc == 2)
    {
        inputfd = fopen(argv[1], "rb");
        assert(inputfd != NULL);
    }
    else if (argc == 1)
    {
        inputfd = stdin;
        assert(inputfd != NULL);
    }

    Seq_T lines = Seq_new(500);

    readlines(lines, inputfd);

    size_t width = findDuplicates(lines);

    outputBinary(lines, width);

    fclose(inputfd);

    return EXIT_SUCCESS;
}
