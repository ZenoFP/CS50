import csv
from sys import argv, exit

if len(argv) != 3:
    print("Incorrect Usage.\nCorrect Usage: python dna.py database.csv text.txt")
    exit(1)

#Open .TXT File and SAVE into 'dna_seq'
txt = open(argv[2], 'r')
dna_seq = txt.read()
txt.close()

#Open csv and read firts line in triplets - other lines in agents
agents = []
with open (argv[1], 'r') as csvfile:
    csvreader = csv.reader(csvfile)
    for row in csvreader:
        if row[0] == 'name':
            triplets = row
            triplets.pop(0)
        else:
            agents.append(row)



values = []

#Fill values list with the values of the various sequences maximum repetitions
for triplet in triplets:

    bus = 0
    counter = 0
    tram = 0
    biglietti = 0

    while (dna_seq.find(triplet, bus, len(dna_seq)) ) != -1:

        bus =  dna_seq.find (triplet, bus)

        if tram == 0:
           counter += 1
           biglietti = counter
           tram = bus

        elif (bus - len(triplet) ) == tram :
            counter +=1
            tram = bus
            if biglietti < counter:
                biglietti = counter

        elif (bus - len(triplet)) != tram :
            counter = 1
            tram = bus
            if biglietti < counter:
                biglietti = counter
        bus += 1

    values.append(biglietti)







for agent in agents:

    name = agent[0]

    agent.pop(0)

    c = 0

    for x in range(len(agent)):
        if int (agent[x]) == values[x]:
            c = c + 1

    if c == len(agent):
        print(name)
        exit(0)


print ("No match")
exit(0)

