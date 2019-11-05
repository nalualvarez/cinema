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

int FILMES = 10;
int SALAS = 5;
int CAPACIDADE = 50;
int PESSOAS = 1000;


typedef struct{ //tipo de variavel com nome e veotr de quais filmes a pessoa ja viu (indice do vetor recebe: 1 = viu, 0 = nao viu)
	char nome[20];
	int filmes[FILMES];
} Pessoa;

Pessoa todos[PESSOAS];

void assiste (){
	
}

int passafilme(){ //gera randomicamente qual filme vai ser passado
	int vaipassar;
	vaipassar = rand()%FILMES;
	return vaipassar;
}



int main(){
	
	int i, j;
	
	//zera vetor de filmes assistidos
	for (i=0; i<PESSOAS; i++){
		for(j=0;j<FILMES;j++)
		todos[i].filmes[j] = 0;
	}
	
	while(1){ //roda direto pegando sala randomicamente + filme randomicamente e testa se as pessoas ja assistiram
		for ()
	}
	
	
	
	return 0;

}


