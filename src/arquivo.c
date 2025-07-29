/**
 * @file arquivo.c
 * @brief Implementa as funções para manipular o arquivo binário que armazenará a árvore binária.
 */

#include "../include/arquivo.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/erros.h"

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
