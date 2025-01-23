from cryptography.fernet import Fernet
import os

#Input to start the program
os.system('cls')
start = input("Would you like to decrypt or encrypt your password(e/d): ")
os.system('cls')

#creates the keys to encrypt
key = Fernet.generate_key()
fernet = Fernet(key)

#Check if they want to decrypt or encrypt
while(start != 'e' or 'd'):
    #Beginning of encryption
    if(start == 'e'):

        #Where the password will be used
        os.system('cls')
        web = input("Where will this password be used: ")
        os.system('cls')

        #What the password is
        password = input("Enter a password to be encrypted: ")
        os.system('cls')

        #Encryption of the password and creating a file to store it
        en = fernet.encrypt(password.encode())
        saved = web + ".txt"
        file = open(saved, 'w')
        
        #Saves the website and encrypted password
        file.write("Website= " + web + "\n" + "Encrypted Password= " + en.decode())

        #Decryption key
        print("Save this key to decrypt: " + key.decode() + '\n')

        #Shows the encrypted password if they want to save it somewhere else
        print("\n" + "Encrypted Password: " + en.decode())
        input("Enter to continue")
        os.system('cls')

        file.close()
        exit()

    #Beginning of decryption
    elif(start == 'd'):

        #Gets the encrypted password and the key needed to decrypt
        os.system('cls')
        encrypted_ps = input("Enter Encrypted Password: ").encode()
        decrypt_key = input("Enter key: ").encode()
        os.system('cls')

        #Decrypting password and printing it
        try:
            fernet = Fernet(decrypt_key)

            decrypted_ps = fernet.decrypt(encrypted_ps).decode()
            print("Decrypted Password: " + decrypted_ps)
            input("Enter to continue")
            os.system('cls')
            exit()

        #Exception if there is one
        except Exception as e:
            print(e)


    #If start isnt 'e' or 'd' it will keep asking
    print("Invalid, try again")
    start = input("Would you like to decrypt or encrypt your password(e/d): ")
    os.system('cls')
