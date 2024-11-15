#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>  


void countRowsCols(FILE *file, int *rows, int *cols) {
    char buffer[1024];
    *rows = 0;
    *cols = 0;

    while (fgets(buffer, sizeof(buffer), file)) {
        (*rows)++;
        int cols_per_row = 0;
        char *token = strtok(buffer, " ");
        while (token != NULL) {
            cols_per_row++;
            token = strtok(NULL, " ");
        }

        if (*rows == 1) {
            *cols = cols_per_row;
        }
    }
}


int** lerArquivo(const char *nomeArquivo, int *linhas, int *colunas) {
    FILE *arquivo = fopen(nomeArquivo, "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return NULL;
    }
    countRowsCols(arquivo, linhas, colunas);
    rewind(arquivo); 


    int **matriz = (int **)malloc(*linhas * sizeof(int *));
    for (int i = 0; i < *linhas; i++) {
        matriz[i] = (int *)malloc(*colunas * sizeof(int));
    }

  
    char buffer[1024];
    for (int i = 0; i < *linhas; i++) {
        fgets(buffer, sizeof(buffer), arquivo);
        int j = 0;
        char *token = strtok(buffer, " ");
        while (token != NULL) {
            matriz[i][j] = atoi(token);
            j++;
            token = strtok(NULL, " ");
        }
    }

    fclose(arquivo);
    return matriz; 
}


void liberarMatriz(int **matriz, int linhas) {
    for (int i = 0; i < linhas; i++) {
        free(matriz[i]);
    }
    free(matriz);
}


void exibirMatriz(int **matriz, int linhas, int colunas) {
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            printf("%d ", matriz[i][j]);
        }
        printf("\n");
    }
}

bool podeDireita (int **matriz, int linhas, int colunas, int linhaPersonagem, int colunaPersonagem){
    
    if (colunaPersonagem+1 <= colunas-1)
    if (matriz[linhaPersonagem][colunaPersonagem+1]!=0)
    return true;
    return false;
}

bool podeEsquerda (int **matriz, int linhas, int colunas, int linhaPersonagem, int colunaPersonagem){
    if (colunaPersonagem-1 >= 0)
    if (matriz[linhaPersonagem][colunaPersonagem-1]!=0)
    return true;
    return false;
}

bool podeCima (int **matriz, int linhas, int colunas, int linhaPersonagem, int colunaPersonagem){
    if (linhaPersonagem-1 >= 0)
    if (matriz[linhaPersonagem-1][colunaPersonagem]!=0)
    return true;
    return false;
}

bool podeBaixo (int **matriz, int linhas, int colunas, int linhaPersonagem, int colunaPersonagem){
    if (linhaPersonagem+1 <= linhas-1)
    if (matriz[linhaPersonagem+1][colunaPersonagem]!=0)
    return true;
    return false;
}

void corrigirVetor(int vetor[]) {

    int i;
    for (i = 0; i < 4; i++) {
        if (vetor[i] == 0) {
            
            int j;
            for (j = i + 1; j < 4; j++) {
                if (vetor[j] != 0) {
                    
                    vetor[i] = vetor[j];
                    
                    vetor[j] = 0;
                    break;
                }
            }
        }
    }
}

int colunasPersonagem(int **matriz, int linhas, int colunas){
        
        int colunaPersonagem;
        for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            if (matriz[i][j] == 5){
                
                colunaPersonagem = j;

            }
            
        }
       
    }
    return colunaPersonagem;
}

int linhasPersonagem(int **matriz, int linhas, int colunas){
        
        int linhaPersonagem;
        for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            if (matriz[i][j] == 5){
                
                linhaPersonagem = i;

            }
            
        }
       
    }
    return linhaPersonagem;
}

