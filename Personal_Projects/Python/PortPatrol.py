# Uses socket in order to scan ports on a network

# Imports
import os
import socket

# Possible vulnerable open ports
ports = {20,21,22,23,25,53,137,139,445,80,443,8080,8443,3389,1433,1434,3306}

# Port scan function
def is_port_open(host, p):
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM) # Sets up the socket to connect
    sock.settimeout(2)  # Set a timeout to avoid indefinite blocking
    is_done = False 
    try:
        # Checks to see if port is open
        while is_done == False:
            sock.connect((host, p))
            sock.close()
            is_done = False
            # True if open
            return True
    # False if port not open 
    except (socket.error, socket.timeout):
        return False

    is_done == True

# Main function
def main():
    os.system('clear')

    # User input for IP/Website
    host = input("IP: \n")

    # Check for each port in the list of ports
    for p in ports:
        is_port_open(host, p)

        # If returned true it will print the port thats open
        if (is_port_open(host,p) == True):
            print(f"Port {p} on {host} is open.\n")

        # Else if returned false it will print which port is closed
        else:
            print(f"Port {p} on {host} is closed.\n")

main()
