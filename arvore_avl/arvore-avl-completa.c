#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int valor;
    int altura;
    struct Node* esquerda;
    struct Node* direita;
} Node;

// Retorna a altura do nó
int altura(Node* no) {
    return (no == NULL) ? -1 : no->altura;
}

int max(int a, int b) {
    return (a > b) ? a : b;
}

// Cria um novo nó
Node* novoNo(int valor) {
    Node* no = (Node*)malloc(sizeof(Node));
    no->valor = valor;
    no->esquerda = NULL;
    no->direita = NULL;
    no->altura = 0; // Altura inicial de um nó folha
    return no;
}

// Calcula o fator de balanceamento
int fatorBalanceamento(Node* no) {
    return (no == NULL) ? 0 : altura(no->esquerda) - altura(no->direita);
}

// Rotação simples à direita
Node* rotacaoDireita(Node* y) {
    Node* x = y->esquerda;
    Node* T2 = x->direita;

    // Realiza a rotação
    x->direita = y;
    y->esquerda = T2;

    // Atualiza as alturas
    y->altura = max(altura(y->esquerda), altura(y->direita)) + 1;
    x->altura = max(altura(x->esquerda), altura(x->direita)) + 1;

    return x;
}

// Rotação simples à esquerda
Node* rotacaoEsquerda(Node* x) {
    Node* y = x->direita;
    Node* T2 = y->esquerda;

    // Realiza a rotação
    y->esquerda = x;
    x->direita = T2;

    // Atualiza as alturas
    x->altura = max(altura(x->esquerda), altura(x->direita)) + 1;
    y->altura = max(altura(y->esquerda), altura(y->direita)) + 1;

    return y;
}

// Inserção com balanceamento AVL
Node* inserir(Node* no, int valor) {
    if (no == NULL)
        return novoNo(valor);

    if (valor < no->valor)
        no->esquerda = inserir(no->esquerda, valor);
    else if (valor > no->valor)
        no->direita = inserir(no->direita, valor);
    else // Valores iguais não são permitidos em uma BST/AVL típica
        return no;

    // Atualiza a altura do nó atual
    no->altura = 1 + max(altura(no->esquerda), altura(no->direita));

    // Obtém o fator de balanceamento para verificar se o nó ficou desbalanceado
    int fb = fatorBalanceamento(no);

    // Casos de Rotação
    // Rotação Simples Direita (LL Case)
    if (fb > 1 && valor < no->esquerda->valor)
        return rotacaoDireita(no);

    // Rotação Simples Esquerda (RR Case)
    if (fb < -1 && valor > no->direita->valor)
        return rotacaoEsquerda(no);

    // Rotação Dupla Direita (LR Case)
    if (fb > 1 && valor > no->esquerda->valor) {
        no->esquerda = rotacaoEsquerda(no->esquerda);
        return rotacaoDireita(no);
    }

    // Rotação Dupla Esquerda (RL Case)
    if (fb < -1 && valor < no->direita->valor) {
        no->direita = rotacaoDireita(no->direita);
        return rotacaoEsquerda(no);
    }

    return no;
}

// Encontra o nó com o menor valor na subárvore (usado para remoção)
Node* encontrarMinimo(Node* no) {
    Node* atual = no;
    // Percorre até o nó mais à esquerda
    while (atual->esquerda != NULL)
        atual = atual->esquerda;
    return atual;
}

