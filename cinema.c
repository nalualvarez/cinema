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

const int N_FILMES = 10;
const int N_SALAS = 5;
const int N_CAPACIDADE = 50;
const int N_PESSOAS = 1000;

int assistido[N_PESSOAS][N_FILMES];

typedef struct {
	int quantidade;
	int filme;
} s_sala;

s_sala sala[N_SALAS];

void pessoa () {
}

void secao () {
	int filme;
	filme = rand() % N_FILMES;
}

int main() {
	pthread_t thread_p[N_PESSOAS];
	pthread_t thread_s[N_SALAS];

	for (long int i = 0; i < N_PESSOAS; i++) {
		pthread_create(&thread_p[i], NULL, pessoa, (void*)i);
	}
	for (long int i = 0; i < N_SALAS; i++) {
		pthread_create(&thread_s[i], NULL, secao, (void*)i);
	}

	for (long int i = 0; i < N_PESSOAS; i++)
		pthread_join(thread_p[i], NULL);
	for (long int i = 0; i < N_SALAS; i++)
		pthread_join(thread_s[i], NULL);

	return 0;
}
