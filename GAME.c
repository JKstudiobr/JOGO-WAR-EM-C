#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define NUM_CONTINENTES 5
#define PAISES_POR_CONTINENTE 5
#define MAX_JOGADORES 5

struct Pais {
    char nome[30];
    int dono;
    int tropas;
};

struct Continente {
    char nome[30];
    struct Pais paises[PAISES_POR_CONTINENTE];
};

struct Jogador {
    char nome[30];
};

// ======= paises =======
const char* nomesPaises[NUM_CONTINENTES][PAISES_POR_CONTINENTE] = {
    {"Brasil", "EUA", "Canada", "Mexico", "Argentina"},
    {"Franca", "Alemanha", "Italia", "Espanha", "Inglaterra"},
    {"China", "Japao", "India", "Coreia", "Russia"},
    {"Egito", "Nigeria", "AfricaSul", "Marrocos", "Quenia"},
    {"Australia", "NovaZelandia", "Fiji", "Samoa", "Tonga"}
};

// ================= ui =================
void mostrarPorJogador(struct Continente mapa[], struct Jogador jogadores[], int numJogadores) {
    printf("\n====== TERRITORIOS POR JOGADOR ======\n");

    for (int j = 0; j < numJogadores; j++) {
        printf("\n%s:\n", jogadores[j].nome);

        for (int i = 0; i < NUM_CONTINENTES; i++) {
            for (int k = 0; k < PAISES_POR_CONTINENTE; k++) {
                if (mapa[i].paises[k].dono == j) {
                    printf("- %s (%s) | Tropas: %d\n",
                        mapa[i].paises[k].nome,
                        mapa[i].nome,
                        mapa[i].paises[k].tropas);
                }
            }
        }
    }
}

// ================= mapa =================
void mostrarMapa(struct Continente mapa[], struct Jogador jogadores[]) {
    printf("\n========== MAPA GLOBAL ==========\n");

    for (int i = 0; i < NUM_CONTINENTES; i++) {
        printf("\n[%d] %s\n", i, mapa[i].nome);

        for (int j = 0; j < PAISES_POR_CONTINENTE; j++) {
            printf("  (%d) %-15s | %-10s | Tropas: %d\n",
                j,
                mapa[i].paises[j].nome,
                jogadores[mapa[i].paises[j].dono].nome,
                mapa[i].paises[j].tropas);
        }
    }
}

// ================= Territorios de cada jogador =================
void mostrarTerritoriosJogador(struct Continente mapa[], int jogadorAtual) {
    printf("\nSeus territorios:\n");

    for (int i = 0; i < NUM_CONTINENTES; i++) {
        for (int j = 0; j < PAISES_POR_CONTINENTE; j++) {
            if (mapa[i].paises[j].dono == jogadorAtual) {
                printf("- %s (%s) | Tropas: %d\n",
                    mapa[i].paises[j].nome,
                    mapa[i].nome,
                    mapa[i].paises[j].tropas);
            }
        }
    }
}

