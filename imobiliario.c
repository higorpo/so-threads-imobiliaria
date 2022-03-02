#include <stdio.h>
#include <pthread.h>
#include "lista.h"

// imoveis_disponiveis
// imoveis_entregues

// 1. Corretor move imóvel da lista de entregue para disponível
// 2. Corretor cria novos imóveis na lista de disponível
// 3. Corretor remove um imóvel disponível

// Imóveis devem conter:
// Código (ex. 1234, 942, ..)
// Endreço (ex. “Av. Florida, 23”, “Rua sem
// fim, 88”, ..)
// Preço (ex.: 1.270, 2.400, ..)
// Bairro (ex.: “Trindade”, “Pantanal”, ..)

// Imovel imoveis_disponiveis[100];
// Imovel imoveis_entregues[100];

// void *thread_corretor_function(void *arg)
// {

//     pthread_exit(NULL);
// }

Imovel *imoveis_disponiveis_cabeca = NULL;

int main()
{
    // pthread_t thread_corretor;

    // pthread_create(&thread_corretor, NULL, thread_corretor_function, NULL);

    append(&imoveis_disponiveis_cabeca, 1, "Av. Florida, 23", 1.270, "Trindade");
    append(&imoveis_disponiveis_cabeca, 2, "João Goulart, 44", 2.270, "Trindade");
    append(&imoveis_disponiveis_cabeca, 3, "Pedro Augusto, 33", 5.270, "Trindade");

    list(imoveis_disponiveis_cabeca);

    printf("\nVamos remover o último\n");

    // Imovel *removido = removeFromPosition(imoveis_disponiveis_cabeca, 2);
    int position = getLastElementPosition(imoveis_disponiveis_cabeca);

    Imovel *removido = removeFromPosition(imoveis_disponiveis_cabeca, position);

    printf("\nListando\n");

    list(imoveis_disponiveis_cabeca);

    printf("\nRemovido código: %d\n", removido->codigo);

    return 0;
}