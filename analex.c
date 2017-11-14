#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "analex.h"

//Lê um caracter e retorna 1 caso seja um sinal da linguagem e 0 (zero), caso contrário
int ehsinal(char c) {
  int i = 0;

  while (i < QTD_SINAIS) {
    if (c == sinais[i].lexema[0])
      return 1;
    i++;
  }

  return 0;
}

//Função que realiza a cópia dos valores dos sinais no Token
token copiaTokenSinal(char* sinal) {
  token T;
  int i = 0;
  while (i<QTD_SINAIS && strcmp(sinal, sinais[i].lexema) != 0) i++;

  strcpy(T.categoria, categorias[1]);
  strcpy(T.sinal, sinais[i].sinal);
  strcpy(T.lexema, sinais[i].lexema);

  return T;
}

void transicao_sinais(int *estado, char *lexema, int *l, char c, FILE *FD) {
  switch (*estado) {
    case 1:
      *estado = 2;
      break;
    case 4:
      *estado = 6;
      break;
    case 7:
      *estado = 8;
      break;
    case 9:
      *estado = 10;
      break;
    case 12:
      *estado = 13;
      break;
    case 15:
      *estado = 17;
      break;
    case 18:
      *estado = 19;
      break;
    case 25:
      *estado = 26;
      lexema[*l] = c;
      *l = *l + 1;
      break;
    case 28:
      *estado = 29;
      lexema[*l] = c;
      *l = *l + 1;
      break;
    case 29:
      *estado = 29;
      lexema[*l] = c;
      *l = *l + 1;
      break;
    case 33:
      *estado = 33;
      break;
    case 34:
      *estado = 33;
      lexema[*l] = c;
      *l = *l + 1;
      break;
  }
}

