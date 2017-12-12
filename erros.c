#include <stdio.h>
#include <stdlib.h>
#include "analex.h"

char erros[][100] =  {"Sem erro", //0
                     "Caracter invalido!", //1
                     "Formato de numero invalido!", //2
                     "Faltando sinal fecha parenteses \')\' !", //3
                     "Operando invalido!", //4
                     "Expressão mal formatada!", //5
                     "ID esperado!", //6
                     "Faltando parâmetros no protótipo da função!", //7
                     "Faltando sinal de ponto e vírgula \';\' !", //8
                     "Função não possui corpo!", //9
                     "Faltando sinal de fecha chaves \'}\' !", //10
                     "Parâmetro vazio especificado. Não espera-se mais parâmetros!", //11
                     "Tipo do Parâmetro esperado!", //12
                     "Nome de Parâmetro de Função omitido!", //13
                     "Tipo de identificador não especificado!", //14
                     "Identificador não declarado!", //15
                     "Tipo do Identificador não compatível com a expressão!", //16
                     "Conflito entre tipos dos identificadores!", //17
                     "Tipo do Parâmetro não compatível com a expressão da função!", //18
                     "Este nome de Identificador já foi declarado antes!", //19
                     "Retorno da função não corresponde com o tipo da função!", //20
                     "Parâmetro na declaração de função não corresponde ao de seu protótipo!", //21
                     "Faltando parâmetro(s) na declaração da função de acordo com seu protótipo!", //22
                     "Excesso de parâmetros na declaração da função de acordo com seu protótipo!" //23
                   };

void erro(int e) {
    printf("\nErro Sintático: %s\nErro na Linha: %d\n", erros[e], qtd_linhas);
    exit(e);
}
