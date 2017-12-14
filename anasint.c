#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "analex.h"
#include "anasem.h"
#include "GerenciadorTS.h"
#include "MP.h"
#include "erros.h"
#include "anasint.h"
#include <unistd.h>

// INICIO DOS PROCEDIMENTOS DE ANÁLISE DE EXPRESSÕES ARITMÉTICAS
int contParam = 0;
int totalParam = 0;
int label = 0, labelPrincipal = 0;
extern token T;
extern FILE *FD;
extern simbolo pilhaSimbolos[100];
extern int topoSimbolos;
extern char *PR[];
extern char *categorias[];
extern char *sinais[];
extern float temRetorno;
extern char tipoFunc[];
extern float retornoFuncao;

//Função OK
float Expressao() {
  float primOp = 0, resultado = 0;
  // static int i = 0; //Contador estático que não mudará seu valor independente da vez que a função seja chamada

  // printf("------ ENTROU NO EXPR %d\n", i);
  primOp = expr_simp();
  if (!strcmp(T.categoria, "SN") && (!strcmp(T.sinal, "maior_igual") || !strcmp(T.sinal, "menor_igual")
                                                                     || !strcmp(T.sinal, "menor")
                                                                     || !strcmp(T.sinal, "maior")
                                                                     || !strcmp(T.sinal, "nao_igual")
                                                                     || !strcmp(T.sinal, "igual_igual")))
  {
    printf("EITA\n");
    char sinal[30];
    printf("MANO QUAL FOI\n");
    strcpy(sinal, T.sinal);
    printf("HUDJSAFNSDJÇFNDSFJA\n");
    T = analex(FD);
    printf("PASSOU ANALEX\n");
    resultado = expr_simp();
    printf("EXPRESSAO SIMPLES FOI DE BOA\n");
    op_rel(sinal);
    printf("OP_REL TAMBEM FOI TRANQUILO\n");
  }
  else
    resultado = primOp; /*Se não houver nenhum operador relacional pra comparar com outra expressão, o
                        resultado é o próprio valor da primeira operação*/
  // printf("----- EXPRESSAO %d: %.0f\n", i++, resultado); //Imprime o resultado. Posso colocar pra retornar depois
  return resultado;
}

