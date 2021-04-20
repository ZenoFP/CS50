#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// Candidates have name and vote count
typedef struct
{
    string name;
    int votes;
}
candidate;

// Array of candidates
candidate candidates[MAX];

// Number of candidates
int candidate_count;

// Function prototypes
bool vote(string name);
void print_winner(void);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: plurality [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i].name = argv[i + 1];
        candidates[i].votes = 0;
    }

    int voter_count = get_int("Number of voters: ");

    // Loop over all voters
    for (int i = 0; i < voter_count; i++)
    {
        string name = get_string("Vote: ");

        // Check for invalid vote
        if (!vote(name))
        {
            printf("Invalid vote.\n");
        }
    }

    // Display winner of election
    print_winner();
}

// Update vote totals given a new vote
bool vote(string name)
{
    //for every candidates.name in the list of candidates
    for (int a = 0; a < candidate_count; a++)
    {
        //if string with name in input == name in candidates list - Update vote + Return True
        if ( strcmp(candidates[a].name, name) )
        {
            candidates[a].votes++;
            return true;
        }

    }
    //if never returned true, return false
    return false;
}

// Print the winner (or winners) of the election
void print_winner(void)
{
    //set variable for copying the max votes in it
    int winner = 0;

    //discover what's the maximum of votes achieved
    for (int b = 0; b < candidate_count; b++)
    {
        if (candidates[b].votes > winner)
        {
            winner = candidates[b].votes;

        }

    }

    //Iterate again through all the elements of the list and print the names of the people with maximum votes
    int c = 0;
    while (c < candidate_count)
    {
        if (candidates[c].votes == winner)
        {
            printf("%s", candidates[c].name);
            printf("\n");
        }

        c++;
    }

    return;
}

