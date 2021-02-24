/*
 * Created by: Matheus Scaketti and Enzo Salvadori
 * Date: 29/11/17
 * 
*/

#include "bib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int numTotalRegra = 0;
int ascii = 90;

void leGramatica(gramatica *gramatica){ 

	FILE *g = fopen("gramatica.txt", "r");

	int isCabeca = 1; //verificação se é cabeça da regra ou corpo
	char aux; //variável que recebe a leitura do arquivo
	int numRegra = 0; //quantidade de regras
	int i = 0, j = 0; //contadores de posição(na string) da cabeça(i) e do corpo(j)

	while(!feof(g)){ //percorre o arquivo
		fflush(stdin);
		aux = fgetc(g);
		if(aux != '=') {
			if(isCabeca) gramatica->cabeca[numRegra][i++] = aux; //le a cabeça da regra
			else{
				if(aux != '\n') //Não chegou final da regra
					gramatica->corpo[numRegra][j++] = aux; //le o corpo da regra
				else { //Próxima regra
					isCabeca = 1;
					gramatica->corpo[numRegra][j] = '\0'; //Determina o final da string do corpo
					j = 0;
					numRegra++;
				}
			}
		}else{ //Encontrou o =, troca leitura de cabeça para o corpo
			gramatica->cabeca[numRegra][i] = '\0'; //Determina o final da string da cabeca
			i = 0;
			isCabeca = 0;
		}

	}
	numTotalRegra = numRegra;

	fclose(g);
}

void printaGramatica(gramatica gramatica){ 
	int i;
	printf("Gramática:\n");
	for(i = 0; i < numTotalRegra; i++){
		printf("\t%s = %s\n", &gramatica.cabeca[i][0], &gramatica.corpo[i][0]);
	}
	printf("\n");
}

int verificaGLC(gramatica gramatica){ 
	int i;
	for(i = 0; i < numTotalRegra; i++)
		if(strlen(gramatica.cabeca[i]) > 1) return 0;

	return 1;
}

int verificaEtapa1(gramatica gramatica){ //Verifica se possui produções vazias(Simplificação)
	int i;
	printaGramatica(gramatica);
	for(i = 0; i < numTotalRegra; i++)
		if(gramatica.corpo[i][0] == '&') {
			printf("Palavra vazia encontrada!\n");
			return 0;
	}
	return 1;
}

int verificaEtapa2(gramatica gramatica){ //Verifica se possui produções que geram somente variaveis(Simplificação)
	int i;
	int valorAscii;
	printaGramatica(gramatica);
	for(i = 0; i < numTotalRegra; i++){
		valorAscii = gramatica.corpo[i][0];
		if(valorAscii >= 65 && valorAscii <= 90 && gramatica.corpo[i][1] == '\0'){
			printf("Produção que gera somente uma variável encontrada!\n");
			return 0;
		}
	}
	return 1;
}

int verificaEtapa3(gramatica gramatica){ //Verifica se possui produções improdutivas(Simplificação)
	int i, j, k;
	int contadorCabeca = 0;
	int contadorCorpo = 0;
	printaGramatica(gramatica);
	for(i = 0; i < numTotalRegra; i++) //Variaveis que não geram regras (vazio)
		if(gramatica.corpo[i][0] == '\0'){
			printf("Variável sem regra encontrada!\n");
			return 0;
		}

	for(i = 0; i < numTotalRegra; i++){ //Variaveis inexistentes
		for(j = 0; j < strlen(gramatica.corpo[i]); j++){ //Verifica as regras
			if(gramatica.corpo[i][j] == '\0') break; //verifica se chegou ao final da regra
			//verifica se possui variavel no meio do corpo
			if(gramatica.corpo[i][j] >= 65 && gramatica.corpo[i][j] <= 90){ //variavel no meio do corpo
				for(k = 0; k < numTotalRegra; k++){ //verifica se a variavel possui uma regra
					if(gramatica.corpo[i][j] == gramatica.cabeca[k][0]) contadorCabeca++;
				}
				if(contadorCabeca == 0){
					printf("Variável sem regra encontrada!\n");
					return 0;
				}
			}
		}
		contadorCabeca = 0;
	}

	//verificar as cabeças que não são atingíveis
	for(i = 1; i < numTotalRegra; i++){ //percorre as cabeças para ver se possui alguma
		for(j = 0; j < numTotalRegra; j++){ //percorre todas as regras para procurar a cabeça selecionada
			for(k = 0; k < strlen(gramatica.corpo[j]); k++){ //percorre todo o corpo de cada regras
				if(gramatica.corpo[j][k] == '\0') break; //verifica se chegou ao final da regra

				//verifica se possui variavel no meio do corpo
				if(gramatica.corpo[j][k] >= 65 && gramatica.corpo[j][k] <= 90){ //variavel no meio do corpo
					if(gramatica.cabeca[i][0] == gramatica.corpo[j][k]) contadorCorpo++;
				}
			}
		}
		if(contadorCorpo == 0){
			printf("Variável não atingivel encontrada!\n");
			return 0; //verifica se a regra atual é atingível a partir do corpo
		}
		contadorCorpo = 0; //se atingir este ponto, zera a contador para prepara-lo para a próxima variavel
	}
	return 1;
}

