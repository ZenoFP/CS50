// Implements a dictionary's functionality

#include <stdbool.h>
#include <ctype.h>
#include <strings.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 26;

// Hash table
node *table[N];


//FUNCTIONS ADDED BY ME FOR HELP
//integer for counting words in dictionary
unsigned int words_in_dictionary;
unsigned int hv;



//TODO: Returns true if word is in dictionary, else false
bool check(const char *word)
{
    //set hash value to access table
    hv = hash(word);

    //create node and set it to the first element
    node *bus = table[hv];

    //if no elements at that table's hash, immediate return NULL
    while((bus != NULL))
    {
        //strcasecmp return 0 if strings are equal
        int r = strcasecmp(word, bus->word);
        //compare string with word in dictionary, case insensitive way
        if (r == 0)
        {return true;}

        //if not equal, point to the next. If next == NULL = end of list, return false;
        else
        {bus = bus->next;}

    }
    return false;
}



//TODO: Hashes word to a number
unsigned int hash(const char *word)
{
    return ((tolower(word[0]) - 'a')%26);
}



//TODO: Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    //Opening my dictionary file
    FILE *dict = fopen(dictionary, "r");
    //Memory Check
    if (dict == NULL)
    {printf("Memory Error\n"); return false;}

    //create char*/string for loading dict words into memory
    char parola[LENGTH+1];

    //set to 0 before loading
    words_in_dictionary = 0;


    //Read strings-rows from file one at a time
    while(fscanf(dict, "%s", parola) != EOF)
    {
        //create a new node for word
        node *new_node = malloc(sizeof(node));
        //Memory Check
        if (new_node == NULL)
        {return false;}

        //set hash value to access table
        hv = hash(parola);

        //copy loaded word into new node
        strcpy(new_node->word, parola);


        //if it's the first word
        if (table[hv] == NULL)
        {table[hv] = new_node;}

        else
        {
            //Insert new node into the hash table
            new_node->next = table[hv];
            table[hv] = new_node;
        }
        words_in_dictionary++;
    }

    //close dict file before end of function, ops :')
    fclose(dict);

    //return success (anyway -__|__-)
    return true;

}

//TODO: Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    //avoiding errors: if counter > 0 prints it's value
    if (words_in_dictionary>0)
    {
        return words_in_dictionary;
    }

    //if = 0, return false(0)
    return false;

}

//TODO: Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    //iterate through every element of the linked list
    for (int end = 0; end < N; end++)
    {
        //set cursor to the first element of the first index
        node *tram = table[end];

        //while there's something in the list
        while (tram != NULL)
        {
            //create secondary cursor to free correctly memory
            node *train = tram;

            //train in now the current node, set tram to next(even if null)
            tram=tram->next;

            //free current node
            free(train);
        }

        //if it's the last loop, evething must be freed, return success
        if (end == N -1)
        {return true;}

    }

    //didn't returned success? it's fail
    return false;
}
