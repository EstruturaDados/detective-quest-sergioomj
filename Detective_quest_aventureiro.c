#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Estrutura para um nó da árvore de pistas (BST)
typedef struct PistaNode {
    char descricao[100];        // Descrição da pista
    struct PistaNode* esquerda; // Subárvore esquerda
    struct PistaNode* direita;  // Subárvore direita
} PistaNode;

// Estrutura para uma sala da mansão
typedef struct Sala {
    char nome[50];              // Nome da sala
    char pista[100];            // Pista encontrada na sala (vazia se não houver)
    struct Sala* esquerda;      // Sala à esquerda
    struct Sala* direita;       // Sala à direita
} Sala;

// Função para criar um novo nó de pista
PistaNode* criarNoPista(const char* descricao) {
    PistaNode* novoNo = (PistaNode*)malloc(sizeof(PistaNode));
    if (novoNo == NULL) {
        printf("Erro ao alocar memória para pista!\n");
        exit(1);
    }
    
    strcpy(novoNo->descricao, descricao);
    novoNo->esquerda = NULL;
    novoNo->direita = NULL;
    
    return novoNo;
}

// Função para inserir uma pista na BST (em ordem alfabética)
PistaNode* inserirPista(PistaNode* raiz, const char* descricao) {
    // Se a árvore estiver vazia, cria o nó raiz
    if (raiz == NULL) {
        return criarNoPista(descricao);
    }
    
    // Comparação alfabética para determinar onde inserir
    int comparacao = strcmp(descricao, raiz->descricao);
    
    if (comparacao < 0) {
        // Inserir na subárvore esquerda
        raiz->esquerda = inserirPista(raiz->esquerda, descricao);
    } else if (comparacao > 0) {
        // Inserir na subárvore direita
        raiz->direita = inserirPista(raiz->direita, descricao);
    }
    // Se a pista já existe (comparacao == 0), não insere duplicata
    
    return raiz;
}

// Função para exibir todas as pistas em ordem alfabética (percurso in-ordem)
void exibirPistas(PistaNode* raiz) {
    if (raiz == NULL) {
        return;
    }
    
    // Percurso in-ordem: esquerda -> raiz -> direita
    exibirPistas(raiz->esquerda);
    printf("  • %s\n", raiz->descricao);
    exibirPistas(raiz->direita);
}

// Função para liberar a memória da árvore de pistas
void liberarArvorePistas(PistaNode* raiz) {
    if (raiz == NULL) {
        return;
    }
    
    liberarArvorePistas(raiz->esquerda);
    liberarArvorePistas(raiz->direita);
    free(raiz);
}

// Função para criar uma nova sala dinamicamente
Sala* criarSala(const char* nome, const char* pista) {
    Sala* novaSala = (Sala*)malloc(sizeof(Sala));
    if (novaSala == NULL) {
        printf("Erro ao alocar memória para sala!\n");
        exit(1);
    }
    
    strcpy(novaSala->nome, nome);
    
    if (pista != NULL) {
        strcpy(novaSala->pista, pista);
    } else {
        novaSala->pista[0] = '\0';  // String vazia para sala sem pista
    }
    
    novaSala->esquerda = NULL;
    novaSala->direita = NULL;
    
    return novaSala;
}

// Função para liberar toda a memória da árvore de salas
void liberarArvoreSalas(Sala* raiz) {
    if (raiz == NULL) {
        return;
    }
    
    liberarArvoreSalas(raiz->esquerda);
    liberarArvoreSalas(raiz->direita);
    free(raiz);
}

// Função auxiliar para contar pistas
int contarPistas(PistaNode* raiz) {
    if (raiz == NULL) {
        return 0;
    }
    return 1 + contarPistas(raiz->esquerda) + contarPistas(raiz->direita);
}

// Função para explorar as salas e coletar pistas
void explorarSalasComPistas(Sala* salaAtual, PistaNode** pistasRaiz) {
    if (salaAtual == NULL) {
        return;
    }
    
    printf("\n========================================\n");
    printf("Você está na: %s\n", salaAtual->nome);
    
    // Verifica se há pista nesta sala
    if (strlen(salaAtual->pista) > 0) {
        printf("\n🔍 PISTA ENCONTRADA: \"%s\"\n", salaAtual->pista);
        
        // Adiciona a pista à BST
        *pistasRaiz = inserirPista(*pistasRaiz, salaAtual->pista);
        
        printf("Pista adicionada ao seu diário de investigação!\n");
    } else {
        printf("Não há pistas visíveis nesta sala...\n");
    }
    
    // Verifica se é uma sala terminal (sem saídas)
    if (salaAtual->esquerda == NULL && salaAtual->direita == NULL) {
        printf("\n  Este é um cômodo sem saída!\n");
    }
    
    // Mostra as opções disponíveis
    printf("\nOpções disponíveis:\n");
    if (salaAtual->esquerda != NULL) {
        printf("  [E] Esquerda → %s\n", salaAtual->esquerda->nome);
    }
    if (salaAtual->direita != NULL) {
        printf("  [D] Direita → %s\n", salaAtual->direita->nome);
    }
    printf("  [S] Sair da exploração\n");
    printf("  [V] Ver pistas coletadas até agora\n");
    
    // Loop para obter uma escolha válida
    char escolha;
    do {
        printf("\nPara onde deseja ir? ");
        scanf(" %c", &escolha);
        escolha = tolower(escolha);
        
        if (escolha == 'e' && salaAtual->esquerda != NULL) {
            explorarSalasComPistas(salaAtual->esquerda, pistasRaiz);
            break;
        } else if (escolha == 'd' && salaAtual->direita != NULL) {
            explorarSalasComPistas(salaAtual->direita, pistasRaiz);
            break;
        } else if (escolha == 'v') {
            // Mostra pistas coletadas até o momento
            int totalPistas = contarPistas(*pistasRaiz);
            printf("\n PISTAS COLETADAS ATÉ AGORA (%d no total):\n", totalPistas);
            
            if (totalPistas == 0) {
                printf("Nenhuma pista coletada ainda...\n");
            } else {
                exibirPistas(*pistasRaiz);
            }
            // Volta para o menu atual
            explorarSalasComPistas(salaAtual, pistasRaiz);
            break;
        } else if (escolha == 's') {
            printf("Finalizando exploração...\n");
            return;
        } else {
            printf("Opção inválida ou caminho bloqueado! Tente novamente.\n");
        }
    } while (1);
}

