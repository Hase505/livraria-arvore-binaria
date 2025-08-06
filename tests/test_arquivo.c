/**
 * @file test_arquivo.c
 * @brief Testes unitários para o módulo de manipulação de arquivos binários.
 *
 * Utiliza a biblioteca CMocka para testar as funções `le_cabecalho` e `escreve_cabecalho`.
 */

#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include <cmocka.h>
#include <string.h>

#include "../include/arquivo.h"
#include "../include/erros.h"

/// @brief Arquivo temporário utilizado nos testes.
static FILE* arquivo_valido = NULL;

/**
 * @brief: Auxiliar: cria um LIVRO, preenchendo os campos com informações básicas.
 *
 * @param[in] codigo Codigo identificador que será escrito no livro
 * @return Estrutura LIVRO preenchida.
 */
LIVRO aux_criar_livro_valido(int codigo) {
        LIVRO l = {0};

        l.codigo = codigo;
        l.ano = 2025;
        l.edicao = 1;
        l.exemplares = 5;
        l.preco = 23.99;
        strncpy(l.autor, "Autor", MAX_AUTOR);
        strncpy(l.editora, "Editora", MAX_EDITORA);
        strncpy(l.titulo, "Titulo", MAX_TITULO);
        l.autor[MAX_AUTOR] = l.editora[MAX_EDITORA] = l.titulo[MAX_TITULO] = '\0';

        return l;
}

/**
 * @brief Setup: cria um arquivo temporário com um cabeçalho válido.
 *
 * @param[out] state Ponteiro para o estado compartilhado entre os testes.
 * @return 0 em caso de sucesso, -1 se falhar.
 */
static int setup_arquivo_valido(void** state) {
        arquivo_valido = tmpfile();
        if (!arquivo_valido) return -1;

        CABECALHO cabecalho = {0};
        cabecalho.raiz = 1;
        cabecalho.livre = 2;
        cabecalho.topo = 3;
        cabecalho.quantidade_livros = 4;

        fwrite(&cabecalho, sizeof(CABECALHO), 1, arquivo_valido);

        *state = arquivo_valido;

        return 0;
}

/**
 * @brief Teardown: fecha o arquivo temporário utilizado no teste.
 *
 * @param[in] state Ponteiro para o estado compartilhado entre os testes.
 * @return 0 sempre.
 */
static int teardown_arquivo_valido(void** state) {
        FILE* arquivo_valido = *state;
        fclose(arquivo_valido);
        return 0;
}

/**
 * @test Verifica se `le_cabecalho` retorna NULL quando o ponteiro de arquivo é NULL.
 */
static void test_le_cabecalho_arquivo_nulo(void** state) {
        (void)state;
        CABECALHO* cabecalho = le_cabecalho(NULL);
        assert_null(cabecalho);
}

/**
 * @test Verifica se `le_cabecalho` retorna NULL quando o arquivo está vazio (sem cabeçalho).
 */
static void test_le_cabecalho_arquivo_sem_cabecalho(void** state) {
        (void)state;
        FILE* arquivo = tmpfile();
        assert_non_null(arquivo);

        CABECALHO* cabecalho = le_cabecalho(arquivo);
        assert_null(cabecalho);

        fclose(arquivo);
}

/**
 * @test Verifica se `le_cabecalho` lê corretamente um cabeçalho válido de um arquivo.
 */
static void test_le_cabecalho_valido(void** state) {
        FILE* arquivo = *state;
        CABECALHO* cabecalho = le_cabecalho(arquivo);

        assert_non_null(cabecalho);
        assert_int_equal(cabecalho->raiz, 1);
        assert_int_equal(cabecalho->livre, 2);
        assert_int_equal(cabecalho->topo, 3);
        assert_int_equal(cabecalho->quantidade_livros, 4);

        free(cabecalho);
}

/**
 * @test Verifica se `escreve_cabecalho` grava corretamente os dados em um arquivo.
 */
static void test_escreve_cabecalho_valido(void** state) {
        (void)state;

        FILE* arquivo = tmpfile();
        assert_non_null(arquivo);

        CABECALHO cabecalho = {0};
        cabecalho.raiz = 1;
        cabecalho.livre = 2;
        cabecalho.topo = 3;
        cabecalho.quantidade_livros = 4;

        int r = escreve_cabecalho(arquivo, &cabecalho);
        assert_int_equal(r, SUCESSO);

        // Lê de volta para verificar se foi escrito corretamente
        rewind(arquivo);
        CABECALHO cabecalho_lido = {0};
        size_t lidos = fread(&cabecalho_lido, sizeof(CABECALHO), 1, arquivo);
        assert_int_equal(lidos, 1);

        // Compara os valores
        assert_int_equal(cabecalho_lido.raiz, 1);
        assert_int_equal(cabecalho_lido.livre, 2);
        assert_int_equal(cabecalho_lido.topo, 3);
        assert_int_equal(cabecalho_lido.quantidade_livros, 4);

        fclose(arquivo);
}

