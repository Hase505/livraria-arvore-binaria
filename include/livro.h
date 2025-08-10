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
 * @brief Cadastra um novo livro na árvore binária de busca.
 *
 * Esta função verifica se já existe um livro com o mesmo código no arquivo,
 * e caso não exista, insere o novo livro na árvore binária de busca.
 *
 * @param arquivo Ponteiro para o arquivo binário aberto em modo leitura/escrita ("rb+").
 * @param livro Estrutura LIVRO com todos os dados preenchidos.
 * @return Código de retorno:
 *         - SUCESSO: livro cadastrado com sucesso.
 *         - ERRO_ARQUIVO_NULO: ponteiro para arquivo é nulo.
 *         - ERRO_CODIGO_DUPLICADO: já existe livro com o mesmo código.
 *         - Demais códigos de erro vindos de verificar_id_livro ou inserir_no_arvore.
 *
 * @note Esta função não fecha o arquivo. O chamador é responsável por
 *       abrir e fechar o arquivo antes e depois da chamada.
 */
int cadastrar_livro(FILE* arquivo, LIVRO livro);


/**
 * @brief Imprime na saída padrão os dados de um livro com base em seu código.
 *
 * Esta função busca um livro na árvore binária armazenada em arquivo, 
 * utilizando o código informado como chave de busca. Caso o livro seja encontrado, 
 * seus dados são exibidos no formato legível ao usuário. 
 * 
 * @param arquivo Ponteiro para o arquivo binário que contém a árvore de livros.
 *                Deve estar aberto no modo adequado para leitura.
 * @param codigo Código único do livro a ser localizado na árvore.
 * 
 * @return
 * - SUCESSO, se o livro foi encontrado e seus dados foram impressos com êxito.
 * - ERRO_ARQUIVO_NULO, se o ponteiro de arquivo for NULL.
 * - ERRO_LIVRO_INVALIDO, se o código não corresponder a nenhum livro armazenado.
 * 
 * @pre O arquivo deve conter a estrutura de dados da árvore binária previamente construída.
 * @pre A função assume que `buscar_no_arvore` preenche corretamente a estrutura RESULTADO_BUSCA.
 * 
 * @post Caso o livro seja encontrado, `res.no` é desalocado com `free` após o uso.
 * 
 * @note A função imprime diretamente na saída padrão (stdout), 
 *       não havendo armazenamento dos dados em outra estrutura.
 * 
 * @warning Certifique-se de que `arquivo` está aberto e corresponde ao formato esperado, 
 *          pois inconsistências podem causar comportamento indefinido.
 */
int imprimir_dados(FILE* arquivo, size_t codigo);

#endif
