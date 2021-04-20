#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

// PHASE 1: GET KEY
int main (int argc, string argv[])
{
//Control correct use
if (argc != 2)
{
    printf("Correct Usage: ./caesar key\n");
    return 1;
}

for (int x = 0; x < strlen(argv[1]); x++ )
{
    if ( !(isdigit(argv[1][x]) ) )
    {
        printf("Correct Usage: ./caesar key\n");
        return 1;
    }

}
//get key into integer named KEY
int key = atoi(argv[1]);


//PHASE 2: GET PLAINTEXT
string plain = get_string("Plaintext: ");


printf("ciphertext: ");
//PHASE 3: ENCYPHER PLAINTEXT AND PRINT IT ONE CHAR AT A TIME
for (int a = 0; a < strlen(plain); a++)
{
    if ( isalpha(plain[a]) && isupper(plain[a]) )
    {
        printf("%c", ( ( ( ( (plain[a]) - 'A')+key) %26) +'A') );
        continue;
    }

    if  (isalpha(plain[a]) && islower(plain[a]) )
    {
        printf("%c", ( ( ( ( (plain[a]) - 'a') +key) %26) +'a') );
        continue;
    }

    else
    {
        printf("%c", plain[a]);
    }

}
//New line for the eyes sake
printf("\n");

}









