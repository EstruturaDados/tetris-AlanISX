#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define CAPACIDADE 5

// Struct Peca com os campos: tipo (char) e id (int)
typedef struct {
    char tipo;
    int id;
} Peca;

// Estrutura da fila circular
typedef struct {
    Peca itens[CAPACIDADE];
    int frente;
    int tras;
    int tamanho;
} FilaCircular;

// Controle do ID sequencial das peças
int proximo_id = 0;

// Inicializa a fila definindo os índices e tamanho iniciais
void inicializarFila(FilaCircular *f) {
    f->frente = 0;
    f->tras = -1;
    f->tamanho = 0;
}

// Verifica se a fila está cheia
int filaCheia(FilaCircular *f) {
    return f->tamanho == CAPACIDADE;
}

// Verifica se a fila está vazia
int filaVazia(FilaCircular *f) {
    return f->tamanho == 0;
}

// Gera automaticamente uma peça com tipo aleatório e id sequencial
Peca gerarPeca() {
    char tipos[] = {'I', 'O', 'T', 'L'};
    Peca nova;
    nova.tipo = tipos[rand() % 4]; // Escolha aleatória do tipo
    nova.id = proximo_id;          // ID sequencial
    proximo_id++;
    return nova;
}

// Insere uma nova peça ao final da fila circular (enqueue)
void enqueue(FilaCircular *f) {
    if (!filaCheia(f)) {
        f->tras = (f->tras + 1) % CAPACIDADE;
        f->itens[f->tras] = gerarPeca();
        f->tamanho++;
    } else {
        printf("Fila cheia! Nao e possivel inserir nova peca.\n\n");
    }
}

// Remove a peça da frente da fila circular (dequeue)
void dequeue(FilaCircular *f) {
    if (!filaVazia(f)) {
        f->frente = (f->frente + 1) % CAPACIDADE;
        f->tamanho--;
    } else {
        printf("Fila vazia! Nao ha pecas para jogar.\n\n");
    }
}

// Exibe o estado atual da fila de peças
void mostrarFila(FilaCircular *f) {
    printf("Fila de Pecas: ");
    for (int i = 0; i < f->tamanho; i++) {
        int indice = (f->frente + i) % CAPACIDADE;
        printf("[%c %d] ", f->itens[indice].tipo, f->itens[indice].id);
    }
    printf("\n\n");
}

int main() {
    // Semente para a geração de tipos aleatórios
    srand(time(NULL));

    FilaCircular fila;
    inicializarFila(&fila);

    // Inicializa a fila preenchendo o espaço fixo de 5 peças
    for (int i = 0; i < CAPACIDADE; i++) {
        // Enfileira usando uma lógica interna direta sem exibir mensagens de erro na carga inicial
        fila.tras = (fila.tras + 1) % CAPACIDADE;
        fila.itens[fila.tras] = gerarPeca();
        fila.tamanho++;
    }

    int opcao = -1;

    // Loop do menu com todas as opções da imagem
    while (opcao != 0) {
        mostrarFila(&fila);

        printf("1 - Jogar Peça (dequeue)\n");
        printf("2 - Inserir Nova Peça (enqueue)\n");
        printf("0 - Sair\n");
        printf("\nEscolha uma opção: ");
        scanf("%d", &opcao);
        printf("\n");

        if (opcao == 1) {
            dequeue(&fila); // Remove a peça da frente
        } 
        else if (opcao == 2) {
            enqueue(&fila); // Insere uma nova peça no fim
        }
    }

    return 0;
}