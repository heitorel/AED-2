//Heitor Lopes Bianchi
//10258730

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct no{
    int chave;
    struct no *dir;
    struct no *esq;
}No;

//inicializa �rvore
No *inicializar(){
    return NULL;
}

//insere elemento na �rvore
No* inserir(No* no, int chave){
    if(no == NULL){
        no = (No*)malloc(sizeof(No));
        no->chave = chave;
        no->dir = NULL;
        no->esq = NULL;
        return no;
    }else if(chave < no->chave){
        no->esq = inserir(no->esq, chave);
    }else{
        no->dir = inserir(no->dir, chave);
    }
    return no;
}

//busca menor elemento em uma �rvore
No* buscaMin(No* no){
    if(no != NULL){
        No* andarilho = no;
        while(andarilho->esq != NULL){
            andarilho = andarilho->esq;
        }
        return andarilho;
    }
    return NULL;
}

//remove elementos da �rvore
No* remover(No* no, int chave){
    if(no != NULL){
        if(chave < no->chave){
            no->esq = remover(no->esq, chave);
            return no;
        }else if(chave > no->chave){
            no->dir = remover(no->dir, chave);
            return no;
        }else{
            if(no->esq == NULL && no->dir == NULL){
                free(no);
                return NULL;
            }else if(no->esq != NULL && no->dir == NULL){
                No* aux = no->esq;
                free(no);
                return aux;
            }else if(no->esq == NULL && no->dir != NULL){
                No* aux = no->dir;
                free(no);
                return aux;
            }else if(no->esq != NULL && no->dir != NULL){
                No* aux = buscaMin(no->dir);
                int chaveAux = aux->chave;
                no = remover(no, chaveAux);
                no->chave = chaveAux;
                return no;
            }
        }
    }
    return NULL;
}

//imprime �rvore no prompt
void imprimeArvore(No* no){
    if(no != NULL){
        printf("%d ", no->chave);
        imprimeArvore(no->esq);
        imprimeArvore(no->dir);
    }
}

//imprime �rvore em arquivo txt;
void imprimeArvoreSaida(No* no, FILE* saida){
    if(no != NULL){
        fprintf(saida, "%d ", no->chave);
        imprimeArvoreSaida(no->esq, saida);
        imprimeArvoreSaida(no->dir, saida);
    }
}

int main()
{
    //inicializa n� raiz
    No* raiz = inicializar();

    //declara arquivos de entrada e sa�da e par�metros para leitura de instru��es
    FILE* arquivoInstrucoes;
    FILE* saida;
    int bufferLength = 11; //tamanho da linha
    char buffer[bufferLength];

    //abre arquivos de entrada e sa�da para leitura e escrita
    arquivoInstrucoes = fopen("arquivoinstrucao.txt", "r");
    saida = fopen("saida.txt", "w+");

    //l� linha por linha at� o fim do arquivo
    while(fgets(buffer, bufferLength, arquivoInstrucoes)) {
        char *containsInsere;
        char *containsRemove;
        char *containsImprime;
        char *containsFim;

        //checa se linha cont�m instru��es
        containsInsere = strstr(buffer, "insere");
        containsRemove = strstr(buffer, "remove");
        containsImprime = strstr(buffer, "imprime");
        containsFim = strstr(buffer, "fim");

        //toma decisao de acordo com instru��o
        if(containsInsere){
            int chave = atoi(&buffer[7]);
            raiz = inserir(raiz, chave);
        }else if(containsRemove){
            int chave = atoi(&buffer[7]);
            raiz = remover(raiz, chave);
        }else if(containsImprime){
            imprimeArvore(raiz);
            printf("\n");
        }else if(containsFim){
            imprimeArvoreSaida(raiz, saida);
        }
    }

    //fecha arquivos de entrada e sa�da
    fclose(saida);
    fclose(arquivoInstrucoes);

    return 0;
}