token analex(FILE *FD) {
  int estado = 0, l = 0, d = 0, i = 0;
  char c, lexema[QTD_LEXEMA], digito[QTD_DIGITO];

  if(strcmp(T.lexema, ""))
    printf("< %s %s >\n", T.categoria, T.lexema);
  //loop principal
  while(1) {
    c = fgetc(FD); //Lê um caractere

    if (feof(FD)) { //Verificar se o arquivo está vazio ou chegou ao fim
      if (!estado) {
        strcpy(T.categoria, categorias[7]);
        strcpy(T.lexema, "EOF");
        T.codigo = 0;
        return T;
      }
      else {
        printf("-- Erro na linha: %d --\n", qtd_linhas);
        getchar();
        exit(1);
      }
    }

    if (isalpha(c)) { //verifica se o caracter lido é uma letra
      switch (estado) {
        case 0:
          estado = 1;
          lexema[l++] = c;
          break;
        case 1:
          estado = 1;
          lexema[l++] = c;
          break;
        case 4:
          estado = 6;
          break;
        case 7:
          estado = 8;
          break;
        case 9:
          estado = 10;
          break;
        case 12:
          estado = 13;
          break;
        case 15:
          estado = 17;
          break;
        case 18:
          estado = 19;
          break;
        case 25:
          estado = 26;
          lexema[l++] = c;
          break;
        case 28:
          estado = 29;
          lexema[l++] = c;
          break;
        case 29:
          estado = 29;
          lexema[l++] = c;
          break;
        case 31:
          estado = 32;
          break;
        case 33:
          estado = 33;
          break;
        case 34:
          estado = 33;
          break;
        case 36:
          switch (c) {
            case 'n':
              estado = 38;
              strcpy(T.categoria, categorias[6]);
              lexema[l++] = '\\';
              lexema[l++] = c;
              strcpy(T.lexema, lexema);
              return T;
            case 't':
              estado = 39;
              strcpy(T.categoria, categorias[6]);
              lexema[l++] = '\\';
              lexema[l++] = c;
              strcpy(T.lexema, lexema);
              return T;
            case 'r':
              estado = 40;
              strcpy(T.categoria, categorias[6]);
              lexema[l++] = '\\';
              lexema[l++] = c;
              strcpy(T.lexema, lexema);
              return T;
          }
        case 43: //caracteres '\n', '\t' e '\r' no estado de aspas
          switch (c) {
            case 'n':
            case 't':
            case 'r':
              estado = 29;
              lexema[l++] = '\\';
              lexema[l++] = c;
              break;
          }
        case 44: //caracteres '\n', '\t' e '\r' no estado de aspostofo
          switch (c) {
            case 'n':
            case 't':
            case 'r':
              estado = 26;
              lexema[l++] = '\\';
              lexema[l++] = c;
              break;
          }
      }
    }

    else if (c == '_') {
      switch (estado) {
        case 1:
          estado = 1;
          lexema[l++] = c;
          break;
        case 18:
          estado = 19;
          break;
        case 25:
          estado = 26;
          lexema[l++] = c;
          break;
        case 28:
          estado = 29;
          lexema[l++] = c;
          break;
        case 29:
          estado = 29;
          lexema[l++] = c;
          break;
        case 33:
          estado = 33;
          break;
        case 34:
          estado = 33;
          break;
      }
    }

    else if (isdigit(c)) { //verifica se o caracter lido é um dígito
      switch (estado) {
        case 0:
          estado = 4;
          digito[d++] = c;
          break;
        case 1:
          estado = 1;
          lexema[l++] = c;
          break;
        case 4:
          estado = 4;
          digito[d++] = c;
          break;
        case 5:
          estado = 7;
          digito[d++] = c;
          break;
        case 7:
          estado = 7;
          digito[d++] = c;
          break;
        case 9:
          estado = 10;
          break;
        case 12:
          estado = 13;
          break;
        case 15:
          estado = 17;
          break;
        case 25:
          estado = 26;
          lexema[l++] = c;
          break;
        case 28:
          estado = 29;
          lexema[l++] = c;
          break;
        case 29:
          estado = 29;
          lexema[l++] = c;
          break;
        case 33:
          estado = 33;
          break;
        case 34:
          estado = 33;
          break;
        case 36:
          switch (c) {
            case '0':
              estado = 45;
              strcpy(T.categoria, categorias[6]);
              lexema[l++] = '\\';
              lexema[l++] = c;
              strcpy(T.lexema, lexema);
              return T;
          }
        case 43:
          if (c == '0') { //caracter barra-zero no estado de aspas
            estado = 29;
            lexema[l++] = '\\';
            lexema[l++] = c;
          }
          break;
        case 44:
          if (c == '0') { //caracter barra-zero no estado de aspostofo
            estado = 26;
            lexema[l++] = '\\';
            lexema[l++] = c;
          }
          break;
      }
    }

    else if (ehsinal(c)) { //verifica se o caracter lido é um sinal
      switch (c) {
        case ';':
          if (!estado) {
            T = copiaTokenSinal(";");
            return T;
          }
          else
            transicao_sinais(&estado, lexema, &l, c, FD);
        break;

        case '+':
          if (!estado) {
            T = copiaTokenSinal("+");
            return T;
          }
          else
            transicao_sinais(&estado, lexema, &l, c, FD);
        break;

        case '-':
          if (!estado) {
            T = copiaTokenSinal("-");
            return T;
          }
          else
            transicao_sinais(&estado, lexema, &l, c, FD);
        break;

        case '*':
          if (!estado) {
            T = copiaTokenSinal("*");
            return T;
          }
          else if (estado == 31)
            estado = 33;
          else if (estado == 33)
            estado = 34;
          else if (estado == 34)
            estado = 33;
          else
            transicao_sinais(&estado, lexema, &l, c, FD);
        break;

        case '/':
        if (!estado){
          T = copiaTokenSinal("/");
          estado = 31;
          return T;
        }
        else if (estado == 34)
          estado = 0;
        else
          transicao_sinais(&estado, lexema, &l, c, FD);

          //printf("DENTRO: %s %s\n", T.lexema, T.sinal);
      break;

        case '=':
          switch (estado) {
            case 0:
              estado = 9;
              lexema[l++] = c;
              break;
            case 9:
              T = copiaTokenSinal("==");
              estado = 11;
              return T;
            case 12:
              T = copiaTokenSinal("<=");
              estado = 14;
              return T;
            case 15:
              T = copiaTokenSinal(">=");
              estado = 16;
              return T;
            case 18:
              T = copiaTokenSinal("!=");
              estado = 20;
              return T;
            default:
              transicao_sinais(&estado, lexema, &l, c, FD);
          }
        break;

        case '.':
          switch (estado) {
            case 0:
              T = copiaTokenSinal(".");
              estado = 3;
              return T;
            case 4:
              estado = 5;
              digito[d++] = c;
              break;
            default:
              transicao_sinais(&estado, lexema, &l, c, FD);
          }
        break;

        case ',':
          if (!estado) {
            T = copiaTokenSinal(",");
            return T;
          }
          else
            transicao_sinais(&estado, lexema, &l, c, FD);
        break;

        case '<':
          if (!estado) {
            estado = 12;
            lexema[l++] = c;
          }
          else
            transicao_sinais(&estado, lexema, &l, c, FD);
        break;

        case '>':
          if (!estado) {
            estado = 15;
            lexema[l++] = c;
          }
          else
            transicao_sinais(&estado, lexema, &l, c, FD);
        break;

        case '!':
          if (!estado) {
            estado = 18;
            lexema[l++] = c;
          }
          else
            transicao_sinais(&estado, lexema, &l, c, FD);
        break;

        case '&':
          if (!estado) {
            estado = 21;
            lexema[l++] = c;
          }
          else if (estado == 21) {
            T = copiaTokenSinal("&&");
            estado = 22;
            return T;
          }
          else
            transicao_sinais(&estado, lexema, &l, c, FD);
        break;

        case '|':
          if (!estado) {
            estado = 23;
            lexema[l++] = c;
          }
          else if (estado == 23) {
            T = copiaTokenSinal("||");
            estado = 24;
            return T;
          }
          else
            transicao_sinais(&estado, lexema, &l, c, FD);
        break;

        case '[':
          if (!estado) {
            T = copiaTokenSinal("[");
            return T;
          }
          else
            transicao_sinais(&estado, lexema, &l, c, FD);
        break;

        case ']':
          if (!estado) {
            T = copiaTokenSinal("]");
            return T;
          }
          else
            transicao_sinais(&estado, lexema, &l, c, FD);
        break;

        case '{':
          if (!estado) {
            T = copiaTokenSinal("{");
            return T;
          }
          else
            transicao_sinais(&estado, lexema, &l, c, FD);
        break;

        case '}':
          if (!estado) {
            T = copiaTokenSinal("}");
            return T;
          }
          else
            transicao_sinais(&estado, lexema, &l, c, FD);
        break;

        case '(':
          if (!estado) {
            T = copiaTokenSinal("(");
            return T;
          }
          else
            transicao_sinais(&estado, lexema, &l, c, FD);
        break;

        case ')':
          if (!estado) {
            T = copiaTokenSinal(")");
            return T;
          }
          else
            transicao_sinais(&estado, lexema, &l, c, FD);
        break;

        case '\"':
          if (!estado) {
            estado = 28;
          }
          else if (estado == 28) {
            T = copiaTokenSinal("\"");
            estado = 30;
            return T;
          }
          else if (estado == 29) {
            estado = 30;
            i=0;
            while (i<qtd_literais && strcmp(lexema, literais[i]) != 0) i++;
            if (i<qtd_literais && strcmp(lexema, literais[i]) == 0) { //verifica se o lexema é um literal existente
              strcpy(T.categoria, categorias[5]);
              lexema[l] = '\0';
              strcpy(T.lexema, lexema);
              return T;
            }
            //criação de um novo literal
            strcpy(T.categoria, categorias[5]);
            lexema[l] = '\0';
            strcpy(T.lexema, lexema);

            strcpy(literais[qtd_literais], lexema); //armazena no vetor de literais o lexema para criar um novo literal
            qtd_literais++;
            return T;
          }
          else
            transicao_sinais(&estado, lexema, &l, c, FD);
        break;

        case '\'':
          if (!estado) {
            estado = 25;
          }
          else if (estado == 25) {
            T = copiaTokenSinal("\'");
            estado = 27;
            return T;
          }
          else if (estado == 26) { //retorna um caracter
            strcpy(T.categoria, categorias[6]);
            lexema[l++] = '\0';
            strcpy(T.lexema, lexema);

            estado = 27;
            return T;
          }
          else
            transicao_sinais(&estado, lexema, &l, c, FD);
        break;

        case '\\':
          if (!estado)
            estado = 36;
          else if (estado == 36) {
            T = copiaTokenSinal("\\");
            estado = 37;
            return T;
          }
          else if (estado == 28)
            estado = 43;
          else if (estado == 29) {
            estado = 43;
            lexema[l++] = c;
          }
          else if (estado == 43)
            estado = 29;
          else if (estado == 25)
            estado = 44;
          else if (estado == 26)
            estado = 44;
          else if (estado == 44)
            estado = 26;
      }
    }

    else if (isspace(c)) { //verifica se o caracter lido é um espaço em branco
      if (c == '\n') {
        if (estado == 0 || estado == 33 || estado == 34) {
          qtd_linhas++;
        }
      }
      if (!estado)
        estado = 0;
      else if (estado == 25)
        estado = 26;
      else if (estado == 28) {
        estado = 29;
        lexema[l++] = c;
      }
      else if (estado == 29) {
        estado = 29;
        lexema[l++] = c;
      }
      else if (estado == 31)
        estado = 32;
      else if (estado == 33)
        estado = 33;
      else if (estado == 34)
        estado = 33;
      else
      transicao_sinais(&estado, lexema, &l, c, FD);
    }

    //Estados de aceitação ainda não retornados
    switch (estado) {
      case 2: //Palavra Reservada ou Identificador
        lexema[l++] = '\0';
        i=0;

        while (i<QTD_PR && strcmp(lexema, PR[i]) != 0)  i++;

        if (i<QTD_PR && strcmp(lexema, PR[i]) == 0) { //verifica se o lexema é uma palavra reservada
          strcpy(T.categoria, categorias[0]);
          strcpy(T.lexema, lexema);
          ungetc(c, FD);
          return T;
        }

        i=0;
        while (i<qtd_ID && strcmp(lexema, ID[i]) != 0) i++;
        if (i<qtd_ID && strcmp(lexema, ID[i]) == 0) { //verifica se o lexema é um ID existente
          strcpy(T.categoria, categorias[2]);
          lexema[l++] = '\0';
          strcpy(T.lexema, lexema);
          ungetc(c, FD);
          return T;
        }

        //criação de um novo ID
        strcpy(T.categoria, categorias[2]);
        lexema[l++] = '\0';
        strcpy(T.lexema, lexema);

        strcpy(ID[qtd_ID], lexema); //armazena no vetor de IDs o lexema para criar um novo ID
        qtd_ID++;
        ungetc(c,FD);
        return T;

      case 6: //Constante Inteira
        digito[d++] = '\0';
        strcpy(T.categoria, categorias[3]);
        strcpy(T.lexema, digito);
        T.valor_int = atoi(digito);
        ungetc(c,FD);
        return T;

      case 8: //Constante Real
        digito[d++] = '\0';
        strcpy(T.categoria, categorias[4]);
        strcpy(T.lexema, digito);
        T.valor_float = atof(digito);
        ungetc(c,FD);
        return T;

      case 10: //Sinal =
        T = copiaTokenSinal("=");
        ungetc(c,FD);
        return T;

      case 13: //Sinal <
        T = copiaTokenSinal("<");
        ungetc(c,FD);
        return T;

      case 17: //Sinal >
        T = copiaTokenSinal(">");
        ungetc(c,FD);
        return T;

      case 19: //Sinal !
        T = copiaTokenSinal("!");
        ungetc(c,FD);
        return T;

      case 32: //Sinal /
        T = copiaTokenSinal("/");
        ungetc(c,FD);
        return T;

      /*case 33:
        strcpy(T.categoria, "COMENT");
        strcpy(T.lexema, lexema);
        ungetc(c,FD);
        break;*/

    }

  } //fim do loop

}
