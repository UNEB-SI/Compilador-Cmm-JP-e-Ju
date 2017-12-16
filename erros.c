#include <stdio.h>
#include <stdlib.h>
#include "erros.h"
#include "analex.h"

extern int qtd_linhas;
extern FILE *MP;
extern const char arquivoMP[];
extern FILE *FD;

char erros[][100] =  {"Sem erro", //0
                     "Caracter invalido!", //1
                     "Formato de numero invalido!", //2
                     "Faltando sinal fecha parenteses \')\' !", //3
                     "Operando invalido!", //4
                     "Expressao mal formatada!", //5
                     "ID esperado!", //6
                     "Faltando parametros no prototipo da funcao!", //7
                     "Faltando sinal de ponto e vírgula \';\' !", //8
                     "Funcao nao possui corpo!", //9
                     "Faltando sinal de fecha chaves \'}\' !", //10
                     "Parametro vazio especificado. Nao espera-se mais parametros!", //11
                     "Tipo do Parametro esperado!", //12
                     "Nome de Parametro de Funcao omitido!", //13
                     "Tipo de identificador nao especificado!", //14
                     "Identificador nao declarado!", //15
                     "Tipo do Identificador nao compatível com a expressao!", //16
                     "Conflito entre tipos dos identificadores!", //17
                     "Tipo do Parametro nao compatível com a declaracao da funcao!", //18
                     "Este nome de Identificador já foi declarado antes!", //19
                     "Retorno da funcao nao corresponde com o tipo da funcao!", //20
                     "Tipo da declaracao da funcao nao corresponde com o tipo de seu prototipo!", //21
                     "Parametro na declaracao de funcao nao corresponde ao de seu prototipo!", //22
                     "Faltando parametro(s) na declaracao da funcao de acordo com seu prototipo!", //23
                     "Excesso de parametros na declaracao da funcao de acordo com seu prototipo!", //24
                     "Funcoes do tipo semretorno nao devem retornar nenhum valor!", //25
                     "Funcao sem retorno!", //26
                     "Funcao com valor de retorno nao deve ser tratado como comando!", //27
                     "A funcao Principal nao deve ser declarada como variavel!", //28
                     "A funcao Principal nao deve ser declarada em escopo local!", //29
                     "Faltando parametro(s) na chamada da funcao de acordo com a declaracao!", //30
                     "Excesso de parametros na chamada da funcao de acordo com a declaracao!" //31
                   };

void erro(int e) {
    printf("\nErro na Linha: %d\nMensagem: \"%s\"\n", qtd_linhas, erros[e]);
    fclose(MP);
    remove(arquivoMP); //deleta o arquivo objeto
    fclose(FD);
    exit(e);
}
