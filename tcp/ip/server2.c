#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

int main() {
    int st;
    int portserver;
    socklen_t lgclient;
    int soc, s;
    struct sockaddr_in server, client;
    char *mes;
    mes = (char*) malloc(256);
    printf("TCP server program\n");
    printf("Enter the server port number: ");
    scanf("%d", &portserver);
    printf("Calling the socket function\n");
    soc = socket(AF_INET, SOCK_STREAM, 0); // create the socket
    if (soc == -1) {
        printf("Invalid socket\n");
        return -1;
    }
    // Specify the server address and port number to the system
    server.sin_family = AF_INET;
    server.sin_port = htons(portserver);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    st = bind(soc, (const struct sockaddr*) &server, sizeof(server));
    if (st != 0) {
        printf("Invalid bind: st = %d\n", st);
        exit(-1);
    }
    printf("Waiting for a connection request on port %d\n", portserver);
    st = listen(soc, 5);
    if (st != 0) {
        printf("Invalid listen\n");
        return -1;
    }
    s = accept(soc, (struct sockaddr*) &client, &lgclient);
    if (s == -1) {
        printf("Invalid accept\n");
        return -1;
    }
    printf("Connection accepted; client %s\n", inet_ntoa(client.sin_addr));
    // The connection is established
    strcpy(mes, "Welcome");
    st = send(s, mes, strlen(mes), 0);
    if (st == -1) {
        printf("Error sending the message to the client\n");
        return -1;
    }
    char rep[128];
    do {
        printf("Waiting for client message\n");
        st = recv(s, rep, 256, 0);
        if (st > 0) {
            rep[st] = '\0';
            printf("Number of characters received: %d\n", st);
            printf("Message received from the client: %s\n", rep);
            st = send(s, rep, strlen(rep), 0); // echo back the message
        } else {
            printf("Return value from recv: %d\n", st);
            st = sleep(3);
        }
    } while (strcmp("end", rep) != 0);
    close(s);
    close(soc);
    free(mes);
    return 0;
}
