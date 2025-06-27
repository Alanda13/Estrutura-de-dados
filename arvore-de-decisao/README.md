# Árvore de Decisão para Jogar Tênis

Este programa em C implementa uma árvore de decisão simples para ajudar a decidir se deve jogar tênis com base no horário do dia e nas condições climáticas. Ele interage com o usuário através de perguntas do tipo "sim" ou "não" para navegar pela árvore e chegar a uma decisão final.

---

## Como funciona

- O programa apresenta uma série de perguntas sobre o horário ("Manhã?", "Tarde?", "Noite?") e o clima ("Ensolarado?", "Nublado?", "Chuvoso?").
- O usuário responde "sim" ou "não" para cada pergunta.
- Com base nas respostas, o programa percorre a árvore de decisão e, ao chegar a uma folha, exibe a decisão final se deve ou não jogar tênis.

---

## Estrutura do código

- **Estrutura `No`**: representa um nó da árvore com um texto e dois ponteiros para os nós "sim" e "não".
- **Função `criarNo`**: cria um nó novo da árvore com o texto recebido.
- **Função `perguntaSimNao`**: faz a pergunta ao usuário, validando que a resposta seja "sim" ou "nao".
- **Função `percorrerArvore`**: navega recursivamente na árvore conforme as respostas do usuário até chegar a uma decisão final.
- **Função `criarArvoreDecisao`**: monta a árvore de decisão completa com todas as perguntas e decisões possíveis.
- **Função `main`**: cria a árvore, inicia o processo e exibe a decisão.

---

## Colaboradores

- **Luiz Felipe**  
GitHub: [Link do GitHub](https://github.com/Luiz-06)

- **Alanda**  
GitHub: [Link do GitHub](https://github.com/Alanda13)

---

## Como usar

1. Compile o código com um compilador C, por exemplo:
   ```bash
   gcc -o arvore_decisao arvore_decisao.c

2. Execute o programa, por exemplo:
```bash
./arvore_decisao
