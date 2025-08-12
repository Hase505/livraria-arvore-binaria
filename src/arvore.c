#include "../include/arvore.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/arquivo.h"
#include "../include/erros.h"
#include "../include/fila.h"
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
        int posicao_pai = POSICAO_INVALIDA;
        NO_ARVORE* no_atual = ler_no_arquivo(arquivo, posicao_atual);
        if (no_atual == NULL) return ERRO_NO_NULO;

        NO_ARVORE* no_pai = NULL;
        int lado = LADO_INVALIDO;

        while (no_atual->filho_esquerdo != POSICAO_INVALIDA) {
                // Atualiza pai e posição pai
                if (no_pai != NULL) free(no_pai);  // libera anterior, ok porque já não será usado
                no_pai = no_atual;
                posicao_pai = posicao_atual;
                lado = LADO_ESQUERDO;

                posicao_atual = no_atual->filho_esquerdo;
                no_atual = ler_no_arquivo(arquivo, posicao_atual);
                if (no_atual == NULL) {
                        if (no_pai) free(no_pai);
                        return ERRO_NO_NULO;
                }
        }

        // Encontrou o mínimo
        resultado->no = no_atual;
        resultado->posicao_no = posicao_atual;
        resultado->pai = no_pai;
        resultado->posicao_pai = posicao_pai;
        resultado->lado = lado;

        return SUCESSO;
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

/**
 * @brief Função recursiva auxiliar para imprimir os livros da árvore em ordem crescente (in-order).
 *
 * Percorre a árvore binária armazenada em arquivo no modo in-order:
 * visita recursivamente o filho esquerdo, imprime o nó atual e depois o filho direito.
 *
 * @param arquivo Ponteiro para o arquivo binário aberto.
 * @param pos_no Posição (índice) do nó atual no arquivo.
 *
 * @pre `arquivo` deve estar aberto para leitura.
 * @pre `pos_no` deve ser válido ou -1 indicando ausência de nó.
 *
 * @post Imprime os dados do livro na saída padrão.
 */
static void imprimir_in_ordem_rec(FILE* arquivo, int pos_no) {
        if (arquivo == NULL) return;
        if (pos_no == POSICAO_INVALIDA) return;

        NO_ARVORE* no = ler_no_arquivo(arquivo, pos_no);
        if (no == NULL) return;

        imprimir_in_ordem_rec(arquivo, no->filho_esquerdo);

        printf(
            "Codigo: %zu\nTitulo: %s\nAutor: %s\nExemplares: "
            "%zu\n\n",
            no->livro.codigo, no->livro.titulo, no->livro.autor, no->livro.exemplares);

        imprimir_in_ordem_rec(arquivo, no->filho_direito);

        free(no);
}

/**
 * @brief Imprime todos os livros da árvore binária armazenada no arquivo em ordem crescente.
 *
 * Esta função inicia o percurso in-order a partir da raiz da árvore, lendo
 * o cabeçalho para obter a posição da raiz.
 *
 * @param arquivo Ponteiro para o arquivo binário aberto em modo leitura.
 * @return Código de retorno:
 *         - SUCESSO (0) se a operação ocorreu normalmente;
 *         - ERRO_ARQUIVO_NULO se o arquivo for NULL;
 *         - ERRO_CABECALHO_NULO se o cabeçalho não puder ser lido.
 *
 * @pre `arquivo` deve estar aberto para leitura.
 * @post Os dados dos livros são impressos na saída padrão.
 */
int imprimir_in_ordem(FILE* arquivo) {
        if (arquivo == NULL) return ERRO_ARQUIVO_NULO;

        CABECALHO* cabecalho = le_cabecalho(arquivo);
        if (cabecalho == NULL) return ERRO_CABECALHO_NULO;

        int raiz = cabecalho->raiz;
        free(cabecalho);

        if (raiz == POSICAO_INVALIDA) {
                printf("Arvore vazia.\n");
                return SUCESSO;
        }

        imprimir_in_ordem_rec(arquivo, raiz);
        return SUCESSO;
}

/**
 * @brief Atualiza o ponteiro do pai ou raiz para um novo filho.
 *
 * Caso o nó não tenha pai (seja a raiz), atualiza o campo `raiz` no cabeçalho
 * do arquivo. Caso tenha pai, atualiza o ponteiro esquerdo ou direito do pai.
 *
 * @param arquivo Ponteiro para o arquivo da árvore.
 * @param resultado Estrutura contendo informações do nó a ser atualizado.
 * @param posicao_filho Posição do novo filho (ou POSICAO_INVALIDA).
 * @return int Código de status da operação.
 */