int* PossibleMoves(int **matriz, int linhas, int colunas) {
    
    int* vetor = (int*)malloc(100 * sizeof(int));
    if (vetor == NULL) {
        
        printf("Erro na alocação de memória!\n");
        exit(1); 
    }

    memset(vetor, 0, 100 * sizeof(int));
    int linhaPersonagem =0;
    int colunaPersonagem =0;


                
    linhaPersonagem = linhasPersonagem(matriz, linhas, colunas);
    colunaPersonagem = colunasPersonagem(matriz, linhas, colunas);

      
    for (int i=0; i<4; i++){

        switch (i) {
            case 0:
            vetor[i]=(i+1)*podeCima(matriz, linhas, colunas, linhaPersonagem, colunaPersonagem);
            break;

            case 1:
            vetor[i]=(i+1)*podeDireita(matriz, linhas, colunas, linhaPersonagem, colunaPersonagem);
            break;

            case 2:
            vetor[i]=(i+1)*podeBaixo(matriz, linhas, colunas, linhaPersonagem, colunaPersonagem);
            break;
            
            case 3:
            vetor[i]=(i+1)*podeEsquerda(matriz, linhas, colunas, linhaPersonagem, colunaPersonagem);
            break;

            default:
            break;
   
        }
       
    }    
    
    corrigirVetor(vetor);
    return vetor; 
}


void moveTo (int position, int **matriz, int **original, int linhas, int colunas){

int linhaAtual = linhasPersonagem(matriz, linhas, colunas);
int colunaAtual = colunasPersonagem(matriz, linhas, colunas);

switch (position){

    case 1:
    matriz[linhaAtual][colunaAtual]=original[linhaAtual][colunaAtual];
    matriz[linhaAtual-1][colunaAtual]=5;
    break;
    
    case 2:
    matriz[linhaAtual][colunaAtual]=original[linhaAtual][colunaAtual];
    matriz[linhaAtual][colunaAtual+1]=5;
    break;

    case 3:
    matriz[linhaAtual][colunaAtual]=original[linhaAtual][colunaAtual];
    matriz[linhaAtual+1][colunaAtual]=5;
    break;

    case 4:
    matriz[linhaAtual][colunaAtual]=original[linhaAtual][colunaAtual];
    matriz[linhaAtual][colunaAtual-1]=5;
    break;

    default:
    printf("INEXISTENTE");
    break;


}

}

int colunaFinal (int **matriz, int linhas, int colunas){

    int colunaFinal =0;

    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            if (matriz[i][j] == 3){
                
                colunaFinal = j;

            }
            
        }
       
    }

    return colunaFinal;
}

int linhaFinal (int **matriz, int linhas, int colunas){

    int linhaFinal =0;

    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            if (matriz[i][j] == 3){
                
                linhaFinal = i;

            }
            
        }
       
    }

    return linhaFinal;
}

void Start (int **matriz, int linhas, int colunas){

    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            if (matriz[i][j] == 2){
                
                matriz[i][j]=5;

            }
            
        }
       
    }    


}

bool Winner (int **matriz, int **original, int linhas, int colunas){

    int linhaAtual = linhasPersonagem(matriz, linhas, colunas);
    int colunaAtual = colunasPersonagem(matriz, linhas, colunas);
    
    if (colunaFinal(original, linhas, colunas)==colunaAtual && linhaFinal(original, linhas, colunas)==linhaAtual)
        return true;
        return false;
}

int** criarMatriz(int linhas, int colunas) {
    
    int** matriz = (int**)malloc(linhas * sizeof(int*));

    if (matriz == NULL) {
        printf("Erro na alocação de memória!\n");
        exit(1); 
    }

 
    for (int i = 0; i < linhas; i++) {
        matriz[i] = (int*)malloc(colunas * sizeof(int));
        if (matriz[i] == NULL) {
            printf("Erro na alocação de memória!\n");
            exit(1); 
        }
    }

    
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            matriz[i][j] = 0;
        }
    }

    return matriz; 
}

