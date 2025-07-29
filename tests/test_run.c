#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>

#include <cmocka.h>

// Declarar os testes externos de outros arquivos
extern const struct CMUnitTest* arquivo_tests(int*);

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
