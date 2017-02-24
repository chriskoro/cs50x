explanation: files in /sentiments directory implement sentiment analysis for tweets (ie happy, sad)
files in pset6/ directory are just .py files ported from c
note: Make sure your API KEY is set from the command line or there will be problems 

Main files: application.py, smile, tweets, caesar.py, greedy.py, mario.py


application.py - Web app that shows a chart of provided user screen name
(ie realDonaldTrump) in web server using flask micro-framework
Usage: Run in your web server and enter screen_name (ie realDonaldTrump) to
generate the chart

smile - Gives the sentiments of a word or words
provided by the user at command prompt
Usage: ./smile word

tweets - Program shows user the 'score' of the given
twitter user's tweets' sentiments (happy, sad, neutral)
in the command prompt, with the user providing the
screen name of the user (ie realDonaldTrump)
Usage: ./tweets screen_name

The other 3 files (caesar, greedy, mario) were also required for this problem set- 
They are implementations of the same programs I made earlier in C problem sets.
Usage (for example): python mario.py   -Remember, no compiling in Python!