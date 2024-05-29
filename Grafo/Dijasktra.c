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
    TIPOPESO peso; //peso associado à aresta que leva ao vertice de destino
    struct adjacencia *prox; //o proximo elemento da lista de adjacências
}ADJACENCIA;

typedef struct vertice{
    /*dados armazenados vão aqui*/
    ADJACENCIA *cab; //possui apenas a cabeça da lista de adjacência
    TIPOPESO distancia; // distância mínima conhecida do vértice inicial ao vértice atual
    int precedente; // vértice anterior na menor rota conhecida do vértice inicial ao vértice atual
}VERTICE;

typedef struct grafo{ //cada grafo possui:
    int vertices; //numero de vertices total do grafo
    int arestas; //numero de arestas total do grafo
    VERTICE *adj; //arranjo de vertices da estrutura
}GRAFO;

/*função para criar um GRAFO*/
GRAFO *criaGrafo(int v){
    int i;

    GRAFO *g = (GRAFO *)malloc(sizeof(GRAFO));
    g->vertices = v;
    g->arestas = 0;
    g->adj = (VERTICE *)malloc(v* sizeof(VERTICE));

    for(i=0; i<v; i++){
        g->adj[i].cab=NULL;
        g->adj[i].distancia = INT_MAX; // Inicializa todas as distâncias como infinito
        g->adj[i].precedente = -1; // Inicializa todos os predecessores como -1
    }
    return(g);
}

/*função para criar uma ADJACENCIA*/
ADJACENCIA *criaAdj(int v, int peso){
    ADJACENCIA *temp = (ADJACENCIA *)malloc(sizeof(ADJACENCIA));//aloca espaço para um nó
    temp->vertice = v; //vertice alvo da adjacencia
    temp->peso = peso; //peso da aresta
    temp->prox = NULL;
    return(temp);//retorna endereço da adjacência
}

/*função para criar uma aresta não direcionada*/
bool criaAresta(GRAFO *gr, int vi, int vf, TIPOPESO p){ //vai de vi a vf
    if(!gr) return(false); //validação se o grafo existe
    if((vf<0)||(vf>= gr->vertices))return(false);//validação se os valores não são negativos
    if((vi<0)||(vi>= gr->vertices))return(false);//ou maiores que o numero de vértices do grafo

    ADJACENCIA *novo = criaAdj(vf,p); //crio adjacencia com o vertice final e o peso
    novo->prox = gr->adj[vi].cab;//o campo prox da adjacencia vai receber a cabeça da lista
    gr->adj[vi].cab = novo; //e a cabeça da lista passa a ser o novo elemento

    // Como é um grafo não direcionado, precisamos adicionar a aresta no sentido oposto
    novo = criaAdj(vi,p);
    novo->prox = gr->adj[vf].cab;
    gr->adj[vf].cab = novo;

    gr->arestas++; //atualizo o numero de arestas no grafo
    return(true);
}

/*função para imprimir o grafo*/
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

/* Função para relaxamento das arestas */
void relaxamento(GRAFO *gr, int u, int v, TIPOPESO peso) {
    if (gr->adj[v].distancia > gr->adj[u].distancia + peso) {
        gr->adj[v].distancia = gr->adj[u].distancia + peso;
        gr->adj[v].precedente = u;
    }
}

/* Função auxiliar para extrair o vértice não visitado com a menor distância */
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

/* Função principal que implementa o algoritmo de Dijkstra */
void dijkstra(GRAFO *gr, int origem) {
    int *visitado = (int *)malloc(gr->vertices * sizeof(int));
    int u, v;

    for (u = 0; u < gr->vertices; u++) {
        gr->adj[u].distancia = INT_MAX; // Inicializa todas as distâncias como infinito
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

    printf("\nMenores distâncias a partir do vértice 0:\n");
    for (int i = 0; i < gr->vertices; i++) {
        printf("Vértice %d: Distância = %d, Precedente = %d\n", i, gr->adj[i].distancia, gr->adj[i].precedente);
    }

    return 0;
}
