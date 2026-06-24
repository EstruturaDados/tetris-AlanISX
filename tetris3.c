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

// Exibe o estado interno da fila de peças
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

// Exibe o estado interno da pilha
void mostrarPilha(PilhaReserva *p) {
    printf("Pilha de Reserva (Topo -> Base): ");
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

// --- INÍCIO DAS FUNÇÕES EXCLUSIVAS DA INTERFACE DA IMAGEM ---

// Exibe as estruturas sob o rótulo de Estado Atual
void exibirEstadoAtual(FilaCircular *f, PilhaReserva *p) {
    printf("=== Estado Atual ===\n");
    mostrarFila(f);
    mostrarPilha(p);
}

// Exibe as estruturas sob o rótulo de Novo Estado após ações
void exibirNovoEstado(FilaCircular *f, PilhaReserva *p) {
    printf("=== Novo Estado ===\n");
    mostrarFila(f);
    mostrarPilha(p);
}

// Exibe as opções textuais mapeadas do menu de controle
void exibirMenu() {
    printf("Opcoes:\n");
    printf("1 - Jogar peca da frente da fila\n");
    printf("2 - Enviar peca da fila para reserva (pilha)\n");
    printf("3 - Usar peca da reserva (pilha)\n");
    printf("4 - Trocar peca da frente da fila com o topo da pilha\n");
    printf("5 - Trocar os 3 primeiros da fila com as 3 pecas da pilha\n");
    printf("0 - Sair\n");
    printf("\nOpcao: ");
}
// --- FIM DAS FUNÇÕES EXCLUSIVAS DA INTERFACE ---

// --- INÍCIO DAS NOVAS FUNCIONALIDADES DE TROCA (MÓDULOS DE REGRA DE NEGÓCIO) ---

// Substitui a peça da frente da fila com o topo da pilha (Opção 4)
void trocarPecaAtual(FilaCircular *f, PilhaReserva *p) {
    if (filaVazia(f)) {
        printf("Fila vazia! Nao e possivel realizar a troca.\n\n");
        return;
    }
    if (pilhaVazia(p)) {
        printf("Pilha de reserva vazia! Nao ha peca no topo para trocar.\n\n");
        return;
    }

    Peca aux = f->itens[f->frente];
    f->itens[f->frente] = p->itens[p->topo];
    p->itens[p->topo] = aux;

    printf("Troca realizada entre a peca da frente da fila e o topo da pilha.\n\n");
}

// Alterna as três primeiras peças da fila com as três peças da pilha (Opção 5)
void trocaMultipla(FilaCircular *f, PilhaReserva *p) {
    if (f->tamanho < 3) {
        printf("Fila possui menos de 3 pecas! Operacao cancelada.\n\n");
        return;
    }
    if (p->topo != 2) {
        printf("Pilha de reserva nao possui 3 pecas! Operacao cancelada.\n\n");
        return;
    }

    int idx_fila1 = f->frente;
    int idx_fila2 = (f->frente + 1) % CAPACIDADE;
    int idx_fila3 = (f->frente + 2) % CAPACIDADE;

    // Troca 1: 1º da Fila <-> Topo da Pilha (Índice 2)
    Peca aux1 = f->itens[idx_fila1];
    f->itens[idx_fila1] = p->itens[2];
    p->itens[2] = aux1;

    // Troca 2: 2º da Fila <-> Meio da Pilha (Índice 1)
    Peca aux2 = f->itens[idx_fila2];
    f->itens[idx_fila2] = p->itens[1];
    p->itens[1] = aux2;

    // Troca 3: 3º da Fila <-> Base da Pilha (Índice 0)
    Peca aux3 = f->itens[idx_fila3];
    f->itens[idx_fila3] = p->itens[0];
    p->itens[0] = aux3;

    printf("Troca realizada entre os 3 primeiros da fila e os 3 da pilha.\n\n");
}
// --- FIM DAS NOVAS FUNCIONALIDADES DE TROCA ---

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
        fila.tras = (fila.tras + 1) % CAPACIDADE;
        fila.itens[fila.tras] = gerarPeca();
        fila.tamanho++;
    }

    int opcao = -1;
    int primeira_execucao = 1;

    // Loop do menu adaptado utilizando as novas funções de interface dedicadas
    while (opcao != 0) {
        
        // Chamada das funções modulares de visualização com base no estado do loop
        if (primeira_execucao) {
            exibirEstadoAtual(&fila, &pilha);
            primeira_execucao = 0;
        } else {
            exibirNovoEstado(&fila, &pilha);
        }

        // Renderiza o menu textual idêntico ao modelo fornecido
        exibirMenu();
        scanf("%d", &opcao);
        printf("\n");

        if (opcao == 1) {
            dequeue(&fila); 
            enqueue(&fila); 
        } 
        else if (opcao == 2) {
            reservarPeca(&fila, &pilha); 
        }
        else if (opcao == 3) {
            pop(&pilha); 
        }
        else if (opcao == 4) {
            trocarPecaAtual(&fila, &pilha); 
        }
        else if (opcao == 5) {
            trocaMultipla(&fila, &pilha); 
        }
    }

    return 0;
}