/**
 * @brief Setup: cria um arquivo temporário com cabeçalho sem lista livre.
 *
 * Inicializa um arquivo com um cabeçalho em que os campos `raiz` e `livre` estão como
 * `POSICAO_INVALIDA`, e `topo` e `quantidade_livros` em 0, simulando um arquivo vazio
 * pronto para a primeira inserção.
 *
 * @param[out] state Ponteiro para o estado compartilhado entre os testes.
 * @return 0 em caso de sucesso, -1 se falhar.
 */
static int setup_criar_arquivo_valido_sem_lista_livre(void** state) {
        arquivo_valido = tmpfile();
        if (!arquivo_valido) return -1;

        CABECALHO cabecalho = {0};
        cabecalho.raiz = POSICAO_INVALIDA;
        cabecalho.livre = POSICAO_INVALIDA;
        cabecalho.topo = 0;
        cabecalho.quantidade_livros = 0;

        if (fwrite(&cabecalho, sizeof(CABECALHO), 1, arquivo_valido) != 1) {
                fclose(arquivo_valido);
                return -1;
        }

        *state = arquivo_valido;

        return 0;
}

/**
 * @test Verifica se a função `inserir_no_arquivo` insere corretamente um nó
 * em um arquivo sem lista livre (inserção no topo).
 *
 * Testa se o cabeçalho e o nó são atualizados corretamente após a inserção.
 *
 * @param[in] state Arquivo preparado no setup.
 */
static void test_inserir_no_valido_sem_lista_livre(void** state) {
        FILE* arquivo = *state;

        LIVRO livro = aux_criar_livro_valido(1);

        NO_ARVORE no = {0};
        no.livro = livro;
        no.filho_direito = -1;
        no.filho_esquerdo = -1;

        int r = inserir_no_arquivo(arquivo, &no);
        assert_int_equal(r, SUCESSO);

        CABECALHO cabecalho = {0};
        fseek(arquivo, 0, SEEK_SET);
        fread(&cabecalho, sizeof(CABECALHO), 1, arquivo);

        assert_int_equal(cabecalho.quantidade_livros, 1);

        NO_ARVORE no_lido = {0};
        fseek(arquivo, sizeof(CABECALHO) + (cabecalho.raiz) * sizeof(NO_ARVORE), SEEK_SET);
        fread(&no_lido, sizeof(NO_ARVORE), 1, arquivo);

        assert_int_equal(no_lido.filho_direito, -1);
        assert_int_equal(no_lido.filho_esquerdo, -1);
}

/**
 * @brief Setup: cria um arquivo com lista livre contendo dois nós removidos.
 *
 * O cabeçalho aponta para a posição 0 da lista livre. O nó na posição 0 aponta para a posição 1,
 * e o nó na posição 1 marca o fim da lista (filho_esquerdo = POSICAO_INVALIDA).
 *
 * @param[out] state Ponteiro para o estado compartilhado entre os testes.
 * @return 0 em caso de sucesso, -1 se falhar.
 */
static int setup_criar_arquivo_valido_com_lista_livre(void** state) {
        arquivo_valido = tmpfile();
        if (!arquivo_valido) return -1;

        CABECALHO cabecalho = {0};
        cabecalho.raiz = POSICAO_INVALIDA;
        cabecalho.livre = 0;
        cabecalho.topo = 2;
        cabecalho.quantidade_livros = 0;

        if (fwrite(&cabecalho, sizeof(CABECALHO), 1, arquivo_valido) != 1) {
                fclose(arquivo_valido);
                return -1;
        }

        NO_ARVORE no_removido_0 = {0};
        no_removido_0.filho_esquerdo = 1;
        no_removido_0.filho_direito = POSICAO_INVALIDA;

        NO_ARVORE no_removido_1 = {0};
        no_removido_1.filho_esquerdo = POSICAO_INVALIDA;
        no_removido_1.filho_direito = POSICAO_INVALIDA;

        if (fwrite(&no_removido_0, sizeof(NO_ARVORE), 1, arquivo_valido) != 1 ||
            fwrite(&no_removido_1, sizeof(NO_ARVORE), 1, arquivo_valido) != 1) {
                fclose(arquivo_valido);
                return -1;
        }

        *state = arquivo_valido;

        return 0;
}

/**
 * @test Verifica se `inserir_no_arquivo` insere corretamente um nó utilizando a lista livre.
 *
 * Após a inserção, a posição 0 (que estava livre) deve ser ocupada, e o campo `livre` do
 * cabeçalho deve ser atualizado para apontar para o próximo nó livre (posição 1).
 *
 * @param[in] state Arquivo preparado com uma lista livre de dois nós.
 */
