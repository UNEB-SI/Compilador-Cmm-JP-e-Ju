#include <stdio.h>
#include "erro.c"
#include <string.h>
#include "analex.h"
#include "anasint.h"

// INICIO DOS PROCEDIMENTOS DE ANÁLISE DE EXPRESSÕES ARITMÉTICAS

//Precisa atualizar essa função
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

// Função que fará a leitura das declarações de variáveis globais, prototipos e criação de funções no código-fonte
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
            } while(!(!strcmp(T.categoria, "SN") && !strcmp(T.sinal, "fecha_parenteses"))); /* Se não for "fecha parenteses )"
                                                                o token atual, o laço continuará rodando. Se for vírgula,
                                                                o primeiro analex(FD) do laço pulará */
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

void op_rela(){

    if(!strcmp(T.categoria, "SN")) {

        if(strcmp(T.lexema, ">=")==0 || strcmp(T.lexema, "<=")==0 ||
           strcmp(T.lexema, "<")==0 || strcmp(T.lexema, ">")==0 ||
           strcmp(T.lexema, "!=")==0 || strcmp(T.lexema, "==")==0){
            T = analex(FD);
        }
    }
}
