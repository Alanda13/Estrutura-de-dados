

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct Node {
    int valor;
    struct Node* esq;
    struct Node* dir;
} Node;

// criando um novo nó
Node* novoNo(int valor) {
    Node* no = (Node*)malloc(sizeof(Node));
    no->valor = valor;
    no->esq = no->dir = NULL;
    return no;
}

// Inserção na árvore binária de busca
Node* inserir(Node* raiz, int valor) {
    if (raiz == NULL)
        return novoNo(valor);

    if (valor < raiz->valor)
        raiz->esq = inserir(raiz->esq, valor);
    else
        raiz->dir = inserir(raiz->dir, valor);
    
    return raiz;
}

// Imprimir árvore (pré-ordem com identação)
void imprimir(Node* raiz, int nivel) {
    if (raiz == NULL) return;

    for (int i = 0; i < nivel; i++) printf("   ");
    printf("%d\n", raiz->valor);

    imprimir(raiz->esq, nivel + 1);
    imprimir(raiz->dir, nivel + 1);
}

// Altura da árvore
int altura(Node* raiz) {
    if (raiz == NULL) return -1;

    int alt_esq = altura(raiz->esq);
    int alt_dir = altura(raiz->dir);

    return 1 + (alt_esq > alt_dir ? alt_esq : alt_dir);
}

// Verifica se a árvore está balanceada
bool estaBalanceada(Node* raiz) {
    if (raiz == NULL) return true;

    int alt_esq = altura(raiz->esq);
    int alt_dir = altura(raiz->dir);

    int dif = abs(alt_esq - alt_dir);

    if (dif > 1) return false;

    return estaBalanceada(raiz->esq) && estaBalanceada(raiz->dir);
}

// Armazena os nós em ordem crescente (in-order)
void emOrdem(Node* raiz, int* vetor, int* index) {
    if (raiz != NULL) {
        emOrdem(raiz->esq, vetor, index);
        vetor[(*index)++] = raiz->valor;
        emOrdem(raiz->dir, vetor, index);
    }
}

// Cria uma árvore balanceada a partir de vetor ordenado
Node* construirBalanceada(int* vetor, int inicio, int fim) {
    if (inicio > fim) return NULL;

    int meio = (inicio + fim) / 2;
    Node* raiz = novoNo(vetor[meio]);

    raiz->esq = construirBalanceada(vetor, inicio, meio - 1);
    raiz->dir = construirBalanceada(vetor, meio + 1, fim);

    return raiz;
}

// Função principal
int main() {
    int valores[] = {7, 6, 22, 14, 40, 63};
    int n = sizeof(valores)/sizeof(valores[0]);
    Node* raiz = NULL;

    // Inserção
    for (int i = 0; i < n; i++) {
        raiz = inserir(raiz, valores[i]);
    }

    printf("Árvore original:\n");
    imprimir(raiz, 0);

    int h = altura(raiz);
    printf("\nAltura da árvore: %d\n", h);
    printf("A árvore %s balanceada.\n", estaBalanceada(raiz) ? "está" : "NÃO está");

    // Se não estiver balanceada, balancear
    if (!estaBalanceada(raiz)) {
        int vetorOrdenado[100];
        int index = 0;
        emOrdem(raiz, vetorOrdenado, &index);

        raiz = construirBalanceada(vetorOrdenado, 0, index - 1);

        printf("\nÁrvore balanceada:\n");
        imprimir(raiz, 0);
        printf("\nNova altura: %d\n", altura(raiz));
    }

    return 0;
}
