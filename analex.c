#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "analex.h"


#define QTD_LEXEMA 100
#define QTD_DIGITO 100


/*char erros[][50] =  {"Sem erro", //0
                     "Caracter invalido!", //1
                     "Formato de numero invalido!", //2
                     "Faltando sinal fecha parenteses \')\' !", //3
                     "Operando invalido!", //4
                     "Expressão mal formatada!", //5
                     "ID esperado!", //6
                     "Faltando parâmetros no protótipo da função!", //7
                     "Faltando sinal de ponto e vírgula \';\' !", //8
                     "Função não possui corpo!", //9
                     "Faltando sinal de fecha chaves \'}\' !" //10
};

void erro(int e) {
    printf("Erro Sintático: %s\n", erros[e]);
    exit(e);
}*/


//variável global - contador de vezes que o ENTER foi pressionado
int qtd_linhas = 1;


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
        printf("< %s, %s >\n", T.categoria, T.lexema);


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
                    if (!estado)
                        estado = 31;
                    else if (estado == 34)
                        estado = 0;
                    else
                        transicao_sinais(&estado, lexema, &l, c, FD);
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

int main() {


    int c, i=0;


    FD = fopen("/Users/jcsantana/CLionProjects/AnaSintForSure/documento.txt", "r");

    if (FD == NULL) { //teste de abertura do arquivo
        printf ("Houve um erro ao abrir o arquivo.\n");
        return 1;
    }

    //int count = 0;



    while(1) {

        T = analex(FD);
        //printf("< %s, %s >\n", T.categoria, T.lexema);

        if (strcmp(T.categoria, "FIM_ARQUIVO") == 0)
            break;

        //prog();

    }

    fclose(FD);
    return 0;
}







