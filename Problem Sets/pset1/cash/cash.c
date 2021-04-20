#include <stdio.h>
#include <cs50.h>

int main (void)
{ 
    int monete = 0;
    float resto = get_float ("Insert the cash return\n");
    while (resto <= 0);
    
    while (resto > 0)
    { 
        if (resto - 0.25 >= 0 )
        { 
        monete++;
        resto = resto - 0.25;
        }
        else if (resto - 0.10 >= 0)
        {
        monete++;
        resto = resto - 0.10;
        }
        else if (resto - 0.05 >= 0)
        {
        monete ++;
        resto = resto - 0.05;
        }
        else  
        {
        monete ++;
        resto = resto - 0.01;
        }
    }
    printf("Monete usate per il resto %i\n", monete);
}