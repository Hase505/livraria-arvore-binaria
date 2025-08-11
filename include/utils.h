
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

void limpar_enter(char* str);

size_t ler_size_t(void);

double ler_double(void);

#endif  // UTILS_H