void atualizaMascara (int **matriz, int **mascara, int linhas, int colunas){

    int linhaAtual = linhasPersonagem(matriz, linhas, colunas);
    int colunaAtual = colunasPersonagem(matriz, linhas, colunas);

    if (linhaAtual+1<=linhas-1)
        mascara[linhaAtual+1][colunaAtual]=1;
    if (linhaAtual-1>0)
        mascara[linhaAtual-1][colunaAtual]=1;
    if (colunaAtual+1<=colunas-1)
        mascara[linhaAtual][colunaAtual+1]=1;
    if (colunaAtual-1>0)
        mascara[linhaAtual][colunaAtual-1]=1;
    mascara[linhaAtual][colunaAtual]=1;

}

int** multiplicarElementoPorElemento(int** matrizA, int** mascara, int linhas, int colunas) {
    
    int** matrizResultado = (int**)malloc(linhas * sizeof(int*));
    for (int i = 0; i < linhas; i++) {
        matrizResultado[i] = (int*)malloc(colunas * sizeof(int));
    }


    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            if(mascara[i][j] ==0){
                matrizResultado[i][j]=4;
            }else{
               matrizResultado[i][j]=matrizA[i][j]; 
            }
        }
    }

    return matrizResultado; 
}



char** converterMatriz(int** matriz, int linhas, int colunas) {
   
    char** matrizChar = (char**)malloc(linhas * sizeof(char*));
    for (int i = 0; i < linhas; i++) {
        matrizChar[i] = (char*)malloc(colunas * sizeof(char));
    }


    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            switch (matriz[i][j]) {
                case 0:
                    matrizChar[i][j] = '#';
                    break;
                case 1:
                    matrizChar[i][j] = '_';
                    break;
                case 2:
                    matrizChar[i][j] = '>';
                    break;
                case 3:
                    matrizChar[i][j] = 'X';
                    break;
                case 4:
                    matrizChar[i][j] = '?';
                    break;
                case 5:
                    matrizChar[i][j] = '+';
                    break;
                default:
                    matrizChar[i][j] = ' '; 
                    break;
            }
        }
    }

    return matrizChar; 
}


void exibirMatrizChar(char** matriz, int linhas, int colunas) {
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            printf("%c ", matriz[i][j]);
        }
        printf("\n");
    }
}

    int main()
    {

        const char *Labirinto = "input/maze.txt"; 
        int linhas, colunas;
    
     
        int **matriz = lerArquivo(Labirinto, &linhas, &colunas);
        int **LabirintoOriginal = lerArquivo(Labirinto, &linhas, &colunas);
        int **mascara = criarMatriz(linhas, colunas);
        char **matrizVisivel;
        Start(matriz, linhas, colunas);    


        int movimento;
            do {
                atualizaMascara(matriz, mascara, linhas, colunas);
                printf("\nMatriz Atual:\n");
                matrizVisivel = converterMatriz(multiplicarElementoPorElemento(matriz, mascara, linhas, colunas),linhas, colunas);
                exibirMatrizChar(matrizVisivel, linhas, colunas);

                printf("\nMovimentos possiveis: \n");
                int *vetor = PossibleMoves(matriz, linhas, colunas);
                for (int loop = 0; loop < 4; loop++)
                    printf("%d ", vetor[loop]);
                free(vetor);  

                printf("\nDigite o movimento (1=Cima, 2=Direita, 3=Baixo, 4=Esquerda, 0=Sair): ");
                scanf("%d", &movimento);

                if (movimento >= 1 && movimento <= 4) {
                    moveTo(movimento, matriz, LabirintoOriginal, linhas, colunas);
                }

                if (Winner(matriz, LabirintoOriginal, linhas, colunas)){
                    printf ("Ganhou!\n");
                    movimento = 0;
                }

                } while (movimento != 0);



        printf("\nLabirinto completo: \n");
        exibirMatriz(mascara, linhas, colunas);
        return 0;

    }