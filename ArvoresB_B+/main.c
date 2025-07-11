#include "arvores.h"

// Função auxiliar para limpar o buffer de entrada
void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int main() {
    // Inicializa a raiz da Árvore B+ como um nó folha
    // Se ela se dividir, a função inserirBMais criará uma nova raiz interna.
    NoBMais *raizBMais = criarNoBMais(1); 

    int opcao;
    do {
        printf("\n--- Menu do Sistema de Clientes (Arvore B+) ---\n");
        printf("1. Inserir cliente\n");
        printf("2. Buscar cliente por codigo\n");
        printf("3. Listar todos os clientes (Busca Ordenada)\n");
        printf("4. Visualizar estrutura da Arvore B+\n");
        printf("0. Sair\n");
        printf("Escolha uma opcao: ");
        
        int res = scanf("%d", &opcao);
        limparBuffer(); // Limpa o buffer após a leitura do inteiro

        if (res != 1) {
            printf("Opcao invalida! Digite um numero.\n");
            continue;
        }

        switch (opcao) {
            case 1: { // Inserir cliente
                Cliente c;
                int res_codigo;

                do {
                    printf("Digite o codigo do cliente: ");
                    res_codigo = scanf("%d", &c.codigo);
                    limparBuffer();
                    if (res_codigo != 1) {
                        printf("Codigo invalido! Digite um numero.\n");
                    }
                } while (res_codigo != 1);

                // Verifica se o código já existe na árvore
                if (buscarBMais(raizBMais, c.codigo) != -1) {
                    printf("Erro: Cliente com codigo %d ja existe!\n", c.codigo);
                    break; // Sai do switch
                }

                printf("Digite o nome do cliente: ");
                fgets(c.nome, sizeof(c.nome), stdin);
                c.nome[strcspn(c.nome, "\n")] = 0; // Remove o '\n' do final

                // Salva o cliente no arquivo e obtém o RRN
                int rrn = getProximoRRN();
                salvarCliente(c);

                // Insere o código e RRN na árvore B+
                inserirBMais(&raizBMais, c.codigo, rrn);
                printf("Cliente %s (Codigo: %d) inserido com sucesso! RRN: %d\n", c.nome, c.codigo, rrn);
                break;
            }
            case 2: { // Buscar cliente por código
                int codigo, res_codigo;
                do {
                    printf("Digite o codigo do cliente para buscar: ");
                    res_codigo = scanf("%d", &codigo);
                    limparBuffer();
                    if (res_codigo != 1) {
                        printf("Codigo invalido! Digite um numero.\n");
                    }
                } while (res_codigo != 1);

                int rrnEncontrado = buscarBMais(raizBMais, codigo);
                if (rrnEncontrado != -1) {
                    Cliente cEncontrado = buscarClientePorRRN(rrnEncontrado);
                    printf("\n--- Cliente Encontrado ---\n");
                    printf("Codigo: %d\n", cEncontrado.codigo);
                    printf("Nome: %s\n", cEncontrado.nome);
                    printf("RRN: %d\n", rrnEncontrado);
                } else {
                    printf("Cliente com codigo %d nao encontrado.\n", codigo);
                }
                break;
            }
            case 3: { // Listar todos os clientes (Busca Ordenada)
                printf("\n--- Clientes em Ordem Crescente de Codigo ---\n");
                buscaOrdenadaBMais(raizBMais);
                break;
            }
            case 4: { // Visualizar estrutura da Árvore B+
                printf("\n--- Estrutura da Arvore B+ ---\n");
                if (raizBMais->numChaves == 0 && raizBMais->folha) { // Se a raiz está vazia
                    printf("A arvore esta vazia (apenas raiz vazia).\n");
                } else {
                    imprimirArvoreBMais(raizBMais, 0); // Começa do nível 0 (raiz)
                }
                break;
            }
            case 0: { // Sair
                printf("Saindo do programa. Ate mais!\n");
                // Libere a memória da árvore aqui em uma aplicação real
                break;
            }
            default: {
                printf("Opcao invalida! Tente novamente.\n");
                break;
            }
        }
    } while (opcao != 0);

    return 0;
}