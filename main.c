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
  prog();
  /*while(1) {
    T = analex(FD);

    printf("< %s, %s >\n", T.categoria, T.lexema);

    if (strcmp(T.categoria, "FIM_ARQUIVO") == 0)
      break;
  }*/

  fclose(FD);
  return 0;
}
