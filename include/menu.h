#ifndef MENU_H
#define MENU_H

#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Remove o caractere de nova linha '\n' do final da string.
 *
 * @param str String que terá o '\n' removido.
 */
void limpar_enter(char* str);

/**
 * @brief Exibe o menu principal com as opções do sistema.
 */
void exibir_menu();

/**
 * @brief Exibe o menu principal com as opções do sistema.
 */
void exibir_menu();

/**
 * @brief Realiza o cadastro de um livro, lendo dados do usuário e salvando no arquivo binário.
 *
 * @param caminho_livros Caminho para o arquivo binário onde os livros estão salvos.
 * @return int Código de status da operação (SUCESSO ou erro).
 */
int opcao_cadastrar_livro(char* caminho_livros);

/**
 * @brief Imprime os dados de um livro dado o código informado pelo usuário.
 *
 * @param caminho_livros Caminho do arquivo binário com os livros.
 * @return int Código de status da operação (SUCESSO ou erro).
 */
int opcao_imprimir_dados(char* caminho_livros);

/**
 * @brief Lista todos os livros presentes no arquivo binário.
 *
 * @param caminho_livros Caminho do arquivo binário com os livros.
 * @return int Código de status da operação.
 */
int opcao_listar_todos(char* caminho_livros);

/**
 * @brief Calcula e exibe o total de livros cadastrados no sistema.
 *
 * @param caminho_livros Caminho do arquivo binário com os livros.
 * @return int Código de status da operação.
 */
int opcao_calcular_total(char* caminho_livros);

/**
 * @brief Remove um livro do sistema dado seu código.
 *
 * @param caminho_livros Caminho do arquivo binário com os livros.
 * @return int Código de status da operação.
 */
int opcao_remover_livro(char* caminho_livros);

/**
 * @brief Imprime a lista de registros livres do arquivo binário.
 *
 * @param caminho Caminho do arquivo binário.
 * @return int Código de status da operação.
 */
int opcao_imprimir_lista_livre(const char* caminho);

/**
 * @brief Abre o arquivo texto, solicita o nome ao usuário e chama a função para carregar os livros
 * no arquivo binário.
 *
 * @param caminho Caminho do arquivo binário para salvar os livros.
 * @return int Código de status da operação.
 */
int opcao_carregar_txt(const char* caminho);


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
int opcao_imprimir_arvore_por_niveis(const char* caminho);

#endif  // MENU_H