// Remove um valor da árvore e reequilibra
Node* remover(Node* raiz, int valor) {
    // Caso base: a árvore está vazia ou o valor não foi encontrado
    if (raiz == NULL)
        return raiz;

    // Se o valor a ser removido é menor que o valor da raiz, vá para a subárvore esquerda
    if (valor < raiz->valor)
        raiz->esquerda = remover(raiz->esquerda, valor);
    // Se o valor a ser removido é maior que o valor da raiz, vá para a subárvore direita
    else if (valor > raiz->valor)
        raiz->direita = remover(raiz->direita, valor);
    // Se o valor é o mesmo que o valor da raiz, então este é o nó a ser removido
    else {
        // Nó com apenas um filho ou sem filhos
        if (raiz->esquerda == NULL || raiz->direita == NULL) {
            Node* temp = (raiz->esquerda) ? raiz->esquerda : raiz->direita;

            // Sem filhos
            if (temp == NULL) {
                temp = raiz;
                raiz = NULL;
            } else // Um filho
                *raiz = *temp; // Copia o conteúdo do filho não-nulo para o nó atual
            free(temp); // Libera o nó temporário
        } else {
            // Nó com dois filhos: Obtém o sucessor in-ordem (menor na subárvore direita)
            Node* temp = encontrarMinimo(raiz->direita);

            // Copia o valor do sucessor in-ordem para este nó
            raiz->valor = temp->valor;

            // Remove o sucessor in-ordem
            raiz->direita = remover(raiz->direita, temp->valor);
        }
    }

    // Se a árvore tinha apenas um nó, então ele se tornou NULL
    if (raiz == NULL)
        return NULL;

    // Atualiza a altura do nó atual
    raiz->altura = 1 + max(altura(raiz->esquerda), altura(raiz->direita));

    // Obtém o fator de balanceamento
    int fb = fatorBalanceamento(raiz);

    // Casos de Rotação após a remoção
    // Rotação Simples Direita (LL Case)
    if (fb > 1 && fatorBalanceamento(raiz->esquerda) >= 0)
        return rotacaoDireita(raiz);

    // Rotação Dupla Direita (LR Case)
    if (fb > 1 && fatorBalanceamento(raiz->esquerda) < 0) {
        raiz->esquerda = rotacaoEsquerda(raiz->esquerda);
        return rotacaoDireita(raiz);
    }

    // Rotação Simples Esquerda (RR Case)
    if (fb < -1 && fatorBalanceamento(raiz->direita) <= 0)
        return rotacaoEsquerda(raiz);

    // Rotação Dupla Esquerda (RL Case)
    if (fb < -1 && fatorBalanceamento(raiz->direita) > 0) {
        raiz->direita = rotacaoDireita(raiz->direita);
        return rotacaoEsquerda(raiz);
    }

    return raiz;
}

// Impressão in-ordem
void imprimirInOrdem(Node* raiz) {
    if (raiz != NULL) {
        imprimirInOrdem(raiz->esquerda);
        printf("%d ", raiz->valor);
        imprimirInOrdem(raiz->direita);
    }
}

// Imprimir fatores de balanceamento
void imprimirFatores(Node* raiz) {
    if (raiz != NULL) {
        imprimirFatores(raiz->esquerda);
        printf("No %d: FB = %d, Altura = %d\n", raiz->valor, fatorBalanceamento(raiz), raiz->altura);
        imprimirFatores(raiz->direita);
    }
}

// Altura da árvore (retorna a altura da raiz)
int calcularAlturaArvore(Node* raiz) {
    return altura(raiz);
}

// Função auxiliar para demonstrar as operações da AVL
void demonstrar(const char* titulo, int valores_inclusao[], int tam_inclusao, int valor_remover) {
    printf("\n--- %s ---\n", titulo);
    Node* raiz = NULL;

    printf("Inserindo valores: ");
    for (int i = 0; i < tam_inclusao; i++) {
        printf("%d ", valores_inclusao[i]);
        raiz = inserir(raiz, valores_inclusao[i]);
    }
    printf("\n");

    printf("Estado da arvore apos inclusao:\n");
    printf("  In-ordem: ");
    imprimirInOrdem(raiz);
    printf("\n  Fatores de balanceamento e Alturas dos nos:\n");
    imprimirFatores(raiz);
    printf("  Altura da arvore: %d\n", calcularAlturaArvore(raiz));

    if (valor_remover != -1) {
        printf("\nRemovendo o valor %d...\n", valor_remover);
        raiz = remover(raiz, valor_remover);
        printf("Estado da arvore apos remocao:\n");
        printf("  In-ordem: ");
        imprimirInOrdem(raiz);
        printf("\n  Fatores de balanceamento e Alturas dos nos:\n");
        imprimirFatores(raiz);
        printf("  Altura da arvore: %d\n", calcularAlturaArvore(raiz));
    }
    printf("----------------------------------\n");
}

