#include <stdio.h>
#include <stdlib.h>
#define true 1
#define false 0

typedef int bool;
typedef int TIPOPESO;

#define BRANCO 0
#define AMARELO 1
#define VERMELHO 2

typedef struct adjacencia{
    int vertice; //vertice de destino
    TIPOPESO peso; //peso associado � aresta que leva ao vertice de destino
    struct adjacencia *prox; //o proximo elemento da lista de adjac�ncias
}ADJACENCIA;

typedef struct vertice{
    /*dados armazenados v�o aqui*/
    ADJACENCIA *cab; //possui apenas a cabe�a da lista de adjac�ncia
}VERTICE;

typedef struct grafo{ //cada grafo possui:
    int vertices; //numero de vertices total do grafo
    int arestas; //numero de arestas total do grafo
    VERTICE *adj; //arranjo de vertices da estrutura
}GRAFO;

/*fun�ao para criar um GRAFO*/
GRAFO *criaGrafo(int v){
    int i;

    GRAFO *g = (GRAFO *)malloc(sizeof(GRAFO));
    g->vertices = v;
    g->arestas = 0;
    g->adj = (VERTICE *)malloc(v* sizeof(VERTICE));

    for(i=0; i<v; i++){
        g->adj[i].cab=NULL;
    }
    return(g);
}

bool excluiAresta(GRAFO *gr, int vi, int vf) {
    if (!gr) return false; // Verifica se o grafo existe
    if (vi < 0 || vi >= gr->vertices || vf < 0 || vf >= gr->vertices) return false; // Verifica se os v�rtices s�o v�lidos

    ADJACENCIA *atual = gr->adj[vi].cab;
    ADJACENCIA *anterior = NULL;

    while (atual != NULL) {
        if (atual->vertice == vf) { // Encontrou a aresta a ser removida
            if (anterior == NULL) {
                gr->adj[vi].cab = atual->prox;
            } else {
                anterior->prox = atual->prox;
            }
            free(atual); // Libera a mem�ria ocupada pela aresta
            return true;
        }
        anterior = atual;
        atual = atual->prox;
    }

    return false; // A aresta n�o foi encontrada
}

bool criaArestaTransposta(GRAFO *gr, int vf, int vi, TIPOPESO p) {
    // Apenas invertemos a ordem dos v�rtices ao criar a aresta
    criaAresta(gr, vf, vi, p);
    return excluiAresta(gr, vf, vi);
}

bool verificaAresta(GRAFO *gr, int vi, int vf) {
    if (!gr) return false; // Verifica se o grafo existe
    if (vi < 0 || vi >= gr->vertices || vf < 0 || vf >= gr->vertices) return false; // Verifica se os v�rtices s�o v�lidos

    ADJACENCIA *atual = gr->adj[vi].cab;

    while (atual != NULL) {
        if (atual->vertice == vf) { // Aresta encontrada
            return true;
        }
        atual = atual->prox;
    }

    return false; // Aresta n�o encontrada
}


// Criando o grafo transposto
GRAFO *criaGrafoTransposto(GRAFO *gr){
    GRAFO * gr_transposto = gr;
    int i;
    for(i = 0; i < gr->vertices; i++) {
        ADJACENCIA *ad = gr->adj[i].cab;
        while(ad) {
            criaArestaTransposta(gr_transposto, ad->vertice, i, ad->peso);
            ad = ad->prox;
        }
    }
}

ADJACENCIA *criaAdj(int v, int peso){
    ADJACENCIA *temp = (ADJACENCIA *)malloc(sizeof(ADJACENCIA));//aloca espa�o para um n�
    temp->vertice = v; //vertice alvo da adjacencia
    temp->peso = peso; //peso da aresta
    temp->prox = NULL;
    return(temp);//retorna endere�o da adjac�ncia
}

bool criaAresta(GRAFO *gr, int vi, int vf, TIPOPESO p){ //vai de vi a vf
    if(!gr) return(false); //valida��o se o grafo existe
    if((vf<0)||(vf>= gr->vertices))return(false);//valida��o se os valores n�o s�o negativos
    if((vi<0)||(vi>= gr->vertices))return(false);//ou maiores que o numero de v�rtices do grafo

    ADJACENCIA *novo = criaAdj(vf,p); //crio adjacencia com o vertice final e o peso
     //coloco a adjacencia na lista do vertice inicial
     novo->prox = gr->adj[vi].cab;//o campo prox da adjacencia vai receber a cabe�a da lista
     gr->adj[vi].cab = novo; //e a cabe�a da lista passa a ser o novo elemento
     gr->arestas++; //atualizo o numero de arestas no grafo

     return(true);
}

void imprime(GRAFO *gr){
    //valida��es se o grafo existe
    printf("GRAFO NA TELA!! \n");
    printf("Vertices: %d. Arestas: %d. \n",gr->vertices,gr->arestas); //printo numero de vertices e arestas

    int i;
    for(i=0; i<gr->vertices;i++){
        printf("v%d: ",i); //imprime a aresta atual
        ADJACENCIA *ad = gr->adj[i].cab; //cham o a cabe�a da lista desta aresta
        while(ad){ //enquanto houver adjacencia
            printf("v%d(%d) ", ad->vertice,ad->peso); //imprime a adjacencia e seu peso
            ad=ad->prox; //passo para proxima adjacencia
        }
        printf("\n");
    }
}

void visitaP(GRAFO *g, int u, int *cor){
    cor[u] = AMARELO; //visitar um n� marca como amarelo
    ADJACENCIA *v = g->adj[u].cab; //visita adjacencia
    while(v){ //ent�o visitamos sua adjacencia, aresta u e v recursivamente
        if(cor[v->vertice]==BRANCO) //se a cor for branco eu visito
            visitaP(g,v->vertice,cor); //visito chamando a fun��o de novo, recursivo
        v = v->prox;
    }
    cor[u] = VERMELHO;
}

void profundidade(GRAFO *g){
    int cor[g->vertices]; //crio um vetor com  arranjo de cores (o valor tem que bater com o do grafo)

    int u;
    for(u=0;u<g->vertices;u++){ //inicialmente todos s�o brancos
        cor[u] = BRANCO;
    }
    for(u=0;u<g->vertices;u++){
        if(cor[u] == BRANCO)//se for branco visita todos em profundidade
        visitaP(g,u,cor); //repare que � recursivo, visita at� o final depois volta
    }
}

int main(){
    GRAFO * gr = criaGrafo(5);
    criaAresta(gr, 0, 1, 2);
    criaAresta(gr, 1, 2, 4);
    criaAresta(gr, 2, 0, 12);
    criaAresta(gr, 2, 4, 40);
    criaAresta(gr, 3, 1, 3);
    criaAresta(gr, 4, 3, 8);
    criaAresta(gr, 0, 4, 7);
    criaAresta(gr, 1, 4, 9);

    imprime(gr);

    GRAFO * gr_transposto = criaGrafoTransposto(gr);
    imprime(gr_transposto);
}