//Função OK
float expr_simp() {
  float primOp, resultado;
  primOp = resultado = 0;

  if (!strcmp(T.categoria, "SN") && (!strcmp(T.sinal, "soma") || !strcmp(T.sinal, "subtracao"))) {
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
    else if (!strcmp(T.lexema, "||")) { // realiza a comparação dos operandos com "ou lógico"
      T = analex(FD);
      resultado = Termo();
      if (primOp != resultado) //confere se os tipos são diferentes e um emite erro caso forem
        erro(17);
      else {
        printf("OR\n");
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
    else if (!strcmp(T.lexema, "&&")) { // realiza a comparação dos operandos com "e_logico"
      T = analex(FD);
      resultado = Termo();
      if (primOp != resultado) //confere se os tipos são diferentes e um emite erro caso forem
        erro(17);
      else {
        printf("AND\n");
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
        if (!strcmp(T.lexema, pilhaSimbolos[i].nome)){
          totalParam = getTotalParam(i);
          if (!pilhaSimbolos[i].ehZumbi)
            break; //Encontra o identificador na pilha e sai do laço
        }
        i++;
      }
      if (i == topoSimbolos)
        erro(15);
      T = analex(FD);
      j=i+1; //contador para verificar se o próximo elementro na pilha é um parâmetro de função
      if(!strcmp(T.sinal, "abre_par")) {
        T = analex(FD);
        if (!strcmp(pilhaSimbolos[j].categoria, "param")) { //AQUI
          contParam++;
          resultado = Expressao();
          if (resultado == 0) { //verifica se a expressão no parâmetro é do tipo inteiro
            if (!strcmp(pilhaSimbolos[j].tipo, PR[2])) { //verifica se o parâmetro é do tipo inteiro
              j++;
            }
            else erro(18); //Erro de tipo de parâmtro incompatível
          }
          else if (resultado == 1) { //verifica se a expressão no parâmetro é do tipo real
            if (!strcmp(pilhaSimbolos[j].categoria, "param")) {
              contParam++;
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
                contParam++;
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
              else if (strcmp(T.sinal, "fecha_par")){
                contParam++;
              }
            }
          }
        }
        if(!strcmp(T.sinal, "fecha_par")) {
          paramChamadaFuncao(totalParam, contParam); //verifica paramentros

          if (!strcmp(pilhaSimbolos[i].tipo, PR[2]) || !strcmp(pilhaSimbolos[i].tipo, PR[1])) //verifica se o número é do tipo inteiro ou char
            resultado = 0;
          else if (!strcmp(pilhaSimbolos[i].tipo, PR[3])) //verifica se o número é do tipo real
            resultado = 1;
          else
            erro(16); // O operando é inválido, pois não possui o tipo compatível com a expressão
          printf("LOAD %d,%d\n", pilhaSimbolos[i].escopo, pilhaSimbolos[i].endereco);
          T = analex(FD);
        }
        else {
          erro(3);
        }
      }
      else if (!strcmp(pilhaSimbolos[i].categoria, "variavel") || !strcmp(pilhaSimbolos[i].categoria, "param")) {
        //verifica se o número é do tipo char, inteiro ou booleano
        if (!strcmp(pilhaSimbolos[i].tipo, PR[1]) || !strcmp(pilhaSimbolos[i].tipo, PR[2]) || !strcmp(pilhaSimbolos[i].tipo, PR[4]))
          resultado = 0;
        else if (!strcmp(pilhaSimbolos[i].tipo, PR[3])) //verifica se o número é do tipo real
          resultado = 1;
        /*else if (!strcmp(pilhaSimbolos[i].tipo, PR[1])) { //verifica se o número é do tipo char
          resultado = 2;
        }*/
        else
          erro(16); // O operando é inválido, pois não possui o tipo compatível com a expressão
        printf("LOAD %d,%d\n", pilhaSimbolos[i].escopo, pilhaSimbolos[i].endereco);
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
    printf("NOT\n");
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
  static int posRelGlobal = 0, posRelLocal = 0; //Armazena a posição relativa das variáveis globais e locais no código cmm
  int qtdVariaveisGlobais = 0, qtdVariaveisLocais = 0, qtdParams = 0;

  //Verifica se o token atual é um protótipo de função
  if (!strcmp(T.categoria, "PR") && !strcmp(T.lexema, "prototipo")) {
    T = analex(FD);
    ehprototipo = 1;
    strcpy(pilhaSimbolos[topoSimbolos].categoria, "prototipo");
  }
  //Analisa o token atual e verifica se é um tipo válido para declaração de uma variável ou função
  if (!strcmp(T.categoria, "PR") && (!strcmp(T.lexema, "caracter")
                                 ||  !strcmp(T.lexema, "inteiro")
                                 ||  !strcmp(T.lexema, "real")
                                 ||  !strcmp(T.lexema, "booleano")
                                 ||  !strcmp(T.lexema, "semretorno")))
  {
    strcpy(tipoAux, T.lexema); //Armazena o token na variavel auxiliar para guardar os tipos dos identificadores na pilha
  }
  else {
    erro(14);
  }

  do { //Fará a leitura e armazenamento dos identificadores para a pilha de simbolos
    T = analex(FD);
    //Verifica se o token atual é um identificador. Se for, armazena-o na pilha de simbolos e lê-se o próximo token.
    if (!strcmp(T.categoria, "ID") || !strcmp(T.lexema, "principal")) {
      if (!existeID(T, global)) { //se o ID não existir na pilha, este poderá ser salvo
        strcpy(pilhaSimbolos[topoSimbolos].nome, T.lexema);
        strcpy(pilhaSimbolos[topoSimbolos].tipo, tipoAux);
        strcpy(pilhaSimbolos[topoSimbolos].categoria, "variavel");
        pilhaSimbolos[topoSimbolos].escopo = global;
        pilhaSimbolos[topoSimbolos].ehZumbi = 0;
        topoSimbolos++;
        T = analex(FD);
      }
      else { //se o nome do ID já existe na pilha
        if (!ehprototipo) { //verifica se não é protótipo
          char nomeID[30]; //variável auxiliar para armazenar o nome do ID
          strcpy(nomeID, T.lexema);
          T = analex(FD); //chama o próximo token

          if (!strcmp(T.categoria, "SN") && !strcmp(T.sinal, "abre_par")) {
            int i=0;
            while (i < topoSimbolos) { //continuará procurando por outros identificadores de mesmo nome
              if (!strcmp(nomeID, pilhaSimbolos[i].nome)) //verifica se o nome do ID já existe na pilha
                if (!strcmp(pilhaSimbolos[i].categoria, "funcao")) //se for uma função já existente, o novo ID não será salvo
                  erro(19); //Emite um erro indicando que o nome do ID não pode ser armazenado
              i++;
            }
            if (i == topoSimbolos) { //Ao chegar no topo da pilha, o novo ID poderá ser armazenado nela
              strcpy(pilhaSimbolos[topoSimbolos].nome, nomeID);
              strcpy(pilhaSimbolos[topoSimbolos].tipo, tipoAux);
              strcpy(pilhaSimbolos[topoSimbolos].categoria, "funcao");
              pilhaSimbolos[topoSimbolos].escopo = global;
              pilhaSimbolos[topoSimbolos].ehZumbi = 0;
              topoSimbolos++;
            }
          }
          else if (!strcmp(T.categoria, "SN") && !strcmp(T.sinal, "ponto_virgula")) /*Caso o token atual
          seja o sinal de ponto e virgula, trata-se de uma variável global e portanto o novo ID não poderá ser
          salvo na pilha*/
            erro(19);
        }
        else
          erro(19);
      }
    }
    else
      erro(6);

    if (!strcmp(T.categoria, "SN") && !strcmp(T.sinal, "abre_par")) {
      int x = --topoSimbolos;
      if (!ehprototipo) {
        strcpy(pilhaSimbolos[x].categoria, "funcao"); // Informa à pilha que o símbolo armazenado anteriormente trata-se de uma função
        printf("GOTO --------------------- L%d\n", ++label);
        printf("LABEL -------------------- L%d\n", ++label);
        printf("INIPR 1\n");
        pilhaSimbolos[x].endereco = label;
        if (!strcmp(pilhaSimbolos[x].nome, "principal")) // Se a função for principal, salva seu label numa variável para uso posterior
          labelPrincipal = label;
        ehfuncao = 1;
        temRetorno = 0;
      }
      else
        strcpy(pilhaSimbolos[x].categoria, "prototipo");
      topoSimbolos++;
      strcpy(tipoFunc, pilhaSimbolos[x].tipo); //Armazena o tipo da função

      do { //armazena os paramentros na pilha de sinais
        T = analex(FD);
        if (!strcmp(T.categoria, "PR") && !strcmp(T.lexema, "semparam")) { // Verifica se a função não possui parâmetros
          T = analex(FD);
          semparam = 1;
          if (!strcmp(T.categoria, "SN") && !strcmp(T.sinal, "fecha_par")) {
            T = analex(FD);
            break; // Constata que a função não possui parâmetros e termina a declaração
          }
          else {
            erro(11);
          }
        }
        else if (!strcmp(T.categoria, "PR") && (!strcmp(T.lexema, "caracter")
                                            ||  !strcmp(T.lexema, "inteiro")
                                            ||  !strcmp(T.lexema, "real")
                                            ||  !strcmp(T.lexema, "booleano")))
        {
          strcpy(pilhaSimbolos[topoSimbolos].tipo, T.lexema);
          T = analex(FD);
        }
        else {
          erro(12);
        }
        strcpy(pilhaSimbolos[topoSimbolos].categoria, "param");
        pilhaSimbolos[topoSimbolos].escopo = local;
        if (!ehprototipo)
          pilhaSimbolos[topoSimbolos].ehZumbi = 0;
        else
          pilhaSimbolos[topoSimbolos].ehZumbi = 1; //se for protótipo, o parâmetro já se torna zumbi

        if (!strcmp(T.categoria, "ID")) {
          if (!ehprototipo) {
            if (!existeID(T, local)) {
              strcpy(pilhaSimbolos[topoSimbolos].nome, T.lexema);
              topoSimbolos++;
            }
            else erro(19);
          }
          else {
            topoSimbolos++;
          }
          T = analex(FD);
        }
        else if (!ehprototipo && !(!strcmp(T.categoria, "ID"))) {
          // Significa dizer que a função terá corpo, logo, deverá ter os nomes dos parâmetros especificados
          erro(13);
        }
        else {
          //if (!ehprototipo) {
            topoSimbolos++;
          //}
        }
      } while(!(!strcmp(T.categoria, "SN") && !strcmp(T.sinal, "fecha_par"))); /* Se não for "fecha parenteses )"
      o token atual, o laço continuará rodando. Se for vírgula,
      o primeiro analex(FD) do laço pulará */
    }
    else if(!strcmp(T.categoria, "SN") && (!strcmp(T.sinal, "ponto_virgula") || !strcmp(T.sinal, "virgula"))) {
      if (!strcmp(pilhaSimbolos[topoSimbolos-1].nome, "principal"))
        erro(28);
      qtdVariaveisGlobais++;
      pilhaSimbolos[topoSimbolos-1].endereco = posRelGlobal++;
    }
    else if (ehprototipo && !(!strcmp(T.categoria, "SN") && !strcmp(T.sinal, "abre_par"))) {
      erro(7);
    }
    if ((ehfuncao || ehprototipo) && !semparam) { //Analex danadinho
      T = analex(FD);
    }
  } while(!strcmp(T.categoria, "SN") && !strcmp(T.sinal, "virgula"));


  if (!ehfuncao && !strcmp(T.categoria, "SN") && !strcmp(T.sinal, "ponto_virgula")) {
    T = analex(FD);
    if (!ehprototipo)
      printf("AMEM %d\n", qtdVariaveisGlobais);
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
    int i=topoSimbolos-1, j=0, naoEhFuncao=1, posFunc=topoSimbolos;

    /*Confere se os tipos dos parâmetros da declaração da função são compatíveis com os parâmetros de seu
    protótipo, caso este existir*/
    while (naoEhFuncao) {
      if (!strcmp(pilhaSimbolos[i].categoria, "funcao")) { //encontra a última função declarada
        naoEhFuncao = 0;
        posFunc = i; // Armazena na variável auxiliar a posição onde se encontra a função
        while (j < i) { //procura o protótipo desta função
          if (!strcmp(pilhaSimbolos[j].nome, pilhaSimbolos[i].nome))
            if (!strcmp(pilhaSimbolos[j].categoria, "prototipo")) {
              if (!(!strcmp(pilhaSimbolos[j].tipo, pilhaSimbolos[i].tipo)))
                erro(21); //Tipo do retorno na declaração da função é diferente do tipo de retorno de seu protótipo
              j++;
              while (!strcmp(pilhaSimbolos[j].categoria, "param")) { //averigua os parâmetros do protótipo
                if (!strcmp(pilhaSimbolos[++i].categoria, "param")) { //compara com os parâmetros da declaração da função
                  if (!(!strcmp(pilhaSimbolos[j].tipo, pilhaSimbolos[i].tipo)))
                    erro(22); //Se os tipos não forem compatíveis, emite-se um erro
                }
                else
                  erro(23); //Emite erro se faltar um parâmetro na declaração da função //AQUI
                j++;
              }
              if (!strcmp(pilhaSimbolos[++i].categoria, "param"))
                erro(24); //Emite erro se tiver parâmetro a mais na declaração da função
            }
          j++; //continua procurando pelo protótipo
        }
      }
      else {
        qtdParams++;
        i--; //continua procurando a declaração da última função, ignorando os parâmetros desta na pilha de Símbolos
      }
    } // Se o protótipo não for encontrado, o fluxo continua normalmente.

    if (qtdParams) //Se a quantidade de parâmetros for diferente de 0, aloca-se a memória para eles
      printf("AMEM %d\n", qtdParams);

    //Adiciona o endereço dos parâmetros da última função declarada
    while (++posFunc < topoSimbolos)
      if (!strcmp(pilhaSimbolos[posFunc].categoria, "param"))
        pilhaSimbolos[posFunc].endereco = (posRelLocal-2) - qtdParams--; /* (posRelLocal-2) é devido aos dois registradores que estão
                                                                           na janela de ativação antes da área destinada aos endereços
                                                                           das variáveis locais */

    T = analex(FD);

    while (!(!strcmp(T.categoria, "SN") && !strcmp(T.sinal, "fecha_chaves"))) {
      if (!strcmp(T.categoria, "PR") && (!strcmp(T.lexema, "caracter")
                                     ||  !strcmp(T.lexema, "inteiro")
                                     ||  !strcmp(T.lexema, "real")
                                     ||  !strcmp(T.lexema, "booleano")))
      { //declaração de variáveis locais da função
        strcpy(tipoAux, T.lexema);
        do { //Fará a leitura e armazenamento das variáveis para a pilha de simbolos
          T = analex(FD);
          //Verifica se o token atual é um identificador. Se for, armazena-o na pilha de simbolos e lê-se o próximo token.
          if (!strcmp(T.categoria, "ID")) {
            if (!existeID(T, local)) {
              strcpy(pilhaSimbolos[topoSimbolos].nome, T.lexema);
              strcpy(pilhaSimbolos[topoSimbolos].tipo, tipoAux);
              strcpy(pilhaSimbolos[topoSimbolos].categoria, "variavel");
              pilhaSimbolos[topoSimbolos].escopo = local;
              pilhaSimbolos[topoSimbolos].endereco = posRelLocal++;
              pilhaSimbolos[topoSimbolos].ehZumbi = 0;
              qtdVariaveisLocais++;
              topoSimbolos++;
            }
            else erro(19);
            T = analex(FD);
          }
          else if (!strcmp(T.lexema, "principal"))
            erro(29);
          else
            erro(6);
        } while(!strcmp(T.categoria, "SN") && !strcmp(T.sinal, "virgula"));

        if (!strcmp(T.categoria, "SN") && !strcmp(T.sinal, "ponto_virgula")) {
          T = analex(FD);
          printf("AMEM %d\n", qtdVariaveisLocais);
          //qtdVariaveisLocais = 0;
        }
        else if (!(!strcmp(T.categoria, "SN") && !strcmp(T.sinal, "ponto_virgula"))) {
          erro(8);
        }
      }

      else if (!strcmp(T.lexema, "principal"))
        erro(14);

      else if (!strcmp(T.lexema, "EOF"))
        erro(10);

      else {
        cmd(); //chama o próximo token
      }
    }
  }

  if(ehfuncao)
    retornoDaFuncao(tipoFunc, 0.0);

  if (!strcmp(T.categoria, "SN") && !strcmp(T.sinal, "fecha_chaves")) {
    int i = topoSimbolos-1;
    while (strcmp(pilhaSimbolos[i].categoria, "funcao")) { //procura na pilha se pela última função
      if (!strcmp(pilhaSimbolos[i].categoria, "param"))
        qtdParams++;
      i--;
    }

    if (!strcmp(pilhaSimbolos[i].tipo, "semretorno")) {
      if (qtdVariaveisLocais)
        printf("DMEM %d\n", qtdVariaveisLocais);
      printf("RET 1,%d\n", qtdParams);
      printf("LABEL L%d\n", pilhaSimbolos[i].endereco-1);
    }

    T = analex(FD);
    posRelLocal = 0;
    apagaSimbolos();
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
      while (i < topoSimbolos) { //Procura na pilha se o identificador já foi declarado antes
        if (!strcmp(T.lexema, pilhaSimbolos[i].nome))
          break;
        i++;
      }
      if (i == topoSimbolos) {
          erro(15); //o identificador não foi declarado antes.
      }
      else {
        if (pilhaSimbolos[i].ehZumbi) {//Se o ID encontrado na pilha for um parâmetro morto, continuará procurando na pilha
          i++;
          while (i < topoSimbolos) { //continuará procurando por outros identificadores de mesmo nome
            if (!strcmp(T.lexema, pilhaSimbolos[i].nome))
              if (!pilhaSimbolos[i].ehZumbi) //se não for parâmetro morto, o ID será tratado com a atribuição
                break;
            i++; //se for parâmetro morto, continuará procurando até o topo da pilha
          }
          if (i == topoSimbolos)
            erro(15); //Emite erro indicando que o ID não foi declarado antes
        }
      }

      T = analex(FD);

      if (!strcmp(T.sinal, "abre_par")) { //verifica se o token atual é abre parenteses
        if (!(!strcmp(pilhaSimbolos[i].tipo, "semretorno"))) { // Verifica se o tipo da função possui valor de retorno
          erro(27); // Se possuir, emite erro indicando que este tipo de função não deve ser tratado como um comando.
        }

        T = analex(FD);
        Expressao();

        while (!strcmp(T.sinal, "virgula")) {
          T = analex(FD);
          Expressao();
        }

        printf("CALL L%d\n", pilhaSimbolos[i].endereco);

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
      printf("STOR 1,%d\n", pilhaSimbolos[i].endereco);
      }
      else if (strcmp(T.sinal, "ponto_virgula")) { // Se o sinal não for ponto e vírgula, apresenta erro
        erro(8); //Falta sinal de ponto e vírgula
      }
    }
    else if (!strcmp(T.categoria, "PR")) {
      if(!strcmp(T.lexema, "retorne")){
        T = analex(FD);

        int qtdVariaveisLocais=0, qtdParams=0, i = topoSimbolos-1;
        while (pilhaSimbolos[i].escopo == local && !strcmp(pilhaSimbolos[i].categoria, "variavel")) { //procura na pilha se é variável local
          qtdVariaveisLocais++;
          i--;
        }
        while (pilhaSimbolos[i].escopo == local && !strcmp(pilhaSimbolos[i].categoria, "param")) { //procura na pilha se é variável local
          qtdParams++;
          i--;
        }

        if (!(!strcmp(T.lexema, ";"))) { // Verifica se o retorno possui expressão associada ou não
          temRetorno = 1; // Entra no caso "retorne expressao;", portanto existe valor de retorno.
          retornoFuncao = Expressao();
          retornoDaFuncao(tipoFunc, retornoFuncao);
          printf("STOR 1,%d\n", -3-qtdParams);
          if (qtdVariaveisLocais)
            printf("DMEM %d\n", qtdVariaveisLocais);
          printf("RET 1,%d\n", qtdParams);
          printf("LABEL L%d\n", pilhaSimbolos[i].endereco-1);
        }
        else
          temRetorno = 0; // No caso de um "retorne;", uma função do tipo "semretorno" pode executar a instrução sem problemas

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
            printf("LABEL L%d\n", ++label);
            T = analex(FD);
            Expressao();
            if(!strcmp(T.sinal, "ponto_virgula")) {
              printf("GOFALSE L%d\n", ++label);
              printf("GOTO L%d\n", ++label);
              printf("LABEL L%d\n", ++label);
              T = analex(FD);
              //Verificar com Atta se é atrib ou expressão
              atrib();
              if (!strcmp(T.sinal, "fecha_par")) {
                printf("GOTO L%d\n", label-3);
                printf("LABEL L%d\n", label-1);
                T = analex(FD);
                cmd();
                printf("GOTO L%d\n", label);
                printf("LABEL L%d\n", label-2);
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
          printf("LABEL L%d\n", ++label);
          T = analex(FD);
          Expressao();
          printf("GOFALSE L%d\n", ++label);
          if (!strcmp(T.sinal, "fecha_par")) {
            T = analex(FD);
            cmd();
            printf("GOTO L%d\n", label-1);
            printf("LABEL L%d\n", label);
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
            printf("GOFALSE %d\n", ++label);
            T = analex(FD);
            cmd();
            if (!strcmp(T.lexema, "senao")) {
              printf("GOTO %d\n", ++label);
              printf("LABEL %d\n", label-1);
              T = analex(FD);
              cmd();
            }
            printf("LABEL %d\n", label);
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
void op_rel(char *sinal){
    if (!strcmp(sinal, "maior_igual") || !strcmp(sinal, "menor_igual") ||
    !strcmp(sinal, "menor") || !strcmp(sinal, "maior") ||
    !strcmp(T.sinal, "nao_igual") || !strcmp(sinal, "igual_igual" )) {
      if (!strcmp(sinal, "igual_igual" )) {
        printf("SUB\n");
        printf("GOFALSE L%d\n", ++label);
        printf("PUSH 0\n");
        printf("GOTO L%d\n", ++label);
        printf("LABEL L%d\n", label-1);
        printf("PUSH 1\n");
        printf("LABEL L%d\n", label);
      }

      else if (!strcmp(sinal, "maior_igual")) {
        printf("SUB\n");
        printf("GOTRUE L%d\n", ++label);
        printf("PUSH 0\n");
        printf("GOTO L%d\n", ++label);
        printf("LABEL L%d\n", label-1);
        printf("PUSH 1\n");
        printf("LABEL L%d\n", label);
      }

      else if (!strcmp(sinal, "menor_igual")) {
        printf("SUB\n");
        printf("COPY\n");
        printf("GOFALSE L%d\n", ++label);
        printf("GOTRUE L%d\n", ++label);
        printf("PUSH 1\n");
        printf("GOTO L%d\n", ++label);
        printf("LABEL L%d\n", label-2);
        printf("POP\n");
        printf("LABEL L%d\n", label-1);
        printf("PUSH 0\n");
        printf("LABEL L%d\n", label);
      }
    }
}
