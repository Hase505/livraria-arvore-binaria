/**
 * @file livro.h
 * @brief Estruturas e funções para manipular livros em uma árvore binária.
 */

#ifndef LIVRO_H
#define LIVRO_H

#include <stddef.h>
#include <stdio.h>

/**
 * @brief Declaração antecipada do tipo NO_ARVORE.
 *
 * Usada para evitar include recursivo entre `livro.h` e `arvore.h`.
 * Veja a definição completa em @ref NO_ARVORE "arvore.h"
 */
typedef struct NO_ARVORE NO_ARVORE;

#define MAX_TITULO 150  //!< Tamanho máximo do campo "titulo"
#define MAX_AUTOR 200   //!< Tamanho máximo do campo "autor"
#define MAX_EDITORA 50  //!< Tamanho máximo do campo "editora"

/**
 * @brief Declaração antecipada do tipo NO_ARVORE
 *
 * Usada para evitar include recursivo entre `livro.h` e `arvore.h`
 * Veja a documentação completa em @ref NO_ARVORE "arvore.h"
 */
typedef struct NO_ARVORE NO_ARVORE;

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
        char titulo[MAX_TITULO + 1];

        /**
         * Autor do livro.
         */
        char autor[MAX_AUTOR + 1];

        /**
         * Editora do livro.
         */
        char editora[MAX_EDITORA + 1];

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

/**
 * @brief Verifica se já existe um livro com o mesmo código na árvore binária.
 *
 * @param arquivo Ponteiro para arquivo binário aberto em modo leitura ("rb") ou leitura/escrita
 * ("rb+").
 * @param codigo_livro Código do livro a ser verificado.
 * @return SUCESSO se não existir, ERRO_ARQUIVO_NULO se arquivo não abrir,
 *         ERRO_ARQUIVO_SEEK/ERRO_ARQUIVO_READ em caso de erro de leitura,
 *         ERRO_ARQUIVO_WRITE se já existir livro com o mesmo código.
 *
 * @warning Sempre feche (fclose) o arquivo após inserir um livro e reabra antes de
 * buscar/verificar. Isso garante que os dados estejam sincronizados no disco.
 */
int verificar_id_livro(FILE* arquivo, size_t codigo_livro);

/**
 * @brief Cadastra um novo livro na árvore binária de busca no arquivo.
 *
 * Esta função verifica se já existe um livro com o mesmo código na árvore binária.
 * Se não existir, monta o nó da árvore e insere o livro na estrutura.
 *
 * @param arquivo Ponteiro para arquivo binário aberto em modo leitura/escrita ("rb+").
 * @param livro Estrutura LIVRO a ser inserida.
 * @return Código de retorno (SUCESSO ou erro definido em erros.h).
 *
 * @warning Sempre feche (fclose) o arquivo após inserir um livro e reabra antes de
 * buscar/verificar. Isso garante que os dados estejam sincronizados no disco.
 */
int cadastrar_livro(FILE* arquivo, LIVRO livro);

/**
 * @brief Busca um nó na árvore pelo código do livro.
 *
 * @param arquivo Ponteiro para o arquivo binário aberto em modo leitura ou leitura/escrita.
 * @param codigo Código do livro a ser buscado.
 * @param posicao_encontrada Ponteiro de saída para armazenar a posição do nó no arquivo.
 * @return Ponteiro para o nó encontrado (alocado dinamicamente) ou NULL se não encontrado/erro.
 *
 * @note O chamador deve liberar a memória retornada com free().
 */
NO_ARVORE* buscar_livro(FILE* arquivo, size_t codigo, int* posicao_encontrada);

#endif
