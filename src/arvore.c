#include "../include/arvore.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/arquivo.h"
#include "../include/erros.h"
#include "../include/livro.h"

/**
 * @brief Busca o nó com o menor valor a partir de uma posição inicial na árvore.
 *
 * Percorre a árvore binária de busca (armazenada em arquivo) a partir de uma posição inicial,
 * sempre seguindo para o filho à esquerda até encontrar o nó mais à esquerda (mínimo).
 * Preenche a estrutura RESULTADO_BUSCA com o nó encontrado, seu pai e as posições correspondentes.
 *
 * @param arquivo Ponteiro para o arquivo que contém a árvore.
 * @param posicao_inicial Posição do nó inicial para a busca.
 * @param resultado Ponteiro para estrutura RESULTADO_BUSCA onde o resultado será armazenado.
 *
 * @return SUCESSO se encontrou o nó mínimo.
 * @return ERRO_ARQUIVO_NULO se o ponteiro para o arquivo for nulo.
 * @return ERRO_NO_NULO se a posição inicial for inválida ou se ocorrer erro ao ler um nó.
 */
static int buscar_no_minimo(FILE* arquivo, int posicao_inicial, RESULTADO_BUSCA* resultado) {
        if (arquivo == NULL) return ERRO_ARQUIVO_NULO;
        if (posicao_inicial == POSICAO_INVALIDA) return ERRO_NO_NULO;

        int posicao_atual = posicao_inicial;
        NO_ARVORE* no_atual = NULL;
        NO_ARVORE* no_pai = NULL;
        int posicao_pai = POSICAO_INVALIDA;
        int lado = LADO_INVALIDO;

        while (posicao_atual != POSICAO_INVALIDA) {
                NO_ARVORE* no_lido = ler_no_arquivo(arquivo, posicao_atual);
                if (no_lido == NULL) {
                        free(no_atual);
                        free(no_pai);
                        return ERRO_NO_NULO;
                }

                if (no_lido->filho_esquerdo == POSICAO_INVALIDA) {
                        // Encontrou o mínimo
                        resultado->no = no_lido;
                        resultado->posicao_no = posicao_atual;
                        resultado->pai = no_pai;
                        resultado->posicao_pai = posicao_pai;
                        resultado->lado = lado;
                        return SUCESSO;
                }

                // Avança para a esquerda
                free(no_pai);
                no_pai = no_atual;
                posicao_pai = posicao_atual;
                lado = LADO_ESQUERDO;

                no_atual = no_lido;
                posicao_atual = no_lido->filho_esquerdo;
        }

        return ERRO_NO_NULO;
}

/**
 * @brief Busca o nó com o maior valor a partir de uma posição inicial na árvore.
 *
 * Percorre a árvore binária de busca (armazenada em arquivo) a partir de uma posição inicial,
 * sempre seguindo para o filho à direita até encontrar o nó mais à direita (máximo).
 * Preenche a estrutura RESULTADO_BUSCA com o nó encontrado, seu pai e as posições correspondentes.
 *
 * @param arquivo Ponteiro para o arquivo que contém a árvore.
 * @param posicao_inicial Posição do nó inicial para a busca.
 * @param resultado Ponteiro para estrutura RESULTADO_BUSCA onde o resultado será armazenado.
 *
 * @return SUCESSO se encontrou o nó máximo.
 * @return ERRO_ARQUIVO_NULO se o ponteiro para o arquivo for nulo.
 * @return ERRO_NO_NULO se a posição inicial for inválida ou se ocorrer erro ao ler um nó.
 */
