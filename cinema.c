// Autor: Ana Luísa Salvador Alvarez 160048036
// Autor: Gabriel Rocha Fontenele 150126760
// Atividade: Trabalho Prático

// CINEMA

// F filmes
// S salas com capacidade C de pessoas
// Z pessoas

// Cada vez que um filme acaba em uma sala, comeca um novo filme que que nao esta sendo exibido em nenhuma outra sala.
// Sempre deverão existir mais filmes do que salas.
// Cada pessoa no cinema quer ver todos os filmes em cartaz 1 vez.
// Pessoa sempre procura por um filme que ainda nao viu.
// Se achar e sala nao estiver cheia, entra na secao.
// Se achar e sala estiver cheia, procura por outro filme que ainda nao viu.

//bibliotecas
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>

//constantes
#define N_FILMES 10
#define N_SALAS 4
#define N_CAPACIDADE 4
#define N_PESSOAS 25
#define T_VOLTACAIXA 1
#define T_FILME 1

//sincronizacao
pthread_mutex_t lock_ve_cartaz = PTHREAD_MUTEX_INITIALIZER;	//lock para olhar os filmes em sessao;
pthread_mutex_t lock_muda_cartaz = PTHREAD_MUTEX_INITIALIZER;	//lock para a mudanca de filmes em sessao;
pthread_mutex_t lock_espera_cartaz = PTHREAD_MUTEX_INITIALIZER;	//lock para esperar cartazes novos serem colocados;
pthread_mutex_t lock_cinema = PTHREAD_MUTEX_INITIALIZER;	//lock para ver quantas pessoas estao no cinema (threads pessoa ainda rodando);
pthread_cond_t cond_espera_sessao = PTHREAD_COND_INITIALIZER;

//globais
int Pessoas_Cartaz = 0;	//contador de pessoas escolhendo filmes para assistir;
int Filmes_Cartaz[N_FILMES];	//marcador de quais filmes estao em cartaz em determinado momento;
int Pessoas_Cinema = N_PESSOAS;	//contador de pessoas no cinema (threads pessoa rodando);

typedef struct {  //struct sala
	int quantidade;
	int filme;
	pthread_cond_t cond_sala;
	pthread_mutex_t lock_sala;
} s_sala;

s_sala Salas[N_SALAS];

//funcoes
void* sala(void* arg);
void* pessoa(void* arg);
void entra_sessao(int sala);
void volta_ao_caixa();
void passa_filme();
int procura_filme(int filmes[], int sala_ant);
int troca_filme (int filme_anterior);

void* sala(void *arg){

		int id, filme_novo;
		id = *((int*) arg);
		printf("Sala %d - Criada sala %d\n", id, id);

		pthread_mutex_lock(&Salas[id].lock_sala);
		Salas[id].quantidade = 0;
		Salas[id].filme = -1;
		pthread_mutex_unlock(&Salas[id].lock_sala);

		while(Pessoas_Cinema > 0){
			printf("Sala %d - Esperando para trocar filme\n", id);
			pthread_mutex_lock(&lock_espera_cartaz);	//trava para pessoas pararem de ver os cartazes;
			pthread_mutex_lock(&lock_muda_cartaz);	//trava para poder modificar os cartazes;
			filme_novo = troca_filme(Salas[id].filme);
			printf("Sala %d - Filme %d escolhido, filme anterior = %d\n", id, filme_novo, Salas[id].filme);

			pthread_mutex_lock(&Salas[id].lock_sala);
			Salas[id].filme = filme_novo;	//troca o filme pelo filme encontrado;
			pthread_mutex_unlock(&Salas[id].lock_sala);
			printf("Sala %d - Acordando pessoas esperando filme novo\n", id);

			pthread_cond_broadcast(&cond_espera_sessao);	//ao escolher um filme novo, aorda todos que estavam esperando por nao ter filme nao visto em sessao
			pthread_mutex_unlock(&lock_muda_cartaz);	//libera o acesso a escrita nos cartazes;
			pthread_mutex_unlock(&lock_espera_cartaz);		//libera para pessoas verem os cartazes;
			printf("Sala %d - Passando filme\n", id);

			passa_filme();	//sleep

			pthread_mutex_lock(&Salas[id].lock_sala);	//trava lock para mudar quantidade de pessoas na sala (esvaziar a sala);
			Salas[id].quantidade = 0;	//esvazia a sala;
			printf("Sala %d - Esvaziando sala\n", id);

			pthread_mutex_unlock(&Salas[id].lock_sala);	//destrava o lock da sala;
			pthread_cond_broadcast(&Salas[id].cond_sala);	//acorda pessoas que estavam vendo filme na sala;
		}

}

