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
int escrever_no(FILE* arquivo, const NO_ARVORE* no, const int posicao) {
        if (arquivo == NULL) return ERRO_ARQUIVO_NULO;
        if (no == NULL) return ERRO_NO_NULO;

        if (fseek(arquivo, sizeof(CABECALHO) + posicao * sizeof(NO_ARVORE), SEEK_SET) != 0)
                return ERRO_ARQUIVO_SEEK;
        if (fwrite(no, sizeof(NO_ARVORE), 1, arquivo) != 1) return ERRO_ARQUIVO_WRITE;

        return SUCESSO;
}

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
int inserir_no_arquivo(FILE* arquivo, const NO_ARVORE* no_arvore, int* posicao_inserida) {
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

                *posicao_inserida = cabecalho->livre;

                cabecalho->livre = no_livre->filho_esquerdo;
                free(no_livre);
        } else {
                int r = escrever_no(arquivo, no_arvore, cabecalho->topo);
                if (r != SUCESSO) {
                        free(cabecalho);
                        return r;
                }

                *posicao_inserida = cabecalho->topo;

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

/**
 * @brief Imprime as posições dos nós livres disponíveis na lista livre do arquivo.
 *
 * Percorre a lista encadeada de posições livres começando do campo `livre` do cabeçalho.
 *
 * @param arquivo Ponteiro para arquivo binário aberto para leitura.
 * @return Código de retorno: SUCESSO ou erro.
 */
int imprimir_lista_livre(FILE* arquivo) {
        if (arquivo == NULL) return ERRO_ARQUIVO_NULO;

        CABECALHO* cabecalho = le_cabecalho(arquivo);
        if (cabecalho == NULL) return ERRO_CABECALHO_NULO;

        int pos = cabecalho->livre;
        printf("Lista de nós livres:\n");
        if (pos == POSICAO_INVALIDA) {
                printf("Nenhum nó livre disponível.\n");
                free(cabecalho);
                return SUCESSO;
        }

        while (pos != POSICAO_INVALIDA) {
                NO_ARVORE* no = ler_no_arquivo(arquivo, pos);
                if (no == NULL) {
                        free(cabecalho);
                        return ERRO_NO_NULO;
                }

                printf("Posição livre: %d\n", pos);
                pos = no->filho_esquerdo;  // próximo nó livre
                free(no);
        }

        free(cabecalho);
        return SUCESSO;
}

int inicializar_arquivo_cabecalho(FILE* arquivo) {
        if (arquivo == NULL) return ERRO_ARQUIVO_NULO;

        // Salva posição atual
        long pos_atual = ftell(arquivo);
        if (pos_atual == -1L) return ERRO_ARQUIVO_NULO;

        // Vai até o fim para descobrir o tamanho
        if (fseek(arquivo, 0, SEEK_END) != 0) return ERRO_ARQUIVO_SEEK;
        long tamanho = ftell(arquivo);
        if (tamanho == -1L) return ERRO_ARQUIVO_NULO;

        if ((size_t)tamanho < sizeof(CABECALHO)) {
                // Arquivo vazio ou menor que o cabeçalho: inicializa
                CABECALHO cab = {0};
                cab.quantidade_livros = 0;
                cab.topo = 0;
                cab.livre = POSICAO_INVALIDA;
                cab.raiz = -1;

                if (fseek(arquivo, 0, SEEK_SET) != 0) return ERRO_ARQUIVO_SEEK;

                if (fwrite(&cab, sizeof(CABECALHO), 1, arquivo) != 1) return ERRO_ARQUIVO_WRITE;

                fflush(arquivo);
        }

        // Retorna para posição anterior
        if (fseek(arquivo, pos_atual, SEEK_SET) != 0) return ERRO_ARQUIVO_SEEK;

        return SUCESSO;
}

void abrir_ou_criar_arquivo(const char* caminho) {
        FILE* arquivo = fopen(caminho, "rb+");
        if (!arquivo) {
                arquivo = fopen(caminho, "wb+");
                if (!arquivo) return;
        }

        if (inicializar_arquivo_cabecalho(arquivo) != SUCESSO) {
                fclose(arquivo);
                return;
        }

        fclose(arquivo);
}
