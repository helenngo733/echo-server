#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 45601
#define LISTEN_BACKLOG 5

void *handle_connection(void *client_socket) {
    int client_fd = *(int *)client_socket;
    free(client_socket);

    char buffer[1024];
    int bytes_read;

    printf("Handling Connection on %d\n", client_fd);

    // Echo back whatever the client sends
    while ((bytes_read = read(client_fd, buffer, sizeof(buffer))) > 0) {
        buffer[bytes_read] = '\0';
        printf("Received: %s\n", buffer);
        write(client_fd, buffer, bytes_read);

        printf("Done with connection %d\n", client_fd);
    }
    close(client_fd);
    return NULL;
}

int main(int argc, char *argv[]) {
    int port = PORT;

    if (argc == 3 && strcmp(argv[1], "-p") == 0) {
        port = atoi(argv[2]);
    } else {
        fprintf(stderr, "Usage: %s -p <port>\n", argv[0]);
        return 1;
    }

    // Create a socket
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);

    // Configure the server address strcuture
    struct sockaddr_in socket_address;
    memset(&socket_address, '\0', sizeof(socket_address));
    socket_address.sin_family = AF_INET;
    socket_address.sin_addr.s_addr = htonl(INADDR_ANY);
    socket_address.sin_port = htons(port);

    // Bind the socket to the address and start listening
    int return_value;
    return_value = bind(socket_fd, (struct sockaddr *)&socket_address, sizeof(socket_address));
    return_value = listen(socket_fd, LISTEN_BACKLOG);

    struct sockaddr_in client_address;
    socklen_t client_address_length = sizeof(client_address);

    // Accept connections and create threads to handle them
    while (1) {
        int *client_fd = malloc(sizeof(int));
        *client_fd =
            accept(socket_fd, (struct sockaddr *)&client_address, &client_address_length);

        printf("Accept connection on %d\n", *client_fd);

        pthread_t thread;
        pthread_create(&thread, NULL, handle_connection, client_fd);
        pthread_detach(thread);
    }
    close(socket_fd);
    return 0;
}