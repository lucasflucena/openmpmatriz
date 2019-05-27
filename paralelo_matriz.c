#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h> 
#include <semaphore.h>
#include <stdbool.h>

const int MAX_THREADS = 64;

int m_threads, n_size;  
int **A, **B, **C, **D;
sem_t* semaphore;

void *multiplication(void* rank);
void print(int** matriz, char* nome);
bool temnegativo(int** matriz);

int main(int argc, char* argv[]) {
	
	int i,j, valor;
	srand(0);
	long       thread;  /* Use long in case of a 64-bit system */
	pthread_t* thread_address; 
	struct timeval start, stop;

   	m_threads = 		strtol(argv[1], NULL, 10);
   	n_size = 			strtol(argv[2], NULL, 10);
   	thread_address = 	malloc (m_threads*sizeof(pthread_t));   //vetor de endereço das threads
   	semaphore = 		malloc(n_size*sizeof(sem_t));
   	
	//iniciando os semáforos
	for (i=0; i<n_size; i++){
		sem_init(&semaphore[i], 0, 0);
	}

	//aloca as matrizes
	A = (int **)malloc(n_size * sizeof(int*));
	B = (int **)malloc(n_size * sizeof(int*));
	C = (int **)malloc(n_size * sizeof(int*));
	D = (int **)malloc(n_size * sizeof(int*));
	for (i=0; i<n_size; i++){
    	A[i] = (int *)malloc(n_size * sizeof(int));
    	B[i] = (int *)malloc(n_size * sizeof(int));
    	C[i] = (int *)malloc(n_size * sizeof(int));
    	D[i] = (int *)malloc(n_size * sizeof(int));
	}

	//preenche as matrizes
   	for (i=0; i<n_size; i++){			
      	for (j=0; j<n_size; j++){
         	A[i][j] = rand()%10;
         	B[i][j] = rand()%10;
         	C[i][j] = -1;
         	D[i][j] = 0;
   		}
	}

	gettimeofday(&start, NULL);

	for (thread = 0; thread < m_threads; thread++){  //Creates thread 0 to m_threads-1
    	pthread_create(&thread_address[thread], NULL, multiplication, (void*) thread);  
	}

	//main part
	bool* foicopiado = malloc(n_size*sizeof(bool));
	for(i=0; i<n_size; i++){
		foicopiado[i]=false;
	}

	for(int cont=0; cont<n_size; cont++){
		
		valor=0;
		i=rand()%n_size;
		sem_getvalue(&semaphore[i], &valor);
		
		if(valor==1){
			if(foicopiado[i]){} //nao faz nada
			else {
				for(j=0; j<n_size; j++){
					D[i][j]=C[i][j];
				}
				
				foicopiado[i]=true;
			}			
		}
	}
	for (thread = 0; thread < m_threads; thread++){    
    	pthread_join(thread_address[thread], NULL); 
    }

	gettimeofday(&stop, NULL);                  // Tempo final              

   	char arquivo[] = "paralelo_matriz_time.txt";
    FILE *fp;     
    fp = fopen(arquivo, "a");                   //Abre o arquivo txt
       
    if (fp == NULL) exit(1);
    else {

        if(temnegativo(D)){
        	fprintf(fp, "0");
        } else {
        	fprintf(fp, "%.3f\t", (stop.tv_sec + stop.tv_usec*1e-6)-(start.tv_sec + start.tv_usec*1e-6));   //Salva o tempo de execução na tabela
        	fclose(fp);
        }
    }

    //paralelo_matriz.out
    print(C, "C");
    print(D, "D");

    //destruindo os semáforos
	for (i=0; i<n_size; i++){
		sem_destroy(&semaphore[i]);
	}
	
	free(thread_address);
	free(semaphore);
	free(A);
	free(B);
	free(C);
	free(D);
	return 0;
}

void *multiplication(void* rank) {
   
   	long my_rank = (long) rank;  /* Use long in case of 64-bit system */ 
   	long inicio = my_rank*(n_size/m_threads);
   	long final = inicio+(n_size/m_threads)-1;
   	
   	if(my_rank == m_threads-1){
    	final=n_size-1;
	}

	int i,j,r;
	for (i=inicio; i<=final; i++){
    	for (j=0; j<n_size; j++){
        	
        	C[i][j]=0;
        	for(r=0; r<n_size; r++){
        		C[i][j] += A[i][r] * B[r][j];
        	}       	 
    	}
        sem_post(&semaphore[i]);
   	}
	return NULL;
}

bool temnegativo(int** matriz) {		
             						            			
  	for (int i=1; i<n_size; i++) {			
        for (int j=1; j<n_size; j++){
            	if(matriz[i][j]<0){
            		return true;   
           		}
        } 
    } 
    return false;
}

void print(int** matriz, char* nome){

	printf("## Matriz %s ##\n",nome);
	for (int i=0; i<n_size; i++){
    	for (int j=0; j<n_size; j++){
        	printf("%d \t",matriz[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}