// ================= Atacar =================
void atacar(struct Continente mapa[], int jogadorAtual) {
    int c1, p1, c2, p2;

    printf("\n--- SELECIONE O ATAQUE ---\n");
    mostrarTerritoriosJogador(mapa, jogadorAtual);

    printf("\nContinente atacante (numero): ");
    scanf("%d", &c1);
    printf("Pais atacante (numero): ");
    scanf("%d", &p1);

    printf("\nContinente defensor (numero): ");
    scanf("%d", &c2);
    printf("Pais defensor (numero): ");
    scanf("%d", &p2);

    if (c1 < 0 || c1 >= NUM_CONTINENTES || c2 < 0 || c2 >= NUM_CONTINENTES ||
        p1 < 0 || p1 >= PAISES_POR_CONTINENTE || p2 < 0 || p2 >= PAISES_POR_CONTINENTE) {
        printf("Escolha invalida!\n");
        return;
    }

    struct Pais *atk = &mapa[c1].paises[p1];
    struct Pais *def = &mapa[c2].paises[p2];

    if (atk->dono != jogadorAtual) {
        printf("Esse pais nao e seu!\n");
        return;
    }

    if (atk->tropas <= 1) {
        printf("Tropas insuficientes!\n");
        return;
    }

    printf("\n%s atacando %s!\n", atk->nome, def->nome);

    int dadosAtk[3], dadosDef[2];
    int nAtk = (atk->tropas >= 3) ? 3 : atk->tropas - 1;
    int nDef = (def->tropas >= 2) ? 2 : 1;

    for (int i = 0; i < nAtk; i++) dadosAtk[i] = rand() % 6 + 1;
    for (int i = 0; i < nDef; i++) dadosDef[i] = rand() % 6 + 1;

    // ordenar
    for (int i = 0; i < nAtk - 1; i++)
        for (int j = i + 1; j < nAtk; j++)
            if (dadosAtk[i] < dadosAtk[j]) {
                int t = dadosAtk[i]; dadosAtk[i] = dadosAtk[j]; dadosAtk[j] = t;
            }

    for (int i = 0; i < nDef - 1; i++)
        for (int j = i + 1; j < nDef; j++)
            if (dadosDef[i] < dadosDef[j]) {
                int t = dadosDef[i]; dadosDef[i] = dadosDef[j]; dadosDef[j] = t;
            }

    printf("\nDados Atacante: ");
    for (int i = 0; i < nAtk; i++) printf("%d ", dadosAtk[i]);

    printf("\nDados Defensor: ");
    for (int i = 0; i < nDef; i++) printf("%d ", dadosDef[i]);
    printf("\n");

    int comp = (nAtk < nDef) ? nAtk : nDef;

    for (int i = 0; i < comp; i++) {
        if (dadosAtk[i] > dadosDef[i]) {
            def->tropas--;
            printf("Defensor perdeu 1 tropa\n");
        } else {
            atk->tropas--;
            printf("Atacante perdeu 1 tropa\n");
        }
    }

    if (def->tropas <= 0) {
        printf("\n🏆 Territorio conquistado!\n");

        def->dono = jogadorAtual;
        def->tropas = 1;
        atk->tropas--;
    }
}

// ================= main =================
int main() {
    srand(time(NULL));

    struct Continente mapa[NUM_CONTINENTES] = {
        {"America"}, {"Europa"}, {"Asia"}, {"Africa"}, {"Oceania"}
    };

    for (int i = 0; i < NUM_CONTINENTES; i++) {
        for (int j = 0; j < PAISES_POR_CONTINENTE; j++) {
            strcpy(mapa[i].paises[j].nome, nomesPaises[i][j]);
        }
    }

    int numJogadores;

    do {
        printf("Quantos jogadores? (2 a 5): ");
        scanf("%d", &numJogadores);
    } while (numJogadores < 2 || numJogadores > MAX_JOGADORES);

    struct Jogador jogadores[MAX_JOGADORES];

    for (int i = 0; i < numJogadores; i++) {
        printf("Nome do jogador %d: ", i);
        scanf("%s", jogadores[i].nome);
    }

    int totalPaises = NUM_CONTINENTES * PAISES_POR_CONTINENTE;
    int donos[25];

    for (int i = 0; i < totalPaises; i++)
        donos[i] = i % numJogadores;

    for (int i = totalPaises - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = donos[i];
        donos[i] = donos[j];
        donos[j] = temp;
    }

    int index = 0;

    for (int i = 0; i < NUM_CONTINENTES; i++) {
        for (int j = 0; j < PAISES_POR_CONTINENTE; j++) {
            mapa[i].paises[j].dono = donos[index++];
            mapa[i].paises[j].tropas = 3;
        }
    }

    int turno = 0;

    while (1) {
        int jogadorAtual = turno % numJogadores;

        printf("\n========================\n");
        printf("VEZ DE %s\n", jogadores[jogadorAtual].nome);
        printf("========================\n");

        mostrarPorJogador(mapa, jogadores, numJogadores);
        mostrarMapa(mapa, jogadores);

        printf("\n1 - Atacar\n0 - Pular\n");
        int op;
        scanf("%d", &op);

        if (op == 1)
            atacar(mapa, jogadorAtual);

        int dono = mapa[0].paises[0].dono;
        int venceu = 1;

        for (int i = 0; i < NUM_CONTINENTES; i++) {
            for (int j = 0; j < PAISES_POR_CONTINENTE; j++) {
                if (mapa[i].paises[j].dono != dono) {
                    venceu = 0;
                }
            }
        }

        if (venceu) {
            printf("\n🏆 %s VENCEU O JOGO!\n", jogadores[dono].nome);
            break;
        }

        turno++;
    }

    return 0;
}
