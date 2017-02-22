import cs50
import sys

#make sure use only enetered one cmd line argument
if len(sys.argv) != 2:
    print("Enter only one whole number as key")
    exit(1)
    
#convert k user provided to int
k = sys.argv[1]
k = int(k)
k = k % 26

if type(k) is not int:
    print("please type an int key")
    exit(2)

#prompt user for plaintext
plaintext = cs50.get_string()
c = 0

#convert to ciphertext
for i in plaintext:
    if(i.islower is False):
        c = (((ord(i) + k - 65) % 26) +65)
        print("{}".format(chr(c)), end = "")
    elif(str.islower(i)):
        c = (((ord(i) + k - 97) % 26) +97)
        print("{}".format(chr(c)), end = "")
    else:
        print("{}".format(i), end = "")
        
        
print("")