#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "analex.c"
#include "anasint.h"
#include "erros.c"

// INICIO DOS PROCEDIMENTOS DE ANÁLISE DE EXPRESSÕES ARITMÉTICAS

//Precisa atualizar essa função
void Expressao() {
    Termo();
    Resto();
}

void Termo() {
    Fator();
    Sobra();
}


//Precisa atualizar essa função
void Resto() {
    if (!strcmp(T.categoria, "SN") && (!strcmp(T.sinal, "soma") || !strcmp(T.sinal, "substituicao"))) {
        T  = analex(stdin);
        Termo();
        Resto();
    }
    else; //retorna vazio
}

void Sobra() {
    if (!strcmp(T.categoria, "SN") && (!strcmp(T.sinal, "multiplicacao") || !strcmp(T.sinal, "divisao"))) {
        analex(stdin);
        Fator();
    }
    else if (!strcmp(T.sinal, "e_logico")){
        analex(stdin);
        Fator();
    }
}

void Fator() {
    if(!strcmp(T.categoria, "ID") || !strcmp(T.categoria, "CT_I") ||
       !strcmp(T.categoria, "CT_R") || !strcmp(T.categoria, "CT_LT")){

        if (!strcmp(T.categoria, "ID")){
            analex(stdin);
            if(!strcmp(T.sinal, "abre_par")){
                analex(stdin);
                Expressao();
                if(!strcmp(T.sinal, "virgula")){
                    while(!strcmp(T.sinal, "virgula")){
                        analex(stdin);
                        Expressao();
                    }
                    if(!strcmp(T.sinal, "fecha_par"))
                        analex(stdin);
                    else {
                        erro(3);
                    }
                }
            }
            else if (!strcmp(T.sinal, "nao_logico")){
                analex(stdin);
                Fator();
            }

        }else
            analex(stdin);
    }

    /*if (!strcmp(T.categoria, "SN") && !strcmp(T.sinal, "abre_parenteses")) {
        T = analex(stdin);
        Expressao();
        if (!strcmp(T.categoria, "SN") && !strcmp(T.sinal, "fecha_parenteses"))
            T = analex(stdin);
        else {
            erro(3);
        }
    }*/

    else if (!strcmp(T.categoria, "digito"))
        T = analex(stdin);

    else {
        erro(5);
    }
}

