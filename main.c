/*
 * Created by: Matheus Scaketti
 * Date: 29/11/17
*/

#include "bib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(){

	gramatica gramatica;

	int choose = 0;
	int continuar = 1;
	int verChomsky1 = 0;

	leGramatica(&gramatica);

	while(continuar){ //Loop para manter no menu
		printf("Selecione uma opção:\n1 - Verificar se é GLC.\n2 - Verificar Etapa 1.\n3 - Verificar Etapa 2.\n");
		printf("4 - Verificar Etapa 3.\n5 - Chomsky Etapa 4.\n6 - Chomsky Etapa 5.\n9 - Sair.\n");
		scanf("%d", &choose);

		switch(choose){ //Escolha da função/verificação
			case 1: //verifica se é GLC
				system("clear");
				printaGramatica(gramatica);
				if(verificaGLC(gramatica)){
					printf("Gramática é uma GLC!\n\n");
				}
				else printf("Gramática não é uma GLC!\n\n");
				break;

			case 2:	//verifica etapa1, simplificação (producoes vazias)
				system("clear");
				if(verificaEtapa1(gramatica)){
					printf("Gramática verificada na Etapa 1 - Simplificação.\n\n");
				}
				else printf("Gramática não OK na Etapa 1 - Simplificação!\n\n");
				break;

			case 3:	//verifica etapa2, simplificação (variavel -> variavel)
				system("clear");
				if(verificaEtapa2(gramatica)){
					printf("Gramática verificada na Etapa 2 - Simplificação.\n\n");
				}
				else printf("Gramática não OK na Etapa 2 - Simplificação!\n\n");
				break;

			case 4:	//verifica etapa3, simplificação (simbolos improdutivos)
				system("clear");
				if(verificaEtapa3(gramatica)){
					printf("Gramática verificada na Etapa 3 - Simplificação.\n\n");
				}
				else printf("Gramática não OK na Etapa 3 - Simplificação!\n\n");
				break;

			case 5: //Simplificação de chomsky produções >= 2
				system("clear");
				if(simplificacao(gramatica)){
					chomsky1(&gramatica);
					verChomsky1 = 1;
				}else printf("Não está simplificado!\n\n");
				break;

			case 6: //Simplificação de chomsky produções >= 3
			system("clear");
				if(verChomsky1) chomsky2(&gramatica);
				else printf("Fazer etapa 4!\n\n");
				break;

			case 9:
				continuar = 0;
				break;

			default:
				printf("Erro!\n");
		}
	}

	return 1;
}