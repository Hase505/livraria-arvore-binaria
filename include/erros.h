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
        ERRO_ARQUIVO_TEXTO = -5, /**< Caminho para arquivo texto invalido. */

        ERRO_CABECALHO_NULO = -10, /**< Cabeçalho não encontrado no arquivo binário. */

        ERRO_NO_NULO = -20,              /**< Nó da árvore é nulo. */
        ERRO_CODIGO_DUPLICADO = -21,     /**< Já existe um nó com o código informado. */
        ERRO_RESULTADO_BUSCA_NULO = -22, /**< Estrutura RESULTADO_BUSCA é nulo. */

        ERRO_LIVRO_INVALIDO = -30,  /**< Não existe um nó com o livro buscado. */
        ERRO_CADASTRAR_LIVRO = -31, /**< erro na tentativa de cadastrar livro na biblioteca. */

        ERRO_FILA_NULA = -40,
        ERRO_ITEM_FILA_NULO = -41,
        ERRO_FILA_CHEIA = -42
} codigo_erro;

#endif  // ERROS_H
