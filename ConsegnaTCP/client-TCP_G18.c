#include <stdio.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string.h>

#pragma comment(lib, "ws2_32.lib")

#define PORTA 5000
#define BUFFER 1024

int main() {
    WSADATA wsa;
    SOCKET sock_fd;
    struct sockaddr_in server_addr;
    char server_name[100];
    char buffer[BUFFER];
    char letter;
    int num1, num2, result;

    WSAStartup(MAKEWORD(2,2), &wsa);

    printf("Inserisci il nome del server (es. localhost): ");
    scanf("%s", server_name);

    struct hostent* host = gethostbyname(server_name);
    if(!host) { printf("Errore risoluzione host.\n"); return 1; }
    char* ip = inet_ntoa(*(struct in_addr*)host->h_addr);

    sock_fd = socket(AF_INET, SOCK_STREAM, 0);

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORTA);
    server_addr.sin_addr.s_addr = inet_addr(ip);

    connect(sock_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));

    int received = recv(sock_fd, buffer, sizeof(buffer)-1, 0);
    if(received > 0) {
        buffer[received] = '\0';
        printf("%s\n", buffer);
    }

    printf("Inserisci una lettera (A/S/M/D): ");
    scanf(" %c", &letter);
    send(sock_fd, &letter, 1, 0);

    received = recv(sock_fd, buffer, sizeof(buffer)-1, 0);
    buffer[received] = '\0';
    printf("%s\n", buffer);

    if(strcmp(buffer, "TERMINE PROCESSO CLIENT") != 0) {
        printf("Inserisci il primo numero: ");
        scanf("%d", &num1);
        printf("Inserisci il secondo numero: ");
        scanf("%d", &num2);

        send(sock_fd, (char*)&num1, sizeof(int), 0);
        send(sock_fd, (char*)&num2, sizeof(int), 0);

        recv(sock_fd, (char*)&result, sizeof(int), 0);
        printf("Risultato: %d\n", result);
    }

    closesocket(sock_fd);
    WSACleanup();
    return 0;
}
