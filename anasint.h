#ifndef ANASINT_H
#define ANASINT_H

// Variáveis Globais
extern token T;

// Assinatura de Funções
float Expressao();
float expr_simp();
float Termo();
float Fator();
void prog();
void cmd();
void atrib();
void op_rel();
void apagaSimbolos();
int existeID(token, Escopo);
#endif
