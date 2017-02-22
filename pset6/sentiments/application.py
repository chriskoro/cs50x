from flask import Flask, redirect, render_template, request, url_for
import os
import sys
import nltk

import helpers
from analyzer import Analyzer

app = Flask(__name__)

@app.route("/")
def index():
    return render_template("index.html")

@app.route("/search")
def search():

    # validate screen_name
    screen_name = request.args.get("screen_name", "").lstrip("@")
    if not screen_name:
        return redirect(url_for("index"))

    # get screen_name's tweets
    tweets = helpers.get_user_timeline(screen_name)
    # (TODO) redirect to index if no tweets for given user
    if tweets == None:
        return redirect(url_for("index"))
        

    # TODO - analyze tweets
    #initalize analyzer like in analyzer.py
    #iterate over tweets and score words in tweets to calc score
    tokenizer = nltk.tokenize.TweetTokenizer()
    positives = os.path.join(sys.path[0], "positive-words.txt")
    negatives = os.path.join(sys.path[0], "negative-words.txt")
    analyzer = Analyzer(positives, negatives)
    positive = 0
    negative = 0
    neutral = 0
    j = 0

    #score, and classify each tweet as pos neg or neutral
    for t in tweets:
        temp = tokenizer.tokenize(t)
        n = len(temp)
        score = 0
        for i in range(n):
            if temp[i].lower() in analyzer.positiveSet:
                score = score + 1
            elif temp[i].lower() in analyzer.negativeSet:
                score = score - 1
            if i+1 == n:
                if score > 0.0:
                    positive+=1
                elif score < 0.0:
                    negative+=1
                else:
                    neutral+=1
    
    chart = helpers.chart(positive, negative, neutral)

    # render results
    return render_template("search.html", chart=chart, screen_name=screen_name)