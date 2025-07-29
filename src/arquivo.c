#include "../include/arquivo.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/erros.h"

CABECALHO* le_cabecalho(FILE* arquivo) {
        if (arquivo == NULL) return NULL;

        if (fseek(arquivo, 0, SEEK_SET) != 0) return NULL;

        CABECALHO* cabecalho = malloc(sizeof(CABECALHO));
        if (cabecalho == NULL) return NULL;

        if (fread(cabecalho, sizeof(CABECALHO), 1, arquivo) != 1) {
                free(cabecalho);
                return NULL;
        }

        return cabecalho;
}

int escreve_cabecalho(FILE* arquivo, CABECALHO cabecalho) {
        if (fseek(arquivo, 0, SEEK_SET) != 0) return ERRO_ARQUIVO_SEEK;
        if (fwrite(&cabecalho, sizeof(CABECALHO), 1, arquivo) != 1) return ERRO_ARQUIVO_WRITE;

        return SUCESSO;
}
