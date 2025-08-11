#ifndef MENU_H
#define MENU_H


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

void exibir_menu();
int opcao_cadastrar_livro(char* caminho_livros);

#endif  // MENU_H