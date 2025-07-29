/**
 * @file test_run.c
 * @brief Função principal para execução dos testes unitários.
 *
 * Este arquivo contém o ponto de entrada (`main`) da suíte de testes.
 * Ele reúne todos os testes definidos em arquivos separados (como `arquivo_tests`) e os executa com
 * CMocka.
 */

#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>

#include <cmocka.h>

/// @brief Declaração externa dos testes do módulo de arquivo.
/// @param[out] n Quantidade de testes retornados.
/// @return Vetor de testes para o módulo de arquivo.
extern const struct CMUnitTest* arquivo_tests(int*);

/**
 * @brief Função principal que executa todos os testes unitários com CMocka.
 *
 * Coleta os testes definidos externamente (por exemplo, de `arquivo_tests`), agrupa-os em um array
 * único e executa todos usando `cmocka_run_group_tests`.
 *
 * @return Resultado da execução dos testes (0 se todos passaram, valor diferente de 0 se falhou).
 */
int main(void) {
        int total_tests = 0;

        int n_arquivo = 0;
        const struct CMUnitTest* arquivo = arquivo_tests(&n_arquivo);

        total_tests = n_arquivo;

        struct CMUnitTest all_tests[total_tests];
        int i = 0;

        for (int j = 0; j < n_arquivo; j++) all_tests[i++] = arquivo[j];

        return cmocka_run_group_tests(all_tests, NULL, NULL);
}
