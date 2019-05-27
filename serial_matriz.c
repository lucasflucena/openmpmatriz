#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>

void print(int** matriz, char* nome);

int n_size;  
int **A, **B, **C;

int main(int argc, char* argv[]) {
	
	int i,j,r;
	srand(0); 
	struct timeval start, stop;
	n_size = strtol(argv[1], NULL, 10);
   	
	//aloca as matrizes
	A = (int **)malloc(n_size * sizeof(int*));
	B = (int **)malloc(n_size * sizeof(int*));
	C = (int **)malloc(n_size * sizeof(int*));
	for (i=0; i<n_size; i++){
    	A[i] = (int *)malloc(n_size * sizeof(int));
    	B[i] = (int *)malloc(n_size * sizeof(int));
    	C[i] = (int *)malloc(n_size * sizeof(int));
	}

	//preenche as matrizes
   	for (i=0; i<n_size; i++){			
      	for (j=0; j<n_size; j++){
         	A[i][j] = rand()%10;
         	B[i][j] = rand()%10;
         	C[i][j] = -1;
   		}
	}

	gettimeofday(&start, NULL);

	//multiplicação das matrizes
	for (i=0; i<n_size; i++){
    	for (j=0; j<n_size; j++){
        	
        	C[i][j]=0;
        	for(r=0; r<n_size; r++){
        		C[i][j] += A[i][r] * B[r][j];
        	}       	 
    	}
   	}

	gettimeofday(&stop, NULL);                  

   	char arquivo[] = "serial_matriz_time.txt";		//imprime o tempo de execução no txt
    FILE *fp;     
    fp = fopen(arquivo, "a");               
       
    if (fp == NULL){
    	exit(1);
    } else {
        fprintf(fp, "%.3f\t", (stop.tv_sec + stop.tv_usec*1e-6)-(start.tv_sec + start.tv_usec*1e-6));   
        fclose(fp);
    }

    //paralelo_matriz.out
    print(C, "C");
	
	free(A);
	free(B);
	free(C);
	return 0;
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
