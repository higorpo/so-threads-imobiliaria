#include <string.h>
#include <stdlib.h>

typedef struct tipo_imovel
{
    int codigo;
    char endereco[100];
    float preco;
    char bairro[100];
    struct tipo_imovel *proximo;
} Imovel;

void append(Imovel **cabeca_lista, int codigo, char endereco[100], float preco, char bairro[100]);
void list(Imovel *cabeca_lista);
Imovel *removeFromPosition(Imovel *cabeca_lista, int position);
int getLastElementPosition(Imovel *cabeca_lista);
int getQtdElementsInList(Imovel *cabeca_lista);

void append(Imovel **cabeca_lista, int codigo, char endereco[100], float preco, char bairro[100])
{
    Imovel *noatual, *nonovo;

    if (*cabeca_lista == NULL)
    {
        *cabeca_lista = (Imovel *)malloc(sizeof(Imovel));
        (*cabeca_lista)->codigo = codigo;
        strcpy((*cabeca_lista)->endereco, endereco);
        (*cabeca_lista)->preco = preco;
        strcpy((*cabeca_lista)->bairro, bairro);
        (*cabeca_lista)->proximo = NULL;
    }
    else
    {
        noatual = *cabeca_lista;
        while (noatual->proximo != NULL)
            noatual = noatual->proximo;

        nonovo = (Imovel *)malloc(sizeof(Imovel));
        nonovo->codigo = codigo;
        strcpy(nonovo->endereco, endereco);
        nonovo->preco = preco;
        strcpy(nonovo->bairro, bairro);
        nonovo->proximo = NULL;
        noatual->proximo = nonovo;
    }
}

void list(Imovel *noatual)
{
    int i = 0;
    while (noatual != NULL)
    {
        i++;
        printf("\n\nImovel numero %d\nCodigo: %d \nRua: %s \nBairro: %s \nPreco:R$%.2lf\n\n", i, noatual->codigo, noatual->endereco, noatual->bairro, noatual->preco);
        noatual = noatual->proximo;
    }
}

Imovel *removeFromPosition(Imovel *noatual, int position)
{
    Imovel *anterior = NULL;
    Imovel *atual = noatual;

    if (position == 0)
    {
        // TODO: Tentar implementar uma maneira de excluir corretamente o nó cabeça.
        printf("\nNao pode remover o no cabeca!");
        return NULL;
    }

    int i = 0;
    while (atual != NULL && i != position)
    {
        i++;
        anterior = atual;
        atual = atual->proximo;
    }

    if (atual == NULL)
    {
        printf("\n\nNao existe nenhum Imovel na posicao %d\n", position);
        return NULL;
    }

    if (anterior == NULL)
    {
        noatual = atual->proximo;
    }
    else
    {
        anterior->proximo = atual->proximo;
    }

    return atual;
}

int getLastElementPosition(Imovel *noatual)
{
    int i = 0;
    while (noatual != NULL)
    {
        i++;
        noatual = noatual->proximo;
    }

    return i - 1;
}
