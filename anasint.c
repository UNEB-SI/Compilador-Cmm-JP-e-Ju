#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "analex.c"
#include "anasint.h"
#include "GerenciadorTS.c"
#include "erros.c"
#include <unistd.h>


// INICIO DOS PROCEDIMENTOS DE ANÁLISE DE EXPRESSÕES ARITMÉTICAS

//Função OK
void Expressao() {
  expr_simp();
  if (!strcmp(T.categoria, "SN") && (!strcmp(T.sinal, "maior_igual") || !strcmp(T.sinal, "menor_igual")
                                                                        || !strcmp(T.sinal, "menor")
                                                                        || !strcmp(T.sinal, "maior")
                                                                        || !strcmp(T.sinal, "nao_igual")
                                                                        || !strcmp(T.sinal, "igual_igual")))
  {
    //T = analex(FD);
    op_rel();
    expr_simp();
  }
}

//Função OK
void expr_simp() {

  if (!strcmp(T.categoria, "SN") && (!strcmp(T.sinal, "soma") || !strcmp(T.sinal, "substituicao"))) {
    T  = analex(FD);
  }

  Termo();
  while (!strcmp(T.categoria, "SN") && (!strcmp(T.lexema, "+") || !strcmp(T.lexema, "-") || !strcmp(T.sinal, "ou_logico"))) {
    T = analex(FD);
    Termo();
  }
}

//Função OK
void Termo() {
  Fator();
  while (!strcmp(T.categoria, "SN") && (!strcmp(T.lexema, "*") || !strcmp(T.lexema, "/") || !strcmp(T.sinal, "e_logico"))) {
    Sobra();
  }
}

//Função OK
void Sobra() {
  T = analex(FD);
  Fator();
}

//Função OK
void Fator() {
  if(!strcmp(T.categoria, "ID") || !strcmp(T.categoria, "CT_I") ||
  !strcmp(T.categoria, "CT_R") || !strcmp(T.categoria, "CT_C") ||
  !strcmp(T.categoria, "CT_LT"))
  {
    if (!strcmp(T.categoria, "ID")){
      T = analex(FD);
      if(!strcmp(T.sinal, "abre_par")){
        T = analex(FD);
        Expressao();
        if(!strcmp(T.sinal, "virgula")){
          while(!strcmp(T.sinal, "virgula")){
            T = analex(FD);
            Expressao();
          }

        }
        if(!strcmp(T.sinal, "fecha_par"))
        T = analex(FD);
        else {
          erro(3);
        }
      }

    } else
      T = analex(FD);
  }


  else if (!strcmp(T.categoria, "SN") && !strcmp(T.sinal, "abre_par")) {
    T = analex(FD);
    Expressao();
    if (!strcmp(T.categoria, "SN") && !strcmp(T.sinal, "fecha_par")){
      T = analex(FD);
    }

    else {
      erro(3);
    }
  }
  else if (!strcmp(T.sinal, "nao_logico")){
    T = analex(FD);
    Fator();
  }

  else if (!strcmp(T.categoria, "digito")){
    T = analex(FD);
  }


  /*else if(strcmp(T.lexema, "EOF")){
    printf("AQ6\n");
    erro(5);
  }*/
}

