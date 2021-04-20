#Asks for user input (stored as a string)
height = input ("Insert the brick's Height: ")
#Se è stata inserita una stringa o numero < 1 o >8  richiedi per input
while height.isdigit() == False or ( int (height) < 1 or int (height) > 8 ):
    height = input ("Insert the brick's Height: ")
height = int(height)
for x in range (height + 1):
    a = 0
    if x < height:
        a = height - x

    if x > 0:
        print (' ' * a  , end = "")
        print ("#" * x, end = "")
        print()