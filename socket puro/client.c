#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 51511

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <hostname>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};
    char message[1024];

    // Criando o socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Erro ao criar socket");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Converte o endereço IP do servidor do formato texto para binário
    if (inet_pton(AF_INET, argv[1], &serv_addr.sin_addr) <= 0) {
        perror("Endereço inválido ou não suportado");
        return -1;
    }

    // Conectando ao servidor
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Erro ao conectar");
        return -1;
    }

    printf("Conectado ao servidor! Você pode começar a conversar.\n");

    while (1) {
        // Solicita uma mensagem do usuário
        printf("Digite a mensagem para o servidor: ");
        fgets(message, sizeof(message), stdin); // Lê uma linha de texto

        // Envia a mensagem ao servidor
        send(sock, message, strlen(message), 0);
        printf("Mensagem enviada ao servidor\n");

        // Se o usuário digitar 'exit', termina o programa
        if (strncmp(message, "exit", 4) == 0) {
            break;
        }

        // Recebe a resposta do servidor
        read(sock, buffer, 1024);
        printf("Mensagem recebida do servidor: %s\n", buffer);
    }

    // Fechando o socket
    close(sock);
    return 0;
}
