#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define CAPACIDADE 5
#define CAPACIDADE_PILHA 3 // Requisito: Pilha com capacidade limitada para 3 peças

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

// --- INÍCIO DA ESTRUTURA DA PILHA DE RESERVA (ISOLADA) ---
typedef struct {
    Peca itens[CAPACIDADE_PILHA];
    int topo;
} PilhaReserva;
// --- FIM DA ESTRUTURA DA PILHA DE RESERVA ---

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
    printf("\n");
}

// --- INÍCIO DAS NOVAS FUNÇÕES DA PILHA (ISOLADAS) ---

// Inicializa a pilha definindo o topo como -1 (vazia)
void inicializarPilha(PilhaReserva *p) {
    p->topo = -1;
}

// Verifica se a pilha está cheia
int pilhaCheia(PilhaReserva *p) {
    return p->topo == CAPACIDADE_PILHA - 1;
}

// Verifica se a pilha está vazia
int pilhaVazia(PilhaReserva *p) {
    return p->topo == -1;
}

// Insere uma peça no topo da pilha (push)
void push(PilhaReserva *p, Peca peca) {
    if (!pilhaCheia(p)) {
        p->topo++;
        p->itens[p->topo] = peca;
    } else {
        printf("Pilha cheia! Nao e possivel reservar mais pecas.\n\n");
    }
}

// Remove e retorna a peça do topo da pilha (pop)
Peca pop(PilhaReserva *p) {
    Peca pecaVazia = {' ', -1};
    if (!pilhaVazia(p)) {
        Peca peca = p->itens[p->topo];
        p->topo--;
        return peca;
    } else {
        printf("Pilha vazia! Nao ha pecas na reserva para usar.\n\n");
        return pecaVazia;
    }
}

// Exibe o estado atual da pilha do topo para a base
void mostrarPilha(PilhaReserva *p) {
    printf("Pilha de reserva (Topo -> Base): ");
    if (pilhaVazia(p)) {
        printf("(Vazia)");
    } else {
        for (int i = p->topo; i >= 0; i--) {
            printf("[%c %d] ", p->itens[i].tipo, p->itens[i].id);
        }
    }
    printf("\n\n");
}

// Move a peça da frente da fila para o topo da pilha de reserva
void reservarPeca(FilaCircular *f, PilhaReserva *p) {
    if (filaVazia(f)) {
        printf("Fila vazia! Nao ha peca para reservar.\n\n");
        return;
    }
    if (pilhaCheia(p)) {
        printf("Pilha de reserva cheia! Nao e possivel mover.\n\n");
        return;
    }

    // Captura a peça que está na frente da fila
    Peca pecaParaReservar = f->itens[f->frente];
    
    // Remove da fila (dequeue manual adaptado do original para não imprimir erro)
    f->frente = (f->frente + 1) % CAPACIDADE;
    f->tamanho--;

    // Insere na pilha
    push(p, pecaParaReservar);

    // Repõe automaticamente uma nova peça no final da fila para mantê-la com 5
    enqueue(f);
}

// --- FIM DAS NOVAS FUNÇÕES DA PILHA ---

int main() {
    // Semente para a geração de tipos aleatórios
    srand(time(NULL));

    FilaCircular fila;
    inicializarFila(&fila);

    // Inicialização da nova Pilha de Reserva
    PilhaReserva pilha;
    inicializarPilha(&pilha);

    // Inicializa a fila preenchendo o espaço fixo de 5 peças
    for (int i = 0; i < CAPACIDADE; i++) {
        // Enfileira usando uma lógica interna direta sem exibir mensagens de erro na carga inicial
        fila.tras = (fila.tras + 1) % CAPACIDADE;
        fila.itens[fila.tras] = gerarPeca();
        fila.tamanho++;
    }

    int opcao = -1;

    // Loop do menu adaptado para os novos requisitos de Tetris Stack
    while (opcao != 0) {
        // Exibição do estado atual de ambas estruturas
        mostrarFila(&fila);
        mostrarPilha(&pilha);

        printf("1 - Jogar Peca\n");
        printf("2 - Reservar Peca\n");
        printf("3 - Usar Peca Reservada\n");
        printf("0 - Sair\n");
        printf("\nEscolha uma opcao: ");
        scanf("%d", &opcao);
        printf("\n");

        if (opcao == 1) {
            dequeue(&fila); // Remove a peça da frente
            enqueue(&fila); // Requisito: A cada ação, repõe e mantém a fila sempre cheia
        } 
        else if (opcao == 2) {
            reservarPeca(&fila, &pilha); // Move da fila para a reserva e repõe automaticamente
        }
        else if (opcao == 3) {
            pop(&pilha); // Remove a peça do topo da pilha, simulando seu uso
        }
    }

    return 0;
}