#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

#define MAXLG 132 // maximum length of messages

int main() {
    int st, portserver, s;
    int end = 0;
    struct sockaddr_in server;
    char *adrserver;
    struct sockaddr name;
    char *mes;
    adrserver = (char*) malloc(32);
    mes = (char*) malloc(256);
    printf("Client TCP Program\n");
    printf("Enter the server address in the form of a.b.c.d: ");
    do {
        scanf("%s", adrserver);
        if (inet_aton(adrserver, &server.sin_addr) != 0)
            end = 2;
        else
            printf("Invalid address, please try again: ");
    } while (end == 0);

    if (end == 2) {
        printf("Enter the server port number: ");
        scanf("%d", &portserver);
        printf("Calling the socket function\n");
        s = socket(AF_INET, SOCK_STREAM, 0);
        if (s == -1) {
            printf("Invalid socket\n");
            return -1;
        }
        printf("Connection request\n");
        server.sin_family = AF_INET;
        server.sin_port = htons(portserver);
        st = connect(s, (const struct sockaddr*) &server, sizeof(server));
        if (st != 0) {
            printf("Connection failed\n");
            return -1;
        }
        printf("Connection established; waiting for the welcome message\n");
        st = recv(s, mes, 256, 0);
        if (st > 0) {
            mes[st] = '\0';
            printf("Message received: %s\n", mes);
        } else if (st < 0) {
            printf("Error in the recv function\n");
            return -1;
        }

        // Dialogue phase
        do {
            printf("Enter a message to send to the server (or 'end' to finish): ");
            scanf("%s", mes);
            st = send(s, mes, strlen(mes), 0);
            if (st != strlen(mes))
                printf("The message was not completely sent or the connection failed.\n");
            else {
                printf("Message sent; waiting for the server response\n");
                st = recv(s, mes, 256, 0);
                if (st > 0) {
                    mes[st] = '\0';
                    printf("Server response: %s\n", mes);
                } else
                    printf("Problem in receiving the server response: %d\n", st);
            }
        } while (strcmp(mes, "end") != 0);
    }

    free(adrserver);
    free(mes);
    return 0;
}
