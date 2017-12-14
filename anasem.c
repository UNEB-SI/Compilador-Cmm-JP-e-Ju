#include <stdio.h>
#include <stdlib.h>
#include "anasem.h"
#include "erros.c"
#include "GerenciadorTS.h"

void retornoDaFuncao(char tipo[], float valor){

  if ((!strcmp(tipo, "inteiro") && valor == 1.0) || (!strcmp(tipo, "real") && valor == 0.0))
    erro(20);

  if (!strcmp(tipo, "semretorno") && temRetorno==1)
    erro(25);

  if(strcmp(tipo, "semretorno") && temRetorno==0)
    erro(26);
}

void paramChamadaFuncao(int qtdPDec, int qtdPCham){
  if(qtdPDec > qtdPCham)
    erro(30);
  if(qtdPDec < qtdPCham)
    erro(31);
}

int getTotalParam(int posicaoId){
  int total = 0;
  int x = posicaoId +1;
  while(!strcmp(pilhaSimbolos[x].categoria, "param")){
    //printf("-------------------------%s\n", pilhaSimbolos[x].categoria);
    total++;
    x++;
  }
  //printf("------------------------------TOTAL: %d\n", total);
  return total;
}
