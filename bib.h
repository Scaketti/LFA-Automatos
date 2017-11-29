/*
 * Created by: Matheus Scaketti
 * Date: 29/11/17
*/

#ifndef BIB_H
#define BIB_H
#include <stdio.h>
#include <stdlib.h>

#define QTD 100

struct gramatica{
	char cabeca[QTD][QTD];
	char corpo[QTD][QTD];
};
typedef struct gramatica gramatica;

void leGramatica(gramatica *gramatica);
void printaGramatica(gramatica gramatica);
int verificaGLC(gramatica gramatica);
int verificaEtapa1(gramatica gramatica);
int verificaEtapa2(gramatica gramatica);
int verificaEtapa3(gramatica gramatica);
void chomsky1(gramatica *gramatica);
void insereRegra1(gramatica *gramatica, char variavel);
void chomsky2(gramatica *gramatica);
char insereRegra2(gramatica *gramatica, char *variavel);
int simplificacao(gramatica gramatica);
void salvaArquivo(gramatica gramatica);

#endif