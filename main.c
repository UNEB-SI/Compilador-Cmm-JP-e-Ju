#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "analex.h"
#include "anasint.h"
#include "erros.h"
#include "GerenciadorTS.h"

FILE *FD;
FILE *MP;
const char arquivoCmm[] = {"documento.txt"};
const char arquivoMP[] = {"objeto.txt"};
extern token T;
extern simbolo pilhaSimbolos[100];
extern int topoSimbolos;
extern int labelPrincipal;
extern int qtd_ID;
extern char ID[100][100];
extern int qtd_ID;

int main() {

  int i=0, qtdVariaveisGlobais = 0;

  FD = fopen(arquivoCmm, "r");
  MP = fopen(arquivoMP, "w");

  if (FD == NULL) { //teste de abertura do arquivo
   printf ("Houve um erro ao abrir o arquivo.\n");
   return 1;
  }

  fprintf(MP, "INIP\n"); //Comando de início do programa
  T = analex(FD);

  //Execução dos comandos do programa
  while(1) {
    prog();

    if (strcmp(T.categoria, "FIM_ARQUIVO") == 0)
      break;
  }

  //Imprime os últimos comandos da MP
  if (labelPrincipal)
    fprintf(MP, "CALL L%d\n", labelPrincipal);
  while (i<topoSimbolos) {
    if (pilhaSimbolos[i].escopo == global && !strcmp(pilhaSimbolos[i].categoria, "variavel"))
      qtdVariaveisGlobais++;
    i++;
  }
  fprintf(MP, "DMEM %d\n", qtdVariaveisGlobais);
  fprintf(MP, "HALT");

  printf("\nAnalise Sintatica efetuada com Sucesso!\n");

  fclose(MP);
  fclose(FD);
  return 0;
}