// Função que fará a leitura das declarações de variáveis globais, prototipos e criação de funções no código-fonte
void prog() {
  int ehprototipo = 0; // 0 - declaração de variável; 1 - prototipo de função
  int ehfuncao = 0; // Verifica se a função possui corpo ou não.
  char tipoAux[30]; // variável auxiliar para armazenar o tipo da variável ou função

  //Verifica se o token atual é um protótipo de função
  if (!strcmp(T.categoria, "PR") && !strcmp(T.lexema, "prototipo")) {
    analex(stdin);
    ehprototipo = 1;
  }

  //Analisa o token atual e verifica se é um tipo válido para declaração de uma variável ou função
  if (!strcmp(T.categoria, "PR") && (!strcmp(T.lexema, "caracter"))
                                 ||  !strcmp(T.lexema, "inteiro")
                                 ||  !strcmp(T.lexema, "real")
                                 ||  !strcmp(T.lexema, "booleano")
                                 ||  !strcmp(T.lexema, "semretorno"))
  {
    strcpy(tipoAux, T.lexema); //Armazena o token na variavel auxiliar para guardar os tipos dos identificadores na pilha
  }

  do { //Fará a leitura e armazenamento dos identificadores para a pilha de simbolos
    analex(stdin);
    //Verifica se o token atual é um identificador. Se for, armazena-o na pilha de simbolos e lê-se o próximo token.
    if (!strcmp(T.categoria, "ID")) {
      strcpy(pilhaSimbolos[topoSimbolos].nome, T.lexema);
      strcpy(pilhaSimbolos[topoSimbolos].tipo, tipoAux);
      strcpy(pilhaSimbolos[topoSimbolos].categoria, "variavel");
      pilhaSimbolos[topoSimbolos].escopo = global;
      pilhaSimbolos[topoSimbolos].ehZumbi = 0;
      topoSimbolos++;
      analex(stdin);
    }
    else
      erro(6);

    if (!strcmp(T.categoria, "SN") && !strcmp(T.sinal, "abre_par")) {
      strcpy(pilhaSimbolos[--topoSimbolos].categoria, "funcao"); // Informa à pilha que o símbolo armazenado anteriormente trata-se de uma função
      topoSimbolos++;
      ehfuncao = 1;
      do { //armazena os paramentros na pilha de sinais
        analex(stdin);
        if (!strcmp(T.categoria, "PR") && !strcmp(T.lexema, "semparam")) { // Verifica se a função não possui parâmetros
          analex(stdin);
          if (!strcmp(T.categoria, "SN") && !strcmp(T.sinal, "fecha_parenteses")) {
            analex(stdin);
            break; // Constata que a função não possui parâmetros e termina a declaração
          }
          else {
            printf("Erro Sintático! Parâmetro vazio especificado. Não espera-se mais parâmetros!\n");
          }
        }
        else if (!strcmp(T.categoria, "PR") && (!strcmp(T.lexema, "caracter"))
                                            ||  !strcmp(T.lexema, "inteiro")
                                            ||  !strcmp(T.lexema, "real")
                                            ||  !strcmp(T.lexema, "booleano"))
        {
          strcpy(pilhaSimbolos[topoSimbolos].tipo, T.lexema);
          analex(stdin);
        }
        else {
          printf("Erro Sintático! Tipo do Parâmetro esperado!\n");
        }
        strcpy(pilhaSimbolos[topoSimbolos].categoria, "param");
        pilhaSimbolos[topoSimbolos].escopo = local;
        pilhaSimbolos[topoSimbolos].ehZumbi = 0;
        pilhaSimbolos[topoSimbolos].codigo = qtd_ID++; //armazena o codigo do simbolo associando a quantidade de identificadores. Depois incrementa o contador
        if (!strcmp(T.categoria, "ID")) {
          strcpy(pilhaSimbolos[topoSimbolos].nome, T.lexema);
          topoSimbolos++;
          analex(stdin);
        }
        else if (!ehprototipo && !(!strcmp(T.categoria, "ID"))) {
          // Significa dizer que a função terá corpo, logo, deverá ter os nomes dos parâmetros especificados
          printf("Erro Sintático! Nome de Parâmetro de Função omitido!\n");
        }
        else {
          topoSimbolos++;
        }
      } while(!(!strcmp(T.categoria, "SN") && !strcmp(T.sinal, "fecha_parenteses"))); /* Se não for "fecha parenteses )"
                                                                o token atual, o laço continuará rodando. Se for vírgula,
                                                                o primeiro analex(stdin) do laço pulará */
    }
    else if (ehprototipo && !(!strcmp(T.categoria, "SN") && !strcmp(T.sinal, "abre_par"))) {
      erro(7);
    }
    analex(stdin);
  } while(!strcmp(T.categoria, "SN") && !strcmp(T.sinal, "virgula"));

  if (!ehfuncao && !strcmp(T.categoria, "SN") && !strcmp(T.sinal, "ponto_virgula")) {
    analex(stdin);
  }
  else if (!ehfuncao && !(!strcmp(T.categoria, "SN") && !strcmp(T.sinal, "ponto_virgula"))) {
    erro(8);
  }
  else if (ehfuncao && !(!strcmp(T.categoria, "SN") && !strcmp(T.sinal, "abre_chaves"))) {
    erro(9);
  }
  else if (ehfuncao && !strcmp(T.categoria, "SN") && !strcmp(T.sinal, "abre_chaves")) { // Abre a construção do corpo da função
    analex(stdin);
    while (!(!strcmp(T.categoria, "SN") && !strcmp(T.sinal, "fecha_chaves"))) {
      if (!strcmp(T.categoria, "PR") && (!strcmp(T.lexema, "caracter"))
                                     ||  !strcmp(T.lexema, "inteiro")
                                     ||  !strcmp(T.lexema, "real")
                                     ||  !strcmp(T.lexema, "booleano"))
      { //declaração de variáveis locais da função
        strcpy(tipoAux, T.lexema);

        do { //Fará a leitura e armazenamento das variáveis para a pilha de simbolos
          analex(stdin);
          //Verifica se o token atual é um identificador. Se for, armazena-o na pilha de simbolos e lê-se o próximo token.
          if (!strcmp(T.categoria, "ID")) {
            strcpy(pilhaSimbolos[topoSimbolos].nome, T.lexema);
            strcpy(pilhaSimbolos[topoSimbolos].tipo, tipoAux);
            strcpy(pilhaSimbolos[topoSimbolos].categoria, "variavel");
            pilhaSimbolos[topoSimbolos].escopo = local;
            pilhaSimbolos[topoSimbolos].ehZumbi = 0;
            topoSimbolos++;
            analex(stdin);
          }
          else
            erro(6);
        } while(!strcmp(T.categoria, "SN") && !strcmp(T.sinal, "virgula"));

        if (!strcmp(T.categoria, "SN") && !strcmp(T.sinal, "ponto_virgula")) {
          analex(stdin);
        }
        else if (!(!strcmp(T.categoria, "SN") && !strcmp(T.sinal, "ponto_virgula"))) {
          erro(8);
        }
      }
      else if (!strcmp(T.categoria, "ID")) {
        cmd();
        // TALVEZ SEJA NECESSÁRIO UM analex(stdin) AQUI. VAI DEPENDER DE COMO TERMINA A FUNÇÃO cmd()
      }
      else
        analex(stdin); //chama o próximo token
    }
  }
}

