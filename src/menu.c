#include "../include/menu.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/arquivo.h"
#include "../include/arvore.h"
#include "../include/erros.h"
#include "../include/livro.h"
#include "../include/utils.h"


void limpar_enter(char* str) {
        str[strcspn(str, "\n")] = '\0';
}

void exibir_menu() {
        printf("\n====MENU PRINCIPAL====\n");
        printf("1  - CADASTRAR LIVRO\n");
        printf("2  - IMPRIMIR DADOS DO LIVRO\n");
        printf("3  - LISTAR TODOS OS LIVROS\n");
        printf("4  - CALCULAR TOTAL\n");
        printf("5  - CADASTRAR USUARIO\n");
        printf("6  - REMOVER LIVRO\n");
        printf("7 - CARREGAR ARQUIVO\n");
        printf("8  - IMPRIMIR LISTA DE REGISTROS LIVRES:\n");
        printf("9  - IMPRIMIR ARVORE POR NIVEIS\n");
        printf("0  - SAIR\n");
        printf("========================\n");
}

int opcao_cadastrar_livro(char* caminho_livros) {
        LIVRO livro = {0};

        printf("\nCodigo do livro: ");
        while ((livro.codigo = ler_size_t()) <= 0) {
                printf("Codigo invalido (deve ser um numero maior que zero)\n");
                printf("Codigo do livro: ");
        }

        printf("\nTitulo: ");
        fgets(livro.titulo, MAX_TITULO + 1, stdin);
        limpar_enter(livro.titulo);

        printf("\nAutor: ");
        fgets(livro.autor, MAX_AUTOR + 1, stdin);
        limpar_enter(livro.autor);

        printf("\nEditora: ");
        fgets(livro.editora, MAX_EDITORA + 1, stdin);
        limpar_enter(livro.editora);

        printf("\nEdicao: ");
        while ((livro.edicao = ler_size_t()) <= 0) {
                printf("Edicao invalida (deve ser um numero maior que zero)\n");
                printf("Edicao: ");
        }

        printf("\nAno: ");
        while ((livro.ano = ler_size_t()) <= 0) {
                printf("Ano invalido (deve ser um numero maior que zero)\n");
                printf("Ano: ");
        }

        printf("\nQuantidade de exemplares: ");
        while (livro.exemplares = ler_size_t() <= 0) {
                printf("Digite um valor valido (nao negativo)\n");
                printf("Quantidade de exemplares: ");
        }

        printf("\nPreco: ");
        livro.preco = ler_double();
        int resposta;
        FILE* arquivo = fopen(caminho_livros, "rb+");
        if (arquivo == NULL) {
                return ERRO_ARQUIVO_NULO;
        }
        if ((resposta = cadastrar_livro(arquivo, livro)) != SUCESSO) {
                printf("\nErro ao cadastrar livro");

                if (resposta == ERRO_CODIGO_DUPLICADO)
                        printf(": Livro com codigo ja cadastrado\n");
                else
                        printf("\n");
                fclose(arquivo);
                return ERRO_CADASTRAR_LIVRO;
        } else {
                printf("\nLivro \"%s\" cadastrado com sucesso!\n", livro.titulo);
                fclose(arquivo);
                return SUCESSO;
        }
}