/**
 * @file arquivo.h
 * @brief Estruturas e funções para manipular o arquivo binário que armazenará a árvore binária.
 */

#ifndef ARQUIVO_H
#define ARQUIVO_H

#include <stdio.h>

#include "arvore.h"

#define POSICAO_INVALIDA -1

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

/**
 * @brief Escreve um nó da árvore na posição especificada do arquivo.
 *
 * Posiciona o ponteiro do arquivo na posição correta (após o cabeçalho) e
 * grava o nó informado.
 *
 * @param[in,out] arquivo Ponteiro para arquivo aberto em modo escrita.
 * @param[in] no Ponteiro para o nó que será escrito.
 * @param[in] posicao Índice no arquivo onde o nó será escrito.
 * @return Código de erro: SUCESSO (0) ou código negativo em caso de falha.
 *
 * @pre `arquivo` e `no` devem ser diferentes de NULL.
 * @pre `posicao` deve ser válida (não negativa).
 *
 * @post O nó será escrito na posição indicada do arquivo.
 */
int escrever_no(FILE* arquivo, const NO_ARVORE* no, const int posicao);

/**
 * @brief Insere um nó na árvore no arquivo, utilizando lista livre se disponível.
 *
 * Se existir posição livre (removida anteriormente), reutiliza-a; caso contrário,
 * insere no topo do arquivo, incrementando o topo. Atualiza o cabeçalho.
 *
 * @param[in,out] arquivo Ponteiro para arquivo aberto para leitura e escrita.
 * @param[in] no_arvore Ponteiro para o nó a ser inserido.
 * @param[out] posicao_inserida Ponteiro utilizado para informar qual posição nó foi inserido
 * @return Código de retorno: SUCESSO (0) ou erro específico.
 *
 * @pre `arquivo` e `no_arvore` não podem ser NULL.
 * @pre Arquivo deve conter cabeçalho válido.
 *
 * @post Nó inserido no arquivo, cabeçalho atualizado.
 * @post Valor de posicao_inserida é alterado, refletindo a posição em que o nó foi inserido
 */
int inserir_no_arquivo(FILE* arquivo, const NO_ARVORE* no_arvore, int* posicao_inserida);

/**
 * @brief Remove um nó da árvore no arquivo e o adiciona à lista livre.
 *
 * Marca a posição como livre, atualiza os campos do nó removido e ajusta
 * a lista livre no cabeçalho. Decrementa a quantidade de livros.
 *
 * @param[in,out] arquivo Ponteiro para arquivo aberto para leitura e escrita.
 * @param[in] posicao Índice do nó a ser removido.
 * @return Código de retorno: SUCESSO (0) ou erro específico.
 *
 * @pre `arquivo` não pode ser NULL.
 * @pre `posicao` deve ser válida e existir no arquivo.
 *
 * @post Nó removido é marcado como livre e lista livre atualizada no cabeçalho.
 */
int remover_no_arquivo(FILE* arquivo, const int posicao);

/**
 * @brief Lê um nó da árvore do arquivo na posição especificada.
 *
 * Lê um NO_ARVORE da posição `posicao` no arquivo binário, considerando o
 * cabeçalho no início. Aloca dinamicamente a estrutura que deve ser liberada pelo chamador.
 *
 * @param[in] arquivo Ponteiro para arquivo aberto para leitura.
 * @param[in] posicao Índice do nó a ser lido (posição relativa após o cabeçalho).
 * @return Ponteiro para o nó lido ou NULL em caso de erro.
 *
 * @pre `arquivo` deve ser diferente de NULL.
 * @pre `posicao` deve ser válida (não negativa).
 *
 * @post Se bem-sucedida, retorna um ponteiro para NO_ARVORE alocado dinamicamente.
 * @post Ponteiro do arquivo é reposicionado para a posição do nó.
 */
NO_ARVORE* ler_no_arquivo(FILE* arquivo, const int posicao);

/**
 * @brief Imprime as posições dos nós livres disponíveis na lista livre do arquivo.
 *
 * Percorre a lista encadeada de posições livres começando do campo `livre` do cabeçalho.
 *
 * @param arquivo Ponteiro para arquivo binário aberto para leitura.
 * @return Código de retorno: SUCESSO ou erro.
 */
int imprimir_lista_livre(FILE* arquivo);

#endif  // ARQUIVO_H
