#!/bin/bash

#SBATCH --partition=test
#SBATCH --job-name=paralelo
#SBATCH --output=paralelo_matriz.out
#SBATCH --error=paralelo_matriz.err
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=32
#SBATCH --time=0-0:30
#SBATCH --hint=compute_bound
#SBATCH --exclusive

#Carrega os módulos do sistema
rm runtime_oddeven_paralelo.txt
module load compilers/gnu/7.3
module load compilers/intel/2018.2.199
eval $loadIntelLibs

#Compila o código
gcc paralelo_matriz.c -o paralelo_matriz -lpthread

tentativas=15 #Quantas vezes o código será executado

for cores in 2 4 8 15 30 #número de cores utilizados
do
	for size in 700 900 1100 1400 #tamanho do problema			
	do   									
		echo -e "\n$cores\t$size\t\t\c" >> "paralelo_matriz_time.txt" 
			
		for tentativa in $(seq $tentativas) #Cria uma vetor de 1 a "tentativas"				
		do
			echo -e `./paralelo_matriz $cores $size`	#$cores como número de processos e $size como tamanho da matriz						
		done			
	done
done	
		
exit 
