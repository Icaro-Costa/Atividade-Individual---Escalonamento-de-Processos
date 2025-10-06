#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// =================================================================
// 1. FILA SIMPLES (QUEUE)
// =================================================================

typedef struct No {
    int dado;
    struct No *proximo;
} No;

typedef struct Fila {
    No *frente; 
    No *tras;   
} Fila;

Fila* criarFila() {
    Fila *f = (Fila*) malloc(sizeof(Fila));
    f->frente = f->tras = NULL;
    return f;
}

int estaVazia(Fila *f) {
    return f->frente == NULL;
}

void enfileirar(Fila *f, int valor) {
    No *novoNo = (No*) malloc(sizeof(No));
    novoNo->dado = valor;
    novoNo->proximo = NULL;

    if (f->tras == NULL) { 
        f->frente = f->tras = novoNo;
        return;
    }
    f->tras->proximo = novoNo;
    f->tras = novoNo;
}

int desenfileirar(Fila *f) {
    if (estaVazia(f)) {
        printf("ERRO: Fila vazia!\n");
        return -1;
    }
    No *temp = f->frente;
    int valor = temp->dado;
    f->frente = f->frente->proximo;

    if (f->frente == NULL) {
        f->tras = NULL;
    }
    free(temp);
    return valor;
}

void exemploFila() {
    printf("--- 1. Exemplo de Fila (Queue) ---\n");
    Fila *minhaFila = criarFila();

    enfileirar(minhaFila, 10);
    printf("Enfileirado: 10\n");
    enfileirar(minhaFila, 20);
    printf("Enfileirado: 20\n");
    enfileirar(minhaFila, 30);
    printf("Enfileirado: 30\n");

    printf("Desenfileirado: %d\n", desenfileirar(minhaFila)); 
    printf("Desenfileirado: %d\n", desenfileirar(minhaFila)); 

    enfileirar(minhaFila, 40);
    printf("Enfileirado: 40\n");

    printf("Desenfileirado: %d\n", desenfileirar(minhaFila));
    printf("Desenfileirado: %d\n", desenfileirar(minhaFila));
    
    free(minhaFila);
}

// =================================================================
// 2 & 3. FILA DE PRIORIDADE (PRIORITY QUEUE)
// =================================================================

typedef struct NoP {
    int dado;
    int prioridade;
    struct NoP *proximo;
} NoP;

NoP* novoNoP(int dado, int prioridade) {
    NoP *temp = (NoP*) malloc(sizeof(NoP));
    temp->dado = dado;
    temp->prioridade = prioridade;
    temp->proximo = NULL;
    return temp;
}

void desenfileirarP(NoP **frente) {
    if (*frente == NULL) {
        return;
    }
    NoP *temp = *frente;
    *frente = (*frente)->proximo; 
    free(temp);
}

void enfileirarP(NoP **frente, int dado, int prioridade) {
    NoP *temp = novoNoP(dado, prioridade);

    if (*frente == NULL || prioridade < (*frente)->prioridade) {
        temp->proximo = *frente;
        *frente = temp;
    } else {
        NoP *inicio = *frente;
        while (inicio->proximo != NULL && 
               inicio->proximo->prioridade <= prioridade) {
            inicio = inicio->proximo;
        }
        temp->proximo = inicio->proximo;
        inicio->proximo = temp;
    }
}

void simularPrioridade() {
    printf("\n--- 2 & 3. Simulação de Fila de Prioridade ---\n");
    NoP *filaP = NULL; 

    for (int i = 1; i <= 5; i++) enfileirarP(&filaP, 30 + i, 3);
    for (int i = 1; i <= 5; i++) enfileirarP(&filaP, 20 + i, 2);
    for (int i = 1; i <= 5; i++) enfileirarP(&filaP, 10 + i, 1);

    printf("Prioridades (1=Alta, 2=Media, 3=Baixa). Ordem de processamento:\n");
    int contador = 1;
    while (filaP != NULL) {
        printf("%d. Item: %d | Prioridade: %d\n", contador++, filaP->dado, filaP->prioridade);
        desenfileirarP(&filaP);
    }
    printf("Observação: Prioridades mais altas (1) são processadas primeiro.\n");
}

// =================================================================
// 4. SIMULAÇÃO ROUND ROBIN (RR)
// =================================================================

typedef struct Processo {
    char id[3]; 
    int burstTime;
} Processo;

typedef struct NoRR {
    Processo p;
    struct NoRR *proximo;
} NoRR;

typedef struct FilaRR {
    NoRR *frente;
    NoRR *tras;
} FilaRR;

FilaRR* criarFilaRR() {
    FilaRR *f = (FilaRR*) malloc(sizeof(FilaRR));
    f->frente = f->tras = NULL;
    return f;
}

int estaVaziaRR(FilaRR *f) {
    return f->frente == NULL;
}

void enfileirarRR(FilaRR *f, Processo p) {
    NoRR *novoNo = (NoRR*) malloc(sizeof(NoRR));
    novoNo->p = p;
    novoNo->proximo = NULL;

    if (f->tras == NULL) {
        f->frente = f->tras = novoNo;
        return;
    }
    f->tras->proximo = novoNo;
    f->tras = novoNo;
}

Processo desenfileirarRR(FilaRR *f) {
    if (estaVaziaRR(f)) {
        Processo p_erro = {"ER", 0};
        return p_erro;
    }
    NoRR *temp = f->frente;
    Processo p = temp->p;
    f->frente = f->frente->proximo;

    if (f->frente == NULL) {
        f->tras = NULL;
    }
    free(temp);
    return p;
}


void simulacaoRoundRobin() {
    printf("\n--- 4. Simulação Round Robin ---\n");
    FilaRR *fila = criarFilaRR();
    const int QUANTUM = 2;

    Processo p1 = {"P1", 5};
    Processo p2 = {"P2", 7};
    Processo p3 = {"P3", 3};

    enfileirarRR(fila, p1);
    enfileirarRR(fila, p2);
    enfileirarRR(fila, p3);
    
    while (!estaVaziaRR(fila)) {
        Processo atual = desenfileirarRR(fila);
        int tempoExecutado = 0;

        if (atual.burstTime > 0) {
            if (atual.burstTime > QUANTUM) {
                tempoExecutado = QUANTUM;
                atual.burstTime -= QUANTUM;
            } else {
                tempoExecutado = atual.burstTime;
                atual.burstTime = 0;
            }

            printf("%s executou %d unidades (restam %d)\n", 
                   atual.id, tempoExecutado, atual.burstTime);

            if (atual.burstTime > 0) {
                enfileirarRR(fila, atual);
            }
        }
    }

    printf("\nTodos os processos foram concluídos!\n");
    free(fila);
}

// =================================================================
// MAIN
// =================================================================

int main() {
    exemploFila();
    printf("\n-------------------------------------\n");

    simularPrioridade();
    printf("\n-------------------------------------\n");

    simulacaoRoundRobin();
    
    return 0;
}