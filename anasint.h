#ifndef ANASINT_H
#define ANASINT_H

// Variáveis Globais
extern token T;

// Assinatura de Funções
void Expressao();
float expr_simp();
float Termo();
float Fator();
void prog();
void cmd();
void atrib();
void op_rel();
void apagaSimbolos();

#endif
