#include <stdio.h>
#include <stdlib.h>
#include "anasem.h"
#include "erros.c"

void retornoDaFuncao(char tipo[], float valor){
  if ((!strcmp(tipo, "inteiro") && valor == 1.0) || (!strcmp(tipo, "real") && valor == 0.0)){
    erro(20);
  }
}
