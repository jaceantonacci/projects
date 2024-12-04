#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

#define BUFFER_SIZE 1024
#define HISTORY_CAPACITY 100

typedef struct {
    char messages[HISTORY_CAPACITY][BUFFER_SIZE];
    int count;
} MessageHistory;

void initHistory(MessageHistory* history) {
    history->count = 0;
}

void addMessage(MessageHistory* history, const char* message) {
    if (history->count < HISTORY_CAPACITY) {
        strcpy_s(history->messages[history->count], BUFFER_SIZE, message);
        history->count++;
    }
    else {
        // Handle history overflow 
    }
}

const char* getLastMessage(MessageHistory* history) {
    if (history->count > 0) {
        return history->messages[history->count - 1];
    }
    return "No messages available.";
}

void getHistory(MessageHistory* history, int n, char* response) {
    response[0] = '\0'; // Initialize response buffer
    for (int i = history->count - 1; i >= 0 && n > 0; --i, --n) {
        strcat_s(response, BUFFER_SIZE * HISTORY_CAPACITY, history->messages[i]);
        strcat_s(response, BUFFER_SIZE * HISTORY_CAPACITY, "\n");
    }
    if (response[0] == '\0') {
        strcpy_s(response, BUFFER_SIZE * HISTORY_CAPACITY, "No messages available.\n");
    }
}

void sendToClient(SOCKET clientSocket, const char* message) {
    send(clientSocket, message, strlen(message), 0);
}

int main() {
    WSADATA wsaData;
    SOCKET serverSocket, clientSocket;
    struct sockaddr_in serverAddr, clientAddr;
    char buffer[BUFFER_SIZE];
    char ip[INET_ADDRSTRLEN]; 
    int port, clientAddrSize;

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("WSAStartup failed. Error: %d\n", WSAGetLastError());
        return 1;
    }

    // Get IP and Port from user
    printf("Enter IP address: ");
    fgets(ip, sizeof(ip), stdin);
    ip[strcspn(ip, "\n")] = 0; 

    printf("Enter port: ");
    scanf_s("%d", &port);

    // Create server socket
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

    // Check if the IP address is valid
    if (serverAddr.sin_addr.s_addr == INADDR_NONE) {
        printf("Invalid IP address format.\n");
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    // Bind socket
    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        printf("Binding failed. Error: %d\n", WSAGetLastError());
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    // Listen for connections
    if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
        printf("Listening failed. Error: %d\n", WSAGetLastError());
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    printf("Server is listening on %s:%d\n", ip, port);

    // Initialize message history
    MessageHistory history;
    initHistory(&history);

    // Accept a client connection
    clientAddrSize = sizeof(clientAddr);
    clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientAddrSize);
    if (clientSocket == INVALID_SOCKET) {
        printf("Accept failed. Error: %d\n", WSAGetLastError());
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    printf("Client connected.\n");

    // Handle client messages
    while (1) {
        memset(buffer, 0, BUFFER_SIZE);
        int bytesReceived = recv(clientSocket, buffer, BUFFER_SIZE - 1, 0);

        if (bytesReceived <= 0) {
            printf("Client disconnected or an error occurred. Closing connection.\n");
            break;
        }

        // Ensure null termination
        buffer[bytesReceived] = '\0';
        printf("Received command: %s\n", buffer);

        // Command processing
        if (strncmp(buffer, "New:", 4) == 0) {
            char user[BUFFER_SIZE], message[BUFFER_SIZE];
            int numTokens = sscanf_s(buffer + 4, "%[^:]:%[^\n]", user, sizeof(user), message, sizeof(message));
            if (numTokens == 2) {
                char fullMessage[BUFFER_SIZE];
                snprintf(fullMessage, BUFFER_SIZE, "%s: %s", user, message);
                addMessage(&history, fullMessage);
                sendToClient(clientSocket, "Message received.\n");
            }
            else {
                sendToClient(clientSocket, "Invalid format. Use New:User:Message.\n");
            }
        }
        else if (strcmp(buffer, "last") == 0) {
            sendToClient(clientSocket, getLastMessage(&history));
            sendToClient(clientSocket, "\n");
        }
        else if (strncmp(buffer, "history ", 8) == 0) {
            int n = atoi(buffer + 8);
            char historyResponse[BUFFER_SIZE * HISTORY_CAPACITY] = { 0 };
            getHistory(&history, n, historyResponse);
            sendToClient(clientSocket, historyResponse);
        }
        else if (strcmp(buffer, "bye") == 0) {
            sendToClient(clientSocket, "Goodbye.\n");
            break;  
        }
        else {
            sendToClient(clientSocket, "Invalid command.\n");
        }
    }

    closesocket(clientSocket);
    closesocket(serverSocket);
    WSACleanup();
    printf("Server shut down.\n");

    return 0;
}