// Função que fará a leitura das declarações de variáveis globais, prototipos e criação de funções no código-fonte
void prog() {
  int ehprototipo = 0; // 0 - declaração de variável; 1 - prototipo de função
  int ehfuncao = 0; // Verifica se a função possui corpo ou não.
  int semparam = 0;
  char tipoAux[30]; // variável auxiliar para armazenar o tipo da variável ou função
  //printf("Token: %s\n", T.lexema);

  //Verifica se o token atual é um protótipo de função
  if (!strcmp(T.categoria, "PR") && !strcmp(T.lexema, "prototipo")) {
    T = analex(FD);
    ehprototipo = 1;
    //printf("Token: %s\n", T.lexema);
  }
  //Analisa o token atual e verifica se é um tipo válido para declaração de uma variável ou função
  if (!strcmp(T.categoria, "PR") && (!strcmp(T.lexema, "caracter"))
                                 ||  !strcmp(T.lexema, "inteiro")
                                 ||  !strcmp(T.lexema, "real")
                                 ||  !strcmp(T.lexema, "booleano")
                                 ||  !strcmp(T.lexema, "semretorno"))
  {
    if (!ehprototipo)
      strcpy(tipoAux, T.lexema); //Armazena o token na variavel auxiliar para guardar os tipos dos identificadores na pilha
  }
  else {
    erro(14);
  }

  do { //Fará a leitura e armazenamento dos identificadores para a pilha de simbolos
    T = analex(FD);
    //printf("Token: %s\n", T.lexema);
    //Verifica se o token atual é um identificador. Se for, armazena-o na pilha de simbolos e lê-se o próximo token.
    if (!strcmp(T.categoria, "ID") || !strcmp(T.lexema, "principal")) {
      if (!ehprototipo) {
        strcpy(pilhaSimbolos[topoSimbolos].nome, T.lexema);
        strcpy(pilhaSimbolos[topoSimbolos].tipo, tipoAux);
        strcpy(pilhaSimbolos[topoSimbolos].categoria, "variavel");
        pilhaSimbolos[topoSimbolos].escopo = global;
        pilhaSimbolos[topoSimbolos].ehZumbi = 0;
        printf("%s | %s | %s | %d \n", pilhaSimbolos[topoSimbolos].nome, pilhaSimbolos[topoSimbolos].tipo, pilhaSimbolos[topoSimbolos].categoria, pilhaSimbolos[topoSimbolos].escopo = global);
        topoSimbolos++;
      }
      T = analex(FD);
      //printf("Token: %s\n", T.lexema);
    }
    else
      erro(6);

    if (!strcmp(T.categoria, "SN") && !strcmp(T.sinal, "abre_par")) {
      if (!ehprototipo) {
        strcpy(pilhaSimbolos[--topoSimbolos].categoria, "funcao"); // Informa à pilha que o símbolo armazenado anteriormente trata-se de uma função
        topoSimbolos++;
        ehfuncao = 1;
      }

      do { //armazena os paramentros na pilha de sinais
        T = analex(FD);
        //printf("Token: %s\n", T.lexema);
        if (!strcmp(T.categoria, "PR") && !strcmp(T.lexema, "semparam")) { // Verifica se a função não possui parâmetros
          T = analex(FD);
          semparam = 1;
          //printf("Token: %s\n", T.lexema);
          if (!strcmp(T.categoria, "SN") && !strcmp(T.sinal, "fecha_par")) {
            T = analex(FD);
            //printf("Token: %s\n", T.lexema);
            break; // Constata que a função não possui parâmetros e termina a declaração
          }
          else {
            erro(11);
          }
        }
        else if (!strcmp(T.categoria, "PR") && (!strcmp(T.lexema, "caracter"))
                                            ||  !strcmp(T.lexema, "inteiro")
                                            ||  !strcmp(T.lexema, "real")
                                            ||  !strcmp(T.lexema, "booleano"))
        {
          if (!ehprototipo)
            strcpy(pilhaSimbolos[topoSimbolos].tipo, T.lexema);
          T = analex(FD);
          //printf("Token: %s\n", T.lexema);
        }
        else {
          erro(12);
        }
        if (!ehprototipo) {
          strcpy(pilhaSimbolos[topoSimbolos].categoria, "param");
          pilhaSimbolos[topoSimbolos].escopo = local;
          pilhaSimbolos[topoSimbolos].ehZumbi = 0;
          pilhaSimbolos[topoSimbolos].codigo = qtd_ID++; //armazena o codigo do simbolo associando a quantidade de identificadores. Depois incrementa o contador
        }
        if (!strcmp(T.categoria, "ID")) {
          if (!ehprototipo) {
            strcpy(pilhaSimbolos[topoSimbolos].nome, T.lexema);
            printf("%s | %s | %s | %d \n", pilhaSimbolos[topoSimbolos].nome, pilhaSimbolos[topoSimbolos].tipo, pilhaSimbolos[topoSimbolos].categoria, pilhaSimbolos[topoSimbolos].escopo);
            topoSimbolos++;
          }
          T = analex(FD);
          //printf("Token: %s\n", T.lexema);
        }
        else if (!ehprototipo && !(!strcmp(T.categoria, "ID"))) {
          // Significa dizer que a função terá corpo, logo, deverá ter os nomes dos parâmetros especificados
          erro(13);
        }
        else {
          if (!ehprototipo) {
            printf("%s | %s | %s | %d \n", pilhaSimbolos[topoSimbolos].nome, pilhaSimbolos[topoSimbolos].tipo, pilhaSimbolos[topoSimbolos].categoria, pilhaSimbolos[topoSimbolos].escopo);
            topoSimbolos++;
          }
        }
      } while(!(!strcmp(T.categoria, "SN") && !strcmp(T.sinal, "fecha_par"))); /* Se não for "fecha parenteses )"
      o token atual, o laço continuará rodando. Se for vírgula,
      o primeiro analex(FD) do laço pulará */
    }
    else if (ehprototipo && !(!strcmp(T.categoria, "SN") && !strcmp(T.sinal, "abre_par"))) {
      erro(7);
    }
    if ((ehfuncao || ehprototipo) && !semparam) { //Analex danadinho
      T = analex(FD);
      //printf("Token: %s\n", T.lexema);
    }
  } while(!strcmp(T.categoria, "SN") && !strcmp(T.sinal, "virgula"));

  if (!ehfuncao && !strcmp(T.categoria, "SN") && !strcmp(T.sinal, "ponto_virgula")) {
    T = analex(FD);
    //printf("Token: %s\n", T.lexema);
  }
  else if (!ehfuncao && !(!strcmp(T.categoria, "SN") && !strcmp(T.sinal, "ponto_virgula"))) {
    if (!strcmp(T.categoria, "FIM_ARQUIVO") && !strcmp(T.lexema, "EOF"))
      erro(0);
    erro(8);
  }
  else if (ehfuncao && !(!strcmp(T.categoria, "SN") && !strcmp(T.sinal, "abre_chaves"))) {

    erro(9);
  }
  else if (ehfuncao && !strcmp(T.categoria, "SN") && !strcmp(T.sinal, "abre_chaves")) { // Abre a construção do corpo da função

    //printf("Token: riaria %s\n", T.lexema);
    T = analex(FD);

    while (!(!strcmp(T.categoria, "SN") && !strcmp(T.sinal, "fecha_chaves"))) {
      if (!strcmp(T.categoria, "PR") && (!strcmp(T.lexema, "caracter"))
                                     ||  !strcmp(T.lexema, "inteiro")
                                     ||  !strcmp(T.lexema, "real")
                                     ||  !strcmp(T.lexema, "booleano"))
      { //declaração de variáveis locais da função

        strcpy(tipoAux, T.lexema);
        do { //Fará a leitura e armazenamento das variáveis para a pilha de simbolos
          T = analex(FD);
          //printf("Token: %s\n", T.lexema);
          //Verifica se o token atual é um identificador. Se for, armazena-o na pilha de simbolos e lê-se o próximo token.
          if (!strcmp(T.categoria, "ID")) {
            //printf("ENTREI\n");
            strcpy(pilhaSimbolos[topoSimbolos].nome, T.lexema);
            strcpy(pilhaSimbolos[topoSimbolos].tipo, tipoAux);
            strcpy(pilhaSimbolos[topoSimbolos].categoria, "variavel");
            pilhaSimbolos[topoSimbolos].escopo = local;
            pilhaSimbolos[topoSimbolos].ehZumbi = 0;
            printf("%s | %s | %s | %d \n", pilhaSimbolos[topoSimbolos].nome, pilhaSimbolos[topoSimbolos].tipo, pilhaSimbolos[topoSimbolos].categoria, pilhaSimbolos[topoSimbolos].escopo);
            topoSimbolos++;
            T = analex(FD);
            //printf("Token: %s\n", T.lexema);
          }
          else
            erro(6);
        } while(!strcmp(T.categoria, "SN") && !strcmp(T.sinal, "virgula"));

        if (!strcmp(T.categoria, "SN") && !strcmp(T.sinal, "ponto_virgula")) {
          T = analex(FD);
          //printf("Token: %s\n", T.lexema);
        }
        else if (!(!strcmp(T.categoria, "SN") && !strcmp(T.sinal, "ponto_virgula"))) {
          erro(8);
        }

      }

      /*else if (!strcmp(T.categoria, "ID")) {
        cmd();
        // TALVEZ SEJA NECESSÁRIO UM analex(FD) AQUI. VAI DEPENDER DE COMO TERMINA A FUNÇÃO cmd()
      }*/

      else if (!strcmp(T.lexema, "EOF"))
        erro(10);

      else {
        cmd(); //chama o próximo token
        //printf("Token: %s\n", T.lexema);
      }

    }
  }
if (!strcmp(T.categoria, "SN") && !strcmp(T.sinal, "fecha_chaves")) {
    T = analex(FD);
    apagaSimbolos();
    //printf("Token: %s\n", T.lexema);
  }

}