static int buscar_no_maximo(FILE* arquivo, int posicao_inicial, RESULTADO_BUSCA* resultado) {
        if (arquivo == NULL) return ERRO_ARQUIVO_NULO;
        if (posicao_inicial == POSICAO_INVALIDA) return ERRO_NO_NULO;

        int posicao_atual = posicao_inicial;
        NO_ARVORE* no_atual = NULL;
        NO_ARVORE* no_pai = NULL;
        int posicao_pai = POSICAO_INVALIDA;
        int lado = LADO_INVALIDO;

        while (posicao_atual != POSICAO_INVALIDA) {
                NO_ARVORE* no_lido = ler_no_arquivo(arquivo, posicao_atual);
                if (no_lido == NULL) {
                        free(no_atual);
                        free(no_pai);
                        return ERRO_NO_NULO;
                }

                if (no_lido->filho_direito == POSICAO_INVALIDA) {
                        // Encontrou o máximo
                        resultado->no = no_lido;
                        resultado->posicao_no = posicao_atual;
                        resultado->pai = no_pai;
                        resultado->posicao_pai = posicao_pai;
                        resultado->lado = lado;
                        return SUCESSO;
                }

                // Avança para a direita
                free(no_pai);
                no_pai = no_atual;
                posicao_pai = posicao_atual;
                lado = LADO_DIREITO;

                no_atual = no_lido;
                posicao_atual = no_lido->filho_direito;
        }

        return ERRO_NO_NULO;
}

/**
 * @brief Libera a memória alocada para um resultado de busca na árvore.
 *
 * Libera os ponteiros `no` e `pai` da estrutura RESULTADO_BUSCA e
 * redefine as posições e o lado como inválidos.
 *
 * @param resultado Ponteiro para a estrutura RESULTADO_BUSCA a ser liberada.
 */
static void liberar_resultado_busca(RESULTADO_BUSCA* resultado) {
        if (!resultado) return;
        if (resultado->no) {
                free(resultado->no);
                resultado->no = NULL;
        }
        if (resultado->pai) {
                free(resultado->pai);
                resultado->pai = NULL;
        }
        resultado->posicao_no = POSICAO_INVALIDA;
        resultado->posicao_pai = POSICAO_INVALIDA;
        resultado->lado = LADO_INVALIDO;
}

/**
 * @brief Busca um nó na árvore binária de busca armazenada no arquivo.
 *
 * Esta função percorre a árvore binária de busca persistida em arquivo,
 * a partir da raiz, até encontrar o nó cujo código do livro seja igual
 * ao informado. Caso o nó seja encontrado, informações sobre sua posição,
 * pai e lado em relação ao pai são preenchidas na estrutura RESULTADO_BUSCA.
 *
 * Se o nó não for encontrado, a função retorna as informações do último nó
 * visitado (pai) e o lado onde a inserção deveria ocorrer.
 *
 * @param arquivo Ponteiro para o arquivo binário aberto em modo leitura/escrita ("rb+").
 * @param codigo Código único do livro a ser buscado.
 * @param resultado Ponteiro para estrutura RESULTADO_BUSCA onde os dados do nó
 *        encontrado (ou informações para inserção) serão armazenados.
 * @return
 * - `SUCESSO` se o nó foi encontrado.
 * - `ERRO_ARQUIVO_NULO` se o ponteiro de arquivo for nulo.
 * - `ERRO_CABECALHO_NULO` se o cabeçalho não puder ser lido.
 * - `ERRO_NO_NULO` se a árvore estiver vazia ou o nó não for encontrado.
 *
 * @note O chamador é responsável por liberar a memória alocada para `resultado->no`
 *       e `resultado->pai` quando não forem mais necessários.
 */
int buscar_no_arvore(FILE* arquivo, size_t codigo, RESULTADO_BUSCA* resultado) {
        if (arquivo == NULL) return ERRO_ARQUIVO_NULO;

        CABECALHO* cabecalho = le_cabecalho(arquivo);
        if (cabecalho == NULL) return ERRO_CABECALHO_NULO;

        if (cabecalho->raiz == POSICAO_INVALIDA) {
                // Árvore vazia: não há pai, lado inválido
                resultado->no = NULL;
                resultado->pai = NULL;
                resultado->posicao_no = POSICAO_INVALIDA;
                resultado->posicao_pai = POSICAO_INVALIDA;
                resultado->lado = LADO_INVALIDO;
                free(cabecalho);
                return ERRO_NO_NULO;
        }

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

                if (no_atual->livro.codigo == codigo) {
                        resultado->no = no_atual;
                        resultado->pai = no_pai;
                        resultado->posicao_no = posicao_atual;
                        resultado->posicao_pai = posicao_pai;
                        resultado->lado = lado;
                        free(cabecalho);
                        return SUCESSO;
                }

                posicao_pai = posicao_atual;
                free(no_pai);  // libera pai antigo
                no_pai = no_atual;

                if (codigo < no_atual->livro.codigo) {
                        posicao_atual = no_atual->filho_esquerdo;
                        lado = LADO_ESQUERDO;
                } else {
                        posicao_atual = no_atual->filho_direito;
                        lado = LADO_DIREITO;
                }
        }

        // Não encontrou, mas retornamos info para inserção
        resultado->no = NULL;  // não encontrado
        resultado->pai = no_pai;
        resultado->posicao_no = POSICAO_INVALIDA;
        resultado->posicao_pai = posicao_pai;
        resultado->lado = lado;

        free(cabecalho);
        return ERRO_NO_NULO;
}

