#include <stdio.h>
#include <winsock2.h>
#include <string.h>

#pragma comment(lib, "ws2_32.lib")

#define PORTA 5000
#define BUFFER 1024

int main() {
    WSADATA wsa;
    SOCKET server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    int client_len = sizeof(client_addr);
    char buffer[BUFFER];
    char letter;

    WSAStartup(MAKEWORD(2,2), &wsa);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORTA);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));
    listen(server_fd, 5);

    printf("SERVER TCP ATTIVO SULLA PORTA %d...\n", PORTA);

    while(1) {
        client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_len);
        if(client_fd == INVALID_SOCKET) continue;

        printf("Client connesso: %s\n", inet_ntoa(client_addr.sin_addr));

        char msg[] = "connessione avvenuta";
        send(client_fd, msg, strlen(msg), 0);

        int rec = recv(client_fd, &letter, 1, 0);
        if(rec <= 0) {
            closesocket(client_fd);
            continue;
        }

        char response[BUFFER];
        int operazione_valida = 1;
        switch(letter) {
            case 'A': case 'a': strcpy(response, "ADDIZIONE"); break;
            case 'S': case 's': strcpy(response, "SOTTRAZIONE"); break;
            case 'M': case 'm': strcpy(response, "MOLTIPLICAZIONE"); break;
            case 'D': case 'd': strcpy(response, "DIVISIONE"); break;
            default: strcpy(response, "TERMINE PROCESSO CLIENT"); operazione_valida = 0; break;
        }

        send(client_fd, response, strlen(response), 0);

        if(operazione_valida) {
            int num1, num2, result;
            recv(client_fd, (char*)&num1, sizeof(int), 0);
            recv(client_fd, (char*)&num2, sizeof(int), 0);

            switch(letter) {
                case 'A': case 'a': result = num1 + num2; break;
                case 'S': case 's': result = num1 - num2; break;
                case 'M': case 'm': result = num1 * num2; break;
                case 'D': case 'd': result = (num2 != 0) ? num1 / num2 : 0; break;
            }

            send(client_fd, (char*)&result, sizeof(int), 0);
        }

        closesocket(client_fd);
        printf("Gestione del client terminata.\n\n");
    }

    closesocket(server_fd);
    WSACleanup();
    return 0;
}

