/**
 * @file utils.c
 * @brief Funções utilitárias para leitura segura e manipulação de strings.
 */

#include "../include/utils.h"

#include <ctype.h>
#include <errno.h>
#include <float.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
void limpar_tela(void) {
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif
}

/**
 * @brief Verifica se o restante da string após conversão numérica contém apenas espaços ou fim de
 * linha.
 *
 * @param endptr Ponteiro para o caractere após a parte numérica da string.
 * @return int Retorna 1 se o resto for válido, 0 caso contrário.
 */
static int resto_eh_valido(char* endptr) {
        while (isspace((unsigned char)*endptr)) {
                endptr++;
        }
        return (*endptr == '\0' || *endptr == '\n');
}

/**
 * @brief Lê um valor do tipo size_t da entrada padrão de forma segura e validada.
 *
 * @return size_t Valor lido convertido.
 */
size_t ler_size_t(void) {
        char buffer[100];
        char* endptr;
        unsigned long long valor;

        if (!fgets(buffer, sizeof(buffer), stdin)) {
                fprintf(stderr, "Erro de leitura.\n");
                exit(EXIT_FAILURE);
        }

        // Se for negativo, retorna 0
        if (buffer[0] == '-') {
                return 0;
        }

        errno = 0;
        valor = strtoull(buffer, &endptr, 10);

        if (errno == ERANGE || valor > SIZE_MAX) {
                return 0;
        }

        if (endptr == buffer) {
                return 0;
        }

        while (isspace((unsigned char)*endptr)) {
                endptr++;
        }

        if (*endptr != '\0' && *endptr != '\n') {
                return 0;
        }

        return (size_t)valor;
}

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
int ler_size_t_com_zero(size_t* saida) {
        char buffer[64];
        char* endptr;
        unsigned long long valor;

        if (!fgets(buffer, sizeof(buffer), stdin)) {
                return 0;  // erro de leitura
        }

        buffer[strcspn(buffer, "\n")] = '\0';

        if (buffer[0] == '-') {
                return 0;  // rejeita número negativo
        }

        errno = 0;
        valor = strtoull(buffer, &endptr, 10);

        if (endptr == buffer || errno == ERANGE) {
                return 0;  // erro na conversão
        }

        while (isspace((unsigned char)*endptr)) {
                endptr++;
        }

        if (*endptr != '\0') {
                return 0;  // caracteres inválidos após número
        }

        if (valor > SIZE_MAX) {
                return 0;  // valor maior que size_t máximo
        }

        *saida = (size_t)valor;
        return 1;  // sucesso
}

/**
 * @brief Lê um valor do tipo double da entrada padrão de forma segura e validada.
 *
 * @return double Valor lido convertido.
 */
double ler_double(void) {
        char buffer[128];
        char* endptr;
        double v;

        while (1) {
                if (!fgets(buffer, sizeof(buffer), stdin)) {
                        fprintf(stderr, "Erro de leitura.\n");
                        exit(EXIT_FAILURE);
                }

                errno = 0;
                v = strtod(buffer, &endptr);

                if (errno == ERANGE || v > DBL_MAX || v < -DBL_MAX) {
                        printf("Fora do intervalo de double.\n");
                        continue;
                }
                if (endptr == buffer || !resto_eh_valido(endptr)) {
                        printf("Entrada inválida.\n");
                        continue;
                }
                return v;
        }
}

/**
 * @brief Remove espaços em branco no início e no fim de uma string.
 *
 * @param str String a ser modificada.
 */
void trim(char* str) {
        if (!str) return;

        // Remover espaços do início
        char* inicio = str;
        while (isspace((unsigned char)*inicio)) inicio++;

        // Se a string for só espaços
        if (*inicio == '\0') {
                str[0] = '\0';
                return;
        }

        // Remover espaços do final
        char* fim = inicio + strlen(inicio) - 1;
        while (fim > inicio && isspace((unsigned char)*fim)) fim--;

        // Novo final de string
        *(fim + 1) = '\0';

        // Copiar resultado para o início da string
        if (str != inicio) memmove(str, inicio, fim - inicio + 2);  // +2 pra incluir '\0'
}
