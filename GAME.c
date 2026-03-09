#include <stdio.h>
#include <stdlib.h>

#define MAX_TROPAS 50
#define NUM_TERRITORIOS 5

struct Territorio {
    char nome[30];
    char cor[10];
    int quantidadeTropas;
    int tropas[MAX_TROPAS];
};

int main(){

    struct Territorio territorios[NUM_TERRITORIOS];

    // Cadastro dos territórios
    for (int i = 0; i < NUM_TERRITORIOS; i++) {

        printf("\nDigite o Territorio %d\n", i + 1);

        printf("Digite o nome do territorio\n Ex:(Africa,Europa,Asia)");
        scanf("%s", territorios[i].nome);

        printf("Digite a cor do territorio\n Ex:(Azul,Verde,Amarelo): ");
        scanf("%s", territorios[i].cor);

        printf("Digite a quantidade de tropas(MAX 50):");
        scanf("%d", &territorios[i].quantidadeTropas);

        if (territorios[i].quantidadeTropas > MAX_TROPAS){
            printf("Quantidade maior que o permitido! Maximo %d tropas.\n", MAX_TROPAS);
            i--; 
        }
    }

    printf("\n--- TERRITORIOS CADASTRADOS ---\n");

    for (int i = 0; i < NUM_TERRITORIOS; i++) {

        printf("Territorio %d\n", i + 1);
        printf("Nome: %s\n", territorios[i].nome);
        printf("Cor: %s\n", territorios[i].cor);
        printf("Quantidade de tropas: %d\n\n", territorios[i].quantidadeTropas);

    }
 system("pause");
    return 0;
}