static void test_inserir_no_valido_com_lista_livre(void** state) {
        FILE* arquivo = *state;

        LIVRO livro = aux_criar_livro_valido(1);

        NO_ARVORE no = {0};
        no.livro = livro;
        no.filho_direito = -1;
        no.filho_esquerdo = -1;

        int r = inserir_no_arquivo(arquivo, &no);
        assert_int_equal(r, SUCESSO);

        CABECALHO cabecalho = {0};
        fseek(arquivo, 0, SEEK_SET);
        fread(&cabecalho, sizeof(CABECALHO), 1, arquivo);

        assert_int_equal(cabecalho.quantidade_livros, 1);
        assert_int_equal(cabecalho.livre, 1);

        NO_ARVORE no_lido = {0};
        fseek(arquivo, sizeof(CABECALHO) + 0 * sizeof(NO_ARVORE), SEEK_SET);
        fread(&no_lido, sizeof(NO_ARVORE), 1, arquivo);

        assert_int_equal(no_lido.filho_direito, -1);
        assert_int_equal(no_lido.filho_esquerdo, -1);
        assert_int_equal(no_lido.livro.codigo, livro.codigo);
}

/**
 * @test Verifica se ler_no_arquivo retorna NULL quando arquivo é NULL ou posição inválida.
 */
static void test_ler_no_arquivo_parametros_invalidos(void** state) {
        (void)state;
        NO_ARVORE* no = ler_no_arquivo(NULL, 0);
        assert_null(no);
        FILE* arquivo = tmpfile();
        assert_non_null(arquivo);
        no = ler_no_arquivo(arquivo, -1);
        assert_null(no);
        fclose(arquivo);
}

/**
 * @test Verifica se ler_no_arquivo lê corretamente um nó válido do arquivo.
 */
static void test_ler_no_arquivo_valido(void** state) {
        FILE* arquivo = *state;
        LIVRO livro = aux_criar_livro_valido(42);

        NO_ARVORE no = {0};
        no.livro = livro;
        no.filho_direito = -1;
        no.filho_esquerdo = -1;
        inserir_no_arquivo(arquivo, &no);

        NO_ARVORE* no_lido = ler_no_arquivo(arquivo, 0);
        assert_non_null(no_lido);
        assert_int_equal(no_lido->livro.codigo, 42);
        assert_int_equal(no_lido->filho_direito, -1);
        assert_int_equal(no_lido->filho_esquerdo, -1);
        free(no_lido);
}

/**
 * @test Verifica se remover_no_arquivo remove corretamente um nó e atualiza a lista livre.
 */
static void test_remover_no_arquivo_valido(void** state) {
        FILE* arquivo = *state;
        LIVRO livro = aux_criar_livro_valido(99);

        NO_ARVORE no = {0};
        no.livro = livro;
        no.filho_direito = -1;
        no.filho_esquerdo = -1;

        inserir_no_arquivo(arquivo, &no);
        int r = remover_no_arquivo(arquivo, 0);
        assert_int_equal(r, SUCESSO);

        CABECALHO cabecalho = {0};
        fseek(arquivo, 0, SEEK_SET);
        fread(&cabecalho, sizeof(CABECALHO), 1, arquivo);

        assert_int_equal(cabecalho.quantidade_livros, 0);
        assert_int_equal(cabecalho.livre, 0);

        NO_ARVORE no_removido = {0};
        fseek(arquivo, sizeof(CABECALHO) + 0 * sizeof(NO_ARVORE), SEEK_SET);
        fread(&no_removido, sizeof(NO_ARVORE), 1, arquivo);
        assert_int_equal(no_removido.filho_esquerdo, POSICAO_INVALIDA);
}

/**
 * @brief Retorna a lista de testes de arquivo a serem executados.
 *
 * @param[out] n Número de testes.
 * @return Vetor com os testes definidos.
 */
const struct CMUnitTest* arquivo_tests(int* n) {
        static const struct CMUnitTest tests[] = {
            cmocka_unit_test(test_le_cabecalho_arquivo_nulo),
            cmocka_unit_test(test_le_cabecalho_arquivo_sem_cabecalho),
            cmocka_unit_test_setup_teardown(test_le_cabecalho_valido, setup_arquivo_valido,
                                            teardown_arquivo_valido),
            cmocka_unit_test(test_escreve_cabecalho_valido),
            cmocka_unit_test_setup_teardown(test_inserir_no_valido_sem_lista_livre,
                                            setup_criar_arquivo_valido_sem_lista_livre,
                                            teardown_arquivo_valido),
            cmocka_unit_test_setup_teardown(test_inserir_no_valido_com_lista_livre,
                                            setup_criar_arquivo_valido_com_lista_livre,
                                            teardown_arquivo_valido),
            cmocka_unit_test(test_ler_no_arquivo_parametros_invalidos),
            cmocka_unit_test_setup_teardown(test_ler_no_arquivo_valido,
                                            setup_criar_arquivo_valido_sem_lista_livre,
                                            teardown_arquivo_valido),
            cmocka_unit_test_setup_teardown(test_remover_no_arquivo_valido,
                                            setup_criar_arquivo_valido_sem_lista_livre,
                                            teardown_arquivo_valido)};

        *n = sizeof(tests) / sizeof(tests[0]);
        return tests;
}
