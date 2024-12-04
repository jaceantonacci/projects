#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

#define BUFFER_SIZE 1024

void sendMessage(SOCKET serverSocket, const char* message) {
    send(serverSocket, message, strlen(message), 0);
}

int main() {
    WSADATA wsaData;
    SOCKET serverSocket;
    struct sockaddr_in serverAddr;
    char buffer[BUFFER_SIZE];
    char ip[INET_ADDRSTRLEN];
    int port;
    char username[BUFFER_SIZE];

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("WSAStartup failed. Error: %d\n", WSAGetLastError());
        return 1;
    }

    // Get server IP and port
    printf("Enter server IP address: ");
    fgets(ip, sizeof(ip), stdin);
    ip[strcspn(ip, "\n")] = 0; // Remove newline character

    printf("Enter server port: ");
    scanf_s("%d", &port);
    getchar(); 

    // Get username
    printf("Enter your username: ");
    fgets(username, sizeof(username), stdin);
    username[strcspn(username, "\n")] = 0; 

    // Create socket
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        printf("Socket creation failed. Error: %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    // Set up server address structure
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = inet_addr(ip);

    // Connect to server
    if (connect(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        printf("Connection to server failed. Error: %d\n", WSAGetLastError());
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    printf("Connected to server.\n");

    // Main loop for client interaction
    while (1) {
        printf("Enter message (or 'last', 'history <n>', 'bye'): ");
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = 0; 

        // Check for specific commands
        if (strcmp(buffer, "bye") == 0) {
            sendMessage(serverSocket, "bye");
            break; 
        }
        else if (strncmp(buffer, "history ", 8) == 0 || strcmp(buffer, "last") == 0) {
            sendMessage(serverSocket, buffer); // Send command as is
        }
        else {
            // Treat input as a new message
            char message[BUFFER_SIZE];
            snprintf(message, sizeof(message), "New:%s:%s", username, buffer);
            sendMessage(serverSocket, message);
        }

        // Receive response from server
        memset(buffer, 0, BUFFER_SIZE);
        int bytesReceived = recv(serverSocket, buffer, BUFFER_SIZE - 1, 0);
        if (bytesReceived > 0) {
            buffer[bytesReceived] = '\0';
            printf("Server response: %s\n", buffer);
        }
        else {
            printf("Server disconnected or an error occurred.\n");
            break;
        }
    }

    closesocket(serverSocket);
    WSACleanup();
    printf("Client shut down.\n");

    return 0;
}
