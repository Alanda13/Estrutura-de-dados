#ifndef ARVORES_H
#define ARVORES_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ORDEM 3 // Ordem da árvore B e B+

// Estrutura do Cliente
typedef struct {
    int codigo;
    char nome[50];
} Cliente;

// --- Funções de Manipulação de Arquivo ---
void salvarCliente(Cliente c);
Cliente buscarClientePorRRN(int rrn);
int getProximoRRN(); // Obtém o próximo RRN disponível para um novo cliente

// --- Estruturas da Árvore B+ ---
// Nó da Árvore B+
typedef struct NoBMais {
    int chaves[ORDEM - 1]; // Chaves/Códigos dos clientes
    int rrns[ORDEM - 1];   // RRNs correspondentes aos clientes (apenas nas folhas)
    struct NoBMais *filhos[ORDEM]; // Ponteiros para nós filhos
    struct NoBMais *proxFolha;     // Ponteiro para a próxima folha (apenas em nós folha)
    int numChaves;                 // Quantidade de chaves no nó
    int folha;                     // 1 se for nó folha, 0 se for nó interno
} NoBMais;

// Resultado de uma inserção que pode causar divisão
typedef struct Promocao {
    int chavePromovida;
    NoBMais *novoNoFilho;
    int houvePromocao; // Flag para indicar se houve uma promoção de chave
} Promocao;

// --- Funções da Árvore B+ ---
NoBMais* criarNoBMais(int folha);
// Função auxiliar para inserir em um nó que não está cheio
void inserirNaoCheioBMais(NoBMais *no, int chave, int rrn);
// Função que lida com a divisão de um nó
Promocao* dividirNoBMais(NoBMais *no);
// Função recursiva para inserção principal
void inserirBMaisRec(NoBMais *no, int chave, int rrn, Promocao **promocao);
// Função wrapper para inserção na árvore B+
void inserirBMais(NoBMais **raiz, int chave, int rrn);
// Função para buscar um cliente na árvore B+
int buscarBMais(NoBMais *no, int chave);
// Função para percorrer e imprimir as chaves em ordem
void buscaOrdenadaBMais(NoBMais *raiz);
// Função para imprimir a estrutura da árvore (para visualização)
void imprimirArvoreBMais(NoBMais *no, int nivel);

#endif