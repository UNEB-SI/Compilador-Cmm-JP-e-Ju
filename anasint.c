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
float Expressao() {
  float primOp = 0, resultado = 0;
  static int i = 0; //Contador estático que não mudará seu valor independente da vez que a função seja chamada

  printf("------ ENTROU NO EXPR %d\n", i);
  primOp = expr_simp();
  if (!strcmp(T.categoria, "SN") && (!strcmp(T.sinal, "maior_igual") || !strcmp(T.sinal, "menor_igual")
                                                                     || !strcmp(T.sinal, "menor")
                                                                     || !strcmp(T.sinal, "maior")
                                                                     || !strcmp(T.sinal, "nao_igual")
                                                                     || !strcmp(T.sinal, "igual_igual")))
  {
    //T = analex(FD);
    op_rel();
    resultado = expr_simp();
  }
  else
    resultado = primOp; /*Se não houver nenhum operador relacional pra comparar com outra expressão, o
                        resultado é o próprio valor da primeira operação*/
  printf("----- EXPRESSAO %d: %.1f\n", i++, resultado); //Imprime o resultado. Posso colocar pra retornar depois
  return resultado;
}

//Função OK
float expr_simp() {
  float primOp, resultado;
  primOp = resultado = 0;

  if (!strcmp(T.categoria, "SN") && (!strcmp(T.sinal, "soma") || !strcmp(T.sinal, "substituicao"))) {
    T  = analex(FD);
  }

  resultado = primOp = Termo();
  while (!strcmp(T.categoria, "SN") && (!strcmp(T.lexema, "+") || !strcmp(T.lexema, "-") || !strcmp(T.sinal, "ou_logico"))) {
    if (!strcmp(T.lexema, "+")) { // realiza a soma dos operandos
      T = analex(FD);
      resultado = Termo();
      if (primOp != resultado)
        erro(17);
      else {
        if (!resultado)
          printf("ADD\n");
        else if (resultado == 1)
          printf("ADDF\n");
      }
      //resultado = primOp + Termo();
    }
    else if (!strcmp(T.lexema, "-")) { // realiza a subtração dos operandos
      T = analex(FD);
      resultado = Termo();
      if (primOp != resultado) //confere se os tipos são diferentes e um emite erro caso forem
        erro(17);
      else {
        if (!resultado)
          printf("SUB\n");
        else if (resultado == 1)
          printf("SUBF\n");
      }
      //resultado = primOp - Termo();
    }
    primOp = resultado; /* Usa 'primOp' como variável auxiliar para outros termos serem calculados junto
                          ao resultado depois da virada do laço */
  }

  return resultado;
}

//Função OK
float Termo() {
  float primOp, resultado;
  resultado = primOp = Fator();

  while (!strcmp(T.categoria, "SN") && (!strcmp(T.lexema, "*") || !strcmp(T.lexema, "/") || !strcmp(T.sinal, "e_logico"))) {
    if (!strcmp(T.lexema, "*")) { // realiza a multiplicação dos operandos
      T = analex(FD);
      resultado = Termo();
      if (primOp != resultado) //confere se os tipos são diferentes e um emite erro caso forem
        erro(17);
      else {
        if (!resultado)
          printf("MUL\n");
        else if (resultado == 1)
          printf("MULF\n");
      }
      //resultado = primOp * Termo();
    }
    else if (!strcmp(T.lexema, "/")) { // realiza a divisão dos operandos
      T = analex(FD);
      resultado = Termo();
      if (primOp != resultado) //confere se os tipos são diferentes e um emite erro caso forem
        erro(17);
      else {
        if (!resultado)
          printf("DIV\n");
        else if (resultado == 1)
          printf("DIVF\n");
      }
      //resultado = primOp / Termo();
    }
    primOp = resultado;
  }

  return resultado;
}

