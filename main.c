#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include "analex.c"
#include "anasint.c"
//#include "MP.h"

extern FILE *FD;

int main() {

  int c, i=0, qtdVariaveisGlobais = 0;

  FD = fopen("documento.txt", "r");

  if (FD == NULL) { //teste de abertura do arquivo
   printf ("Houve um erro ao abrir o arquivo.\n");
   return 1;
  }

  printf("INIP\n"); //Comando de início do programa
  T = analex(FD);

  //Execução dos comandos do programa
  while(1) {
    prog();
    //printf("< %s, %s >\n", T.categoria, T.lexema);

    if (strcmp(T.categoria, "FIM_ARQUIVO") == 0)
      break;
  }

  //Imprime os últimos comandos da MP
  printf("CALL L%d\n", labelPrincipal);
  while (i<topoSimbolos) {
    if (pilhaSimbolos[i].escopo == global && !strcmp(pilhaSimbolos[i].categoria, "variavel"))
      qtdVariaveisGlobais++;
    i++;
  }
  printf("DMEM %d\n", qtdVariaveisGlobais);
  printf("HALT\n");
  printf("\nAnálise Sintática efetuada com Sucesso!\n");

  //Imprime a pilha de Símbolos
  printf("\nPilha de Simbolos (Deve ficar apenas identificadores globais):\n");
  for (i=0; i<topoSimbolos; i++) {
    printf("Nome: %s - Catg: %s - Tipo: %s - Escp: %d - ", pilhaSimbolos[i].nome, pilhaSimbolos[i].categoria, pilhaSimbolos[i].tipo, pilhaSimbolos[i].escopo);
    if (!strcmp(pilhaSimbolos[i].categoria, "funcao"))
      printf("Endr: L%d - ", pilhaSimbolos[i].endereco);
    else
      printf("Endr: %d - ", pilhaSimbolos[i].endereco);
    printf("ehZumbi: %d\n", pilhaSimbolos[i].ehZumbi);
  }

  //Imprime a pilha de Identificadores
  printf("\nPilha de identificadores: (Pertencente ao Analex. Cogita-se retirar essa pilha)\n");
  for (i=0; i<qtd_ID; i++) {
    printf("Nome: %s\n", ID[i]);
  }

  fclose(FD);
  return 0;
}
