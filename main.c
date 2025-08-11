#include <stdio.h>
#include <stdlib.h>

#include "include/arquivo.h"
#include "include/erros.h"
#include "include/menu.h"   // seu arquivo de cabeçalho com as funções do menu
#include "include/utils.h"  // para ler_size_t e ler_double, se precisar

#define CAMINHO_ARQUIVO "livros.bin"

int main(void) {
        int opcao = -1;
        abrir_ou_criar_arquivo(CAMINHO_ARQUIVO);
        while (opcao != 0) {
                exibir_menu();

                printf("Escolha uma opção: ");
                opcao = (int)ler_size_t();

                int status;

                switch (opcao) {
                        case 1:
                                status = opcao_cadastrar_livro(CAMINHO_ARQUIVO);
                                break;
                        case 2:
                                status = opcao_imprimir_dados(CAMINHO_ARQUIVO);
                                if (status != SUCESSO) printf("Erro ao imprimir dados do livro.\n");
                                break;
                        case 3:
                                status = opcao_listar_todos(CAMINHO_ARQUIVO);
                                if (status != SUCESSO) printf("Erro ao listar livros.\n");
                                break;
                        case 4:
                                status = opcao_calcular_total(CAMINHO_ARQUIVO);
                                if (status != SUCESSO)
                                        printf("Erro ao calcular total de livros.\n");
                                break;

                        case 5:
                                status = opcao_remover_livro(CAMINHO_ARQUIVO);
                                if (status != SUCESSO)
                                        printf("Erro ao remover livro.\n");
                                else
                                        printf("Livro removido com sucesso\n");
                                break;
                        case 6:
                                status = opcao_carregar_txt(CAMINHO_ARQUIVO);
                                if (status != SUCESSO) printf("Erro ao carregar arquivo texto.\n");
                                break;
                        case 7:
                                status = opcao_imprimir_lista_livre(CAMINHO_ARQUIVO);
                                if (status != SUCESSO)
                                        printf("Erro ao imprimir lista de registros livres.\n");
                                break;
                        case 8:
                                status = opcao_imprimir_arvore_por_niveis(CAMINHO_ARQUIVO);
                                break;
                        case 0:
                                printf("Saindo do programa...\n");
                                break;
                        default:
                                printf("Opcao invalida! Tente novamente.\n");
                                break;
                }
        }

        return 0;
}
