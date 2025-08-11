#include "../include/fila.h"

#include <stdlib.h>

#include "../include/erros.h"

/**
 * @brief Cria e inicializa uma fila vazia.
 *
 * @return Ponteiro para a nova fila alocada, ou NULL em caso de falha de alocação.
 */
FILA* criar_fila() {
        FILA* fila = malloc(sizeof(FILA));
        if (fila == NULL) return NULL;

        fila->frente = NULL;
        fila->tras = NULL;
        fila->tamanho = 0;

        return fila;
}

/**
 * @brief Adiciona um novo item ao final da fila.
 *
 * @param fila Ponteiro para a fila.
 * @param posicao Posição do nó na estrutura/arquivo.
 * @param nivel Nível do nó na árvore.
 * @return Código de status (ex.: SUCESSO ou código de erro).
 */
int enfileirar(FILA* fila, int posicao, int nivel) {
        if (fila == NULL) return ERRO_FILA_NULA;

        ITEM_FILA* novo_item = malloc(sizeof(ITEM_FILA));
        if (novo_item == NULL) return ERRO_ITEM_FILA_NULO;

        novo_item->posicao = posicao;
        novo_item->nivel = nivel;
        novo_item->proximo = NULL;

        if (fila->tras) {
                fila->tras->proximo = novo_item;
        } else {
                fila->frente = novo_item;
        }

        fila->tras = novo_item;
        fila->tamanho++;

        return SUCESSO;
}

/**
 * @brief Verifica se a fila está vazia.
 *
 * @param fila Ponteiro para a fila.
 * @return 1 se vazia ou ponteiro nulo, 0 caso contrário.
 */
int fila_vazia(FILA* fila) {
        return (fila == NULL || fila->tamanho == 0);
}

/**
 * @brief Remove e retorna o primeiro item da fila.
 *
 * @param fila Ponteiro para a fila.
 * @return Estrutura ITEM_FILA removida. Se a fila estiver vazia, retorna ITEM_FILA com valores
 * inválidos.
 */
ITEM_FILA desenfileirar(FILA* fila) {
        ITEM_FILA item = {0};
        item.posicao = -1;
        item.nivel = -1;
        item.proximo = NULL;

        if (fila_vazia(fila)) return item;

        ITEM_FILA* frente = fila->frente;
        item.posicao = frente->posicao;
        item.nivel = frente->nivel;

        fila->frente = frente->proximo;
        if (!fila->frente) fila->tras = NULL;
        free(frente);
        fila->tamanho--;

        return item;
}

/**
 * @brief Libera toda a memória associada à fila.
 *
 * @param fila Ponteiro para a fila a ser destruída.
 */
void destruir_fila(FILA* fila) {
        if (fila == NULL) return;
        ITEM_FILA* item_atual = fila->frente;

        while (item_atual) {
                ITEM_FILA* tmp = item_atual;
                item_atual = item_atual->proximo;
                free(tmp);
        }
        free(fila);
}
