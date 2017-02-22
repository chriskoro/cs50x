import cs50

# get dollars from user
while True: 
    print("change in dollars: ", end = "")
    dollars = cs50.get_float()
    if dollars >= 0:
        break
    
# convert to cents        
cents = dollars * 100
coins = 0

# Greedy algorithm
while cents > 0:
    
    if (cents - 25) >= 0:
        cents -= 25
        coins += 1
        
    elif (cents - 10) >= 0:
        cents -= 10
        coins += 1
        
    elif (cents - 5) >= 0:
        cents -= 5
        coins += 1
        
    else:
        cents -= 1
        coins += 1
        
# print minimum coins from given amount of change
print("{}".format(coins))