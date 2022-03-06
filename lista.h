#include <string.h>
#include <stdlib.h>

typedef struct tipo_imovel
{
    int codigo;
    char endereco[100];
    int preco;
    char bairro[100];
    struct tipo_imovel *proximo;
} Imovel;

void append(Imovel **cabeca_lista, int codigo, char endereco[100], int preco, char bairro[100]);
void list(Imovel *cabeca_lista);
Imovel *removeFromPosition(Imovel *cabeca_lista, int position);
int getLastElementPosition(Imovel *cabeca_lista);
int getQtdElementsInList(Imovel *cabeca_lista);

void append(Imovel **cabeca_lista, int codigo, char endereco[100], int preco, char bairro[100])
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
        while (noatual->proximo != NULL && noatual->proximo->codigo != (*cabeca_lista)->codigo)
        {
            noatual = noatual->proximo;
        }

        nonovo = (Imovel *)malloc(sizeof(Imovel));
        nonovo->codigo = codigo;
        strcpy(nonovo->endereco, endereco);
        nonovo->preco = preco;
        strcpy(nonovo->bairro, bairro);
        nonovo->proximo = *cabeca_lista;
        noatual->proximo = nonovo;
    }
}

void list(Imovel *noatual)
{
    Imovel *cabeca = noatual;
    int i = 0;
    // while (noatual != NULL)
    while (noatual->proximo->codigo != cabeca->codigo)
    {
        i++;
        printf("\n\nImovel numero %d\nCodigo: %d \nRua: %s \nBairro: %s \nPreco:R$%.2d\n Codigo do proximo: %d\n\n", i, noatual->codigo, noatual->endereco, noatual->bairro, noatual->preco, noatual->proximo->codigo);
        noatual = noatual->proximo;
    }
}

Imovel *removeFromPosition(Imovel *noatual, int position)
{
    Imovel *anterior = NULL;
    Imovel *atual = noatual;
    Imovel *cabeca = noatual;

    if (position == 0)
    {
        int i = 0;
        while (atual->proximo->codigo != cabeca->codigo)
        {
            i++;
            anterior = atual;
            atual = atual->proximo;
        }

        if (cabeca->proximo->codigo == atual->codigo)
        {
            atual->proximo = NULL;
        }
        else if (atual->proximo == NULL)
        {
            atual = NULL;
        }
        else
        {
            atual->proximo = cabeca->proximo;
        }
        return cabeca;
    }

    int i = 0;
    // while (atual != NULL && i != position)
    while (atual->proximo->codigo != cabeca->codigo && i != position)
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

    free(atual);

    return atual;
}

int getLastElementPosition(Imovel *noatual)
{
    Imovel *no_cabeca = noatual;
    int i = 0;
    while (noatual->proximo->codigo != no_cabeca->codigo)
    {
        // printf("CODIGO DO PROXIMO NO: %d\n", noatual->proximo->codigo);
        i++;
        noatual = noatual->proximo;
    }

    return i - 1;
}
