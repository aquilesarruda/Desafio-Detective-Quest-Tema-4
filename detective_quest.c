#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/*
	Detective Quest – Tema 4

	Este programa simula o mapa de uma mansão usando uma árvore binária.
	Cada nó representa uma sala identificada por um nome e possui até
	dois caminhos: esquerda e direita. O jogador explora a partir do
	Hall de entrada escolhendo os caminhos disponíveis.
*/

/* Estrutura que representa uma sala da mansão (nó da árvore binária) */
typedef struct Sala {
	char *nome;
	struct Sala *esquerda;
	struct Sala *direita;
} Sala;

/*
	criarSala
	Cria dinamicamente uma sala com o nome informado. Inicialmente,
	as ligações esquerda/direita são nulas.
*/
static Sala *criarSala(const char *nome) {
	Sala *nova = (Sala *)malloc(sizeof(Sala));
	if (nova == NULL) {
		fprintf(stderr, "Erro: falha ao alocar memória para Sala.\n");
		exit(EXIT_FAILURE);
	}

	nova->nome = (char *)malloc(strlen(nome) + 1);
	if (nova->nome == NULL) {
		fprintf(stderr, "Erro: falha ao alocar memória para nome da Sala.\n");
		free(nova);
		exit(EXIT_FAILURE);
	}
	strcpy(nova->nome, nome);

	nova->esquerda = NULL;
	nova->direita = NULL;
	return nova;
}

/*
	liberarArvore
	Libera toda a memória alocada para a árvore binária (pós-ordem).
*/
static void liberarArvore(Sala *raiz) {
	if (raiz == NULL) {
		return;
	}
	liberarArvore(raiz->esquerda);
	liberarArvore(raiz->direita);
	free(raiz->nome);
	free(raiz);
}

/* Lê uma opção do usuário, ignorando espaços e convertendo para minúscula */
static char lerOpcao(void) {
	char buffer[64];
	if (fgets(buffer, (int)sizeof buffer, stdin) == NULL) {
		return 's';
	}
	for (size_t i = 0; buffer[i] != '\0'; ++i) {
		if (!isspace((unsigned char)buffer[i])) {
			return (char)tolower((unsigned char)buffer[i]);
		}
	}
	return 's';
}

/*
	explorarSalas
	Permite a navegação interativa do jogador pela árvore a partir
	da sala raiz (Hall de entrada). O jogador pode escolher: esquerda (e),
	direita (d) ou sair (s). A exploração termina ao chegar a um nó-folha
	ou quando o jogador optar por sair.
*/
static void explorarSalas(Sala *raiz) {
	Sala *atual = raiz;
	if (atual == NULL) {
		printf("Mapa vazio.\n");
		return;
	}

	printf("\nBem-vindo(a) à mansão Detective Quest!\n");
	printf("Explore a partir do Hall de entrada escolhendo os caminhos.\n\n");

	while (atual != NULL) {
		printf("Você está em: %s\n", atual->nome);

		int temEsquerda = (atual->esquerda != NULL);
		int temDireita = (atual->direita != NULL);

		if (!temEsquerda && !temDireita) {
			printf("Não há saídas a partir daqui. Fim da exploração.\n");
			break;
		}

		printf("Caminhos disponíveis: ");
		if (temEsquerda) {
			printf("[e] esquerda");
		}
		if (temDireita) {
			printf("%s[d] direita", temEsquerda ? ", " : "");
		}
		printf(" | [s] sair\n");
		printf("Escolha (e/d/s): ");

		char opcao = lerOpcao();
		switch (opcao) {
			case 'e':
				if (temEsquerda) {
					atual = atual->esquerda;
				} else {
					printf("Não há caminho à esquerda a partir desta sala.\n\n");
				}
				break;
			case 'd':
				if (temDireita) {
					atual = atual->direita;
				} else {
					printf("Não há caminho à direita a partir desta sala.\n\n");
				}
				break;
			case 's':
				printf("Exploração encerrada pelo jogador.\n");
				return;
			default:
				printf("Opção inválida. Tente novamente.\n\n");
		}
	}
}

/*
	main
	Monta a árvore binária (mapa fixo da mansão) e inicia a exploração.
*/
int main(void) {
	/* Construção manual do mapa (árvore binária) */
	Sala *hall = criarSala("Hall de entrada");
	Sala *salaDeEstar = criarSala("Sala de estar");
	Sala *jardim = criarSala("Jardim");
	Sala *cozinha = criarSala("Cozinha");
	Sala *biblioteca = criarSala("Biblioteca");
	Sala *garagem = criarSala("Garagem");
	Sala *escritorio = criarSala("Escritório");
	Sala *salaSecreta = criarSala("Sala secreta");

	/* Ligações (edges) da árvore */
	hall->esquerda = salaDeEstar;
	hall->direita = jardim;

	salaDeEstar->esquerda = cozinha;      /* folha */
	salaDeEstar->direita = biblioteca;

	jardim->esquerda = garagem;           /* folha */
	jardim->direita = escritorio;         /* folha */

	biblioteca->direita = salaSecreta;    /* folha */

	/* Inicia exploração */
	explorarSalas(hall);

	/* Libera toda a memória antes de sair */
	liberarArvore(hall);
	return 0;
}