void chomsky1(gramatica *gramatica){ //Aplica a segunda etapa de chomsky (Produções >= 2)
	int i, j;

	for(i = 0; i < numTotalRegra; i++){
		if(strlen(gramatica->corpo[i])>=2){
			for(j = 0; j < strlen(gramatica->corpo[i]); j++){
				if((*gramatica).corpo[i][j] >= 97 && gramatica->corpo[i][j] <= 122){
					insereRegra1(gramatica, gramatica->corpo[i][j]); //Insere nova regra
					gramatica->corpo[i][j] = gramatica->corpo[i][j]-26; //coloca a variavel no lugar do terminal
				}
			}
		}
	}
	printaGramatica(*gramatica);
}

void insereRegra1(gramatica *gramatica, char variavel){ //insere uma nova regra para a segunda etapa de chomsky
	int i;
	for(i = 0; i < numTotalRegra; i++) //verifica se ja foi criado a regra nova
		if(gramatica->cabeca[i][0] == variavel-26 ) return;

	gramatica->cabeca[numTotalRegra][0] = variavel-26; //cria variável em relação ao terminal
	gramatica->cabeca[numTotalRegra][1] = '\0';

	gramatica->corpo[numTotalRegra][0] = variavel; //copia o terminal para o corpo da regra
	gramatica->corpo[numTotalRegra][1] = '\0';

	numTotalRegra++; //aumenta a quantidade de regras
}

void chomsky2(gramatica *gramatica){ //Aplica a terceira etapa de chomsky (Produções >= 3)
	int i;

	for(i = 0; i < numTotalRegra; i++){
		while(strlen((*gramatica).corpo[i])>=3){
			gramatica->corpo[i][strlen(gramatica->corpo[i]) - 2] = insereRegra2(gramatica, &(gramatica->corpo[i][strlen(gramatica->corpo[i]) - 2]));
			gramatica->corpo[i][strlen(gramatica->corpo[i]) - 1] = '\0';
		}
	}

	printaGramatica(*gramatica);

	salvaArquivo(*gramatica);
}

char insereRegra2(gramatica *gramatica, char *variavel){ //insere uma nova regra para a terceira etapa de chomsky
	gramatica->cabeca[numTotalRegra][0] = ascii--;
	gramatica->corpo[numTotalRegra][0] = variavel[0];
	gramatica->corpo[numTotalRegra][1] = variavel[1];
	gramatica->corpo[numTotalRegra][2] = '\0';
	numTotalRegra++;

	return (char) (ascii+1);
}

int simplificacao(gramatica gramatica){ //verifica se a gramática está pronta para ser simplificada
	if(verificaGLC(gramatica)){
		system("clear");
		if(verificaEtapa1(gramatica)){
			system("clear");
			if(verificaEtapa2(gramatica)){
				system("clear");
				if(verificaEtapa3(gramatica)){
					system("clear");
					return 1;
				}
			}
		}
	}

	return 0;
}

void salvaArquivo(gramatica gramatica){ //salva a gramática na FNC em um arquivo de saida
	FILE *g = fopen("gramaticaFNC.txt", "w");

	int i;

	for(i = 0; i < numTotalRegra; i++) fprintf(g, "%s=%s\n", gramatica.cabeca[i], gramatica.corpo[i]);

	fclose(g);
}
