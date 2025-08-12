#include <stdio.h>

#include "include/arquivo.h"
#include "include/erros.h"
#include "include/menu.h"
#include "include/utils.h"

#define CAMINHO_ARQUIVO "livros.bin"

/**
 * @brief Função principal do programa de gerenciamento de livros.
 *
 * Esta função executa o loop principal do sistema, exibindo um menu com opções
 * para cadastrar, imprimir, listar, calcular total, remover livros, carregar
 * dados de arquivo texto, imprimir lista de registros livres e imprimir árvore
 * por níveis.
 *
 * O programa continua executando até que o usuário escolha a opção de sair (0).
 *
 * @return int Retorna 0 ao finalizar a execução com sucesso.
 */
int main(void) {
        int opcao = -1;
        abrir_ou_criar_arquivo(CAMINHO_ARQUIVO);
        while (opcao != 0) {
                exibir_menu();

                printf("Escolha uma opcao: ");
                opcao = (int)ler_size_t();
                limpar_tela();

                int status;

                switch (opcao) {
                        case 1:
                                status = opcao_cadastrar_livro(CAMINHO_ARQUIVO);
                                break;
                        case 2:
                                status = opcao_imprimir_dados(CAMINHO_ARQUIVO);
                                if (status != SUCESSO)
                                        printf("Erro ao imprimir dados do livro.\n\n");
                                break;
                        case 3:
                                status = opcao_listar_todos(CAMINHO_ARQUIVO);
                                if (status != SUCESSO) printf("Erro ao listar livros.\n\n");
                                break;
                        case 4:
                                status = opcao_calcular_total(CAMINHO_ARQUIVO);
                                if (status != SUCESSO)
                                        printf("Erro ao calcular total de livros.\n\n");
                                break;

                        case 5:
                                status = opcao_remover_livro(CAMINHO_ARQUIVO);
                                if (status != SUCESSO)
                                        printf("Erro ao remover livro.\n\n");
                                else
                                        printf("Livro removido com sucesso\n\n");
                                break;
                        case 6:
                                status = opcao_carregar_txt(CAMINHO_ARQUIVO);
                                if (status != SUCESSO)
                                        printf("Erro ao carregar arquivo texto.\n\n");
                                break;
                        case 7:
                                status = opcao_imprimir_lista_livre(CAMINHO_ARQUIVO);
                                if (status != SUCESSO)
                                        printf("Erro ao imprimir lista de registros livres.\n\n");
                                break;
                        case 8:
                                status = opcao_imprimir_arvore_por_niveis(CAMINHO_ARQUIVO);
                                break;
                        case 0:
                                printf("Saindo do programa...");
                                break;
                        default:
                                printf("Opcao invalida! Tente novamente.\n\n");
                                break;
                }
        }

        return 0;
}
