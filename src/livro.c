#include "../include/livro.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/arquivo.h"
#include "../include/arvore.h"
#include "../include/erros.h"

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
int verificar_id_livro(FILE* arquivo, size_t codigo_livro) {
        CABECALHO* cabecalho = le_cabecalho(arquivo);
        if (!cabecalho) {
                return ERRO_ARQUIVO_READ;
        }

        int pos = cabecalho->raiz;
        NO_ARVORE* no;
        while (pos != -1) {
                no = ler_no_arquivo(arquivo, pos);
                if (!no) {
                        free(cabecalho);
                        return ERRO_ARQUIVO_READ;
                }
                if (no->livro.codigo == codigo_livro) {
                        free(no);
                        free(cabecalho);
                        return ERRO_ARQUIVO_WRITE;  // Já existe livro com esse código
                } else if (codigo_livro < no->livro.codigo) {
                        pos = no->filho_esquerdo;
                } else {
                        pos = no->filho_direito;
                }
                free(no);
        }
        free(cabecalho);
        return SUCESSO;
}

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
NO_ARVORE* buscar_livro(FILE* arquivo, size_t codigo, int* posicao_encontrada) {
        *posicao_encontrada = -1;
        CABECALHO* cabecalho = le_cabecalho(arquivo);

        if (!cabecalho) {
                return NULL;
        }

        int pos = cabecalho->raiz;
        free(cabecalho);

        while (pos != -1) {
                NO_ARVORE* no = ler_no_arquivo(arquivo, pos);
                if (!no) {
                        return NULL;
                }
                if (no->livro.codigo == codigo) {
                        *posicao_encontrada = pos;
                        return no;  // Encontrado → retornamos o nó
                } else if (codigo < no->livro.codigo) {
                        pos = no->filho_esquerdo;
                } else {
                        pos = no->filho_direito;
                }
                free(no);
        }
        return NULL;
}

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
int cadastrar_livro(FILE* arquivo, LIVRO livro) {
        // Verifica se já existe livro com o mesmo código
        int status = verificar_id_livro(arquivo, livro.codigo);
        if (status != SUCESSO) {
                return status;
        }

        // Monta o nó da árvore
        NO_ARVORE no_novo;
        no_novo.livro = livro;
        no_novo.filho_esquerdo = -1;
        no_novo.filho_direito = -1;

        // Chama inserir_no_arvore
        return inserir_no_arvore(arquivo, &no_novo);
}
