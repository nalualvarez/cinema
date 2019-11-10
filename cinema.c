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
#define CAPACIDADE 50
#define N_PESSOAS 1000

pthread_mutex_t gerenciar_sala;
pthread_mutex_t gerenciar_secao;

typedef struct {
    int assistido[N_FILMES];    // valor 1 se filme[i] ja foi assistido por pessoa
    int contagem;               // numero de filmes ja assistidos
} s_pessoa;

typedef struct {
    int exibicao;       // valor 1 se filme estiver sendo exibido
    int sala;           // sala em que filme esta sendo exibido
} s_filme;

typedef struct {
    int exibicao;       // valor 1 se sala estiver em secao
    int quantidade;     // quantidade de pessoas dentro da sala
} s_sala;

s_pessoa pessoa_estado[N_PESSOAS];          // estado de cada pessoa no cinema
s_filme filme_estado[N_FILMES];             // estado de cada filme em cartaz
s_sala sala_estado[N_SALAS];                // estado de cada sala no cinema
int pessoa_assistiu[N_PESSOAS][N_FILMES];   // pessoa [] ja assistiu filme [] = 1

void* pessoa (void* id) {
    int tid = (long int)id;
    int filme, sala_id;

    // escolhe um filme aleatoriamente
    srand(time(NULL));
    filme = rand() % N_FILMES;

    if (pessoa_estado[tid].assistido[filme] == 0) {
        sala_id = filme_estado[filme].sala;
        pthread_mutex_lock(&gerenciar_sala);
        
        if ((sala_estado[sala_id].exibicao == 1) && (sala_estado[sala_id].quantidade < CAPACIDADE)) {
            sala_estado[sala_id].quantidade++;
            pessoa_estado[tid].assistido[filme] == 1;
        } pthread_mutex_unlock(&gerenciar_sala);
    }
}

void* sala (void* id) {
    int tid = (long int)id;
    int filme;

    int loop = 1;
    while (loop) {

	    // escolhe um filme aleatoriamente
        srand(time(NULL));
    	filme = rand() % N_FILMES;
        
	    // verifica se filme esta sendo exibido em alguma sala 
        pthread_mutex_lock(&gerenciar_secao);
        if (filme_estado[filme].exibicao == 0) {    // se filme nao esta sendo exibido
            filme_estado[filme].exibicao = 1;       // exibir filme na sala atual
            filme_estado[filme].sala = tid;
            loop = 0;
        } pthread_mutex_unlock(&gerenciar_secao);
    }

    // tempo de exibicao do filme
    printf("INI %d : %d\n", tid+1, filme+1);
    // printf("Sala %d comecou a exibir o filme %d\n", tid+1, filme+1);
    int duracao = 0;
    while (duracao < 5) {
        srand(time(NULL));
        duracao = rand()%10;
    } sleep(duracao);

    // termina exibicao do filme na sala atual
    pthread_mutex_lock(&gerenciar_secao);
    filme_estado[filme].exibicao = 0;
    printf("FIM %d : %d\n", tid+1, filme+1);
    // printf("Sala %d terminou a exibicao do filme %d\n", tid+1, filme+1);
    pthread_mutex_unlock(&gerenciar_secao);
}

int main() {
	pthread_t thread_p[N_PESSOAS];
	pthread_t thread_s[N_SALAS];

    pthread_mutex_init(&gerenciar_sala, NULL);
    pthread_mutex_init(&gerenciar_secao, NULL);

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

    pthread_mutex_destroy(&gerenciar_secao);
    pthread_mutex_destroy(&gerenciar_sala);

	return 0;
}
