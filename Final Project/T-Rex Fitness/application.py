import os
import datetime

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash
from helpers import push, pull, iso

# Configure application
app = Flask(__name__)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///fitness.db")

@app.route("/")
def index():
    return render_template("t-rex.html")

@app.route("/test")
def test():
    return render_template("test.html")


@app.route("/beast")
def beast():
    return render_template("beast.html")

@app.route("/form")
def form():
    return render_template("form.html")

@app.route("/scheda", methods=["GET", "POST"])
def scheda():

    #no get method allowed
    if request.method  == "POST":

        if not request.form.get("push") or not request.form.get("pull") or not request.form.get("wall") or not request.form.get("plank"):
            return redirect("/form")

        #takes input if not null
        flessioni= int(request.form.get("push"))
        trazioni= int(request.form.get("pull"))
        gambe= int(request.form.get("wall"))
        addominali= int(request.form.get("plank"))

        #set levels by functions
        livello_push = int(flessioni)//5
        livello_pull = int(trazioni)//4
        livello_gambe = int(gambe)//20
        livello_abs = int(addominali)//20

        #create list of levels
        levels = [livello_push, livello_pull, livello_gambe, livello_abs]

        #discover lower level
        livello_inferiore = 6
        for x in range(len(levels)):

            if levels[x] > 5:
                levels[x] = 5

            if levels[x] < livello_inferiore:
                livello_inferiore = levels[x]

        #if all the levels are at 5
        if livello_inferiore == 5:
            return redirect("/beast")

        #search for pushing exercise of that level
        push = db.execute("SELECT name, link FROM push WHERE level=(?)", livello_inferiore)
        #search for pulling exercise of that level
        pull = db.execute("SELECT name, link FROM pull WHERE level=(?)", livello_inferiore)
        #search for leg exercise of that level
        legs = db.execute("SELECT name, link FROM legs WHERE level=(?)", livello_inferiore)
        #search for abs exercise of that level
        ab = db.execute("SELECT name, link FROM abs WHERE level=(?)", livello_inferiore)

        tirata = push[0]
        spinta = pull[0]
        gambe = legs[0]
        addominalis = ab[0]

        #return scheda to webpage in order to show exercises, series, repetitions, pause time
        return render_template("scheda.html", tirata=tirata, spinta=spinta, gambe=gambe, addominali=addominalis)

    #if method get, redirect to form
    else:
        return redirect("/form")