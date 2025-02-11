# Uses socket in order to scan ports on a network

# Imports
import os
import socket
import concurrent.futures

# Possible vulnerable open ports
ports = {20,21,22,23,25,53,137,139,445,80,443,8080,8443,3389,1433,1434,3306}

# Port scan function
def is_port_open(host, p):
    try:
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as sock:
            sock.settimeout(1)  # Short timeout for faster scanning
            if sock.connect_ex((host, p)) == 0:
                return True
    except socket.gaierror:
        print(f"Error: Could not resolve hostname {host}")
        return False
    except socket.error:
        return False
    return False


# Main function
def main():
    os.system('clear')

    # User input for IP/Website
    host = input("Enter IP or Domain: ")

    print(f"\nScanning {host}...\n")

    ports_open = []

    # Threads to run faster
    with concurrent.futures.ThreadPoolExecutor(max_workers=10) as executor:
        results = {executor.submit(is_port_open, host, port): port for port in ports}
        
        for future in concurrent.futures.as_completed(results):
            port = results[future]
            if future.result():
                ports_open.append(port)

    # Print results
    if ports_open:
        print("\nOpen Ports Found:")
        for port in ports_open:
            print(f" - Port {port}\nService Name {socket.getservbyport(port)}\n")
    else:
        print("\nNo open ports detected.")


main()
