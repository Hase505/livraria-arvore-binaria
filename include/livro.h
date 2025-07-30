/**
 * @file livro.h
 * @brief Estruturas e funções para manipular livros em uma árvore binária.
 */

#ifndef LIVRO_H
#define LIVRO_H

#include <stddef.h>

#define MAX_TITULO 150  //!< Tamanho máximo do campo "titulo"
#define MAX_AUTOR 200   //!< Tamanho máximo do campo "autor"
#define MAX_EDITORA 50  //!< Tamanho máximo do campo "editora"

/**
 * Estrutura que representa um livro na árvore binária.
 */
typedef struct {
        /**
         * Código único do livro.
         */
        size_t codigo;

        /**
         * Título do livro.
         */
        char titulo[MAX_TITULO];

        /**
         * Autor do livro.
         */
        char autor[MAX_AUTOR];

        /**
         * Editora do livro.
         */
        char editora[MAX_EDITORA];

        /**
         * Edição do livro.
         */
        size_t edicao;

        /**
         * Ano de publicação do livro.
         */
        size_t ano;

        /**
         * Número de exemplares disponíveis do livro.
         */
        size_t exemplares;

        /**
         * Preço do livro.
         */
        double preco;
} LIVRO;

#endif
