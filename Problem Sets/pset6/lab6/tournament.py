# Simulate a sports tournament

import csv
import sys
import random

# Number of simluations to run
N = 1000


def main():

    # Ensure correct usage
    if len(sys.argv) != 2:
        sys.exit("Usage: python tournament.py FILENAME")

    teams = []
    # TODO: Read teams into memory from file
    with open(sys.argv[1], 'r') as teamsfile:
        #read csv as dictionary in tm
        tm= csv.DictReader(teamsfile)
        #for each element in dictionary
        for t in tm:
            #modify string containing rating to an int
            t['rating'] = int(t['rating'])
            #add to the list of teams(dictionaries) the new team and his rating
            teams.append(t)



    counts = {}
    # TODO: Simulate N tournaments and keep track of win counts
    for tournament in range(N):
       #give to winner the team name only
       winner = simulate_tournament(teams)

       #if team already in counts, value of key 'team' +=1
       if winner in counts:
           counts[winner] += 1
           continue

       #if first victory
       counts[winner] = 1



    # Print each team's chances of winning, according to simulation
    for team in sorted(counts, key=lambda team: counts[team], reverse=True):
        print(f"{team}: {counts[team] * 100 / N:.1f}% chance of winning")


def simulate_game(team1, team2):
    """Simulate a game. Return True if team1 wins, False otherwise."""
    rating1 = team1["rating"]
    rating2 = team2["rating"]
    probability = 1 / (1 + 10 ** ((rating2 - rating1) / 600))
    return random.random() < probability


def simulate_round(teams):
    """Simulate a round. Return a list of winning teams."""
    winners = []

    # Simulate games for all pairs of teams
    for i in range(0, len(teams), 2):
        if simulate_game(teams[i], teams[i + 1]):
            winners.append(teams[i])
        else:
            winners.append(teams[i + 1])

    return winners

# TODO
def simulate_tournament(teams):
    """Simulate a tournament. Return name of winning team."""
    #give to l the list of teams that passed first round
    l = simulate_round(teams)

    #repeat until one only winner
    while len(l) > 1:
        l = simulate_round(l)

    #return winner team name
    return l[0]['team']



if __name__ == "__main__":
    main()
