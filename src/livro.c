#include "../include/livro.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/arquivo.h"
#include "../include/arvore.h"
#include "../include/erros.h"

static int verificar_id_livro(FILE* arquivo, size_t codigo_livro) {
        if (arquivo == NULL) return ERRO_ARQUIVO_NULO;

        CABECALHO* cabecalho = le_cabecalho(arquivo);
        if (cabecalho == NULL) return ERRO_CABECALHO_NULO;

        int status;

        RESULTADO_BUSCA resultado;
        status = buscar_no_arvore(arquivo, codigo_livro, &resultado);

        free(resultado.no);
        free(resultado.pai);
        free(cabecalho);

        return status;
}

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
int cadastrar_livro(FILE* arquivo, LIVRO livro) {
        if (arquivo == NULL) return ERRO_ARQUIVO_NULO;

        // Verifica se já existe livro com o mesmo código
        int status = verificar_id_livro(arquivo, livro.codigo);
        if (status == SUCESSO) {
                return ERRO_CODIGO_DUPLICADO;
        }

        // Monta o nó da árvore
        NO_ARVORE no_novo;
        no_novo.livro = livro;
        no_novo.filho_esquerdo = POSICAO_INVALIDA;
        no_novo.filho_direito = POSICAO_INVALIDA;

        // Chama inserir_no_arvore
        return inserir_no_arvore(arquivo, &no_novo);
}
