#include <stdio.h>
#include <stdlib.h>

// =====================================
// 1. ESTRUTURA DO NO
// =====================================

typedef struct No {
    int valor;
    int altura;
    struct No *esq;
    struct No *dir;
} No;

// =====================================
// 2. FUNCOES AUXILIARES
// =====================================

int max(int a, int b) {
    return (a > b) ? a : b;
}

int obterAltura(No *n) {
    if (n == NULL)
        return 0;

    return n->altura;
}

int obterFB(No *n) {
    if (n == NULL)
        return 0;

    return obterAltura(n->esq) - obterAltura(n->dir);
}

No *novoNo(int valor) {
    No *n = malloc(sizeof(No));

    if (n == NULL) {
        printf("Erro de memoria!\n");
        exit(EXIT_FAILURE);
    }

    n->valor = valor;
    n->altura = 1;
    n->esq = NULL;
    n->dir = NULL;

    return n;
}

void atualizarAltura(No *n) {
    n->altura = 1 + max(obterAltura(n->esq), obterAltura(n->dir));
}

// =====================================
// 3. IMPRESSAO DA ARVORE
// =====================================

void imprimirEmOrdem(No *raiz) {
    if (raiz != NULL) {
        imprimirEmOrdem(raiz->esq);
        printf("%d ", raiz->valor);
        imprimirEmOrdem(raiz->dir);
    }
}

// Imprime a arvore deitada: subarvore direita, raiz e esquerda.
void imprimirArvore(No *raiz, int nivel) {
    if (raiz == NULL)
        return;

    imprimirArvore(raiz->dir, nivel + 1);

    for (int i = 0; i < nivel; i++)
        printf("    ");

    printf("%d\n", raiz->valor);

    imprimirArvore(raiz->esq, nivel + 1);
}

// =====================================
// 4. ROTACOES AVL
// =====================================

No *rotacaoDireita(No *y) {
    No *x = y->esq;
    No *t2 = x->dir;

    x->dir = y;
    y->esq = t2;

    atualizarAltura(y);
    atualizarAltura(x);

    return x;
}

No *rotacaoEsquerda(No *x) {
    No *y = x->dir;
    No *t2 = y->esq;

    y->esq = x;
    x->dir = t2;

    atualizarAltura(x);
    atualizarAltura(y);

    return y;
}

No *rotacaoLR(No *n) {
    n->esq = rotacaoEsquerda(n->esq);
    return rotacaoDireita(n);
}

No *rotacaoRL(No *n) {
    n->dir = rotacaoDireita(n->dir);
    return rotacaoEsquerda(n);
}

// =====================================
// 5. INSERCAO NA ARVORE AVL
// =====================================

No *inserir(No *raiz, int valor) {
    if (raiz == NULL)
        return novoNo(valor);

    if (valor < raiz->valor)
        raiz->esq = inserir(raiz->esq, valor);
    else if (valor > raiz->valor)
        raiz->dir = inserir(raiz->dir, valor);
    else
        return raiz; // Valores duplicados nao sao inseridos.

    atualizarAltura(raiz);

    int fb = obterFB(raiz);

    if (fb > 1 && valor < raiz->esq->valor) // LL
        return rotacaoDireita(raiz);

    if (fb < -1 && valor > raiz->dir->valor) // RR
        return rotacaoEsquerda(raiz);

    if (fb > 1 && valor > raiz->esq->valor) // LR
        return rotacaoLR(raiz);

    if (fb < -1 && valor < raiz->dir->valor) // RL
        return rotacaoRL(raiz);

    return raiz;
}

// =====================================
// 6. BUSCA NA ARVORE AVL
// =====================================

int buscar(No *raiz, int valor) {
    if (raiz == NULL)
        return 0;

    if (raiz->valor == valor)
        return 1;

    if (valor < raiz->valor)
        return buscar(raiz->esq, valor);

    return buscar(raiz->dir, valor);
}

// =====================================
// 7. REMOCAO NA ARVORE AVL
// =====================================

No *menorValor(No *raiz) {
    No *atual = raiz;

    while (atual->esq != NULL)
        atual = atual->esq;

    return atual;
}

