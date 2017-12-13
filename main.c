#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include "analex.c"
#include "anasint.c"

extern FILE *FD;

int main() {

  int c, i=0;

  FD = fopen("documento.txt", "r");

  if (FD == NULL) { //teste de abertura do arquivo
   printf ("Houve um erro ao abrir o arquivo.\n");
   return 1;
  }
  T = analex(FD);
  while(1) {
    //T = analex(FD);
    prog();

    //printf("< %s, %s >\n", T.categoria, T.lexema);

    if (strcmp(T.categoria, "FIM_ARQUIVO") == 0)
      break;
  }

  printf("\nAnálise Sintática efetuada com Sucesso!\n");

  printf("\nPilha de Simbolos (Deve ficar apenas identificadores globais):\n");
  for (i=0; i<topoSimbolos; i++) {
    printf("Nome: %s - Catg: %s - Tipo: %s - Escp: %d - Endr: %d - ehZumbi: %d\n", pilhaSimbolos[i].nome, pilhaSimbolos[i].categoria, pilhaSimbolos[i].tipo, pilhaSimbolos[i].escopo, pilhaSimbolos[i].endereco, pilhaSimbolos[i].ehZumbi);
  }

  printf("\nPilha de identificadores: (Pertencente ao Analex. Cogita-se retirar essa pilha)\n");
  for (i=0; i<qtd_ID; i++) {
    printf("Nome: %s\n", ID[i]);
  }

  fclose(FD);
  return 0;
}
