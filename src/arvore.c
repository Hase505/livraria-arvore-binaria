#include "../include/arvore.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/arquivo.h"
#include "../include/erros.h"
#include "../include/livro.h"

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
int buscar_no_arvore(FILE* arquivo, size_t codigo, RESULTADO_BUSCA* resultado) {
        if (arquivo == NULL) return ERRO_ARQUIVO_NULO;

        CABECALHO* cabecalho = le_cabecalho(arquivo);
        if (cabecalho == NULL) return ERRO_CABECALHO_NULO;
        if (cabecalho->raiz == POSICAO_INVALIDA) return ERRO_NO_NULO;

        int posicao_atual = cabecalho->raiz;
        int posicao_pai = POSICAO_INVALIDA;

        NO_ARVORE* no_atual = NULL;
        NO_ARVORE* no_pai = NULL;

        lado_filho lado = LADO_INVALIDO;

        while (posicao_atual != POSICAO_INVALIDA) {
                no_atual = ler_no_arquivo(arquivo, posicao_atual);
                if (no_atual == NULL) {
                        free(cabecalho);
                        free(no_pai);
                        return ERRO_NO_NULO;
                }

                if (no_atual->livro.codigo == codigo) break;

                posicao_pai = posicao_atual;

                // libera nó pai antes de atualizar
                free(no_pai);
                no_pai = no_atual;

                if (codigo < no_atual->livro.codigo) {
                        posicao_atual = no_atual->filho_esquerdo;
                        lado = LADO_ESQUERDO;
                } else {
                        posicao_atual = no_atual->filho_direito;
                        lado = LADO_DIREITO;
                }
        }

        if (posicao_atual == POSICAO_INVALIDA) {
                free(cabecalho);
                free(no_atual);
                free(no_pai);
                return ERRO_NO_NULO;
        }

        resultado->no = no_atual;
        resultado->pai = no_pai;
        resultado->posicao_no = posicao_atual;
        resultado->posicao_pai = posicao_pai;
        resultado->lado = lado;

        free(cabecalho);

        return SUCESSO;
}