static int atualizar_pai_ou_raiz(FILE* arquivo, RESULTADO_BUSCA* resultado, int posicao_filho) {
        if (arquivo == NULL) return ERRO_ARQUIVO_NULO;
        if (resultado == NULL) return ERRO_RESULTADO_BUSCA_NULO;

        int status;

        if (resultado->pai == NULL) {  // Atualiza a raiz
                CABECALHO* cabecalho = le_cabecalho(arquivo);
                if (cabecalho == NULL) return ERRO_CABECALHO_NULO;

                cabecalho->raiz = posicao_filho;
                status = escreve_cabecalho(arquivo, cabecalho);

                free(cabecalho);
                return status;
        } else {  // Atualiza ponteiro do pai
                if (resultado->lado == LADO_ESQUERDO)
                        resultado->pai->filho_esquerdo = posicao_filho;
                else if (resultado->lado == LADO_DIREITO)
                        resultado->pai->filho_direito = posicao_filho;
                else
                        return ERRO_NO_NULO;

                return escrever_no(arquivo, resultado->pai, resultado->posicao_pai);
        }
}

/**
 * @brief Remove um nó folha da árvore.
 *
 * Atualiza o ponteiro do pai ou raiz para POSICAO_INVALIDA e libera o nó no arquivo.
 *
 * @param arquivo Ponteiro para o arquivo da árvore.
 * @param resultado Estrutura contendo informações do nó a ser removido.
 * @return int Código de status da operação.
 */
static int remover_no_folha(FILE* arquivo, RESULTADO_BUSCA* resultado) {
        if (arquivo == NULL) return ERRO_ARQUIVO_NULO;
        if (resultado == NULL) return ERRO_RESULTADO_BUSCA_NULO;

        int status = remover_no_arquivo(arquivo, resultado->posicao_no);
        if (status != SUCESSO) return status;

        return atualizar_pai_ou_raiz(arquivo, resultado, POSICAO_INVALIDA);
}

/**
 * @brief Remove um nó interno da árvore, substituindo pelo sucessor ou antecessor.
 *
 * Caso tenha filho direito, substitui pelo nó mínimo da subárvore direita (sucessor).
 * Caso contrário, substitui pelo nó máximo da subárvore esquerda (antecessor).
 * O nó substituto é sempre uma folha ou nó com um único filho.
 *
 * @param arquivo Ponteiro para o arquivo da árvore.
 * @param resultado Estrutura contendo informações do nó a ser removido.
 * @return int Código de status da operação.
 */
static int remover_no_interno(FILE* arquivo, RESULTADO_BUSCA* resultado) {
        if (arquivo == NULL) return ERRO_ARQUIVO_NULO;
        if (resultado == NULL) return ERRO_RESULTADO_BUSCA_NULO;

        // Caso especial: nó com apenas um filho
        if (resultado->no->filho_esquerdo == POSICAO_INVALIDA ||
            resultado->no->filho_direito == POSICAO_INVALIDA) {
                int filho = (resultado->no->filho_esquerdo != POSICAO_INVALIDA)
                                ? resultado->no->filho_esquerdo
                                : resultado->no->filho_direito;

                // Libera o nó e aponta pai/raiz para o filho
                int status = remover_no_arquivo(arquivo, resultado->posicao_no);
                if (status != SUCESSO) return status;

                return atualizar_pai_ou_raiz(arquivo, resultado, filho);
        }

        // Caso clássico: nó com dois filhos
        int status;
        RESULTADO_BUSCA res_sub = {0};

        if (resultado->no->filho_direito != POSICAO_INVALIDA) {
                status = buscar_no_minimo(arquivo, resultado->no->filho_direito, &res_sub);
                if (status != SUCESSO) {
                        liberar_resultado_busca(&res_sub);
                        return status;
                }

                resultado->no->livro = res_sub.no->livro;
                status = escrever_no(arquivo, resultado->no, resultado->posicao_no);
                if (status != SUCESSO) {
                        liberar_resultado_busca(&res_sub);
                        return status;
                }

                int pos_filho_substituto = res_sub.no->filho_direito;
                status = atualizar_pai_ou_raiz(arquivo, &res_sub, pos_filho_substituto);
                if (status != SUCESSO) {
                        liberar_resultado_busca(&res_sub);
                        return status;
                }

                status = remover_no_arquivo(arquivo, res_sub.posicao_no);
                liberar_resultado_busca(&res_sub);
                return status;
        }

        return ERRO_NO_NULO;
}

