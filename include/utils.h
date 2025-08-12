
/**
 * @file utils.h
 * @brief Estruturas e funções auxiliares.
 */

#ifndef UTILS_H
#define UTILS_H

#include <stddef.h>

/**
 * @brief Limpa a tela do terminal.
 *
 * Esta função limpa a tela do terminal de forma portátil,
 * suportando sistemas operacionais Windows e Unix-like (Linux, macOS).
 *
 * Utiliza o comando "cls" no Windows e "clear" em sistemas Unix-like,
 * executando via a função system().
 *
 * @note O uso de system() pode ter implicações de segurança se usado com entradas não confiáveis.
 */
void limpar_tela(void);

/**
 * @brief Lê um valor do tipo size_t da entrada padrão de forma segura e validada.
 *
 * @return size_t Valor lido convertido.
 */
size_t ler_size_t(void);

/**
 * @brief Lê um valor do tipo size_t da entrada padrão de forma segura e validada.
 *
 * Esta função lê uma linha da entrada padrão, remove o caractere de nova linha,
 * verifica se o conteúdo é um número inteiro não negativo válido (incluindo zero),
 * e converte para size_t.
 *
 * A função rejeita números negativos, entradas inválidas (letras, símbolos, etc),
 * valores que excedam o máximo permitido para size_t e retorna sucesso ou falha.
 *
 * @param saida Ponteiro para size_t onde será armazenado o valor lido.
 * @return int Retorna 1 se a leitura e conversão foram bem-sucedidas,
 *             ou 0 se ocorreu algum erro ou valor inválido.
 */
int ler_size_t_com_zero(size_t* saida);

/**
 * @brief Lê um valor do tipo double da entrada padrão de forma segura e validada.
 *
 * @return double Valor lido convertido.
 */
double ler_double(void);

/**
 * @brief Remove espaços em branco no início e no fim de uma string.
 *
 * @param str String a ser modificada.
 */
void trim(char* str);

#endif  // UTILS_H
