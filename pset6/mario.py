'''
builds mario pyramid
with user entering desired height during runtime
like in pset1
Usage: python mario.py
'''

import cs50

while True:
    print("Height: ", end = "")
    height = cs50.get_int()
    if (height > 0) and (height < 24):
        break
    
#for each row
for row in range(height):
    
    #print spaces
    for j in range(height - (row+1)):
        print(" ", end = "")
    
    #print hashes
    for k in range(row+2):
        print("#", end = "")
    
    print("")
    



        