/**
 * @brief Remove um nó da árvore binária de busca no arquivo.
 *
 * Determina se o nó é folha ou interno e chama a função apropriada.
 *
 * @param arquivo Ponteiro para o arquivo da árvore.
 * @param codigo Código do livro a ser removido.
 * @return int Código de status da operação.
 */
int remover_no_arvore(FILE* arquivo, size_t codigo) {
        if (arquivo == NULL) return ERRO_ARQUIVO_NULO;

        CABECALHO* cabecalho = le_cabecalho(arquivo);
        if (cabecalho == NULL) return ERRO_CABECALHO_NULO;

        if (cabecalho->raiz == POSICAO_INVALIDA) {
                free(cabecalho);
                return ERRO_NO_NULO;
        }

        int status;
        RESULTADO_BUSCA resultado = {0};
        status = buscar_no_arvore(arquivo, codigo, &resultado);
        if (status != SUCESSO) {
                free(cabecalho);
                liberar_resultado_busca(&resultado);
                return ERRO_NO_NULO;
        }

        if (resultado.no->filho_esquerdo == POSICAO_INVALIDA &&
            resultado.no->filho_direito == POSICAO_INVALIDA) {
                status = remover_no_folha(arquivo, &resultado);
        } else {
                status = remover_no_interno(arquivo, &resultado);
        }

        free(cabecalho);
        liberar_resultado_busca(&resultado);
        return status;
}

/**
 * @brief Imprime a árvore binária armazenada em arquivo por níveis (ordem por largura).
 *
 * Esta função realiza uma travessia em largura (level order) na árvore binária de busca,
 * lendo os nós do arquivo e imprimindo os códigos dos livros por nível, um nível por linha.
 *
 * @param arquivo Ponteiro para o arquivo binário aberto em modo leitura.
 * @return Código de status da operação:
 *         - SUCESSO: árvore impressa com sucesso ou árvore vazia.
 *         - ERRO_ARQUIVO_NULO: ponteiro para arquivo é NULL.
 *         - ERRO_CABECALHO_NULO: cabeçalho não encontrado no arquivo.
 *         - ERRO_FILA_NULA: falha ao criar fila para a travessia.
 *         - ERRO_FILA_CHEIA: erro ao enfileirar nó na fila.
 *         - ERRO_NO_NULO: erro ao ler nó da árvore no arquivo.
 *
 * @pre O arquivo deve conter uma árvore binária previamente construída e um cabeçalho válido.
 * @pre Funções auxiliares como criar_fila, enfileirar, desenfileirar, destruir_fila e
 * ler_no_arquivo devem estar implementadas e funcionando corretamente.
 *
 * @post A função imprime no stdout os códigos dos livros da árvore, um nível por linha.
 *
 * @note Caso a árvore esteja vazia (raiz inválida), a função retorna SUCESSO sem imprimir nada.
 */
int imprimir_arvore_por_niveis(FILE* arquivo) {
        if (arquivo == NULL) return ERRO_ARQUIVO_NULO;

        CABECALHO* cabecalho = le_cabecalho(arquivo);
        if (cabecalho == NULL) return ERRO_CABECALHO_NULO;

        if (cabecalho->raiz == POSICAO_INVALIDA) {
                free(cabecalho);
                return SUCESSO;  // árvore vazia
        }

        FILA* fila = criar_fila();
        if (fila == NULL) {
                free(cabecalho);
                return ERRO_FILA_NULA;
        }

        if (enfileirar(fila, cabecalho->raiz, 0) != SUCESSO) {
                destruir_fila(fila);
                free(cabecalho);
                return ERRO_FILA_CHEIA;
        }

        int nivel_atual = 0;

        while (!fila_vazia(fila)) {
                ITEM_FILA item = desenfileirar(fila);
                if (item.posicao == -1) break;  // fila vazia, segurança

                NO_ARVORE* no = ler_no_arquivo(arquivo, item.posicao);
                if (no == NULL) {
                        destruir_fila(fila);
                        free(cabecalho);
                        return ERRO_NO_NULO;
                }

                if (item.nivel != nivel_atual) {
                        printf("\n");
                        nivel_atual = item.nivel;
                }

                printf("%zu ", no->livro.codigo);

                if (no->filho_esquerdo != POSICAO_INVALIDA)
                        enfileirar(fila, no->filho_esquerdo, item.nivel + 1);

                if (no->filho_direito != POSICAO_INVALIDA)
                        enfileirar(fila, no->filho_direito, item.nivel + 1);

                free(no);
        }

        printf("\n");
        destruir_fila(fila);
        free(cabecalho);
        return SUCESSO;
}
