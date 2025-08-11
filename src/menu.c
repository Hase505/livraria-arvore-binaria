#include "../include/menu.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/arquivo.h"
#include "../include/arvore.h"
#include "../include/erros.h"
#include "../include/livro.h"
#include "../include/utils.h"

/**
 * @brief Remove o caractere de nova linha '\n' do final da string.
 *
 * @param str String que terá o '\n' removido.
 */
void limpar_enter(char* str) {
        str[strcspn(str, "\n")] = '\0';
}

/**
 * @brief Exibe o menu principal com as opções do sistema.
 */
void exibir_menu() {
        printf("====MENU PRINCIPAL====\n");
        printf("1  - CADASTRAR LIVRO\n");
        printf("2  - IMPRIMIR DADOS DO LIVRO\n");
        printf("3  - LISTAR TODOS OS LIVROS\n");
        printf("4  - CALCULAR TOTAL\n");
        printf("5  - REMOVER LIVRO\n");
        printf("6  - CARREGAR ARQUIVO\n");
        printf("7  - IMPRIMIR LISTA DE REGISTROS LIVRES\n");
        printf("8  - IMPRIMIR ARVORE POR NIVEIS\n");
        printf("0  - SAIR\n");
        printf("========================\n");
}

/**
 * @brief Realiza o cadastro de um livro, lendo dados do usuário e salvando no arquivo binário.
 *
 * @param caminho_livros Caminho para o arquivo binário onde os livros estão salvos.
 * @return int Código de status da operação (SUCESSO ou erro).
 */
int opcao_cadastrar_livro(char* caminho_livros) {
        LIVRO livro = {0};

        printf("Codigo do livro: ");
        while ((livro.codigo = ler_size_t()) <= 0) {
                printf("Codigo invalido (deve ser um numero maior que zero)\n\n");
                printf("Codigo do livro: ");
        }
        printf("\n");

        printf("Titulo: ");
        fgets(livro.titulo, MAX_TITULO + 1, stdin);
        limpar_enter(livro.titulo);
        printf("\n");

        printf("Autor: ");
        fgets(livro.autor, MAX_AUTOR + 1, stdin);
        limpar_enter(livro.autor);
        printf("\n");

        printf("Editora: ");
        fgets(livro.editora, MAX_EDITORA + 1, stdin);
        limpar_enter(livro.editora);
        printf("\n");

        printf("Edicao: ");
        while ((livro.edicao = ler_size_t()) <= 0) {
                printf("Edicao invalida (deve ser um numero maior que zero)\n\n");
                printf("Edicao: ");
        }
        printf("\n");

        printf("Ano: ");
        while ((livro.ano = ler_size_t()) <= 0) {
                printf("Ano invalido (deve ser um numero maior que zero)\n\n");
                printf("Ano: ");
        }
        printf("\n");

        printf("Quantidade de exemplares: ");
        while (!ler_size_t_com_zero(&livro.exemplares)) {
                printf("Digite um valor valido (nao negativo)\n\n");
                printf("Quantidade de exemplares: ");
        }
        printf("\n");

        printf("Preco: ");
        livro.preco = ler_double();
        printf("\n");

        int resposta;
        FILE* arquivo = fopen(caminho_livros, "rb+");
        if (arquivo == NULL) {
                return ERRO_ARQUIVO_NULO;
        }
        if ((resposta = cadastrar_livro(arquivo, livro)) != SUCESSO) {
                printf("Erro ao cadastrar livro");
                if (resposta == ERRO_CODIGO_DUPLICADO)
                        printf(": Livro com codigo ja cadastrado\n");
                else
                        printf("\n");
                printf("\n");
                fclose(arquivo);
                return ERRO_CADASTRAR_LIVRO;
        } else {
                printf("Livro \"%s\" cadastrado com sucesso!\n\n", livro.titulo);
                fclose(arquivo);
                return SUCESSO;
        }
}

