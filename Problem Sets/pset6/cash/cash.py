from cs50 import get_float

resto = get_float("Inserisci il resto dovuto: ")

while resto < 0:
    resto = get_float("Inserisci il resto dovuto: ")

resto = resto *100

quartini = resto // 25
resto = resto % 25


decini = resto // 10
resto = resto % 10


cinquini = resto // 5
resto = resto % 5


centesimi = resto // 1


counter = quartini + decini + cinquini + centesimi

print (f"{counter}")