// Função para montar o mapa da mansão com pistas
Sala* montarMapaComPistas() {
    // Nível 0: Raiz - Hall de entrada
    Sala* hall = criarSala("Hall de Entrada", 
                          "Uma carta anônima no chão: 'Encontre-me antes que seja tarde'");
    
    // Nível 1: Salas conectadas ao Hall
    Sala* salaEstar = criarSala("Sala de Estar", 
                               "Um copo de vinho meio cheio na mesa de centro");
    Sala* corredor = criarSala("Corredor Principal", 
                              "Um relógio de parede parado exatamente às 23:45");
    
    hall->esquerda = salaEstar;
    hall->direita = corredor;
    
    // Nível 2: Salas conectadas à Sala de Estar
    Sala* biblioteca = criarSala("Biblioteca", 
                                "Um livro sobre venenos está aberto na página 45");
    Sala* jardim = criarSala("Jardim de Inverno", 
                            "Marcas de lama fresca no piso de mármore");
    
    salaEstar->esquerda = biblioteca;
    salaEstar->direita = jardim;
    
    // Nível 2: Salas conectadas ao Corredor
    Sala* cozinha = criarSala("Cozinha", 
                             "Uma faca está faltando do jogo de facas na parede");
    Sala* salaJogos = criarSala("Sala de Jogos", 
                               "Um jogo de xadrez com as peças pretas em vantagem");
    
    corredor->esquerda = cozinha;
    corredor->direita = salaJogos;
    
    // Nível 3: Salas finais (folhas)
    biblioteca->esquerda = criarSala("Escritório Secreto", 
                                    "Uma agenda com uma reunião marcada para ontem");
    biblioteca->direita = criarSala("Sala de Leitura", NULL);  // Sem pista
    
    jardim->esquerda = criarSala("Estufa", 
                                "Uma espécie rara de flor está amassada no chão");
    jardim->direita = criarSala("Pérgola", NULL);  // Sem pista
    
    cozinha->esquerda = criarSala("Despensa", 
                                 "Um frasco de remédio com o rótulo rasgado");
    cozinha->direita = criarSala("Lavanderia", 
                                "Uma camisa manchada com uma substância avermelhada");
    
    salaJogos->esquerda = criarSala("Sala de Música", 
                                   "Uma partitura incompleta no piano");
    salaJogos->direita = criarSala("Sala de Bilhar", 
                                  "Um bilhete embaixo da mesa: 'Cuidado com o mordomo'");
    
    return hall;
}

// Função principal
int main() {
    printf("====================================================\n");
    printf("    DETECTIVE QUEST - A Mansão Misteriosa\n");
    printf("                Coleção de Pistas\n");
    printf("====================================================\n");
    printf("Explore a mansão para encontrar pistas do crime!\n");
    printf("Cada pista será automaticamente organizada em ordem alfabética.\n\n");
    printf("COMANDOS DISPONÍVEIS:\n");
    printf("  E - Ir para a sala à esquerda\n");
    printf("  D - Ir para a sala à direita\n");
    printf("  V - Ver pistas coletadas até agora\n");
    printf("  S - Sair da exploração\n");
    printf("====================================================\n\n");
    
    // Monta o mapa da mansão com pistas
    Sala* mansao = montarMapaComPistas();
    
    // Inicializa a árvore de pistas (inicialmente vazia)
    PistaNode* pistasRaiz = NULL;
    
    // Inicia a exploração a partir do Hall de entrada
    explorarSalasComPistas(mansao, &pistasRaiz);
    
    // Exibe todas as pistas coletadas ao final
    printf("\n====================================================\n");
    printf("          RESUMO DA INVESTIGAÇÃO\n");
    printf("====================================================\n");
    
    int totalPistas = contarPistas(pistasRaiz);
    printf("Total de pistas coletadas: %d\n", totalPistas);
    
    if (totalPistas == 0) {
        printf("Nenhuma pista foi encontrada durante a exploração.\n");
    } else {
        printf("\n PISTAS ORGANIZADAS EM ORDEM ALFABÉTICA:\n");
        exibirPistas(pistasRaiz);
        
        printf("\n DICA: Analise as pistas cuidadosamente para resolver o mistério!\n");
    }
    
    printf("====================================================\n");
    
    // Libera a memória alocada
    liberarArvoreSalas(mansao);
    liberarArvorePistas(pistasRaiz);
    
    printf("\nObrigado por jogar Detective Quest - Coleção de Pistas!\n");
    printf("A investigação continua...\n");
    
    return 0;
}