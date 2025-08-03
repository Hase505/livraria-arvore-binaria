/**
 * @file erros.h
 * @brief Define enum com códigos de retorno.
 */

#ifndef ERROS_H
#define ERROS_H

/**
 * @enum codigo_erro
 * @brief Códigos de retorno das funções do sistema
 */
typedef enum {
        SUCESSO = 0,

        ERRO_ARQUIVO_NULO = -1,  /**< Handle para arquivo é nulo. */
        ERRO_ARQUIVO_SEEK = -2,  /**< Erro na função fseek. */
        ERRO_ARQUIVO_READ = -3,  /**< Erro na função fread. */
        ERRO_ARQUIVO_WRITE = -4, /**< Erro na função fwrite. */

        ERRO_CABECALHO_NULO = -10, /**< Cabeçalho não encontrado no arquivo binário. */

        ERRO_NO_NULO = -20 /**< Nó da árvore é nulo. */
} codigo_erro;

#endif  // ERROS_H
