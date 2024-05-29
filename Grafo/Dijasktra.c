#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

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
    TIPOPESO distancia; // dist�ncia m�nima conhecida do v�rtice inicial ao v�rtice atual
    int precedente; // v�rtice anterior na menor rota conhecida do v�rtice inicial ao v�rtice atual
}VERTICE;

typedef struct grafo{ //cada grafo possui:
    int vertices; //numero de vertices total do grafo
    int arestas; //numero de arestas total do grafo
    VERTICE *adj; //arranjo de vertices da estrutura
}GRAFO;

/*fun��o para criar um GRAFO*/
GRAFO *criaGrafo(int v){
    int i;

    GRAFO *g = (GRAFO *)malloc(sizeof(GRAFO));
    g->vertices = v;
    g->arestas = 0;
    g->adj = (VERTICE *)malloc(v* sizeof(VERTICE));

    for(i=0; i<v; i++){
        g->adj[i].cab=NULL;
        g->adj[i].distancia = INT_MAX; // Inicializa todas as dist�ncias como infinito
        g->adj[i].precedente = -1; // Inicializa todos os predecessores como -1
    }
    return(g);
}

/*fun��o para criar uma ADJACENCIA*/
ADJACENCIA *criaAdj(int v, int peso){
    ADJACENCIA *temp = (ADJACENCIA *)malloc(sizeof(ADJACENCIA));//aloca espa�o para um n�
    temp->vertice = v; //vertice alvo da adjacencia
    temp->peso = peso; //peso da aresta
    temp->prox = NULL;
    return(temp);//retorna endere�o da adjac�ncia
}

/*fun��o para criar uma aresta n�o direcionada*/
bool criaAresta(GRAFO *gr, int vi, int vf, TIPOPESO p){ //vai de vi a vf
    if(!gr) return(false); //valida��o se o grafo existe
    if((vf<0)||(vf>= gr->vertices))return(false);//valida��o se os valores n�o s�o negativos
    if((vi<0)||(vi>= gr->vertices))return(false);//ou maiores que o numero de v�rtices do grafo

    ADJACENCIA *novo = criaAdj(vf,p); //crio adjacencia com o vertice final e o peso
    novo->prox = gr->adj[vi].cab;//o campo prox da adjacencia vai receber a cabe�a da lista
    gr->adj[vi].cab = novo; //e a cabe�a da lista passa a ser o novo elemento

    // Como � um grafo n�o direcionado, precisamos adicionar a aresta no sentido oposto
    novo = criaAdj(vi,p);
    novo->prox = gr->adj[vf].cab;
    gr->adj[vf].cab = novo;

    gr->arestas++; //atualizo o numero de arestas no grafo
    return(true);
}

/*fun��o para imprimir o grafo*/
void imprime(GRAFO *gr){
    printf("GRAFO NA TELA!! \n");
    printf("Vertices: %d. Arestas: %d. \n",gr->vertices,gr->arestas);

    int i;
    for(i=0; i<gr->vertices;i++){
        printf("v%d: ",i);
        ADJACENCIA *ad = gr->adj[i].cab;
        while(ad){
            printf("v%d(%d) ", ad->vertice,ad->peso);
            ad=ad->prox;
        }
        printf("\n");
    }
}

/* Fun��o para relaxamento das arestas */
void relaxamento(GRAFO *gr, int u, int v, TIPOPESO peso) {
    if (gr->adj[v].distancia > gr->adj[u].distancia + peso) {
        gr->adj[v].distancia = gr->adj[u].distancia + peso;
        gr->adj[v].precedente = u;
    }
}

/* Fun��o auxiliar para extrair o v�rtice n�o visitado com a menor dist�ncia */
int extraiMinimo(GRAFO *gr, int *visitado) {
    int minimo = INT_MAX;
    int indice = -1;
    int i;

    for (i = 0; i < gr->vertices; i++) {
        if (!visitado[i] && gr->adj[i].distancia < minimo) {
            minimo = gr->adj[i].distancia;
            indice = i;
        }
    }

    return indice;
}

/* Fun��o principal que implementa o algoritmo de Dijkstra */
void dijkstra(GRAFO *gr, int origem) {
    int *visitado = (int *)malloc(gr->vertices * sizeof(int));
    int u, v;

    for (u = 0; u < gr->vertices; u++) {
        gr->adj[u].distancia = INT_MAX; // Inicializa todas as dist�ncias como infinito
        gr->adj[u].precedente = -1; // Inicializa todos os predecessores como -1
        visitado[u] = false;
    }

    gr->adj[origem].distancia = 0;

    for (u = 0; u < gr->vertices; u++) {
        u = extraiMinimo(gr, visitado);
        visitado[u] = true;

        ADJACENCIA *ad = gr->adj[u].cab;
        while (ad) {
            v = ad->vertice;
            relaxamento(gr, u, v, ad->peso);
            ad = ad->prox;
        }
    }

    free(visitado);
}

int main(){
    GRAFO *gr = criaGrafo(5);
    criaAresta(gr, 0, 1, 2);
    criaAresta(gr, 1, 2, 4);
    criaAresta(gr, 2, 0, 12);
    criaAresta(gr, 2, 4, 40);
    criaAresta(gr, 3, 1, 3);
    criaAresta(gr, 4, 3, 8);
    criaAresta(gr, 0, 4, 7);
    criaAresta(gr, 1, 4, 9);

    printf("Grafo Original:\n");
    imprime(gr);

    dijkstra(gr, 0);

    printf("\nMenores dist�ncias a partir do v�rtice 0:\n");
    for (int i = 0; i < gr->vertices; i++) {
        printf("V�rtice %d: Dist�ncia = %d, Precedente = %d\n", i, gr->adj[i].distancia, gr->adj[i].precedente);
    }

    return 0;
}
