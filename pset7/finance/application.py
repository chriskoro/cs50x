from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session, url_for
from flask_session import Session
from passlib.apps import custom_app_context as pwd_context
from tempfile import gettempdir
from datetime import datetime
import time

from helpers import *

# configure application
app = Flask(__name__)

# ensure responses aren't cached (get fresh data every time)
if app.config["DEBUG"]:
    @app.after_request
    def after_request(response):
        response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
        response.headers["Expires"] = 0
        response.headers["Pragma"] = "no-cache"
        return response

# custom filter
app.jinja_env.filters["usd"] = usd

# configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = gettempdir()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

@app.route("/")
@login_required
def index():

    # query all stocks that were bought for this user so we can show it in the view
    stocks = db.execute("SELECT symbol, stock, Sum(quantity) AS quantities FROM portfolio WHERE user_id_fk = :user_id_fk AND transaction_type = :transaction_type "+
    "GROUP BY symbol, stock ORDER BY stock", user_id_fk = session["user_id"], transaction_type = "buy")
    
    currentprices = []
    
    # get current price for each group (ie AAPL) with help from lookup function (which remember, returns a dict)
    for stock in stocks:
        symbol = str(stock["symbol"])
        quote = lookup(symbol)
        currentprices.append(quote['price'])
    
    cash = db.execute("SELECT cash FROM users WHERE id = :id", id = session["user_id"])
    
    grandtotal = 0
    i = 0
    
    # for grand total
    for stock in stocks:
        grandtotal = grandtotal + (currentprices[i] * stock["quantities"])
        i = i + 1
        
    grandtotal = grandtotal + cash[0]["cash"]
    
    # render the template with the stored result sets from queries
    return render_template("index.html", stocks = stocks, cash = cash, currentprices = currentprices, grandtotal = grandtotal)



@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock."""
    if request.method == "POST":
        
        # ensure user entered all values
        if not request.form.get("symbol") and not request.form.get("quantity"):
            return apology("Grumpy cat says", "please enter stock and quantity desired")
        elif not request.form.get("symbol"):
            return apology("Grumpy cat says", "please enter a stock symbol")
        elif not request.form.get("quantity"):
            return apology("Grumpy cat says", "please enter a quantity")
        
        # make sure quantity entered is positive
        if int(request.form.get("quantity")) < 1:
            return apology("enter 1 or more for quantity")
        
        # (js in bootstrap takes care of isalpha for us)
            
        # make sure stock symbol exists
        quote = lookup(request.form.get("symbol"))
        if not quote:
            return apology("This quote does not exist")
        
        # see if user has enough cash to buy what they requested to
        price = quote['price'] * int(request.form.get("quantity"))

        cash = db.execute("SELECT cash FROM users WHERE id = :id", id = session["user_id"])
        
        if price > cash[0]["cash"]:
            return apology("You don't have enough money, sorry!")
        
        # if user can indeed afford it, add it to (update) their portfolio (created new db here)
        db.execute("INSERT INTO portfolio (user_id_fk, stock, price, quantity, date, symbol, transaction_type) VALUES"+
        " (:user_id_fk, :stock, :price, :quantity, :date, :symbol, :transaction_type)",
        user_id_fk = session["user_id"], symbol = quote['symbol'], stock = quote['name'], price = quote['price'], quantity = request.form.get("quantity"),
        date = str(datetime.utcnow()), transaction_type = "buy")
        
        # update user's cash
        db.execute("UPDATE users SET cash = cash - :price WHERE id = :id", price = price, id = session["user_id"])
        
        return redirect(url_for("index"))
        
    else:
        return render_template("buy.html")
    
    
@app.route("/history", methods=["GET", "POST"])
@login_required
def history():
    """Show history of transactions."""
    # Whether stock was bought or sold
    # stocks symbol
    # price bought or sold at
    # number of stocks bought or sold
    # time or date
    
    # for if user searched for a symbol POST (EXTRA FUNCTIONALITY)
    #if request.method == "POST":
        #stocks = db.execute("SELECT * FROM portfolio WHERE user_id_fk = :user_id_fk ORDER BY price", user_id_fk = session["user_id"])
    
    # normal GET
    #else:
    stocks = db.execute("SELECT * FROM portfolio WHERE user_id_fk = :user_id_fk ORDER BY date", user_id_fk = session["user_id"])
    
    
    return render_template("history.html", stocks = stocks)

@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in."""
    #already implemented for us

    # forget any user_id
    session.clear()

    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username")

        # ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password")

        # query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username", username=request.form.get("username"))

        # ensure username exists and password is correct
        if len(rows) != 1 or not pwd_context.verify(request.form.get("password"), rows[0]["hash"]):
            return apology("invalid username and/or password")

        # remember which user has logged in
        session["user_id"] = rows[0]["id"]
        

        # redirect user to home page
        return redirect(url_for("index"))

    # else if user reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")

@app.route("/logout")
def logout():
    """Log user out."""
    #fully implemented for us as well
    
    # forget any user_id
    session.clear()

    # redirect user to login form
    return redirect(url_for("login"))