//Função OK
float Fator() {
  float resultado=0;

  if(!strcmp(T.categoria, "ID") || !strcmp(T.categoria, "CT_I") ||
   !strcmp(T.categoria, "CT_R") || !strcmp(T.categoria, "CT_C") ||
   !strcmp(T.categoria, "CT_LT"))
  {
    if (!strcmp(T.categoria, "ID")) { // Tratamento de operações com identificador: variável ou retorno de chamada de função
      int i=0, j;
      while (i < topoSimbolos) { // Busca encontrar o identificador na pilha de Símbolos
        if (!strcmp(T.lexema, pilhaSimbolos[i].nome))
          break; //Encontra o identificador na pilha e sai do laço
        i++;
      }
      if (i == topoSimbolos)
        erro(15);
      T = analex(FD);
      j=i+1; //contador para verificar se o próximo elementro na pilha é um parâmetro de função
      if(!strcmp(T.sinal, "abre_par")) {
        T = analex(FD);
        if (!strcmp(pilhaSimbolos[j].categoria, "param")) {
          resultado = Expressao();
          if (resultado == 0) { //verifica se a expressão no parâmetro é do tipo inteiro
            if (!strcmp(pilhaSimbolos[j].tipo, PR[2])) { //verifica se o parâmetro é do tipo inteiro
              j++;
            }
            else erro(18); //Erro de tipo de parâmtro incompatível
          }
          else if (resultado == 1) { //verifica se a expressão no parâmetro é do tipo real
            if (!strcmp(pilhaSimbolos[j].categoria, "param")) {
              if (!strcmp(pilhaSimbolos[j].tipo, PR[3])) { //verifica se o parâmetro é do tipo real
                j++;
              }
              else erro(18);
            }
          }

          if(!strcmp(T.sinal, "virgula")) {
            while(!strcmp(T.sinal, "virgula")) {
              T = analex(FD);
              if (!strcmp(pilhaSimbolos[j].categoria, "param")) {
                resultado = Expressao();
                if (resultado == 0) {
                  if (!strcmp(pilhaSimbolos[j].tipo, PR[2])) {
                    j++;
                  }
                  else erro(18);
                }
                else if (resultado == 1) {
                  if (!strcmp(pilhaSimbolos[j].categoria, "param")) {
                    if (!strcmp(pilhaSimbolos[j].tipo, PR[3])) {
                      j++;
                    }
                    else erro(18);
                  }
                }
              }
            }
          }
        }
        if(!strcmp(T.sinal, "fecha_par")) {
          if (!strcmp(pilhaSimbolos[i].tipo, PR[2]) || !strcmp(pilhaSimbolos[i].tipo, PR[1])) //verifica se o número é do tipo inteiro ou char
            resultado = 0;
          else if (!strcmp(pilhaSimbolos[i].tipo, PR[3])) //verifica se o número é do tipo real
            resultado = 1;
          else
            erro(16); // O operando é inválido, pois não possui o tipo compatível com a expressão
          printf("LOAD %d\n", pilhaSimbolos[i].endereco);
          T = analex(FD);
        }
        else {
          erro(3);
        }
      }
      else if (!strcmp(pilhaSimbolos[i].categoria, "variavel")) {
        if (!strcmp(pilhaSimbolos[i].tipo, PR[2]) || !strcmp(pilhaSimbolos[i].tipo, PR[1])) //verifica se o número é do tipo inteiro ou char
          resultado = 0;
        else if (!strcmp(pilhaSimbolos[i].tipo, PR[3])) //verifica se o número é do tipo real
          resultado = 1;
        /*else if (!strcmp(pilhaSimbolos[i].tipo, PR[1])) { //verifica se o número é do tipo char
          resultado = 2;
        }*/
        else
          erro(16); // O operando é inválido, pois não possui o tipo compatível com a expressão
        printf("LOAD %d\n", pilhaSimbolos[i].endereco);
      }
    }
    else { // Obtenção de valores para cálculos de expressões aritméticas
      if (!strcmp(T.categoria, categorias[3]) || !strcmp(T.categoria, categorias[6])) { //verifica se o número é do tipo inteiro ou char
        resultado = 0;
        printf("PUSH %.0f\n", T.valor);
      }
      else if (!strcmp(T.categoria, categorias[4])) { //verifica se o número é real (float)
        resultado = 1;
        printf("PUSH %f\n", T.valor);
      }
      /*else if (!strcmp(T.categoria, categorias[6])) { //verifica se o número é caracter (char)
        resultado = 2;
      }*/
      T = analex(FD);
    }
  }

  else if (!strcmp(T.categoria, "SN") && !strcmp(T.sinal, "abre_par")) {
    T = analex(FD);
    resultado = Expressao();
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
    resultado = T.valor;
    T = analex(FD);
  }

  /*else if(strcmp(T.lexema, "EOF")){
    printf("AQ6\n");
    erro(5);
  }*/

  return resultado;
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
        if (!existeID(T, global)) {
          strcpy(pilhaSimbolos[topoSimbolos].nome, T.lexema);
          strcpy(pilhaSimbolos[topoSimbolos].tipo, tipoAux);
          strcpy(pilhaSimbolos[topoSimbolos].categoria, "variavel");
          pilhaSimbolos[topoSimbolos].escopo = global;
          pilhaSimbolos[topoSimbolos].ehZumbi = 0;
          topoSimbolos++;
        }
        else erro(19);
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
          pilhaSimbolos[topoSimbolos].endereco = qtd_ID++; //armazena a posição do simbolo associando a quantidade de identificadores. Depois incrementa o contador
        }
        if (!strcmp(T.categoria, "ID")) {
          if (!ehprototipo) {
            strcpy(pilhaSimbolos[topoSimbolos].nome, T.lexema);
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
            if (!existeID(T, local)) {
              strcpy(pilhaSimbolos[topoSimbolos].nome, T.lexema);
              strcpy(pilhaSimbolos[topoSimbolos].tipo, tipoAux);
              strcpy(pilhaSimbolos[topoSimbolos].categoria, "variavel");
              pilhaSimbolos[topoSimbolos].escopo = local;
              pilhaSimbolos[topoSimbolos].ehZumbi = 0;
              topoSimbolos++;
            }
            else erro(19);
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
      int i=0;
      while (i < topoSimbolos) {
        if (!strcmp(T.lexema, pilhaSimbolos[i].nome))
          break;
        i++;
      }
      if (i == topoSimbolos) {
          erro(15); //o identificador não foi declarado antes.
      }

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
        pilhaSimbolos[i].valor = Expressao();
        if(!strcmp(T.sinal, "ponto_virgula"))
          T = analex(FD);
        else
          erro(8);
      }
      else if (strcmp(T.sinal, "ponto_virgula")) { // Se o sinal não for ponto e vírgula, apresenta erro
        erro(8); //Falta sinal de ponto e vírgula
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
    int i=0;
    while (i < topoSimbolos) {
      if (!strcmp(T.lexema, pilhaSimbolos[i].nome))
        break;
      i++;
    }
    if (i == topoSimbolos)
      erro(15);
    else {
      T = analex(FD);
      if (!strcmp(T.sinal, "igual")){
        T = analex(FD);
        pilhaSimbolos[i].valor = Expressao();
      }
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

//Função que confere se um identificador existe ou não. 1 se Sim, 0 se Não.
int existeID(token TOKEN, Escopo escopo) {
  int i=0;
  while (i < topoSimbolos) {
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
        return 0;
      else
        return 1;
    }

    else if (escopo == local && pilhaSimbolos[i].escopo == local) {
      if (!pilhaSimbolos[i].ehZumbi)
        return 1;
    }
    
    else
      return 1; //o identificador já foi declarado antes.
  }

  return 0; //o identificador não existe, portanto, pode ser salvo na Pilha de Símbolos
}
