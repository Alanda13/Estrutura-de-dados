#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct No {
    char texto[100];
    struct No* sim;  
    struct No* nao;  
} No;

No* criarNo(const char* texto) {
    No* novo = (No*) malloc(sizeof(No));
    strcpy(novo->texto, texto);
    novo->sim = NULL;
    novo->nao = NULL;
    return novo;
}

int perguntaSimNao(const char* texto) {
    char resposta[10];
    while (1) {
        printf("%s (sim/nao): ", texto);
        scanf("%9s", resposta);
        if (strcmp(resposta, "sim") == 0) return 1;
        else if (strcmp(resposta, "nao") == 0) return 0;
        else printf("Resposta invalida! Digite 'sim' ou 'nao'.\n");
    }
}

void percorrerArvore(No* no) {
    if (!no) return;

    if (no->sim == NULL && no->nao == NULL) {
        printf("Decisao: %s\n", no->texto);
        return;
    }

    int resposta = perguntaSimNao(no->texto);
    if (resposta == 1) {
        percorrerArvore(no->sim);
    } else {
        percorrerArvore(no->nao);
    }
}

No* criarArvoreDecisao() {

    //nós finais
    No* jogarManhaEnsolarado = criarNo("Jogar tenis (Manha - Ensolarado)");
    No* jogarManhaNublado   = criarNo("Jogar tenis (Manha - Nublado)");
    No* naoJogarManhaChuvoso= criarNo("Nao jogar tenis (Manha - Chuvoso)");

    No* jogarTardeEnsolarado = criarNo("Jogar tenis (Tarde - Ensolarado)");
    No* jogarTardeNublado    = criarNo("Jogar tenis (Tarde - Nublado)");
    No* naoJogarTardeChuvoso = criarNo("Nao jogar tenis (Tarde - Chuvoso)");

    No* naoJogarNoite = criarNo("Nao jogar tenis (Noite)");
    
    No* naoJogarNenhumHorario = criarNo("Nao jogar tenis");

    //nós intermediários 
    //perguntas sobre o clima
    No* climaManhaEnsolarado = criarNo("Clima: Ensolarado?");
    climaManhaEnsolarado->sim = jogarManhaEnsolarado;
    climaManhaEnsolarado->nao = criarNo("Clima: Nublado?");

    climaManhaEnsolarado->nao->sim = jogarManhaNublado;
    climaManhaEnsolarado->nao->nao = criarNo("Clima: Chuvoso?");
    climaManhaEnsolarado->nao->nao->sim = naoJogarManhaChuvoso;
    climaManhaEnsolarado->nao->nao->nao = criarNo("Nenhum clima selecionado no Manha");
    climaManhaEnsolarado->nao->nao->nao->sim = naoJogarNenhumHorario;
    climaManhaEnsolarado->nao->nao->nao->nao = naoJogarNenhumHorario;

    No* climaTardeEnsolarado = criarNo("Clima: Ensolarado?");
    climaTardeEnsolarado->sim = jogarTardeEnsolarado;
    climaTardeEnsolarado->nao = criarNo("Clima: Nublado?");
    climaTardeEnsolarado->nao->sim = jogarTardeNublado;
    climaTardeEnsolarado->nao->nao = criarNo("Clima: Chuvoso?");
    climaTardeEnsolarado->nao->nao->sim = naoJogarTardeChuvoso;
    climaTardeEnsolarado->nao->nao->nao = criarNo("Nenhum clima selecionado no Tarde");
    climaTardeEnsolarado->nao->nao->nao->sim = naoJogarNenhumHorario;
    climaTardeEnsolarado->nao->nao->nao->nao = naoJogarNenhumHorario;


    //perguntas sobre horário
    No* horarioManha = criarNo("Horario: Manha?");
    horarioManha->sim = climaManhaEnsolarado;

    No* horarioTarde = criarNo("Horario: Tarde?");
    horarioTarde->sim = climaTardeEnsolarado;

    No* horarioNoite = criarNo("Horario: Noite?");
    horarioNoite->sim = naoJogarNoite;

    horarioManha->nao = horarioTarde;
    horarioTarde->nao = horarioNoite;
    horarioNoite->nao = naoJogarNenhumHorario;

    //criação do nó raiz
    No* raiz = criarNo("Quer jogar tenis?");
    raiz->sim = horarioManha; 
    raiz->nao = naoJogarNenhumHorario;

    return raiz;
}

int main() {
    No* raiz = criarArvoreDecisao();

    printf("Arvore de decisao para jogar tenis:\n");
    percorrerArvore(raiz);

    return 0;
}