No *remover(No *raiz, int valor) {
    if (raiz == NULL)
        return raiz;

    if (valor < raiz->valor) {
        raiz->esq = remover(raiz->esq, valor);
    } else if (valor > raiz->valor) {
        raiz->dir = remover(raiz->dir, valor);
    } else {
        if (raiz->esq == NULL || raiz->dir == NULL) {
            No *temp = (raiz->esq != NULL) ? raiz->esq : raiz->dir;

            if (temp == NULL) {
                free(raiz);
                raiz = NULL;
            } else {
                *raiz = *temp;
                free(temp);
            }
        } else {
            No *temp = menorValor(raiz->dir);
            raiz->valor = temp->valor;
            raiz->dir = remover(raiz->dir, temp->valor);
        }
    }

    if (raiz == NULL)
        return raiz;

    atualizarAltura(raiz);

    int fb = obterFB(raiz);

    if (fb > 1 && obterFB(raiz->esq) >= 0) // LL
        return rotacaoDireita(raiz);

    if (fb > 1 && obterFB(raiz->esq) < 0) { // LR
        raiz->esq = rotacaoEsquerda(raiz->esq);
        return rotacaoDireita(raiz);
    }

    if (fb < -1 && obterFB(raiz->dir) <= 0) // RR
        return rotacaoEsquerda(raiz);

    if (fb < -1 && obterFB(raiz->dir) > 0) { // RL
        raiz->dir = rotacaoDireita(raiz->dir);
        return rotacaoEsquerda(raiz);
    }

    return raiz;
}

// =====================================
// 8. LIBERAR MEMORIA
// =====================================

void liberarArvore(No *raiz) {
    if (raiz != NULL) {
        liberarArvore(raiz->esq);
        liberarArvore(raiz->dir);
        free(raiz);
    }
}

// =====================================
// 9. PROGRAMA PRINCIPAL
// =====================================

int main(void) {
    No *raiz = NULL;
    int opcao, valor;

    do {
        printf("\n===== MENU ARVORE AVL =====\n");
        printf("1 - Inserir numero\n");
        printf("2 - Buscar numero\n");
        printf("3 - Remover numero\n");
        printf("4 - Mostrar valores em ordem\n");
        printf("5 - Mostrar altura e balanceamento\n");
        printf("6 - Imprimir arvore visualmente\n");
        printf("7 - Sair\n");
        printf("\nEscolha uma opcao: ");

        if (scanf("%d", &opcao) != 1) {
            printf("Entrada invalida! Digite um numero.\n");
            int caractere;
            while ((caractere = getchar()) != '\n' && caractere != EOF) {
                // Descarta o restante da entrada invalida.
            }
            continue;
        }

        switch (opcao) {
            case 1:
                printf("Digite o numero para inserir: ");
                if (scanf("%d", &valor) != 1) {
                    printf("Valor invalido!\n");
                    while (getchar() != '\n');
                    break;
                }
                raiz = inserir(raiz, valor);
                printf("Numero inserido (ou ja existente) na arvore.\n");
                break;

            case 2:
                printf("Digite o numero para buscar: ");
                if (scanf("%d", &valor) != 1) {
                    printf("Valor invalido!\n");
                    while (getchar() != '\n');
                    break;
                }
                if (buscar(raiz, valor))
                    printf("Numero encontrado!\n");
                else
                    printf("Numero nao encontrado!\n");
                break;

            case 3:
                printf("Digite o numero para remover: ");
                if (scanf("%d", &valor) != 1) {
                    printf("Valor invalido!\n");
                    while (getchar() != '\n');
                    break;
                }
                if (buscar(raiz, valor)) {
                    raiz = remover(raiz, valor);
                    printf("Numero removido com sucesso!\n");
                } else {
                    printf("Numero nao encontrado!\n");
                }
                break;

            case 4:
                printf("\nValores em ordem: ");
                imprimirEmOrdem(raiz);
                printf("\n");
                break;

            case 5:
                if (raiz == NULL) {
                    printf("A arvore esta vazia!\n");
                } else {
                    printf("Raiz: %d\n", raiz->valor);
                    printf("Altura: %d\n", obterAltura(raiz));
                    printf("Fator de balanceamento: %d\n", obterFB(raiz));
                }
                break;

            case 6:
                printf("\n=== ARVORE AVL ===\n");
                if (raiz == NULL)
                    printf("A arvore esta vazia!\n");
                else
                    imprimirArvore(raiz, 0);
                break;

            case 7:
                printf("Encerrando o programa...\n");
                break;

            default:
                printf("Opcao invalida!\n");
        }

    } while (opcao != 7);

    liberarArvore(raiz);
    return 0;
}
