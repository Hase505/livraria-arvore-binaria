#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include <cmocka.h>

#include "../include/arquivo.h"
#include "../include/erros.h"

// Setup
static FILE* arquivo_valido = NULL;

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

// Teardown
static int teardown_arquivo_valido(void** state) {
        FILE* arquivo_valido = *state;
        fclose(arquivo_valido);
        return 0;
}

// Testes para le_cabecalho
static void test_le_cabecalho_arquivo_nulo(void** state) {
        (void)state;
        CABECALHO* cabecalho = le_cabecalho(NULL);
        assert_null(cabecalho);
}

static void test_le_cabecalho_arquivo_sem_cabecalho(void** state) {
        (void)state;
        FILE* arquivo = tmpfile();
        assert_non_null(arquivo);

        CABECALHO* cabecalho = le_cabecalho(arquivo);
        assert_null(cabecalho);

        fclose(arquivo);
}

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

// Teste para escreve_cabecalho
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

const struct CMUnitTest* arquivo_tests(int* n) {
        static const struct CMUnitTest tests[] = {
            cmocka_unit_test(test_le_cabecalho_arquivo_nulo),
            cmocka_unit_test(test_le_cabecalho_arquivo_sem_cabecalho),
            cmocka_unit_test_setup_teardown(test_le_cabecalho_valido, setup_arquivo_valido,
                                            teardown_arquivo_valido),
            cmocka_unit_test(test_escreve_cabecalho_valido)};

        *n = sizeof(tests) / sizeof(tests[0]);
        return tests;
}
