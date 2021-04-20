#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max voters and candidates
#define MAX_VOTERS 100
#define MAX_CANDIDATES 9

// preferences[i][j] is jth preference for voter i
int preferences[MAX_VOTERS][MAX_CANDIDATES];

// Candidates have name, vote count, eliminated status
typedef struct
{
    string name;
    int votes;
    bool eliminated;
}
candidate;

// Array of candidates
candidate candidates[MAX_CANDIDATES];

// Numbers of voters and candidates
int voter_count;
int candidate_count;

// Function prototypes
bool vote(int voter, int rank, string name);
void tabulate(void);
bool print_winner(void);
int find_min(void);
bool is_tie(int min);
void eliminate(int min);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: runoff [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX_CANDIDATES)
    {
        printf("Maximum number of candidates is %i\n", MAX_CANDIDATES);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i].name = argv[i + 1];
        candidates[i].votes = 0;
        candidates[i].eliminated = false;
    }

    voter_count = get_int("Number of voters: ");
    if (voter_count > MAX_VOTERS)
    {
        printf("Maximum number of voters is %i\n", MAX_VOTERS);
        return 3;
    }

    // Keep querying for votes
    for (int i = 0; i < voter_count; i++)
    {

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            // Record vote, unless it's invalid
            if (!vote(i, j, name))
            {
                printf("Invalid vote.\n");
                return 4;
            }
        }

        printf("\n");
    }

    // Keep holding runoffs until winner exists
    while (true)
    {
        // Calculate votes given remaining candidates
        tabulate();

        // Check if election has been won
        bool won = print_winner();
        if (won)
        {
            break;
        }

        // Eliminate last-place candidates
        int min = find_min();
        bool tie = is_tie(min);

        // If tie, everyone wins
        if (tie)
        {
            for (int i = 0; i < candidate_count; i++)
            {
                if (!candidates[i].eliminated)
                {
                    printf("%s\n", candidates[i].name);
                }
            }
            break;
        }

        // Eliminate anyone with minimum number of votes
        eliminate(min);

        // Reset vote counts back to zero
        for (int i = 0; i < candidate_count; i++)
        {
            candidates[i].votes = 0;
        }
    }
    return 0;
}



//FROM NOW ON THE CODE IS  MADE BY THE STUDENT (ME) :)



// Record preference if vote is valid
bool vote(int voter, int rank, string name)
{
    //for each candidate
    for (int a = 0; a < voter_count; a++ )
    {
        if (strcmp(candidates[a].name, name))
        {
            preferences[voter][rank] = a;
            return true;
        }

    }
    return false;
}

// Tabulate votes for non-eliminated candidates
void tabulate(void)
{
   //for each voter
    for (int c = 0; c < voter_count; c++)
    {
        //variable for iterating in the preferences of the selected voter
        int d = 0;
        int argument_passer = 0;
        do
        {
            argument_passer = preferences[c][d];

            if (candidates[argument_passer].eliminated != true)
            {
                candidates[argument_passer].votes++;
            }

            d++;
        }
        while (candidates[argument_passer].eliminated == true);


    }

    return;
}

// Print the winner of the election, if there is one
bool print_winner(void)
{
    //calculate votes needed to win
    int votes_needed = (voter_count / 2) + 1;
    //search for a winner and print + return true if found
    for (int e = 0; e < candidate_count; e++)
    {
        if (candidates[e].votes >= votes_needed )
        {
            printf("%s", candidates[e].name);
            return true;
        }


    }


    return false;
}

// Return the minimum number of votes any remaining candidate has
int find_min(void)
{
    //variable for storing the min of votes
    int votes_minimum = MAX_VOTERS;

    //iterate through all the candidates to know the minimum votes acquired by a non eliminated candidate
    for (int f = 0; f < candidate_count; f++)
    {
        if (candidates[f].votes < MAX_VOTERS && candidates[f].eliminated == false)
        {
            votes_minimum = candidates[f].votes;

        }

    }



    return votes_minimum;
}

// Return true if the election is tied between all candidates, false otherwise
bool is_tie(int min)
{

    //count the number of candidates still in the election
    int still_in_election = 0;

    //count the number of candidate with the same minimun number of votes
    int candidates_with_minimum_votes = 0;

    //start to iterate through candidates
    for (int g = 0; g < candidate_count; g++)
    {
        if (candidates[g].eliminated == false)
        {
            still_in_election++;
            if (candidates[g].votes == min)
            {
                candidates_with_minimum_votes++;

            }
        }
    }

    //if number of candidates still in election = candidates with the minimum number of votes -> all are at the same level of votes -> is tie
    if (still_in_election == candidates_with_minimum_votes)
    {
        return true;
    }

    return false;
}


// Eliminate the candidate (or candidates) in last place
void eliminate(int min)
{
    //for each candidate
    for (int h = 0; h < candidate_count; h++)
    {
        //if candidate has min number of votes
        if (candidates[h].votes == min)
        {
            //set to eliminated
            candidates[h].eliminated = true;
        }


    }

    return;
}
