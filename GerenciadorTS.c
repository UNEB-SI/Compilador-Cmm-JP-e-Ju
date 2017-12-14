#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "analex.h"
#include "erros.h"
#include "GerenciadorTS.h"

simbolo pilhaSimbolos[100];
int topoSimbolos = 0;

//Função que apaga identificadores da pilha de Símbolos
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

//Função que confere se um identificador existe ou não. 1 se Sim, 0 se Não.
int existeID(token TOKEN, Escopo escopo) {
  int i=0;
  while (i < topoSimbolos) { //procura na pilha se o nome do identificador já existe
    if (!strcmp(TOKEN.lexema, pilhaSimbolos[i].nome))
      break;
    i++;
  }

  if (i != topoSimbolos) {
    if (escopo == global && pilhaSimbolos[i].escopo != global)
      return 0; //o identificador "não existe" dentro de seu escopo

    else if (escopo == local && pilhaSimbolos[i].escopo != local) {
      i++;
      while (i < topoSimbolos) {
        if (!strcmp(TOKEN.lexema, pilhaSimbolos[i].nome))
          break;
        i++;
      }
      if (i == topoSimbolos)
        return 0; //se chegar no topo da pilha, é porque o nome do identificador não existe e pode ser colocado o novo id
      else
        return 1; //o identificador já existe na pilha e não poderá ser salvo o novo ID
    }

    else if (escopo == local && pilhaSimbolos[i].escopo == local) {
      if (!pilhaSimbolos[i].ehZumbi) //Se o ID encontrado na pilha não for um parâmetro morto, o novo ID não será salvo
        return 1;
      else {
        i++;
        while (i < topoSimbolos) { //continuará procurando por outros identificadores de mesmo nome
          if (!strcmp(TOKEN.lexema, pilhaSimbolos[i].nome))
            if (!pilhaSimbolos[i].ehZumbi) //se não for parâmetro morto, o novo ID não será salvo
              return 1;
          i++;
        }
        if (i == topoSimbolos)
          return 0; //ao chegar no final da pilha, o novo ID poderá ser salvo
      }
    }

    else
      return 1; //o identificador já foi declarado antes.
  }

  return 0; //o identificador não existe, portanto, pode ser salvo na Pilha de Símbolos
}
