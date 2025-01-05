#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

struct sensor_message {
char[12] type;
int coords[2];
float measurement;
};

void *handle_client(void *arg) {
    int client_socket = *((int *)arg);
    free(arg);

    char buffer[1024];
    while (1) {
        memset(buffer, 0, sizeof(buffer));
        int received = recv(client_socket, buffer, sizeof(buffer), 0);
        if (received <= 0) {
            perror("Erro ao receber dados do cliente");
            break;
        }
        printf("Cliente disse: %s\n", buffer);

        if (send(client_socket, "Oi", 3, 0) == -1) {
            perror("Erro ao enviar dados para o cliente");
            break;
        }
    }

    close(client_socket);
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Uso: %s <v4|v6> <porta>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int server_fd;
    struct sockaddr_in addr4;
    struct sockaddr_in6 addr6;
    int porta = atoi(argv[2]);

    if (porta <= 0) {
        fprintf(stderr, "Porta inválida: %d\n", porta);
        exit(EXIT_FAILURE);
    }

    if (strcmp(argv[1], "v4") == 0) {
        // IPv4
        server_fd = socket(AF_INET, SOCK_STREAM, 0);
        if (server_fd == -1) {
            perror("Erro ao criar socket IPv4");
            exit(EXIT_FAILURE);
        }

        addr4.sin_family = AF_INET;
        addr4.sin_port = htons(porta);
        addr4.sin_addr.s_addr = INADDR_ANY;

        if (bind(server_fd, (struct sockaddr*)&addr4, sizeof(addr4)) == -1) {
            perror("Erro ao fazer bind em IPv4");
            close(server_fd);
            exit(EXIT_FAILURE);
        }

        printf("Servidor IPv4 aguardando conexões na porta %d...\n", porta);

    } else if (strcmp(argv[1], "v6") == 0) {
        // IPv6
        server_fd = socket(AF_INET6, SOCK_STREAM, 0);
        if (server_fd == -1) {
            perror("Erro ao criar socket IPv6");
            exit(EXIT_FAILURE);
        }

        addr6.sin6_family = AF_INET6;
        addr6.sin6_port = htons(porta);
        addr6.sin6_addr = in6addr_any;

        if (bind(server_fd, (struct sockaddr*)&addr6, sizeof(addr6)) == -1) {
            perror("Erro ao fazer bind em IPv6");
            close(server_fd);
            exit(EXIT_FAILURE);
        }

        printf("Servidor IPv6 aguardando conexões na porta %d...\n", porta);
    } else {
        fprintf(stderr, "Uso inválido. Escolha entre v4 ou v6\n");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 10) == -1) {
        perror("Erro ao ouvir");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    while (1) {
        struct sockaddr_storage client_addr;
        socklen_t addr_size = sizeof(client_addr);
        int *new_socket = malloc(sizeof(int));
        *new_socket = accept(server_fd, (struct sockaddr *)&client_addr, &addr_size);

        if (*new_socket == -1) {
            perror("Erro ao aceitar conexão");
            free(new_socket);
            continue;
        }

        pthread_t client_thread;
        pthread_create(&client_thread, NULL, handle_client, new_socket);
        pthread_detach(client_thread);
    }

    close(server_fd);
    return 0;
}