/**
 * @brief Imprime os dados de um livro dado o código informado pelo usuário.
 *
 * @param caminho_livros Caminho do arquivo binário com os livros.
 * @return int Código de status da operação (SUCESSO ou erro).
 */
int opcao_imprimir_dados(char* caminho_livros) {
        printf("Insira o codigo do livro: ");
        size_t codigo = ler_size_t();
        printf("\n");

        FILE* arquivo = fopen(caminho_livros, "rb");
        if (!arquivo) return ERRO_ARQUIVO_NULO;

        int status = imprimir_dados(arquivo, codigo);
        fclose(arquivo);

        printf("\n");

        return status;
}

/**
 * @brief Lista todos os livros presentes no arquivo binário.
 *
 * @param caminho_livros Caminho do arquivo binário com os livros.
 * @return int Código de status da operação.
 */
int opcao_listar_todos(char* caminho_livros) {
        FILE* arquivo = fopen(caminho_livros, "rb");
        if (!arquivo) return ERRO_ARQUIVO_NULO;

        int status = imprimir_in_ordem(arquivo);
        fclose(arquivo);

        return status;
}

/**
 * @brief Calcula e exibe o total de livros cadastrados no sistema.
 *
 * @param caminho_livros Caminho do arquivo binário com os livros.
 * @return int Código de status da operação.
 */
int opcao_calcular_total(char* caminho_livros) {
        FILE* arquivo = fopen(caminho_livros, "rb");
        if (!arquivo) return ERRO_ARQUIVO_NULO;

        CABECALHO* cab = le_cabecalho(arquivo);
        if (!cab) {
                fclose(arquivo);
                return ERRO_CABECALHO_NULO;
        }

        printf("Total de livros cadastrados: %zu\n", cab->quantidade_livros);

        free(cab);
        fclose(arquivo);

        printf("\n");

        return SUCESSO;
}

/**
 * @brief Remove um livro do sistema dado seu código.
 *
 * @param caminho_livros Caminho do arquivo binário com os livros.
 * @return int Código de status da operação.
 */
int opcao_remover_livro(char* caminho_livros) {
        printf("Codigo do livro a remover: ");
        size_t codigo = ler_size_t();

        FILE* arquivo = fopen(caminho_livros, "rb+");
        if (!arquivo) return ERRO_ARQUIVO_NULO;

        int status = remover_no_arvore(arquivo, codigo);
        fclose(arquivo);

        printf("\n");

        return status;
}

/**
 * @brief Imprime a lista de registros livres do arquivo binário.
 *
 * @param caminho Caminho do arquivo binário.
 * @return int Código de status da operação.
 */
int opcao_imprimir_lista_livre(const char* caminho) {
        FILE* arq = fopen(caminho, "rb");
        if (!arq) {
                printf("Erro ao abrir arquivo\n");
                return ERRO_ARQUIVO_NULO;
        }

        printf("Lista de nós livres: \n\n");
        int r = imprimir_lista_livre(arq);

        fclose(arq);

        printf("\n");

        return r;
}

/**
 * @brief Função auxiliar que lê um arquivo texto já aberto e cadastra os livros no arquivo binário.
 *
 * @param txt Ponteiro para o arquivo texto aberto para leitura.
 * @param arq_bin Ponteiro para o arquivo binário aberto para leitura/escrita.
 * @return int Código de status da operação.
 */
