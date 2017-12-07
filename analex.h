#ifndef ANALEX_H
#define ANALEX_H

// Conjunto de constantes
#define QTD_SINAIS sizeof(sinais)/sizeof(token)+1
#define QTD_PR sizeof(PR)/sizeof(char*)
#define QTD_CATEGORIAS sizeof(categorias)/sizeof(char*)
#define QTD_LEXEMA 100
#define QTD_DIGITO 100

typedef enum escopo {
  global, local
} Escopo;

//Definição do tipo Token
typedef struct Token {
  char categoria[30];
  char lexema[30];
  char sinal[30];
  int codigo;
  float valor;
} token;

                    // 0     1     2      3        4       5       6          7
char *categorias[] = {"PR", "SN", "ID", "CT_I", "CT_R", "CT_LT", "CT_C", "FIM_ARQUIVO"};

// Conjunto de sinais e operadores da linguagem
token sinais[] = { {"SN", "+", "mais"}, {"SN", "-", "menos"}, {"SN", "*", "asterisco"},
                   {"SN", "/", "barra"}, {"SN", "=", "igual"}, {"SN", "<", "menor"},
                   {"SN", ">", "maior"}, {"SN", "<=", "menor_igual"}, {"SN", ">=", "maior_igual"},
                   {"SN", "(", "abre_par"}, {"SN", ")", "fecha_par"}, {"SN", "{", "abre_chaves"},
                   {"SN", "}", "fecha_chaves"}, {"SN", "[", "abre_colchetes"}, {"SN", "]", "fecha_colchetes"},
                   {"SN", "&&", "e_logico"}, {"SN", "||", "ou_logico"}, {"SN", "!", "nao_logico"},
                   {"SN", "!=", "nao_igual"}, {"SN", "==", "igual_igual"}, {"SN", "\'", "apostofo"}, {"SN", "\"", "aspas"},
                   {"SN", ";", "ponto_virgula"}, {"SN", ".", "ponto"}, {"SN", ",", "virgula"}, {"SN", "\\", "contra-barra"}
                };

// Conjunto de Palavras Reservadas da linguagem Cmm
char *PR[] = {
  "semretorno", "caracter", "inteiro", "real", "booleano", "semparam", "se", "senao",
  "enquanto", "para", "retorne", "principal", "prototipo"
};

// Variáveis Globais
token T;
FILE *FD;
int qtd_linhas = 1; // contador de vezes que o ENTER foi pressionado

char ID[100][100]; //pode ser substituido pela pilha de simbolos
int qtd_ID = 0;

char literais[100][100];
int qtd_literais = 0;

// Assinatura de funções
int ehsinal(char);
token copiaTokenSinal(char*);
void transicao_sinais(int *, char *, int *, char, FILE *);
token analex(FILE *);

#endif
