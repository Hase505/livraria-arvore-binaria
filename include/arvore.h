/**
 * @file arvore.h
 * @brief Estruturas e funções para manipular nós da árvore binária.
 */

#ifndef ARVORE_BINARIA_H
#define ARVORE_BINARIA_H

#include <stddef.h>
#include <stdio.h>

#include "livro.h"

/**
 * Estrutura que representa um nó em uma árvore binária,
 * armazenando um livro como dado.
 */
typedef struct NO_ARVORE {
        /**
         * Estrutura que representa um livro no registro.
         */
        LIVRO livro;

        /**
         * Posição no arquivo do filho esquerdo do nó da árvore.
         */
        int filho_esquerdo;

        /**
         * Posição no arquivo do filho direito do nó da árvore.
         */
        int filho_direito;
} NO_ARVORE;

/**
 * @enum lado_filho
 * @brief Indica se um nó é filho esquerdo, direito ou inválido.
 */
typedef enum {
        LADO_INVALIDO = -1, /**< Valor inválido para lado do filho */
        LADO_ESQUERDO = 0,  /**< Nó é filho esquerdo */
        LADO_DIREITO = 1    /**< Nó é filho direito */
} lado_filho;

/**
 * @struct RESULTADO_BUSCA
 * @brief Estrutura que armazena o resultado da busca em uma árvore binária.
 *
 * Contém o nó encontrado, sua posição no arquivo,
 * o nó pai e sua posição, e o lado do nó em relação ao pai.
 */
typedef struct {
        NO_ARVORE* no;   /**< Ponteiro para o nó encontrado (deve ser liberado pelo usuário) */
        int posicao_no;  /**< Posição no arquivo do nó encontrado */
        NO_ARVORE* pai;  /**< Ponteiro para o nó pai (pode ser NULL se o nó for raiz) */
        int posicao_pai; /**< Posição no arquivo do nó pai */
        lado_filho lado; /**< Indica se o nó é filho esquerdo ou direito do pai */
} RESULTADO_BUSCA;

/**
 * @brief Busca um nó na árvore binária de busca armazenada no arquivo.
 *
 * Esta função percorre a árvore binária de busca persistida em arquivo,
 * a partir da raiz, até encontrar o nó cujo código do livro seja igual
 * ao informado. Caso o nó seja encontrado, informações sobre sua posição,
 * pai e lado em relação ao pai são preenchidas na estrutura RESULTADO_BUSCA.
 *
 * Se o nó não for encontrado, a função retorna as informações do último nó
 * visitado (pai) e o lado onde a inserção deveria ocorrer.
 *
 * @param arquivo Ponteiro para o arquivo binário aberto em modo leitura/escrita ("rb+").
 * @param codigo Código único do livro a ser buscado.
 * @param resultado Ponteiro para estrutura RESULTADO_BUSCA onde os dados do nó
 *        encontrado (ou informações para inserção) serão armazenados.
 * @return
 * - `SUCESSO` se o nó foi encontrado.
 * - `ERRO_ARQUIVO_NULO` se o ponteiro de arquivo for nulo.
 * - `ERRO_CABECALHO_NULO` se o cabeçalho não puder ser lido.
 * - `ERRO_NO_NULO` se a árvore estiver vazia ou o nó não for encontrado.
 *
 * @note O chamador é responsável por liberar a memória alocada para `resultado->no`
 *       e `resultado->pai` quando não forem mais necessários.
 */
int buscar_no_arvore(FILE* arquivo, size_t codigo, RESULTADO_BUSCA* resultado);

/**
 * @brief Insere um novo nó na árvore binária de busca armazenada no arquivo.
 *
 * Esta função insere um novo nó (representando um livro) na árvore binária
 * de busca persistida em arquivo. A função verifica se o código do livro já
 * existe na árvore e, caso exista, retorna erro.
 *
 * Caso a árvore esteja vazia, o novo nó será definido como raiz. Caso contrário,
 * ele será inserido como filho esquerdo ou direito do nó pai, de acordo com
 * a ordem binária de busca.
 *
 * @param arquivo Ponteiro para arquivo binário aberto em modo leitura/escrita ("rb+").
 * @param novo Ponteiro para estrutura NO_ARVORE a ser inserida.
 * @return
 * - `SUCESSO` em caso de inserção bem-sucedida.
 * - `ERRO_ARQUIVO_NULO` se o ponteiro de arquivo for nulo.
 * - `ERRO_NO_NULO` se o ponteiro para o novo nó for nulo.
 * - `ERRO_CODIGO_DUPLICADO` se já existir um livro com o mesmo código.
 * - Outros códigos de erro definidos em erros.h podem ser retornados
 *   dependendo do erro ocorrido durante gravação no arquivo.
 *
 * @warning Sempre feche (`fclose`) o arquivo após inserir um livro e reabra antes de
 * buscar ou verificar. Isso garante que os dados estejam sincronizados no disco.
 *
 * @note Esta função chama internamente `inserir_no_arquivo()` para decidir se
 *       utilizará a lista livre ou adicionar no final do arquivo.
 */
int inserir_no_arvore(FILE* arquivo, NO_ARVORE* novo);

/**
 * @brief Imprime todos os livros da árvore binária armazenada no arquivo em ordem crescente.
 *
 * Esta função inicia o percurso in-order a partir da raiz da árvore, lendo
 * o cabeçalho para obter a posição da raiz.
 *
 * @param arquivo Ponteiro para o arquivo binário aberto em modo leitura.
 * @return Código de retorno:
 *         - SUCESSO (0) se a operação ocorreu normalmente;
 *         - ERRO_ARQUIVO_NULO se o arquivo for NULL;
 *         - ERRO_CABECALHO_NULO se o cabeçalho não puder ser lido.
 *
 * @pre `arquivo` deve estar aberto para leitura.
 * @post Os dados dos livros são impressos na saída padrão.
 */
int imprimir_in_ordem(FILE* arquivo);

/**
 * @brief Remove um nó da árvore binária de busca no arquivo.
 *
 * Determina se o nó é folha ou interno e chama a função apropriada.
 *
 * @param arquivo Ponteiro para o arquivo da árvore.
 * @param codigo Código do livro a ser removido.
 * @return int Código de status da operação.
 */
int remover_no_arvore(FILE* arquivo, size_t codigo);

int imprimir_arvore_por_niveis(FILE* arquivo);

#endif
