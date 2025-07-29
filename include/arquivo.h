#ifndef ARQUIVO_H
#define ARQUIVO_H

#include <stdio.h>

typedef struct {
        int raiz;
        int topo;
        int livre;
        size_t quantidade_livros;
} CABECALHO;

CABECALHO* le_cabecalho(FILE* arquivo);
int escreve_cabecalho(FILE* arquivo, const CABECALHO* cabecalho);

#endif  // ARQUIVO_H
