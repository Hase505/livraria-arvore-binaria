/**
 * @file arquivo.h
 * @brief Estruturas e funções para manipular o arquivo binário que armazenará a árvore binária.
 */

#ifndef ARQUIVO_H
#define ARQUIVO_H

#include <stdio.h>

#include "arvore.h"

/**
 * Estrutura que contém informações necessárias
 * para armazenar árvore binária de livros em arquivo.
 */
typedef struct {
        /**
         * Armazena a posição da raiz da árvore binária.
         */
        int raiz;

        /**
         * Armazena a primeira posição não utilizada no fim do arquivo.
         */
        int topo;

        /**
         * Armazena a posição do início da lista de nós livres
         * (encadeada via nó esquerdo da árvore).
         */
        int livre;

        /**
         * Armazena a quantidade total de livros registrados.
         */
        size_t quantidade_livros;
} CABECALHO;

/**
 * @brief Lê cabeçalho inserido em arquivo binário.
 *
 * @param[in] arquivo Ponteiro para o arquivo aberto para leitura.
 * @return Ponteiro para CABECALHO lido do arquivo ou NULL em caso de erro.
 *
 * @pre `arquivo` deve ser diferente de NULL.
 * @pre O ponteiro do arquivo deve estar posicionado corretamente ou a função irá reposicionar.
 *
 * @post Se bem-sucedida, retorna um ponteiro para CABECALHO alocado dinamicamente.
 * @post O ponteiro do arquivo pode estar reposicionado após a leitura.
 */
CABECALHO* le_cabecalho(FILE* arquivo);

/**
 * @brief Escreve o cabeçalho em um arquivo binário.
 *
 * Esta função posiciona o ponteiro do arquivo no início e escreve a estrutura CABECALHO.
 *
 * @param[in,out] arquivo Ponteiro para arquivo aberto em modo de escrita binária.
 * @param[in] cabecalho Ponteiro para estrutura CABECALHO a ser escrita.
 *
 * @return Código de retorno:
 *  - @c SUCESSO (0) em caso de sucesso;
 *  - @c ERRO_ARQUIVO_NULO (-1) se o arquivo for NULL;
 *  - @c ERRO_CABECALHO_NULO (-2) se o ponteiro para o cabeçalho for NULL;
 *  - @c ERRO_ARQUIVO_SEEK (-3) se falhar ao reposicionar o ponteiro do arquivo;
 *  - @c ERRO_ARQUIVO_WRITE (-4) se ocorrer erro na escrita.
 *
 * @pre `arquivo` deve ser um ponteiro válido para arquivo aberto em modo de escrita.
 * @pre `cabecalho` não pode ser NULL.
 *
 * @post Se retornar @c SUCESSO, os dados do cabeçalho terão sido escritos no início do arquivo.
 * @post O ponteiro do arquivo será reposicionado (normalmente para depois do cabeçalho).
 * @post Em caso de erro, o conteúdo do arquivo pode estar indefinido.
 */
int escreve_cabecalho(FILE* arquivo, const CABECALHO* cabecalho);

int inserir_no_arquivo(FILE* arquivo, const NO_ARVORE* no_arvore);
int remover_no_arquivo(FILE* arquivo, const int codigo);
NO_ARVORE* ler_no(FILE* arquivo, const int posicao);

#endif  // ARQUIVO_H