// Função principal
int main() {
    // --- CASOS DE INCLUSÃO ---

    // Caso 1: Rotação Simples Direita (LL) na Inclusão
    int inc_rsd[] = {30, 20, 10}; // Inserção de 10 desequilibra em 30 (fb=2), rotação em 30
    demonstrar("Caso 1 - Rotacao Simples Direita (Inclusao)", inc_rsd, 3, -1); // -1 indica sem remoção

    // Caso 1: Rotação Simples Esquerda (RR) na Inclusão
    int inc_rse[] = {10, 20, 30}; // Inserção de 30 desequilibra em 10 (fb=-2), rotação em 10
    demonstrar("Caso 1 - Rotacao Simples Esquerda (Inclusao)", inc_rse, 3, -1);

    // Caso 2: Rotação Dupla Direita (LR) na Inclusão
    int inc_rdd[] = {30, 10, 20}; // Inserção de 20 desequilibra em 30 (fb=2), rotação dupla (LR)
    demonstrar("Caso 2 - Rotacao Dupla Direita (Inclusao)", inc_rdd, 3, -1);

    // Caso 2: Rotação Dupla Esquerda (RL) na Inclusão
    int inc_rde[] = {10, 30, 20}; // Inserção de 20 desequilibra em 10 (fb=-2), rotação dupla (RL)
    demonstrar("Caso 2 - Rotacao Dupla Esquerda (Inclusao)", inc_rde, 3, -1);

    // --- CASOS DE REMOÇÃO ---

    // Caso 1: Rotação Simples Direita (LL) na Remoção
    // Exemplo: Remover 70. 50 (fb= -2) fica desbalanceado, 40 (fb= -1), 30 fica com fb= -1. Precisamos de um que desbalanceie para a esquerda.
    // Sequência: 50, 20, 60, 10, 30, 70 (Árvore balanceada). Remover 70. O 60 é removido. 50 FB=2, no esquerdo 20 tem FB=0. Rotação Simples Direita.
    int rem_rsd_val[] = {50, 20, 60, 10, 30, 70};
    demonstrar("Caso 1 - Rotacao Simples Direita (Remocao)", rem_rsd_val, 6, 70); // Remover 70 para forçar RSD

    // Caso 1: Rotação Simples Esquerda (RR) na Remoção
    // Exemplo: Remover 10. 50 (fb=2) fica desbalanceado, 60 (fb=1). Rotação Simples Esquerda.
    int rem_rse_val[] = {50, 20, 60, 10, 30, 70};
    demonstrar("Caso 1 - Rotacao Simples Esquerda (Remocao)", rem_rse_val, 6, 10); // Remover 10 para forçar RSE

    // Caso 2: Rotação Dupla Direita (LR) na Remoção
    // Exemplo: Remover 80. A remoção de 80 desbalanceia 50 (fb=2). O filho esquerdo 20 tem fb=-1 (subárvore direita maior). Força LR.
    int rem_rdd_val[] = {50, 20, 70, 10, 30, 60, 80};
    demonstrar("Caso 2 - Rotacao Dupla Direita (Remocao)", rem_rdd_val, 7, 80); // Remover 80 para forçar RDD (LR)

    // Caso 2: Rotação Dupla Esquerda (RL) na Remoção
    // Exemplo: Remover 10. A remoção de 10 desbalanceia 50 (fb=-2). O filho direito 70 tem fb=1 (subárvore esquerda maior). Força RL.
    int rem_rde_val[] = {50, 20, 70, 10, 30, 60, 80};
    demonstrar("Caso 2 - Rotacao Dupla Esquerda (Remocao)", rem_rde_val, 7, 10); // Remover 10 para forçar RDE (RL)

    return 0;
}