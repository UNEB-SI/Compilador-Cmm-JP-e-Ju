#ifndef GerenciadorTS_H
#define GerenciadorTS_H

//Definição do tipo Símbolo
typedef struct simbolos {
  char nome[30], tipo[30], categoria[30];
  Escopo escopo;
  int ehZumbi;
  int endereco;
  float valor;
} simbolo;

simbolo pilhaSimbolos[100];
int topoSimbolos = 0;

#endif
