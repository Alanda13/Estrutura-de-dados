#include "arvores.h"

// --- Funções de Manipulação de Arquivo ---

// Salva um cliente no final do arquivo "clientes.dat"
void salvarCliente(Cliente c) {
    FILE *arquivo = fopen("clientes.dat", "ab");
    if (!arquivo) {
        perror("Erro ao abrir o arquivo para escrita");
        return;
    }
    fwrite(&c, sizeof(Cliente), 1, arquivo);
    fclose(arquivo);
}

// Busca um cliente no arquivo binário pelo seu RRN
Cliente buscarClientePorRRN(int rrn) {
    FILE *arquivo = fopen("clientes.dat", "rb");
    Cliente c = {0, ""}; // Inicializa com valores padrão
    if (!arquivo) {
        perror("Erro ao abrir o arquivo para leitura");
        exit(1); // Sai do programa em caso de erro crítico
    }
    // Posiciona o ponteiro do arquivo no início do registro RRN
    fseek(arquivo, rrn * sizeof(Cliente), SEEK_SET);
    fread(&c, sizeof(Cliente), 1, arquivo); // Lê o registro
    fclose(arquivo);
    return c;
}

// Obtém o próximo RRN disponível, que é o número total de registros existentes
int getProximoRRN() {
    FILE *arquivo = fopen("clientes.dat", "rb");
    if (!arquivo) {
        // Se o arquivo não existe, o primeiro RRN é 0
        return 0;
    }
    fseek(arquivo, 0, SEEK_END); // Vai para o final do arquivo
    int tamanho = ftell(arquivo); // Pega a posição atual (tamanho em bytes)
    fclose(arquivo);
    return tamanho / sizeof(Cliente); // Retorna o número de registros
}

// --- Funções da Árvore B+ ---

// Cria um novo nó da árvore B+
NoBMais* criarNoBMais(int folha) {
    NoBMais *no = malloc(sizeof(NoBMais));
    if (!no) {
        perror("Erro ao alocar memória para o nó");
        exit(1);
    }
    no->folha = folha;
    no->numChaves = 0;
    no->proxFolha = NULL; // Importante para as folhas
    for (int i = 0; i < ORDEM; i++) {
        no->filhos[i] = NULL;
    }
    return no;
}

// Insere uma chave e RRN em um nó que NÃO está cheio, mantendo a ordem
void inserirNaoCheioBMais(NoBMais *no, int chave, int rrn) {
    int i = no->numChaves - 1;

    // Desloca as chaves e RRNs maiores para abrir espaço
    while (i >= 0 && chave < no->chaves[i]) {
        no->chaves[i + 1] = no->chaves[i];
        if (no->folha) { // RRNs apenas nas folhas
            no->rrns[i + 1] = no->rrns[i];
        }
        // Se for nó interno, também desloca os filhos
        if (!no->folha) {
            no->filhos[i + 2] = no->filhos[i + 1];
        }
        i--;
    }
    // Insere a nova chave e RRN
    no->chaves[i + 1] = chave;
    if (no->folha) { // RRNs apenas nas folhas
        no->rrns[i + 1] = rrn;
    }
    no->numChaves++;
}

// Divide um nó cheio em dois, retornando a chave promovida e o novo nó
Promocao* dividirNoBMais(NoBMais *no) {
    Promocao *promocao = malloc(sizeof(Promocao));
    if (!promocao) {
        perror("Erro ao alocar memória para promocao");
        exit(1);
    }
    promocao->houvePromocao = 1; // Indica que uma chave foi promovida

    NoBMais *novoNo = criarNoBMais(no->folha); // O novo nó terá a mesma característica (folha/interno)

    // Índice do meio para a divisão
    int meio = (ORDEM - 1) / 2;

    if (no->folha) {
        // Para nós folha, a chave mediana é COPIADA para o pai e permanece na folha
        promocao->chavePromovida = no->chaves[meio];
        promocao->novoNoFilho = novoNo;

        // Copia a segunda metade das chaves e RRNs para o novo nó
        for (int i = 0; i < ORDEM - 1 - meio; i++) {
            novoNo->chaves[i] = no->chaves[meio + i];
            novoNo->rrns[i] = no->rrns[meio + i];
        }
        novoNo->numChaves = ORDEM - 1 - meio; // Ex: ORDEM 3, meio 1. 2 chaves. 3-1-1 = 1 chave pra novoNo
        no->numChaves = meio; // Ex: ORDEM 3, meio 1. 2 chaves. no->numChaves = 1

        // Atualiza os ponteiros de folha encadeada
        novoNo->proxFolha = no->proxFolha;
        no->proxFolha = novoNo;

    } else {
        // Para nós internos, a chave mediana é MOVIDA para o pai e não fica no nó
        promocao->chavePromovida = no->chaves[meio];
        promocao->novoNoFilho = novoNo;

        // Copia a segunda metade das chaves e filhos para o novo nó
        for (int i = 0; i < ORDEM - 1 - meio -1; i++) { // ORDEM-1-meio-1 porque a chave do meio não vai para os filhos
            novoNo->chaves[i] = no->chaves[meio + 1 + i];
        }
        for (int i = 0; i < ORDEM - meio; i++) { // Copia os ponteiros para filhos
             novoNo->filhos[i] = no->filhos[meio + 1 + i];
        }
        
        novoNo->numChaves = ORDEM - 1 - meio - 1; // Ex: ORDEM 3, meio 1. 2 chaves. (3-1-1-1)=0 chaves para novo no.
                                                // Se ORDEM = 5, meio = 2. (5-1-2-1) = 1 chave para novoNo.
        no->numChaves = meio; // Ex: ORDEM 3, meio 1. 2 chaves. no->numChaves = 1
    }
    return promocao;
}