/*
void Expr_Simp(){
    if (!strcmp(T.categoria, "SN") && (!strcmp(T.lexema, "+") || !strcmp(T.lexema, "-"))){
        T = analex(FD);

    }
    Termo();
    while(!strcmp(T.categoria, "SN") &&
          (!strcmp(T.lexema, "+") || !strcmp(T.lexema, "-") || !strcmp(T.lexema, "||"))) {
        if (!strcmp(T.categoria, "SN") &&
            (!strcmp(T.lexema, "+") || !strcmp(T.lexema, "-") || !strcmp(T.lexema, "||"))) {
            T = analex(FD);
            Termo();
        }
    }
}

void op_rela(){

    if(!strcmp(T.categoria, "SN")) {

        if(strcmp(T.lexema, ">=")==0 || strcmp(T.lexema, "<=")==0 ||
           strcmp(T.lexema, "<")==0 || strcmp(T.lexema, ">")==0 ||
           strcmp(T.lexema, "!=")==0 || strcmp(T.lexema, "==")==0){
            T = analex(FD);
        }
    }
}

void Expressao() {
    Expr_Simp();
    op_rela();
    Expr_Simp();
}

void Termo() {
    Fator();
    while (!strcmp(T.categoria, "SN") && (!strcmp(T.sinal, "multiplicacao") || !strcmp(T.sinal, "divisao") || !strcmp(T.sinal, "e_logico"))) {
        Sobra();
    }
}

void atrib(){
    if(!strcmp(T.categoria, "ID")){
        T = analex(FD);
        if (!strcmp(T.sinal, "igual")){
            T = analex(FD);
            Expressao();
        }

    }
}

void Sobra() {
    if (!strcmp(T.categoria, "SN") && (!strcmp(T.sinal, "multiplicacao") || !strcmp(T.sinal, "divisao") || !strcmp(T.sinal, "e_logico"))) {
        T = analex(FD);
        Fator();
    }
}

void Fator() {
    if (!strcmp(T.categoria, "CT_I") ||
        !strcmp(T.categoria, "CT_R") || !strcmp(T.categoria, "CT_C")) {
        T = analex(FD);
    } else if (!strcmp(T.categoria, "ID")) {
        T = analex(FD);
        if (!strcmp(T.sinal, "abre_par")) {
            T = analex(FD);
            Expressao();
            if (!strcmp(T.sinal, "virgula")) {
                while (!strcmp(T.sinal, "virgula")) {
                    T = analex(FD);
                    Expressao();
                }
            }
            if (!strcmp(T.sinal, "fecha_par"))
                T = analex(FD);
            else {
                erro(3);
            }
        }

    } else if (!strcmp(T.sinal, "abre_par")) {
        T = analex(FD);
        Expressao();
        if (!strcmp(T.sinal, "fecha_par"))
            T = analex(FD);
        else {
            erro(3);
        }
    } else if (!strcmp(T.sinal, "nao_logico")) {
        T = analex(FD);
        Fator();
    } else if (!strcmp(T.categoria, "digito")) {
        T = analex(stdin);
    }
    else if(strcmp(T.lexema, "EOF")){
        erro(5);
    }


}


void prog() {
    int ehprototipo = 0; // 0 - declaração de variável; 1 - prototipo de função
    int ehfuncao = 0; // Verifica se a função possui corpo ou não.
    char tipoAux[30]; // variável auxiliar para armazenar o tipo da variável ou função

    //Verifica se o token atual é um protótipo de função
    if (!strcmp(T.categoria, "PR") && !strcmp(T.lexema, "prototipo")) {
        T = analex(FD);
        ehprototipo = 1;
    }

    //Analisa o token atual e verifica se é um tipo válido para declaração de uma variável ou função
    if (!strcmp(T.categoria, "PR") && ((!strcmp(T.lexema, "caracter"))
                                       ||  !strcmp(T.lexema, "inteiro")
                                       ||  !strcmp(T.lexema, "real")
                                       ||  !strcmp(T.lexema, "booleano")
                                       ||  !strcmp(T.lexema, "semretorno")))
    {
        strcpy(tipoAux, T.lexema); //Armazena o token na variavel auxiliar para guardar os tipos dos identificadores na pilha
    }

    do { //Fará a leitura e armazenamento dos identificadores para a pilha de simbolos
        T = analex(FD);
        //Verifica se o token atual é um identificador. Se for, armazena-o na pilha de simbolos e lê-se o próximo token.
        if (!strcmp(T.categoria, "ID")) {
            strcpy(pilhaSimbolos[topoSimbolos].nome, T.lexema);
            strcpy(pilhaSimbolos[topoSimbolos].tipo, tipoAux);
            strcpy(pilhaSimbolos[topoSimbolos].categoria, "variavel");
            pilhaSimbolos[topoSimbolos].escopo = global;
            pilhaSimbolos[topoSimbolos].ehZumbi = 0;
            topoSimbolos++;
            T = analex(FD);
        }
        else
            erro(6);

        if (!strcmp(T.categoria, "SN") && !strcmp(T.sinal, "abre_par")) {
            strcpy(pilhaSimbolos[--topoSimbolos].categoria, "funcao"); // Informa à pilha que o símbolo armazenado anteriormente trata-se de uma função
            topoSimbolos++;
            ehfuncao = 1;
            do { //armazena os paramentros na pilha de sinais
                T = analex(FD);
                if (!strcmp(T.categoria, "PR") && !strcmp(T.lexema, "semparam")) { // Verifica se a função não possui parâmetros
                    T = analex(FD);
                    if (!strcmp(T.categoria, "SN") && !strcmp(T.sinal, "fecha_parenteses")) {
                        T = analex(FD);
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
                    T = analex(FD);
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
                    T = analex(FD);
                }
                else if (!ehprototipo && !(!strcmp(T.categoria, "ID"))) {
                    // Significa dizer que a função terá corpo, logo, deverá ter os nomes dos parâmetros especificados
                    printf("Erro Sintático! Nome de Parâmetro de Função omitido!\n");
                }
                else {
                    topoSimbolos++;
                }
            } while(!(!strcmp(T.categoria, "SN") && !strcmp(T.sinal, "fecha_parenteses")));
            }
            else if (ehprototipo && !(!strcmp(T.categoria, "SN") && !strcmp(T.sinal, "abre_par"))) {
                erro(7);
            }
            T = analex(FD);
        } while(!strcmp(T.categoria, "SN") && !strcmp(T.sinal, "virgula"));

        if (!ehfuncao && !strcmp(T.categoria, "SN") && !strcmp(T.sinal, "ponto_virgula")) {
            T = analex(FD);
        }
        else if (!ehfuncao && !(!strcmp(T.categoria, "SN") && !strcmp(T.sinal, "ponto_virgula"))) {
            erro(8);
        }
        else if (ehfuncao && !(!strcmp(T.categoria, "SN") && !strcmp(T.sinal, "abre_chaves"))) {
            erro(9);
        }
        else if (ehfuncao && !strcmp(T.categoria, "SN") && !strcmp(T.sinal, "abre_chaves")) { // Abre a construção do corpo da função
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
                        //Verifica se o token atual é um identificador. Se for, armazena-o na pilha de simbolos e lê-se o próximo token.
                        if (!strcmp(T.categoria, "ID")) {
                            strcpy(pilhaSimbolos[topoSimbolos].nome, T.lexema);
                            strcpy(pilhaSimbolos[topoSimbolos].tipo, tipoAux);
                            strcpy(pilhaSimbolos[topoSimbolos].categoria, "variavel");
                            pilhaSimbolos[topoSimbolos].escopo = local;
                            pilhaSimbolos[topoSimbolos].ehZumbi = 0;
                            topoSimbolos++;
                            T = analex(FD);
                        }
                        else
                            erro(6);
                    } while(!strcmp(T.categoria, "SN") && !strcmp(T.sinal, "virgula"));

                    if (!strcmp(T.categoria, "SN") && !strcmp(T.sinal, "ponto_virgula")) {
                        T = analex(FD);
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
                    T = analex(FD); //chama o próximo token

                if(!strcmp(T.lexema, "EOF")){
                    erro(10);
                }
            }
        }
    }

//Testar e clocar erros de má formação
    // Função que realiza os comandos da linguagem
    void cmd() {
        if (!strcmp(T.categoria, "PR") || !strcmp(T.categoria, "ID") || !strcmp(T.categoria, "SN")) {
            if (!strcmp(T.categoria, "SN")) {
                if (!strcmp(T.sinal, "ponto_virgula"))
                    T = analex(FD);
                else if (!strcmp(T.sinal, "abre_chaves")) {
                    T = analex(FD);
                    cmd();
                    if (!strcmp(T.sinal, "fecha_chaves"))
                        T = analex(FD);
                    else {
                        erro(10);
                    }
                }
            }
            else if (!strcmp(T.categoria, "ID")){
                T = analex(FD);
                if (!strcmp(T.sinal, "igual")){
                    T = analex(FD);
                    Expressao();
                }
                else if (!strcmp(T.sinal, "abre_par")){
                    T = analex(FD);
                    Expressao();

                    while(!strcmp(T.sinal, "virgula")) {
                        if (!strcmp(T.sinal, "virgula")) {
                            T = analex(FD);
                            Expressao();
                        }
                    }

                    if (!strcmp(T.sinal, "fecha_par")) {
                        T = analex(FD);
                        if (!strcmp(T.sinal, "ponto_virgula"))
                            T = analex(FD);
                        else {
                            erro(8);
                        }
                    } else {
                        erro(3);
                    }

                }

            }
            else if (!strcmp(T.categoria, "PR")){
                if(!strcmp(T.lexema, "retorne")){
                    T = analex(FD);
                    Expressao();
                    if(!strcmp(T.sinal, "ponto_virgula"))
                        T = analex(FD);
                    else {
                        erro(8);
                    }
                }
                else if (!strcmp(T.lexema, "para")){
                    T = analex(FD);
                    if (!strcmp(T.sinal, "abre_par")) {
                        T = analex(FD);
                        atrib();

                        if(!strcmp(T.sinal, "ponto_virgula")){
                            T = analex(FD);
                            Expressao();
                            if(!strcmp(T.sinal, "ponto_virgula")){
                                T = analex(FD);
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
    }*/

