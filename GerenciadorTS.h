#ifndef GerenciadorTS_H
#define GerenciadorTS_H

#include "analex.h"

typedef enum escopo {
  global, local
} Escopo;

//Definição do tipo Símbolo
typedef struct simbolos {
  char nome[30], tipo[30], categoria[30];
  Escopo escopo;
  int ehZumbi;
  int endereco;
  float valor;
} simbolo;

void apagaSimbolos();
int existeID(token, Escopo);

#endif
