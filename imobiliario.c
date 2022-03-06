#include <stdio.h>
#include <pthread.h>
#include "lista.h"

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

#define NUM_THREADS 10

// Define os nós iniciais de cada lista
Imovel *imoveis_disponiveis_cabeca = NULL;
Imovel *imoveis_entregues_cabeca = NULL;

pthread_mutex_t mutex;

void *thread_inquilino_function(void *arg)
{
    Imovel *imovel_alugado;
    Imovel *imovel_disponivel_cabeca;
    imovel_disponivel_cabeca = (Imovel *)arg;
    int randomNumberBetween1And10Seconds = rand() % 10 + 1;

    int lista_vazia = 1;
    while (lista_vazia == 1)
    {
        int lastElementPosition = getLastElementPosition(imovel_disponivel_cabeca);
        if (lastElementPosition != 0)
        {

            int randomBetween1AndLastElementPosition = rand() % lastElementPosition + 1;
            printf("Inquilino %ld aluga um imovel disponivel\n", pthread_self());
            pthread_mutex_lock(&mutex);
            imovel_alugado = removeFromPosition(imovel_disponivel_cabeca, randomBetween1AndLastElementPosition);
            pthread_mutex_unlock(&mutex);
            lista_vazia = 0;
            break;
        }

        printf("Inquilino %ld espera por imovel disponivel\n", pthread_self());
        sleep(randomNumberBetween1And10Seconds);
        printf("Inquilino %ld volta a tentar alugar um imovel\n", pthread_self());
    }

    sleep(randomNumberBetween1And10Seconds);

    pthread_mutex_lock(&mutex);

    printf("Inquilino %ld entrega um imovel\n", pthread_self());

    if (imovel_alugado != NULL)
    {
        append(&imoveis_entregues_cabeca, imovel_alugado->codigo, imovel_alugado->endereco, imovel_alugado->preco, imovel_alugado->bairro);
    }

    pthread_mutex_unlock(&mutex);

    pthread_exit(NULL);
}

void *thread_corretor_function(void *arg)
{
    Imovel *imovel_disponivel_cabeca;
    imovel_disponivel_cabeca = (Imovel *)arg;

    int randomNumberBetween1And10Seconds = rand() % 10 + 1;
    sleep(randomNumberBetween1And10Seconds);

    int lastElementPosition = getLastElementPosition(imovel_disponivel_cabeca);

    if (lastElementPosition != 0)
    {
        int randomBetween1AndLastElementPosition = rand() % lastElementPosition + 1;
        printf("Corretor %ld removendo um imovel disponivel\n", pthread_self());
        pthread_mutex_lock(&mutex);
        removeFromPosition(imovel_disponivel_cabeca, randomBetween1AndLastElementPosition);
        pthread_mutex_unlock(&mutex);
    }

    Imovel *cabeca = imoveis_entregues_cabeca;
    while (imoveis_entregues_cabeca != NULL)
    {
        if (imoveis_entregues_cabeca->proximo->codigo != cabeca->codigo)
        {
            pthread_mutex_lock(&mutex);
            printf("Corretor %ld movendo imoveis da lista de entregues para disponiveis\n", pthread_self());
            append(&imoveis_disponiveis_cabeca, imoveis_entregues_cabeca->codigo, imoveis_entregues_cabeca->endereco, imoveis_entregues_cabeca->preco, imoveis_entregues_cabeca->bairro);
            imoveis_entregues_cabeca = imoveis_entregues_cabeca->proximo;

            pthread_mutex_unlock(&mutex);
        }
        else
        {
            break;
        }
    }

    int randomCodeBetween100And999 = rand() % 900 + 100;
    int randomPriceBetween700And5000 = rand() % 4000 + 700;

    char endereco[100];
    char bairro[100];

    snprintf(endereco, sizeof(endereco), "Rua %d", randomCodeBetween100And999);
    snprintf(bairro, sizeof(bairro), "Bairro %d", randomCodeBetween100And999);

    pthread_mutex_lock(&mutex);

    printf("Corretor %ld adicionando um novo imovel disponivel\n", pthread_self());
    append(&imoveis_disponiveis_cabeca, randomCodeBetween100And999, endereco, randomPriceBetween700And5000, bairro);

    pthread_mutex_unlock(&mutex);

    pthread_exit(NULL);
}

int main()
{
    // Define as threads (NUM_THREADS eh o num de threads criadas)
    pthread_t threads_corretor[NUM_THREADS];
    pthread_t threads_inquilino[NUM_THREADS];

    // Faz o init do mutex
    pthread_mutex_init(&mutex, NULL);

    printf("Executando o programa, aguarde a inicializacao...\n");

    // Adiciona imoveis default nas listas de entregues e disponíveis
    append(&imoveis_disponiveis_cabeca, 1, "Av. Florida, 23", 1270, "Trindade");
    append(&imoveis_disponiveis_cabeca, 2, "João Goulart, 44", 2270, "Trindade");
    append(&imoveis_disponiveis_cabeca, 3, "Pedro Augusto, 33", 5270, "Trindade");

    append(&imoveis_entregues_cabeca, 4, "Av. Florida, 43", 2270, "Trindade");
    append(&imoveis_entregues_cabeca, 5, "João Goulart, 77", 6270, "Trindade");
    append(&imoveis_entregues_cabeca, 6, "Pedro Augusto, 23", 1270, "Trindade");

    // Cria as threads de corretores e inquilinos
    for (int i = 0; i < NUM_THREADS; i++)
    {
        printf("Thread de corretor %d criada.\n", i);
        pthread_create(&threads_corretor[i], NULL, thread_corretor_function, (void *)imoveis_disponiveis_cabeca);
        printf("Thread de inquilino %d criada.\n", i);
        pthread_create(&threads_inquilino[i], NULL, thread_inquilino_function, (void *)imoveis_disponiveis_cabeca);
    }

    // Faz o join das threads (volta pra thread principal)
    for (int i = 0; i < NUM_THREADS; i++)
    {
        pthread_join(threads_corretor[i], NULL);
        pthread_join(threads_inquilino[i], NULL);
    }

    // Finaliza o mutex
    pthread_mutex_destroy(&mutex);

    printf("Aguardando termino das threads\n");

    // Lista o imóveis da lista de disponíveis
    list(imoveis_disponiveis_cabeca);

    return 0;
}