// Função que realiza os comandos da linguagem
void cmd() {
    if (!strcmp(T.categoria, "PR") || !strcmp(T.categoria, "ID") || !strcmp(T.categoria, "SN")) {
        if (!strcmp(T.categoria, "SN")) {
            if (!strcmp(T.sinal, "ponto_virgula"))
                analex(stdin);
            else if (!strcmp(T.sinal, "abre_chaves")) {
                analex(stdin);
                cmd();
                if (!strcmp(T.sinal, "fecha_chaves"))
                    analex(stdin);
                else {
                    erro(10);
                }
            }
        }
        else if (!strcmp(T.categoria, "ID")){
            analex(stdin);
            if (!strcmp(T.sinal, "abre_par")){
                analex(stdin);
                Expressao();

                if (!strcmp(T.sinal, "virgula")){
                    analex(stdin);
                    Expressao();
                    if (!strcmp(T.sinal, "fecha_par")){
                        analex(stdin);
                        if(!strcmp(T.sinal, "ponto_virgula"))
                            analex(stdin);
                        else {
                            erro(8);
                        }
                    }
                    else {
                        erro(3);
                    }
                }

            }
            else if (!strcmp(T.sinal, "igual")){
                analex(stdin);
                Expressao();
            }
        }
        else if (!strcmp(T.categoria, "PR")){
            if(!strcmp(T.lexema, "retorne")){
                analex(stdin);
                Expressao();
                if(!strcmp(T.sinal, "ponto_virgula"))
                    analex(stdin);
                else {
                    erro(8);
                }
            }
            else if (!strcmp(T.lexema, "para")){
                analex(stdin);
                if (!strcmp(T.sinal, "abre_par")) {
                    analex(stdin);
                    atrib();

                    if(!strcmp(T.sinal, "ponto_virgula")){
                        analex(stdin);
                        Expressao();
                        if(!strcmp(T.sinal, "ponto_virgula")){
                            analex(stdin);
                            atrib();
                            if (!strcmp(T.sinal, "fecha_par")) {
                                analex(stdin);
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
                analex(stdin);
                if (!strcmp(T.sinal, "abre_par")) {
                    analex(stdin);
                    Expressao();
                    if (!strcmp(T.sinal, "fecha_par")) {
                        analex(stdin);
                        cmd();
                    }
                    else {
                        erro(3);
                    }
                }
            }
            else if (!strcmp(T.lexema, "se")) {
                analex(stdin);
                if (!strcmp(T.sinal, "abre_par")) {
                    analex(stdin);
                    Expressao();
                    if (!strcmp(T.sinal, "fecha_par")) {
                        analex(stdin);
                        cmd();
                        if (!strcmp(T.lexema, "senao")) {
                            analex(stdin);
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

void atrib(){
    if(!strcmp(T.categoria, "ID")){
        analex(stdin);
        if (!strcmp(T.sinal, "igual"))
            Expressao();
    }
}

void op_rel(){
    if(!strcmp(T.categoria, "SN")) {
        if(!strcmp(T.sinal, "maior_igual") || !strcmp(T.sinal, "menor_igual") ||
           !strcmp(T.sinal, "menor") || !strcmp(T.sinal, "maior") ||
           !strcmp(T.sinal, "nao_igual") || !strcmp(T.sinal, "igual_igual" )){

            analex(stdin);
        }

    }
}
