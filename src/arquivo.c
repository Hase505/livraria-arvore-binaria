/**
 * @file arquivo.c
 * @brief Implementa as funções para manipular o arquivo binário que armazenará a árvore binária.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/arquivo.h"
#include "../include/arvore.h"
#include "../include/erros.h"

#define POSICAO_INVALIDA -1

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
CABECALHO* le_cabecalho(FILE* arquivo) {
        if (arquivo == NULL) return NULL;

        if (fseek(arquivo, 0, SEEK_SET) != 0) return NULL;

        CABECALHO* cabecalho = malloc(sizeof(CABECALHO));
        if (cabecalho == NULL) return NULL;

        if (fread(cabecalho, sizeof(CABECALHO), 1, arquivo) != 1) {
                free(cabecalho);
                return NULL;
        }

        return cabecalho;
}

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
int escreve_cabecalho(FILE* arquivo, const CABECALHO* cabecalho) {
        if (fseek(arquivo, 0, SEEK_SET) != 0) return ERRO_ARQUIVO_SEEK;
        if (fwrite(cabecalho, sizeof(CABECALHO), 1, arquivo) != 1) return ERRO_ARQUIVO_WRITE;

        return SUCESSO;
}

NO_ARVORE* ler_no_arquivo(FILE* arquivo, const int posicao) {
        if (arquivo == NULL) return NULL;

        NO_ARVORE* no = malloc(sizeof(NO_ARVORE));
        if (no == NULL) return NULL;

        if (fseek(arquivo, sizeof(CABECALHO) + posicao * sizeof(NO_ARVORE), SEEK_SET) != 0) {
                free(no);
                return NULL;
        }
        if (fread(no, sizeof(NO_ARVORE), 1, arquivo) != 1) {
                free(no);
                return NULL;
        }

        return no;
}

static int escrever_no(FILE* arquivo, const NO_ARVORE* no, const int posicao) {
        if (arquivo == NULL) return ERRO_ARQUIVO_NULO;
        if (no == NULL) return ERRO_NO_NULO;

        if (fseek(arquivo, sizeof(CABECALHO) + posicao * sizeof(NO_ARVORE), SEEK_SET) != 0)
                return ERRO_ARQUIVO_SEEK;
        if (fwrite(no, sizeof(NO_ARVORE), 1, arquivo) != 1) return ERRO_ARQUIVO_WRITE;

        return SUCESSO;
}

int inserir_no_arquivo(FILE* arquivo, const NO_ARVORE* no_arvore) {
        if (arquivo == NULL) return ERRO_ARQUIVO_NULO;

        if (no_arvore == NULL) return ERRO_NO_NULO;

        CABECALHO* cabecalho = le_cabecalho(arquivo);
        if (cabecalho == NULL) return ERRO_CABECALHO_NULO;

        if (cabecalho->livre != POSICAO_INVALIDA) {
                NO_ARVORE* no_livre = ler_no_arquivo(arquivo, cabecalho->livre);
                if (no_livre == NULL) {
                        free(cabecalho);
                        return ERRO_NO_NULO;
                }

                int r = escrever_no(arquivo, no_arvore, cabecalho->livre);
                if (r != SUCESSO) {
                        free(cabecalho);
                        free(no_livre);
                        return r;
                }

                cabecalho->livre = no_livre->filho_esquerdo;
                free(no_livre);
        } else {
                int r = escrever_no(arquivo, no_arvore, cabecalho->topo);
                if (r != SUCESSO) {
                        free(cabecalho);
                        return r;
                }

                cabecalho->topo++;
        }

        cabecalho->quantidade_livros++;
        int r = escreve_cabecalho(arquivo, cabecalho);
        if (r != SUCESSO) {
                free(cabecalho);
                return r;
        }

        free(cabecalho);

        return SUCESSO;
}

int remover_no_arquivo(FILE* arquivo, const int posicao) {
        if (arquivo == NULL) return ERRO_ARQUIVO_NULO;

        CABECALHO* cabecalho = le_cabecalho(arquivo);
        if (cabecalho == NULL) return ERRO_CABECALHO_NULO;

        NO_ARVORE* no_removido = ler_no_arquivo(arquivo, posicao);
        if (no_removido == NULL) {
                free(cabecalho);
                return ERRO_NO_NULO;
        }

        memset(&no_removido->livro, 0, sizeof(LIVRO));
        no_removido->filho_direito = POSICAO_INVALIDA;
        no_removido->filho_esquerdo = cabecalho->livre;

        cabecalho->livre = posicao;
        cabecalho->quantidade_livros--;

        int r = escrever_no(arquivo, no_removido, posicao);
        if (r != SUCESSO) {
                free(cabecalho);
                free(no_removido);
                return r;
        }

        r = escreve_cabecalho(arquivo, cabecalho);
        if (r != SUCESSO) {
                free(cabecalho);
                free(no_removido);
                return r;
        }

        free(cabecalho);
        free(no_removido);

        return SUCESSO;
}
