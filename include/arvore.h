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
 * @brief Busca um nó na árvore binária persistida em arquivo pelo código do livro.
 *
 * Esta função realiza a busca de um nó que contenha um livro com o código
 * especificado, retornando informações detalhadas sobre o nó encontrado, seu pai,
 * suas posições no arquivo e o lado do nó em relação ao pai.
 *
 * @param[in] arquivo Ponteiro para o arquivo que contém a árvore binária.
 * @param[in] codigo Código do livro a ser buscado.
 * @param[out] resultado Ponteiro para estrutura que será preenchida com os dados da busca.
 *
 * @return Código de status da operação:
 * - SUCESSO (0) se o nó foi encontrado com sucesso.
 * - ERRO_ARQUIVO_NULO se o ponteiro do arquivo é NULL.
 * - ERRO_CABECALHO_NULO se não foi possível ler o cabeçalho do arquivo.
 * - ERRO_NO_NULO se a árvore estiver vazia ou o nó não for encontrado.
 *
 * @note Os ponteiros no e pai dentro de @p resultado são alocados dinamicamente
 * e devem ser liberados pelo usuário após o uso.
 */
int buscar_no_arvore(FILE* arquivo, size_t codigo, RESULTADO_BUSCA* resultado);

#endif