// Função recursiva para inserir uma chave na árvore B+
void inserirBMaisRec(NoBMais *no, int chave, int rrn, Promocao **promocao) {
    int i = no->numChaves - 1; // Começa pelo final para encontrar a posição correta

    if (no->folha) {
        // Se for folha, insere diretamente no nó
        inserirNaoCheioBMais(no, chave, rrn);

        // Verifica se o nó ficou cheio após a inserção
        if (no->numChaves == ORDEM) {
            *promocao = dividirNoBMais(no); // Divide o nó e retorna a promoção
        } else {
            *promocao = NULL; // Não houve promoção
        }
    } else {
        // Se for nó interno, encontra o filho correto para descer
        while (i >= 0 && chave < no->chaves[i]) {
            i--;
        }
        i++; // 'i' agora é o índice do filho correto para descer

        Promocao *promocaoFilho = NULL;
        inserirBMaisRec(no->filhos[i], chave, rrn, &promocaoFilho);

        if (promocaoFilho != NULL && promocaoFilho->houvePromocao) {
            // Se o filho se dividiu, insere a chave promovida no nó atual
            inserirNaoCheioBMais(no, promocaoFilho->chavePromovida, -1); // -1 porque RRNs não importam em nós internos
            
            // Adiciona o novo nó filho (do lado direito da chave promovida)
            // Desloca os filhos para abrir espaço
            for (int j = no->numChaves; j > i + 1; j--) {
                no->filhos[j] = no->filhos[j - 1];
            }
            no->filhos[i + 1] = promocaoFilho->novoNoFilho;
            
            free(promocaoFilho); // Libera a struct de promoção do filho

            // Verifica se o nó atual ficou cheio após a inserção da chave promovida
            if (no->numChaves == ORDEM) {
                *promocao = dividirNoBMais(no); // Divide o nó atual
            } else {
                *promocao = NULL; // Não houve promoção neste nível
            }
        } else {
            *promocao = NULL; // Não houve promoção do filho
        }
    }
}

// Função wrapper para inserir na árvore B+
void inserirBMais(NoBMais **raiz, int chave, int rrn) {
    Promocao *promocao = NULL;
    inserirBMaisRec(*raiz, chave, rrn, &promocao);

    if (promocao != NULL && promocao->houvePromocao) {
        // Se a raiz se dividiu, cria uma nova raiz
        NoBMais *novaRaiz = criarNoBMais(0); // Nova raiz é um nó interno
        novaRaiz->chaves[0] = promocao->chavePromovida;
        novaRaiz->filhos[0] = *raiz; // O lado esquerdo é a antiga raiz
        novaRaiz->filhos[1] = promocao->novoNoFilho; // O lado direito é o novo nó
        novaRaiz->numChaves = 1;
        *raiz = novaRaiz; // A árvore agora tem uma nova raiz
        free(promocao); // Libera a struct de promoção
    }
}

// Função para buscar o RRN de um cliente na árvore B+
int buscarBMais(NoBMais *no, int chave) {
    if (!no) return -1; // Árvore vazia ou chave não encontrada

    int i = 0;
    while (i < no->numChaves && chave > no->chaves[i]) {
        i++;
    }

    if (no->folha) {
        // Se for folha, verifica se a chave foi encontrada
        if (i < no->numChaves && chave == no->chaves[i]) {
            return no->rrns[i]; // Retorna o RRN
        } else {
            return -1; // Chave não encontrada na folha
        }
    } else {
        // Se for nó interno, desce para o filho apropriado
        return buscarBMais(no->filhos[i], chave);
    }
}

// Percorre as folhas da árvore B+ em ordem crescente e imprime as chaves
void buscaOrdenadaBMais(NoBMais *raiz) {
    if (!raiz) {
        printf("A arvore esta vazia.\n");
        return;
    }

    // Primeiro, encontra a folha mais à esquerda (menor chave)
    NoBMais *current = raiz;
    while (!current->folha) {
        current = current->filhos[0];
    }

    // Agora percorre a lista encadeada de folhas
    while (current != NULL) {
        for (int i = 0; i < current->numChaves; i++) {
            Cliente c = buscarClientePorRRN(current->rrns[i]);
            printf("Código: %d | Nome: %s\n", c.codigo, c.nome);
        }
        current = current->proxFolha; // Vai para a próxima folha
    }
}

// Imprime a estrutura da árvore B+ (para visualização)
void imprimirArvoreBMais(NoBMais *no, int nivel) {
    if (no == NULL) {
        return;
    }

    // Imprime as chaves do nó atual
    for (int i = 0; i < nivel; i++) {
        printf("  "); // Indentação para mostrar o nível
    }
    printf("Nivel %d (%s): [", nivel, no->folha ? "Folha" : "Interno");
    for (int i = 0; i < no->numChaves; i++) {
        printf("%d", no->chaves[i]);
        // Correção
        if (no->folha && no->rrns[i] != -1) { // Só mostra RRN em folhas // Só mostra RRN em folhas
             printf("(RRN:%d)", no->rrns[i]);
        }
        if (i < no->numChaves - 1) {
            printf(", ");
        }
    }
    printf("]\n");

    // Chama recursivamente para os filhos
    if (!no->folha) {
        for (int i = 0; i <= no->numChaves; i++) {
            imprimirArvoreBMais(no->filhos[i], nivel + 1);
        }
    }
}