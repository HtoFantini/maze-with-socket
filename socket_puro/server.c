#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 51511

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    char buffer[1024] = {0};

    // Criando o socket do servidor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Erro ao criar o socket");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;  // Aceitar conexões de qualquer IP
    address.sin_port = htons(PORT);

    // Ligando o socket ao endereço e porta
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Erro no bind");
        exit(EXIT_FAILURE);
    }

    // Colocando o servidor para escutar conexões
    if (listen(server_fd, 3) < 0) {
        perror("Erro ao escutar");
        exit(EXIT_FAILURE);
    }

    printf("Servidor aguardando conexão...\n");

    socklen_t addrlen = sizeof(address);  // Variável para armazenar o tamanho da estrutura sockaddr_in
    // Aceitar a primeira conexão
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, &addrlen)) < 0) {
        perror("Erro ao aceitar conexão");
        exit(EXIT_FAILURE);
    }

    printf("Conexão estabelecida com o cliente\n");

    while (1) {
        // Ler dados enviados pelo cliente
        int valread = read(new_socket, buffer, 1024);
        if (valread == 0) {
            // Se o cliente fechar a conexão
            printf("Cliente desconectado\n");
            break;
        }
        printf("Mensagem recebida do cliente: %s\n", buffer);

        // Enviar resposta ao cliente
        send(new_socket, "Message Received", strlen("Message Received"), 0);
    }

    close(new_socket);
    close(server_fd);
    return 0;
}