/**
 * @brief Insere um novo nó na árvore binária de busca armazenada no arquivo.
 *
 * Esta função insere um novo nó (representando um livro) na árvore binária
 * de busca persistida em arquivo. A função verifica se o código do livro já
 * existe na árvore e, caso exista, retorna erro.
 *
 * Caso a árvore esteja vazia, o novo nó será definido como raiz. Caso contrário,
 * ele será inserido como filho esquerdo ou direito do nó pai, de acordo com
 * a ordem binária de busca.
 *
 * @param arquivo Ponteiro para arquivo binário aberto em modo leitura/escrita ("rb+").
 * @param novo Ponteiro para estrutura NO_ARVORE a ser inserida.
 * @return
 * - `SUCESSO` em caso de inserção bem-sucedida.
 * - `ERRO_ARQUIVO_NULO` se o ponteiro de arquivo for nulo.
 * - `ERRO_NO_NULO` se o ponteiro para o novo nó for nulo.
 * - `ERRO_CODIGO_DUPLICADO` se já existir um livro com o mesmo código.
 * - Outros códigos de erro definidos em erros.h podem ser retornados
 *   dependendo do erro ocorrido durante gravação no arquivo.
 *
 * @warning Sempre feche (`fclose`) o arquivo após inserir um livro e reabra antes de
 * buscar ou verificar. Isso garante que os dados estejam sincronizados no disco.
 *
 * @note Esta função chama internamente `inserir_no_arquivo()` para decidir se
 *       utilizará a lista livre ou adicionar no final do arquivo.
 */
int inserir_no_arvore(FILE* arquivo, NO_ARVORE* novo) {
        if (arquivo == NULL) return ERRO_ARQUIVO_NULO;
        if (novo == NULL) return ERRO_NO_NULO;

        RESULTADO_BUSCA res;
        int status = buscar_no_arvore(arquivo, novo->livro.codigo, &res);

        if (status == SUCESSO) {
                // Já existe um nó com este código
                if (res.no) free(res.no);
                if (res.pai) free(res.pai);
                return ERRO_CODIGO_DUPLICADO;
        }

        // Caso especial: árvore vazia
        if (res.pai == NULL) {
                int pos_novo;
                status = inserir_no_arquivo(arquivo, novo, &pos_novo);
                if (status != SUCESSO) {
                        return status;
                }
                // Atualiza cabeçalho
                CABECALHO* cab = le_cabecalho(arquivo);
                if (!cab) return ERRO_CABECALHO_NULO;
                cab->raiz = pos_novo;
                status = escreve_cabecalho(arquivo, cab);
                free(cab);
                return status;
        }

        // Inserir o novo nó no arquivo
        int pos_novo;
        status = inserir_no_arquivo(arquivo, novo, &pos_novo);
        if (status != SUCESSO) {
                if (res.pai) free(res.pai);
                return status;
        }

        // Atualizar ponteiro do pai
        if (res.lado == LADO_ESQUERDO)
                res.pai->filho_esquerdo = pos_novo;
        else if (res.lado == LADO_DIREITO)
                res.pai->filho_direito = pos_novo;
        else {
                if (res.pai) free(res.pai);
                return ERRO_NO_NULO;  // lado inválido
        }

        // Gravar pai atualizado
        status = escrever_no(arquivo, res.pai, res.posicao_pai);

        if (res.pai) free(res.pai);
        return status;
}
