// Autor: Ana Luísa Salvador Alvarez 160048036
// Autor: Gabriel Rocha Fontenele 150126760
// Atividade: Trabalho Prático

// CINEMA

// F filmes
// S salas com capacidade C de pessoas
// Z pessoas

// Cada vez que um filme acaba em uma sala, comeca um novo filme que que nao esta sendo exibido em nenhuma outra sala.
// Cada pessoa no cinema quer ver todos os filmes em cartaz 1 vez.
// Pessoa sempre procura por um filme que ainda nao viu.
// Se achar e sala nao estiver cheia, entra na secao.
// Se achar e sala estiver cheia, procura por outro filme que ainda nao viu.

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>

#define N_FILMES 10
#define N_SALAS 5
#define N_CAPACIDADE 50
#define N_PESSOAS 1000

pthread_mutex_t gerenciar_sala;

int sala_estado[N_SALAS];			// quantidade de pessoas dentro da sala
int filme_estado[N_FILMES];			// valor 1 se filme esta sendo exibido em alguma sala
int pessoa_assistiu[N_PESSOAS][N_FILMES];	// pessoa [] ja assistiu filme [] = 1

void* pessoa () {
}

void* sala (void* id) {
    int tid = (long int)id;
    int loop = 1;

    int filme;
    do {
	// escolhe um filme aleatoriamente
        srand(time(NULL));
    	filme = rand() % N_FILMES;
        
	// verifica se filme esta sendo exibido em alguma sala 
        pthread_mutex_lock(&gerenciar_sala);
        if (filme_estado[filme] == 0) {		// se filme nao esta sendo exibido
            filme_estado[filme] = 1;		// exibir filme na sala atual
            loop = 0;
        } pthread_mutex_unlock(&gerenciar_sala);
    } while (loop);

    // tempo de exibicao do filme
    printf("Sala %d exibindo filme %d\n", tid+1, filme+1);
    srand(time(NULL));
    sleep(rand()%10);

    // termina exibicao do filme na sala atual
    pthread_mutex_lock(&gerenciar_sala);
    filme_estado[filme] = 0;
    printf("Fim da exibicao do filme %d\n", filme+1);
    pthread_mutex_unlock(&gerenciar_sala);
}

int main() {
	pthread_t thread_p[N_PESSOAS];
	pthread_t thread_s[N_SALAS];

    pthread_mutex_init(&gerenciar_sala, NULL);

	for (long int i = 0; i < N_PESSOAS; i++) {
		pthread_create(&thread_p[i], NULL, pessoa, (void*)i);
	}
	for (long int i = 0; i < N_SALAS; i++) {
		pthread_create(&thread_s[i], NULL, sala, (void*)i);
	}

	for (long int i = 0; i < N_PESSOAS; i++)
		pthread_join(thread_p[i], NULL);
	for (long int i = 0; i < N_SALAS; i++)
		pthread_join(thread_s[i], NULL);

    pthread_mutex_destroy(&gerenciar_sala);

	return 0;
}
