// Autores: Ana Luísa Salvador Alvarez 160048036 e Gabriel Rocha Fontenele 150126760
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

const int FILMES = 10;
const int SALAS = 5;
const int CAPACIDADE = 50;
const int PESSOAS = 1000;


int assistidos[FILMES][PESSOAS]; //VETOR DE PESSOAS DE VETORES DE FILMES ASSISTIDOS

typedef struct {
	int id
	int quantidade;
	int filmepassando;
}sala;

sala passando[SALAS];

void pessoa (){
	
}

void sala (){
	int filme;
	
	filme = rand()%FILMES; //gera randomicamente qual filme vai ser passado
}

int main(){
	
	int i, j;
	
	pthread_t threadp[PESSOAS];
	pthread_t threads[SALAS];
	
	for (i=0; i<PESSOAS; i++){
		pthread_create(&threadp[i], NULL, pessoa, (void*)i);
	}
	
	for (j=0; j<SALAS; j++){
		pthread_create(&threads[j], NULL, sala, (void*)j);
	}
	
			
	return 0;

}


