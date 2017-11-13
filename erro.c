#include <stdio.h>
#include <stdlib.h>

char erros[][50] =  {"Sem erro", //0
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
}
