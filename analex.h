#ifndef ANALEX_H
#define ANALEX_H

// Conjunto de constantes
#define QTD_SINAIS sizeof(sinais)/sizeof(token)+1
#define QTD_PR sizeof(PR)/sizeof(char*)
#define QTD_CATEGORIAS sizeof(categorias)/sizeof(char*)
#define QTD_LEXEMA 100
#define QTD_DIGITO 100


//Definição do tipo Token
typedef struct Token {
  char categoria[30];
  char lexema[30];
  char sinal[30];
  int codigo;
  float valor;
} token;






// Variáveis Globais
//token T;


// Assinatura de funções
int ehsinal(char);
token copiaTokenSinal(char*);
void transicao_sinais(int *, char *, int *, char, FILE *);
token analex(FILE *);

#endif
