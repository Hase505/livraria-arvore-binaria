
/**
 * @file utils.h
 * @brief Estruturas e funções auxiliares.
 */

#ifndef UTILS_H
#define UTILS_H

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
 * @brief Lê um valor do tipo size_t da entrada padrão de forma segura e validada.
 *
 * @return size_t Valor lido convertido.
 */
size_t ler_size_t(void);

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