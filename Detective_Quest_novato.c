#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// representacao de uma sala da mansão
typedef struct Sala {
    char nome[50];          // Nome da sala
    struct Sala *esquerda;  // Ponteiro para a sala à esquerda
    struct Sala *direita;   // Ponteiro para a sala à direita
} Sala;

// função para criar uma nova sala dinamicamente
Sala* criarSala(const char* nome) {
    Sala* novaSala = (Sala*)malloc(sizeof(Sala));
    if (novaSala == NULL) {
        printf("Erro ao alocar memória!\n");
        exit(1);
    }
    
    strcpy(novaSala->nome, nome);
    novaSala->esquerda = NULL;
    novaSala->direita = NULL;
    
    return novaSala;
}

// função para liberar toda a memória alocada para a árvore
void liberarArvore(Sala* raiz) {
    if (raiz == NULL) {
        return;
    }
    
    liberarArvore(raiz->esquerda);
    liberarArvore(raiz->direita);
    free(raiz);
}

// função para explorar as salas da mansão
void explorarSalas(Sala* salaAtual) {
    if (salaAtual == NULL) {
        return;
    }
    
    printf("\nVocê está na: %s\n", salaAtual->nome);
    
    // verifica se é uma folha (sem saídas)
    if (salaAtual->esquerda == NULL && salaAtual->direita == NULL) {
        printf("Esta sala não tem mais saídas. Fim da exploração nesta direção.\n");
        return;
    }
    
    // Mostra as opções disponíveis
    printf("Opções disponíveis:\n");
    if (salaAtual->esquerda != NULL) {
        printf("  [E]squerda - %s\n", salaAtual->esquerda->nome);
    }
    if (salaAtual->direita != NULL) {
        printf("  [D]ireita - %s\n", salaAtual->direita->nome);
    }
    printf("  [S]air da exploração\n");
    
    // Loop para obter uma escolha válida
    char escolha;
    do {
        printf("Para onde deseja ir? ");
        scanf(" %c", &escolha);  // O espaço antes de %c ignora whitespace
        escolha = tolower(escolha);
        
        if (escolha == 'e' && salaAtual->esquerda != NULL) {
            explorarSalas(salaAtual->esquerda);
            break;
        } else if (escolha == 'd' && salaAtual->direita != NULL) {
            explorarSalas(salaAtual->direita);
            break;
        } else if (escolha == 's') {
            printf("Saindo da exploração...\n");
            return;
        } else {
            printf("Opção inválida ou caminho bloqueado! Tente novamente.\n");
        }
    } while (1);
}

// Função para montar o mapa da mansão (árvore binária)
Sala* montarMapa() {
    // Nível 0: Raiz - Hall de entrada
    Sala* hall = criarSala("Hall de Entrada");
    
    // Nível 1: Salas conectadas ao Hall
    Sala* salaEstar = criarSala("Sala de Estar");
    Sala* corredor = criarSala("Corredor Principal");
    
    hall->esquerda = salaEstar;
    hall->direita = corredor;
    
    // Nível 2: Salas conectadas à Sala de Estar
    Sala* biblioteca = criarSala("Biblioteca");
    Sala* jardim = criarSala("Jardim de Inverno");
    
    salaEstar->esquerda = biblioteca;
    salaEstar->direita = jardim;
    
    // Nível 2: Salas conectadas ao Corredor
    Sala* cozinha = criarSala("Cozinha");
    Sala* salaJogos = criarSala("Sala de Jogos");
    
    corredor->esquerda = cozinha;
    corredor->direita = salaJogos;
    
    // Nível 3: Salas finais (folhas)
    biblioteca->esquerda = criarSala("Escritório Secreto");
    jardim->direita = criarSala("Estufa");
    
    cozinha->esquerda = criarSala("Despensa");
    cozinha->direita = criarSala("Lavanderia");
    
    salaJogos->esquerda = criarSala("Sala de Música");
    salaJogos->direita = criarSala("Sala de Bilhar");
    
    return hall;
}

// Função principal
int main() {
    printf("========================================\n");
    printf("    DETECTIVE QUEST - A Mansão Misteriosa\n");
    printf("========================================\n");
    printf("Explore a mansão para encontrar pistas!\n");
    printf("Em cada sala, escolha uma direção:\n");
    printf("  E - ir para a esquerda\n");
    printf("  D - ir para a direita\n");
    printf("  S - sair da exploração\n");
    printf("========================================\n\n");
    
    // Monta o mapa da mansão como uma árvore binária
    Sala* raiz = montarMapa();
    
    // Inicia a exploração a partir do Hall de entrada
    explorarSalas(raiz);
    
    // Libera a memória alocada para a árvore
    liberarArvore(raiz);
    
    printf("\n========================================\n");
    printf("Obrigado por jogar Detective Quest!\n");
    printf("Volte sempre para continuar a investigação!\n");
    printf("========================================\n");
    
    return 0;
}