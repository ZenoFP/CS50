from cs50 import get_string

frase = get_string ("Inserisci il testo: ")

#sum letters
letters = 0.00
#sum sentences
sentences = 0.00
#number of words
words = 0.00

#Iterate throught frase for counting words/letters/sentences
for letter in range(len(frase)):

    #if char is letter -> letter ++
    if frase[letter].isalpha():
        letters += 1.00

    #if char is . or ! or ? (end of a sentence) -> sentence and word ++ as is also a end for a word
    if frase[letter] == "." or frase[letter] == "!" or frase[letter] == "?":
        sentences +=  1.00
        words += 1.00

    #if char is space and last char was letter -> end of a word, so word++
    if frase[letter].isspace() and (frase[letter - 1].isalpha() or frase[letter - 1] == "," or frase[letter - 1] == ":" or frase[letter - 1] == ";") :
        words +=  1.00


#Calculate the L S and Coleman Liau Index
letters = (letters / words) * 100.00
sentences = (sentences / words) * 100.00
colemanliau = (0.0588 * letters) - (0.296 * sentences) - 15.80



#round to nearest integer because check50 is freaking unhappy
colemanliau = round(colemanliau)

#Print grade: above 16/below 1/Actual Grade
if colemanliau > 16:
    print ("Grade 16+")


elif colemanliau < 1:
    print ("Before Grade 1")

else:
    print (f"Grade {colemanliau}")