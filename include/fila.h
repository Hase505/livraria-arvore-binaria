/**
 * @brief Estrutura que representa um item na fila.
 *
 * Cada item armazena a posição de um nó na árvore, o nível em que ele se encontra
 * e um ponteiro para o próximo item na fila.
 */

#ifndef FILA_H
#define FILA_H

typedef struct item_fila {
        int posicao;               /**< Posição do nó no arquivo ou estrutura. */
        int nivel;                 /**< Nível do nó na árvore (raiz = 0). */
        struct item_fila* proximo; /**< Ponteiro para o próximo item na fila. */
} ITEM_FILA;

/**
 * @brief Estrutura que representa a fila.
 *
 * A fila é implementada como lista encadeada, possuindo ponteiros para o início (frente),
 * para o final (tras) e armazenando também o tamanho atual.
 */
typedef struct {
        ITEM_FILA* frente; /**< Ponteiro para o primeiro item da fila. */
        ITEM_FILA* tras;   /**< Ponteiro para o último item da fila. */
        int tamanho;       /**< Quantidade de itens na fila. */
} FILA;

/**
 * @brief Cria e inicializa uma fila vazia.
 *
 * @return Ponteiro para a nova fila alocada, ou NULL em caso de falha de alocação.
 */
FILA* criar_fila();

/**
 * @brief Adiciona um novo item ao final da fila.
 *
 * @param fila Ponteiro para a fila.
 * @param posicao Posição do nó na estrutura/arquivo.
 * @param nivel Nível do nó na árvore.
 * @return Código de status (ex.: SUCESSO ou código de erro).
 */
int enfileirar(FILA* fila, int posicao, int nivel);

/**
 * @brief Verifica se a fila está vazia.
 *
 * @param fila Ponteiro para a fila.
 * @return 1 se vazia ou ponteiro nulo, 0 caso contrário.
 */
int fila_vazia(FILA* fila);

/**
 * @brief Remove e retorna o primeiro item da fila.
 *
 * @param fila Ponteiro para a fila.
 * @return Estrutura ITEM_FILA removida. Se a fila estiver vazia, retorna ITEM_FILA com valores
 * inválidos.
 */
ITEM_FILA desenfileirar(FILA* fila);

/**
 * @brief Libera toda a memória associada à fila.
 *
 * @param fila Ponteiro para a fila a ser destruída.
 */
void destruir_fila(FILA* fila);

#endif
