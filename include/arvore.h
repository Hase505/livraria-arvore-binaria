/**
 * @file arvore.h
 * @brief Estruturas e funções para manipular nós da árvore binária.
 */

#ifndef ARVORE_BINARIA_H
#define ARVORE_BINARIA_H

#include "livro.h"

/**
 * Estrutura que representa um nó em uma árvore binária,
 * armazenando um livro como dado.
 */
typedef struct {
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

#endif