// Função que realiza os comandos da linguagem
void cmd() {
  if (!strcmp(T.categoria, "PR") || !strcmp(T.categoria, "ID") || !strcmp(T.categoria, "SN")) {
    if (!strcmp(T.categoria, "SN")) {
      if (!strcmp(T.lexema, ";"))
        T = analex(FD);
      else if (!strcmp(T.lexema, "{")) {
        T = analex(FD);
        while (strcmp(T.lexema, "}")) {
          cmd();
        }
        if (!strcmp(T.lexema, "}"))
          T = analex(FD);
        else {
          erro(10);
        }
      }
    }
    else if (!strcmp(T.categoria, "ID")) {
      T = analex(FD);
      if (!strcmp(T.sinal, "abre_par")) { //verifica se o token atual é abre parenteses
        T = analex(FD);
        Expressao();

        while (!strcmp(T.sinal, "virgula")) {
          T = analex(FD);
          Expressao();
        }
        if (!strcmp(T.sinal, "fecha_par")) {
          T = analex(FD);
          if(!strcmp(T.sinal, "ponto_virgula"))
          T = analex(FD);
          else {
            erro(8);
          }
        }
        else {
          erro(3);
        }
      }
      else if (!strcmp(T.sinal, "igual")) { //verifica se o token atual é igual
        T = analex(FD);
        Expressao();
        if(!strcmp(T.sinal, "ponto_virgula"))
          T = analex(FD);
        else
          erro(8);
      }
    }
    else if (!strcmp(T.categoria, "PR")) {

      if(!strcmp(T.lexema, "retorne")){
        T = analex(FD);
        Expressao();
        if(!strcmp(T.sinal, "ponto_virgula"))
        T = analex(FD);
        else {
          erro(8);
        }
      }
      else if (!strcmp(T.lexema, "para")) {
        T = analex(FD);
        if (!strcmp(T.sinal, "abre_par")) {
          T = analex(FD);

          atrib();
          if(!strcmp(T.sinal, "ponto_virgula")) {
            T = analex(FD);
            Expressao();
            if(!strcmp(T.sinal, "ponto_virgula")) {
              T = analex(FD);
              //Verificar com Atta se é atrib ou expressão
              atrib();
              if (!strcmp(T.sinal, "fecha_par")) {
                T = analex(FD);
                cmd();
              }
              else {
                erro(3);
              }
            }
            else {
              erro(8);
            }
          }
          else {
            erro(8);
          }
        }
      }
      else if (!strcmp(T.lexema, "enquanto")) {
        T = analex(FD);
        if (!strcmp(T.sinal, "abre_par")) {
          T = analex(FD);
          Expressao();
          if (!strcmp(T.sinal, "fecha_par")) {
            T = analex(FD);
            cmd();
          }
          else {
            erro(3);
          }
        }
      }
      else if (!strcmp(T.lexema, "se")) {
        T = analex(FD);
        if (!strcmp(T.sinal, "abre_par")) {
          T = analex(FD);
          Expressao();
          if (!strcmp(T.sinal, "fecha_par")) {
            T = analex(FD);
            cmd();
            if (!strcmp(T.lexema, "senao")) {
              T = analex(FD);
              cmd();
            }
          }
          else {
            erro(3);
          }
        }
      }
    }
  }
}

//Função OK
void atrib(){
  if(!strcmp(T.categoria, "ID")){
    T = analex(FD);
    if (!strcmp(T.sinal, "igual")){
      T = analex(FD);
      Expressao();
    }

  }
}

//Função OK
void op_rel(){
  if (!strcmp(T.categoria, "SN")) {
    if (!strcmp(T.sinal, "maior_igual") || !strcmp(T.sinal, "menor_igual") ||
    !strcmp(T.sinal, "menor") || !strcmp(T.sinal, "maior") ||
    !strcmp(T.sinal, "nao_igual") || !strcmp(T.sinal, "igual_igual" )) {
      T = analex(FD);
    }
  }
}
