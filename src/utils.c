/**
 * @file utils.c
 * @brief Funções utilitárias para leitura segura e manipulação de strings.
 */

#include "../include/utils.h"

#include <ctype.h>
#include <errno.h>
#include <float.h>
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/arquivo.h"
#include "../include/arvore.h"
#include "../include/erros.h"
#include "../include/livro.h"
#include "../include/menu.h"

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

        while (1) {
                if (!fgets(buffer, sizeof(buffer), stdin)) {
                        fprintf(stderr, "Erro de leitura.\n");
                        exit(EXIT_FAILURE);
                }

                errno = 0;
                valor = strtoull(buffer, &endptr, 10);  // base 10

                if (errno == ERANGE || valor > SIZE_MAX) {
                        printf("Número fora do intervalo de size_t.\n");
                        continue;
                }

                if (endptr == buffer) {
                        printf("Entrada inválida. Tente novamente.\n");
                        continue;
                }

                while (isspace((unsigned char)*endptr)) {
                        endptr++;
                }

                if (*endptr != '\0' && *endptr != '\n') {
                        printf("Entrada contém caracteres inválidos.\n");
                        continue;
                }

                return (size_t)valor;
        }
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