static int ler_txt(FILE* txt, FILE* arq_bin) {
        char linha[512];
        while (fgets(linha, sizeof(linha), txt)) {
                LIVRO livro = {0};
                char preco_str[50];
                char* token = strtok(linha, ";");
                if (!token) continue;
                trim(token);
                livro.codigo = (size_t)strtoull(token, NULL, 10);

                token = strtok(NULL, ";");
                if (!token) continue;
                trim(token);
                strncpy(livro.titulo, token, sizeof(livro.titulo) - 1);
                livro.titulo[sizeof(livro.titulo) - 1] = '\0';
                livro.titulo[strcspn(livro.titulo, "\n")] = '\0';

                token = strtok(NULL, ";");
                if (!token) continue;
                trim(token);
                strncpy(livro.autor, token, sizeof(livro.autor) - 1);
                livro.autor[sizeof(livro.autor) - 1] = '\0';
                livro.autor[strcspn(livro.autor, "\n")] = '\0';

                token = strtok(NULL, ";");
                if (!token) continue;
                trim(token);
                strncpy(livro.editora, token, sizeof(livro.editora) - 1);
                livro.editora[sizeof(livro.editora) - 1] = '\0';
                livro.editora[strcspn(livro.editora, "\n")] = '\0';

                token = strtok(NULL, ";");
                if (!token) continue;
                trim(token);
                livro.edicao = (size_t)strtoull(token, NULL, 10);

                token = strtok(NULL, ";");
                if (!token) continue;
                trim(token);
                livro.ano = (size_t)strtoull(token, NULL, 10);

                token = strtok(NULL, ";");
                if (!token) continue;
                trim(token);
                livro.exemplares = (size_t)strtoull(token, NULL, 10);

                token = strtok(NULL, ";");
                if (!token) continue;
                trim(token);
                strncpy(preco_str, token, sizeof(preco_str) - 1);
                preco_str[sizeof(preco_str) - 1] = '\0';
                for (char* p = preco_str; *p; p++) {
                        if (*p == ',') *p = '.';
                }
                livro.preco = strtod(preco_str, NULL);

                if (cadastrar_livro(arq_bin, livro) != SUCESSO) {
                        printf("ERRO AO CADASTRAR LIVRO (codigo %zu)\n", livro.codigo);
                }
        }
        printf("Operacao de leitura de arquivo texto concluida!\n");

        return SUCESSO;
}

/**
 * @brief Abre o arquivo texto, solicita o nome ao usuário e chama a função para carregar os livros
 * no arquivo binário.
 *
 * @param caminho Caminho do arquivo binário para salvar os livros.
 * @return int Código de status da operação.
 */
int opcao_carregar_txt(const char* caminho) {
        char nome_arquivo[256];
        printf("Digite o nome do arquivo texto: ");
        if (!fgets(nome_arquivo, sizeof(nome_arquivo), stdin)) return ERRO_ARQUIVO_TEXTO;
        nome_arquivo[strcspn(nome_arquivo, "\n")] = '\0';

        printf("\n");

        FILE* txt = fopen(nome_arquivo, "r");
        if (!txt) {
                return ERRO_ARQUIVO_NULO;
        }

        FILE* arq_bin = fopen(caminho, "rb+");
        if (!arq_bin) {
                fclose(txt);
                return ERRO_ARQUIVO_NULO;
        }

        int status = ler_txt(txt, arq_bin);

        fclose(txt);
        fclose(arq_bin);

        printf("\n");

        return status;
}

/**
 * @brief Abre o arquivo binário e imprime a árvore binária por níveis.
 *
 * Esta função abre o arquivo de livros no caminho informado, chama a função
 * que imprime a árvore por níveis e fecha o arquivo após a operação.
 *
 * @param caminho Caminho do arquivo binário contendo a árvore de livros.
 * @return Código de status da operação:
 *         - SUCESSO: árvore impressa com sucesso.
 *         - ERRO_ARQUIVO_NULO: falha ao abrir o arquivo.
 *         - Códigos de erro retornados por imprimir_arvore_por_niveis.
 *
 * @note A função imprime diretamente no stdout.
 */
int opcao_imprimir_arvore_por_niveis(const char* caminho) {
        printf("Arvore por niveis: \n\n");

        FILE* arquivo = fopen(caminho, "rb");
        if (!arquivo) return ERRO_ARQUIVO_NULO;

        int status = imprimir_arvore_por_niveis(arquivo);

        fclose(arquivo);

        printf("\n");

        return status;
}
