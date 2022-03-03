#include <stdio.h>
#include <pthread.h>
#include "lista.h"

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

#define NUM_THREADS 10

Imovel *imoveis_disponiveis_cabeca = NULL;
Imovel *imoveis_entregues_cabeca = NULL;

pthread_mutex_t mutex;

void *thread_corretor_function(void *arg)
{
    Imovel *imovel_disponivel_cabeca;
    imovel_disponivel_cabeca = (Imovel *)arg;

    int randomNumberBetween1And10Seconds = rand() % 10 + 1;

    sleep(randomNumberBetween1And10Seconds);

    pthread_mutex_lock(&mutex);

    printf("Thread %ld removendo um imóvel disponível\n", pthread_self());
    int lastElementPosition = getLastElementPosition(imovel_disponivel_cabeca);
    int randomBetween1AndLastElementPosition = rand() % lastElementPosition + 1;
    removeFromPosition(imovel_disponivel_cabeca, randomBetween1AndLastElementPosition);

    pthread_mutex_unlock(&mutex);

    pthread_mutex_lock(&mutex);
    while (imoveis_entregues_cabeca != NULL)
    {
        printf("Thread %ld movendo imóveis da lista de entregues para disponíveis\n", pthread_self());
        append(&imoveis_disponiveis_cabeca, imoveis_entregues_cabeca->codigo, imoveis_entregues_cabeca->endereco, imoveis_entregues_cabeca->preco, imoveis_entregues_cabeca->bairro);
        imoveis_entregues_cabeca = imoveis_entregues_cabeca->proximo;
    }
    pthread_mutex_unlock(&mutex);

    int randomCodeBetween100And999 = rand() % 900 + 100;
    int randomPriceBetween700And5000 = rand() % 4000 + 700;

    char endereco[100];
    char bairro[100];

    snprintf(endereco, sizeof(endereco), "Rua %d", randomCodeBetween100And999);
    snprintf(bairro, sizeof(bairro), "Bairro %d", randomCodeBetween100And999);

    printf("Thread %ld adicionando um novo imóvel disponível\n", pthread_self());
    append(&imoveis_disponiveis_cabeca, randomCodeBetween100And999, endereco, randomPriceBetween700And5000, bairro);

    pthread_exit(NULL);
}

int main()
{
    pthread_t threads_corretor[NUM_THREADS];

    pthread_mutex_init(&mutex, NULL);

    printf("Executando o programa, aguarde a inicialização...\n");

    // Cria alguns imóveis disponíveis por default.
    append(&imoveis_disponiveis_cabeca, 1, "Av. Florida, 23", 1.270, "Trindade");
    append(&imoveis_disponiveis_cabeca, 2, "João Goulart, 44", 2.270, "Trindade");
    append(&imoveis_disponiveis_cabeca, 3, "Pedro Augusto, 33", 5.270, "Trindade");

    // Cria alguns imóveis entregues por default.
    append(&imoveis_entregues_cabeca, 4, "Av. Florida, 43", 2.270, "Trindade");
    append(&imoveis_entregues_cabeca, 5, "João Goulart, 77", 6.270, "Trindade");
    append(&imoveis_entregues_cabeca, 6, "Pedro Augusto, 23", 1.270, "Trindade");

    for (int i = 0; i < NUM_THREADS; i++)
    {
        printf("Thread de corretor %d criada.\n", i);
        pthread_create(&threads_corretor[i], NULL, thread_corretor_function, (void *)imoveis_disponiveis_cabeca);
    }

    for (int i = 0; i < NUM_THREADS; i++)
        pthread_join(threads_corretor[i], NULL);

    pthread_mutex_destroy(&mutex);

    printf("Aguardando termino das threads\n");

    list(imoveis_disponiveis_cabeca);

    return 0;
}