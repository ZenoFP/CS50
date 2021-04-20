#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

int main(void)
{
    //Input string
    string text = get_string("text: ");

    //counters
    float words = 0;
    float sentences = 0;
    float letters = 0;

    int len = strlen(text);

    //Loops through all the letters in imputted string to calculate n° of Sentences/Words/Letters
    for (int a = 0; a < len; a++)
    {
        if (isalpha(text[a]) )
        {
            letters++;
        }

        if (text[a] == '.' || text[a] == '?' || text[a] == '!')
        {
            sentences++;
            words++;
        }

        if ( (isspace(text[a]) || text[a] == ',' || text[a] == ':' || text[a] == ';') && isalpha(text[a-1]) )
        {
            words++;
        }

    }


    //Calculate L/S/ColemanLiau's formula
    float L = (letters / words) * 100;
    float S = (sentences / words) * 100;
    int colemanliau = round( (0.0588 * L) - (0.296 * S) - 15.8 );

    if (colemanliau > 16)
    {
        printf ("Grade 16+\n");
        return false;
    }

    if (colemanliau < 1)
    {
        printf("Before Grade 1\n");
        return false;
    }


    printf ("Grade %i\n", colemanliau);












}

