#ifndef ERROS_H
#define ERROS_H

typedef enum {
        SUCESSO = 0,

        // Erros genéricos de arquivos
        ERRO_ARQUIVO_NULO = -1,
        ERRO_ARQUIVO_SEEK = -2,
        ERRO_ARQUIVO_READ = -3,
        ERRO_ARQUIVO_WRITE = -4
} codigo_erro;

#endif  // ERROS_H
