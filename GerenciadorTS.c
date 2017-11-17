#include <stdio.h>
#include <stdlib.h>
#include "GerenciadorTS.h"

void apagaSimbolos() {
  topoSimbolos--;
  /* Apaga as variáveis locais e parâmetros da pilha de símbolos para
  que seus nomes possam ser reutilizados*/
  while (pilhaSimbolos[topoSimbolos].escopo == local) {
    if (!strcmp(pilhaSimbolos[topoSimbolos].categoria, "param")) {
      pilhaSimbolos[topoSimbolos].ehZumbi = 1; //transforma paramentro em ehZumbi
      /* isso ocorre para que o analisador semantico possa saber quantos
      paramentros uma função possui */
    }
    topoSimbolos--; //elimina o identificador local da pilha de simbolos
  }
  topoSimbolos++; // Faz com que o topo aponte pro primeiro parâmetro após a função
  while (!strcmp(pilhaSimbolos[topoSimbolos].categoria, "param")) {
    topoSimbolos++; /* enquanto houver parâmetros, topo sobe até apontar pra
    uma região livre pra alocar os próximos identificadores */
  }
}
