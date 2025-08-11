#include "../include/livro.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/arquivo.h"
#include "../include/arvore.h"
#include "../include/erros.h"

/**
 * @brief Verifica a existência de um livro na árvore binária pelo código.
 *
 * Esta função busca um nó na árvore binária de busca com o código de livro informado.
 * Retorna SUCESSO caso o livro seja encontrado e o código de erro correspondente caso contrário.
 *
 * @param arquivo Ponteiro para o arquivo binário aberto em modo leitura/escrita ("rb+").
 * @param codigo_livro Código único do livro a ser verificado.
 * @return Código de retorno:
 *         - SUCESSO: livro encontrado na árvore.
 *         - ERRO_ARQUIVO_NULO: ponteiro para arquivo é nulo.
 *         - ERRO_CABECALHO_NULO: cabeçalho não encontrado no arquivo.
 *         - ERRO_NO_NULO: livro não encontrado na árvore.
 *         - Demais códigos de erro vindos de buscar_no_arvore.
 *
 * @note Esta função é interna (static) e deve ser utilizada apenas por funções
 *       que precisam validar a existência de um livro antes de realizar operações.
 * @note A função desaloca automaticamente as estruturas utilizadas na busca.
 */
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
int imprimir_dados(FILE* arquivo, size_t codigo) {
        if (arquivo == NULL) return ERRO_ARQUIVO_NULO;
        RESULTADO_BUSCA res;
        int status = buscar_no_arvore(arquivo, codigo, &res);
        if (status == SUCESSO) {
                printf(
                    "Codigo: %zu\nTitulo: %s\nAutor: %s\nEditora: %s\nEdicao: %zu\nAno: "
                    "%zu\nExemplares: %zu\nPreco: R$ %.2lf\n",
                    res.no->livro.codigo, res.no->livro.titulo, res.no->livro.autor,
                    res.no->livro.editora, res.no->livro.edicao, res.no->livro.ano,
                    res.no->livro.exemplares, res.no->livro.preco);
                free(res.no);
                free(res.pai);
                return SUCESSO;
        }
        printf("Livro com codigo:%zu, nao foi encontrado.", codigo);
        return ERRO_LIVRO_INVALIDO;
}