@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    if request.method == "POST":
        """Get stock quote."""
        #returns dict with name, price, and symbol of that stock (found in helpers.py)
        quote = lookup(request.form.get("symbol"))
        
        #now that we know how much the stock costs, lets tell the user. 
        if quote == None:
            return apology("This symbol does not exist")
        
        # @app.route("/stockinfo", methods=["GET", "POST"])
        # def stockinfo():
        return render_template("stockinfo.html", name = quote['name'], price = quote['price'])
        
    else:
        return render_template("quote.html")
        

@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user."""
    
    if request.method == "POST":
        # ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username")
            
        elif not request.form.get("password") or not request.form.get("confirm-password"):
            return apology("must provide password")
            
        elif request.form.get("password") != request.form.get("confirm-password"):
            return apology("passwords must match")
        
        #if user provided info passed all those tests (valid), proceed to add user to database
        #first, hash password for security
        hash = pwd_context.encrypt(request.form.get("password"))
        
        #add row / record to user table (new user)
        result = db.execute("INSERT INTO users (username, hash) VALUES (:username, :hash)", username = request.form.get("username"), hash = hash)
        
        if not result:
            return apology("username already exists in database")
        
        #final step: log them in automatically (since if they made it to this point, that means they've registered successfully)
        rows = db.execute("SELECT id FROM users WHERE username = :username", username = request.form.get("username"))
        session["user_id"] = rows[0]["id"]
        
        #after we've added to the database, redirect the user back to the home page!
        return redirect(url_for("index"))
        
        
    else:
        return render_template("register.html")
        
        

@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock."""
    
    if request.method == "POST":
        # ensure user entered values
        if not request.form.get("symbol"):
            return apology("Please enter a symbol")
        
        # make sure stock exists in db
        rows = db.execute("SELECT * FROM portfolio WHERE user_id_fk = :user_id_fk AND symbol = :symbol AND transaction_type = :transaction_type",
        user_id_fk = session["user_id"], symbol = request.form.get("symbol"), transaction_type = "buy")
        
        if not rows:
            return apology("You have not bought this stock", "so you can't sell it")
        
        
        #determine amount of stocks sold back for later arithmetic
        qty_sold_resultset = db.execute("SELECT symbol, date, Sum(quantity) AS quantity_sold FROM portfolio WHERE symbol = :symbol"+
        " AND user_id_fk = :user_id_fk AND transaction_type = :transaction_type GROUP BY symbol, date", symbol = request.form.get("symbol"), 
        user_id_fk = session["user_id"], transaction_type = "buy")
        
        
        
        
       # get the quote (including current price) of the stock from yahoofinance
        quote = lookup(request.form.get("symbol"))
        currentprice = quote['price']
        
        # if all tests passed, then record sale (for use in history) then delete all (obviously you can't sell stocks you sold!)
        db.execute("INSERT INTO portfolio (user_id_fk, symbol, quantity, price, date, transaction_type) VALUES"+
        "(:user_id_fk, :symbol, :quantity, :price, :date, :transaction_type)", user_id_fk = session["user_id"],
        symbol = qty_sold_resultset[0]['symbol'], quantity = qty_sold_resultset[0]['quantity_sold'], price = currentprice,
        date = qty_sold_resultset[0]['date'], transaction_type = "sell")
        
        # NOTE we dont have to delete because we just query out buy vs sell every time
        db.execute("UPDATE portfolio SET transaction_type = :transaction_type WHERE user_id_fk = :user_id_fk AND symbol = :symbol AND "+
        "transaction_type = :t", user_id_fk = session["user_id"], symbol = request.form.get("symbol"), transaction_type =
        "bought", t = "buy")
        
        # determine cash to add back to user cash (current price of stock * how many stocks sold back)
        qty_sold = qty_sold_resultset[0]["quantity_sold"]
        cash_to_add = currentprice * qty_sold
        
        db.execute("UPDATE users SET cash = cash + :cash_to_add WHERE id = :id", cash_to_add = cash_to_add, id = session["user_id"])
        
        return index()
    
    else:
        return render_template("sell.html")
        
        
@app.route("/add", methods=["GET", "POST"])
@login_required
def add():
    if request.method == "POST":
        
        if not request.form.get("cash_to_add"):
            return apology("Please enter cash amount to add")
        #elif int(request.form.get("cash_to_add")) < 0:
            #return apology("Please enter positive amount!")
        
        db.execute("UPDATE users SET cash = cash + :cash_to_add WHERE id = :id", cash_to_add = request.form.get("cash_to_add"), id = session["user_id"])
        
        get_username = db.execute("SELECT username FROM users WHERE id = :id", id = session["user_id"])
        
        return render_template("success.html", username = get_username[0]['username'], cash_to_add = request.form.get("cash_to_add"))
        
        
    else:
        rows = db.execute("SELECT username, cash FROM users WHERE id = :id", id = session["user_id"])
        return render_template("add.html", username = rows[0]['username'], cash = rows[0]['cash'])