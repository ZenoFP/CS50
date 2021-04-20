import os
import datetime

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True


# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    #set user
    user = session["user_id"]
    #create list of stocks and quantity
    wallet = db.execute("SELECT symbol, quantity FROM wallet WHERE user_id=(?)", user)

    #add to this list of dictionarie the fields 'price' + 'total value' using lookup function
    for a in range(len(wallet)):
        #lookup for current stock
        symbol = wallet[a]["symbol"]
        quantity = wallet[a]["quantity"]
        look = lookup(symbol)
        #set total to current value of stock * quantity owned
        total = float(look["price"]) * float(quantity)
        #add price + total value of holdings to list of dictionaries
        wallet[a]["price"] = float(look["price"])
        wallet[a]["total"] = total

    #finally add current cash by setting x to the number of cash owned
    x = db.execute("SELECT cash FROM users WHERE id=(?)", user)
    #get the value
    y = float(x[0]["cash"])
    #format like other dictionaries of the list 'wallet'
    z = {"symbol":"cash", "quantity":1, "price":y, "total":y}
    #append to dictionary list
    wallet.append(z)


    return render_template('index.html', stocks=wallet)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""

    if request.method  == "GET":
        #return template with form to compile to buy
        return render_template("buy.html")


    elif request.method == "POST":
        #control if stock exists/input is submitted
        if not request.form.get("symbol")  or not request.form.get("shares"):
            return apology("Stock/Shares field Empty")

        if lookup(request.form.get("symbol")) == None:
            return apology("Symbol not in stock market")

        #lookup for current price and set variables
        user = session["user_id"]
        symbol = request.form.get("symbol").upper()
        stock = lookup(symbol)
        quantity = request.form.get("shares")
        cash = db.execute("SELECT cash FROM users WHERE id= (?)", user)
        cash=cash[0]["cash"]


        price= float(stock["price"])
        spent = price * float (quantity)


        #control if user have enough money
        if spent > cash:
            return apology("Not enough cash to buy this stock!")

        else:
            #set new variable now to the money left after buy
            now = float(cash) - float(spent)
            #set money left in the wallet(database table users.cash)
            db.execute("UPDATE users SET cash=(?) WHERE id=(?)", now, user)

            #update movement in history of movement(finance.db/movements)
            tipo = 'buy'
            date = datetime.datetime.now()
            db.execute("INSERT INTO movements (user_id, type, symbol, price, quantity, date) VALUES(?, ?, ?, ?, ?, ?)", user, tipo, symbol, price, quantity, date)

            #add to the wallet table
            possiedi = db.execute("SELECT quantity FROM wallet WHERE user_id=(?) AND symbol=(?)", user, symbol)
            #if already in wallet
            if len(possiedi) == 1:
                quantity = int(quantity) + int(possiedi[0]["quantity"])
                db.execute("UPDATE wallet SET quantity=(?) WHERE user_id=(?) AND symbol=(?)", quantity, user, symbol)

            #never been in wallet?
            else:
                db.execute("INSERT INTO wallet (user_id, symbol, quantity) VALUES(?, ?, ?)", user, symbol, quantity)

            #redirect to index as requested
            return redirect("/")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""

    #set variable to pass to template
    useragent = session["user_id"]
    history_c = db.execute("SELECT * FROM movements WHERE user_id=(?)", useragent)

    #already created history of transactions, time to show on :)
    return render_template("history.html", history_channel=history_c)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    #get method = display form to compile
    if request.method == "GET":
        return render_template("quote.html")

    #form already compiled?
    elif request.method == "POST":

        #if symbol field is empty or lookout outputs none (symbol not in list of stocks)
        if not request.form.get("symbol") or lookup(request.form.get("symbol")) == None:
            return apology("Symbol not found or not existing")

        #give to symbol the dictionary that lookup outputs
        symbol = lookup(request.form.get("symbol"))

        #return another page that manages all of the data of that stock
        return render_template("quoted.html", symbol=symbol )


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    #IF REQUEST METHOD == GET -> DISPLAY FORM
    if request.method == "GET":
        return render_template("register.html")

    #ELSE IF REQUEST METHOD = POST -> CHECK INPUTS, HASH PSW, CHECK IF USER ALREADY IN TABLE
    elif request.method == "POST":

        #control that some input has been submitted, the items arent' blank
        if not request.form.get("username") or not request.form.get("password") or not request.form.get("confirmation"):
            return apology("username and/or password fields can't be blank")

        #set values equals to the one submited to do some calculation
        username = request.form.get("username")
        password = request.form.get("password")
        confirmation = request.form.get("confirmation")

        #control if password and confirmation are ok
        if not password == confirmation:
            return apology("The two passwords didn't matched!")

        #upload in users the id of people with the same username, if there are
        users = db.execute("SELECT id FROM users WHERE username = (?)", username)

        #if there isn't duplicates, i can finally insert user into database and redirect to homepage
        if not len(users) >= 1:

            #hash user's password using hash function
            psw = generate_password_hash(password)

            #insert into database new user
            db.execute("INSERT INTO users (username, hash) VALUES (?, ?)", username, psw)

            #redirect to homepage
            return redirect("/login")

        #if not already redirected, return apology for the current error (username taken)
        return apology("Username already taken")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    user = session["user_id"]
    h = db.execute("SELECT * FROM wallet WHERE user_id=(?)", user)
    i = db.execute("SELECT cash FROM users WHERE id=(?)", user)

    #get
    if request.method == "GET":
        return render_template("sell.html", stocks=h)

    #post
    elif request.method == "POST":
        #control for empty fields
        if not request.form.get("symbol") or not request.form.get("shares"):
            return apology("Empty Stock/Shares Field")

        #set variables with inputs for later
        sell_shares=int(request.form.get("shares"))
        sell_symbol=request.form.get("symbol").upper()

        #control if user own so many shares of that stock
        HII=db.execute("SELECT quantity FROM wallet WHERE user_id=(?) AND symbol=(?)", user, sell_symbol)
        HI=int(HII[0]["quantity"])
        #if requested to sell more that had in wallet
        if sell_shares > HI:
            return apology("Request exceeds wallet holdings")

        #if sold exact quantity that i had
        elif sell_shares == HI:
            #delete from wallet
            db.execute("DELETE FROM wallet WHERE user_id=(?) AND symbol=(?)", user, sell_symbol)

        #if not sold whole bag
        else:
            #toggle sales from wallet
            actual = sell_shares - HI
            db.execute("UPDATE wallet SET quantity=(?) WHERE user_id=(?) AND symbol=(?)", actual, user, sell_symbol)


        #set sell in history
        tipe = 'sell'
        fullstack = lookup(sell_symbol)
        prezzo = float(fullstack["price"])
        data = datetime.datetime.now()
        db.execute("INSERT INTO movements (user_id, type, symbol, price, quantity, date) VALUES(?, ?, ?, ?, ?, ?)", user, tipe, sell_symbol, prezzo, sell_shares, data)

        #update cash owned
        ora = float(i[0]["cash"]) + float((prezzo * sell_shares))
        db.execute("UPDATE users SET cash=(?) WHERE id=(?)", ora, user)
        return redirect("/")


def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