void* pessoa (void* arg) {

	int id, sala_prox = -1, i;
	int filmes_assistidos[N_FILMES];
	int n_filmes_assistidos = 0;
	id = *((int*) arg);

	printf("Pessoa %d - Criada pessoa %d\n", id, id);

	for(i=0; i<N_FILMES; i++){
		filmes_assistidos[i]=0;
	}

	while(n_filmes_assistidos < N_FILMES){
		printf("Pessoa %d - Esperando para procurar filmes\n", id);

		pthread_mutex_lock(&lock_espera_cartaz);
		pthread_mutex_lock(&lock_ve_cartaz);	//trava as pessoas indo escolher filmes (variavel Pessoas_Cartaz);
		Pessoas_Cartaz++;
		if(Pessoas_Cartaz == 1){
			pthread_mutex_lock(&lock_muda_cartaz);	//se for a primeira pessoa a entrar na sala de cartazes, travar a mudanca de filmes em cartaz;
			printf("Pessoa %d - Primeiro na sala de cartazes\n", id);
		}

		pthread_mutex_unlock(&lock_ve_cartaz);	//libera para mais pessoas escolherem filmes (Pessoas_Cartaz);
		pthread_mutex_unlock(&lock_espera_cartaz);
		sala_prox = procura_filme(filmes_assistidos, sala_prox);
		printf("Pessoa %d - Encontrada sala %d com filme\n", id, sala_prox);

		pthread_mutex_lock(&lock_ve_cartaz);	//trava o acesso aos filmes em cartaz (Pessoas_Cartaz);
		Pessoas_Cartaz--;

		if(Pessoas_Cartaz == 0){
			pthread_mutex_unlock(&lock_muda_cartaz);	//se for a ultima pessoa, libera para as salas trocarem os filmes em cartaz;
			printf("Pessoa %d - Ultima pessoa a sair da sala de cartazes\n", id);
		}

		pthread_mutex_unlock(&lock_ve_cartaz);	//libera para mais pessoas verem os filmes em cartaz;

		if(sala_prox != -1){	//checa se encontrou uma sala passando um filme que ainda nao foi assistido
			pthread_mutex_lock(&Salas[sala_prox].lock_sala);
			printf("Pessoa %d - Vendo se ha vagas na sala %d\n", id, sala_prox);

			if(Salas[sala_prox].quantidade < N_CAPACIDADE){	//checa se a sala escolhida tem capacidade;
				entra_sessao(sala_prox);	//entra na sessao
				filmes_assistidos[Salas[sala_prox].filme] = 1;	//marca que viu o filme;
				n_filmes_assistidos++;
				printf("Pessoa %d - Assistiu filme %d na sala %d, %d filmes assistidos\n", id, Salas[sala_prox].filme, sala_prox, n_filmes_assistidos);

				pthread_cond_wait(&Salas[sala_prox].cond_sala, &Salas[sala_prox].lock_sala);
				pthread_mutex_unlock(&Salas[sala_prox].lock_sala);
			}

			else{
				pthread_mutex_unlock(&Salas[sala_prox].lock_sala);
				printf("Pessoa %d - Sala %d cheia, voltando ao caixa\n", id, sala_prox);
				volta_ao_caixa();	//sleep - tempo para voltar e escolher outro filme (aumenta as chances de ter acabado uma sessao e liberado uma vaga na proxima);
			}

		}

		else{
			pthread_mutex_lock(&lock_ve_cartaz);
			printf("Pessoa %d - Ja assistiu todo os filmes em cartaz, andando pelo shopping\n", id);
			pthread_cond_wait(&cond_espera_sessao, &lock_ve_cartaz);	//dorme caso nao tenha nenhum filme que ainda nao tenha visto em sessao;
			pthread_mutex_unlock(&lock_ve_cartaz);
		}
	}



	pthread_mutex_lock(&lock_cinema);
	printf("Pessoa %d - Assistiu todos os filmes, indo embora do cinema. Pessoas restantes = %d\n", id, Pessoas_Cinema);
	Pessoas_Cinema--;
	pthread_mutex_unlock(&lock_cinema);
}



void entra_sessao(int sala){
	Salas[sala].quantidade++;
}

void volta_ao_caixa(){
	sleep(T_VOLTACAIXA);
}

void passa_filme(){
	sleep(T_FILME);
}

int procura_filme(int filmes[], int sala_ant){
	int i, j;
	j=sala_ant + 1;

	if((j == -1) || (j == N_SALAS)){
		j=0;
	}

	for(i=0; i<N_SALAS; i++){
		if((Salas[j].filme != -1) && (filmes[Salas[j].filme] == 0)){
			return j;
		}
		j++;

		if(j == N_SALAS){
			j=0;
		}
	}
	return -1;
}

int troca_filme (int filme_anterior) {
	int filme = 0;

	while(Filmes_Cartaz[filme] == 1){
		filme = rand() % N_FILMES;
	}

	Filmes_Cartaz[filme_anterior] = 0;
	Filmes_Cartaz[filme] = 1;
	return filme;
}

int main() {
	pthread_t thread_p[N_PESSOAS];
	pthread_t thread_s[N_SALAS];

	int i, *id;
	srand(time(0));

	for(i=0; i<N_FILMES; i++){
		Filmes_Cartaz[i] = 0;
	}

	for (i = 0; i < N_SALAS; i++) {
		id = (int*)malloc(sizeof(int));
		*id = i;
		pthread_create(&thread_s[i], NULL, sala, (void*)id);
		pthread_mutex_init(&Salas[i].lock_sala, NULL);
		pthread_cond_init(&Salas[i].cond_sala, NULL);
	}

	for (i = 0; i < N_PESSOAS; i++) {
		id = (int*)malloc(sizeof(int));
		*id = i;
		pthread_create(&thread_p[i], NULL, pessoa, (void*)id);
	}

	for (i = 0; i < N_PESSOAS; i++){
		pthread_join(thread_p[i], NULL);
	}

	for (i = 0; i < N_SALAS; i++){
		pthread_join(thread_s[i], NULL);
	}

	return 0;
}
