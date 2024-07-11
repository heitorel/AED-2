#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define INFINITY 99999
#define MAX_VERTICES 100

// Estrutura para representar o grafo em matriz de adjacência
typedef struct GrafoMatriz {
    int matriz[MAX_VERTICES][MAX_VERTICES];
    int numVertices;
} GrafoMatriz;

// Estrutura para representar uma aresta
typedef struct Aresta {
    int dest;
    int peso;
    struct Aresta* prox;
} Aresta;

// Estrutura para representar um vértice
typedef struct Vertice {
    Aresta* arestas;
    int peso;
} Vertice;

// Estrutura para representar o grafo
typedef struct Grafo {
    Vertice vertices[MAX_VERTICES];
    int numVertices;
} Grafo;

// Função para inicializar o grafo em matriz de adjacência
void inicializarGrafoMatriz(GrafoMatriz* grafo, int numVertices) {
    grafo->numVertices = numVertices;
    // Inicializa todas as entradas da matriz com 0 (nenhuma aresta)
    for (int i = 0; i < numVertices; i++) {
        for (int j = 0; j < numVertices; j++) {
            grafo->matriz[i][j] = 0;
        }
    }
}

// Inicializa um grafo vazio
void initializeGrafo(Grafo* grafo) {
    grafo->numVertices = 0;
    for (int i = 0; i < MAX_VERTICES; ++i) {
        grafo->vertices[i].arestas = NULL;
    }
}

// Adiciona um vértice ao grafo
void addVertice(Grafo* grafo,int peso) {
    if (grafo->numVertices < MAX_VERTICES) {
        grafo->numVertices++;
        grafo->vertices[grafo->numVertices].peso = peso;
    } else {
        printf("Número maximo de vertices alcancado!\n");
    }
}

// Adiciona uma aresta ao grafo
void addAresta(Grafo* grafo, int ini, int dest, int peso) {
    if (ini >= 0 && ini < grafo->numVertices && dest >= 0 && dest < grafo->numVertices) {
        Aresta* newAresta = (Aresta*)malloc(sizeof(Aresta));
        newAresta->dest = dest;
        newAresta->peso = peso;
        newAresta->prox = grafo->vertices[ini].arestas;
        grafo->vertices[ini].arestas = newAresta;
    } else {
        printf("Vertices invalidos!\n");
    }
}

// Remove um vértice do grafo
void removeVertice(Grafo* grafo, int vertice) {
    if (vertice >= 0 && vertice < grafo->numVertices) {
        // Remove todas as arestas conectadas a este vértice
        for (int i = 0; i < grafo->numVertices; ++i) {
            Aresta* atualAresta = grafo->vertices[i].arestas;
            Aresta* antAresta = NULL;
            while (atualAresta != NULL) {
                if (atualAresta->dest == vertice) {
                    if (antAresta == NULL) {
                        grafo->vertices[i].arestas = atualAresta->prox;
                    } else {
                        antAresta->prox = atualAresta->prox;
                    }
                    atualAresta = atualAresta->prox;
                } else {
                    antAresta = atualAresta;
                    atualAresta = atualAresta->prox;
                }
            }
        }

        // Remove o vértice
        grafo->numVertices--;
        for (int i = vertice; i < grafo->numVertices; ++i) {
            grafo->vertices[i] = grafo->vertices[i + 1];
        }
    } else {
        printf("Vertice invalido!\n");
    }
}

// Remove uma aresta do grafo
void removeAresta(Grafo* grafo, int ini, int dest) {
    if (ini >= 0 && ini < grafo->numVertices && dest >= 0 && dest < grafo->numVertices) {
        Aresta* atualAresta = grafo->vertices[ini].arestas;
        Aresta* antAresta = NULL;
        while (atualAresta != NULL) {
            if (atualAresta->dest == dest) {
                if (antAresta == NULL) {
                    grafo->vertices[ini].arestas = atualAresta->prox;
                } else {
                    antAresta->prox = atualAresta->prox;
                }
                return;
            }
            antAresta = atualAresta;
            atualAresta = atualAresta->prox;
        }
        printf("Aresta nao encontrada!\n");
    } else {
        printf("Vertices invalidos!\n");
    }
}

// Função para verificar se existe uma aresta entre dois vértices
bool existeAresta(Grafo* graph, int origem, int destino) {
    // Verifica se os vértices fornecidos estão dentro dos limites do grafo
    if (origem < 0 || origem >= graph->numVertices || destino < 0 || destino >= graph->numVertices)
        return false;

    // Percorre a lista de adjacência do vértice de origem
    Aresta* listaAdj = graph->vertices[origem].arestas;
    while (listaAdj != NULL) {
        // Se encontrar o destino na lista de adjacência, retorna true
        if (listaAdj->dest == destino)
            return true;
        listaAdj = listaAdj->prox;
    }

    // Se não encontrar a aresta, retorna false
    return false;
}


// Busca em profundidade que detecta ciclo (DFS)
int DFSCiclo(Grafo* grafo, int ini, int visitado[]) {
    visitado[ini] = 1;
    printf("%d ", ini);
    int tamCiclo = 0;

    Aresta* atualAresta = grafo->vertices[ini].arestas;
    while (atualAresta != NULL) {
        if (!visitado[atualAresta->dest]) {
            ++tamCiclo;
            DFS(grafo, atualAresta->dest, visitado);
        } else {
            return tamCiclo;
        }
        tamCiclo = 0;
        atualAresta = atualAresta->prox;
    }
    return tamCiclo;
}

// Busca em profundidade(DFS)
void DFS(Grafo* grafo, int ini, int visitado[]) {
    visitado[ini] = 1;
    printf("%d ", ini);

    Aresta* atualAresta = grafo->vertices[ini].arestas;
    while (atualAresta != NULL) {
        if (!visitado[atualAresta->dest]) {
            DFS(grafo, atualAresta->dest, visitado);
        }
        atualAresta = atualAresta->prox;
    }
}

// Busca em largura (BFS)
void BFS(Grafo* grafo, int ini) {
    int visitado[MAX_VERTICES] = {0};
    int fila[MAX_VERTICES];
    int frent = 0, tras = -1;

    visitado[ini] = 1;
    fila[++tras] = ini;

    while (frent <= tras) {
        int atual = fila[frent++];
        printf("%d ", atual);

        Aresta* atualAresta = grafo->vertices[atual].arestas;
        while (atualAresta != NULL) {
            if (!visitado[atualAresta->dest]) {
                visitado[atualAresta->dest] = 1;
                fila[++tras] = atualAresta->dest;
            }
            atualAresta = atualAresta->prox;
        }
    }
}

// Algoritmo de Dijkstra para encontrar o menor caminho ponderado
void Dijkstra(Grafo* grafo, int ini) {
    int dist[MAX_VERTICES];
    int visitado[MAX_VERTICES] = {0};

    for (int i = 0; i < grafo->numVertices; ++i) {
        dist[i] = INFINITY;
    }
    dist[ini] = 0;

    for (int cont = 0; cont < grafo->numVertices - 1; ++cont) {
        int minDist = INFINITY;
        int minInd = -1;

        for (int v = 0; v < grafo->numVertices; ++v) {
            if (!visitado[v] && dist[v] < minDist) {
                minDist = dist[v];
                minInd = v;
            }
        }

        visitado[minInd] = 1;
        Aresta* atualAresta = grafo->vertices[minInd].arestas;
        while (atualAresta != NULL) {
            int dest = atualAresta->dest;
            if (!visitado[dest] && dist[minInd] + atualAresta->peso < dist[dest]) {
                dist[dest] = dist[minInd] + atualAresta->peso;
            }
            atualAresta = atualAresta->prox;
        }
    }
    printf("Menores custos a partir do vartice %d:\n", ini);
    for (int i = 0; i < grafo->numVertices; ++i) {
        printf("Para o vertice %d: %d\n", i, dist[i]);
    }
}

// Função para imprimir o grafo na tela
void imprimeGrafo(Grafo* grafo) {
    printf("\nGrafo: \n");
    // Itera sobre todos os vértices
    for (int i = 0; i < grafo->numVertices; ++i) {
        printf("Vertice %d:", i);
        Aresta* atualAresta = grafo->vertices[i].arestas;
        // Itera sobre todas as arestas do vértice atual
        while (atualAresta != NULL) {
            printf(" -> %d (peso: %d)", atualAresta->dest, atualAresta->peso);
            atualAresta = atualAresta->prox;
        }
        printf("\n");
    }
}

//exercicios

//Escreva um algoritmo para contar a quantidade de laços em um grafo
void lacosNoGrafo(Grafo* grafo){
    int lacos = 0;

    for(int i = 0; i < grafo->numVertices; i++){
        Aresta* aresta = grafo->vertices[i].arestas;
        while(aresta != NULL){
            if(aresta->dest == i){
                lacos++;
            }
            aresta = aresta->prox;
        }
    }
    printf("\nO grafo tem %d lacos.", lacos);
}

//remover todos os laços encontrados
int removeLacosNoGrafo(Grafo* grafo){
    int lacos = 0;

    for(int i = 0; i < grafo->numVertices; i++){
        Aresta* aresta = grafo->vertices[i].arestas;
        while(aresta != NULL){
            if(aresta->dest == i){
                lacos++;
                removeAresta(grafo, i, i);
            }
            aresta = aresta->prox;
        }
    }
    printf("\nForam removidos %d lacos.", lacos);
    return lacos;
}

//Escreva um algoritmo para destruir as arestas de um grafo, tornando-o vazio.
void removeTodasArestas(Grafo* grafo){
    for(int i = 0; i < grafo->numVertices; i++){
        Aresta* aresta = grafo->vertices[i].arestas;
        while(aresta != NULL){
            removeAresta(grafo, i, aresta->dest);
            aresta = aresta->prox;
        }
    }
    printf("\nGrafo sem arestas:");
    imprimeGrafo(grafo);
}

//Seja um grafo g dirigido. Escreva um algoritmo para retornar o grafo transposto de g
void transpoeGrafo(Grafo* grafo){
    for(int i = 0; i < grafo->numVertices; i++){
        Aresta* aresta = grafo->vertices[i].arestas;
        while(aresta != NULL){
            addAresta(grafo,aresta->dest,i,aresta->peso);
            removeAresta(grafo, i, aresta->dest);
            aresta = aresta->prox;
        }
    }
    printf("\nGrafo transposto:");
    imprimeGrafo(grafo);
}

//Escreva um algoritmo que dado um grafo m representado em matriz, retorne o mesmo grafo em listas de adjacências.
void matrizParaListAdj(GrafoMatriz* grafoMatriz){
    Grafo grafo;
    initializeGrafo(&grafo);

    for (int i = 0; i < grafoMatriz->numVertices; i++) {
        for (int j = 0; j < grafoMatriz->numVertices; j++) {
            addVertice(&grafo,grafoMatriz->matriz[i][j]);
        }
    }
    for (int i = 0; i < grafoMatriz->numVertices; i++) {
        for (int j = 0; j < grafoMatriz->numVertices; j++) {
            addAresta(&grafo, i, j, grafoMatriz->matriz[i][j]);
        }
    }
}

//Uma árvore enraizada é um grafo acíclico, conexo e dirigido, com um único vértice fonte de onde todas as arestas partem. Escreva um algoritmo que, dado um grafo g, verifique se é uma árvore
//enraizada ou não, retornando true/false conforme o caso
int arvoreEnraizada(Grafo* grafo){

}

//Seja um grafo g não-dirigido ponderado (com um peso inteiro associado a cada aresta). Escreva um algoritmo que, dado g e um custo mínimo int c, retorne uma cópia de g contendo apenas as arestas
//de custo maior do que c.
Grafo arestasDeValor(Grafo* grafo, int custo){
    Grafo grafoCaro;
    initializeGrafo(&grafoCaro);

    for (int i = 0; i < grafo->numVertices; i++) {
        addVertice(&grafoCaro,grafo->vertices[i].peso);
    }

    for(int i = 0; i < grafo->numVertices; i++){
        Aresta* aresta = grafo->vertices[i].arestas;
        while(aresta != NULL){
            if(aresta->peso < custo){
                addAresta(&grafoCaro,i,aresta->dest,aresta->peso);
            }
            aresta = aresta->prox;
        }
    }
    return grafoCaro;
}

// Função para verificar se g2 é um subgrafo de g1
bool verificarSubgrafo(Grafo* g1, Grafo* g2) {
    // Verifica se o número de vértices em g2 é maior que o número de vértices em g1
    if (g2->numVertices > g1->numVertices)
        return false;

    // Verifica cada vértice e aresta de g2 em g1
    for (int i = 0; i < g2->numVertices; i++) {
        Aresta* arestaAtual = g2->vertices[i].arestas;
        while (arestaAtual != NULL) {
            // Verifica se a aresta de g2 está presente em g1
            if (arestaAtual->dest >= g1->numVertices || g1->vertices[i].arestas == NULL)
                return false;
            bool encontrada = false;
            Aresta* arestaG1 = g1->vertices[i].arestas;
            while (arestaG1 != NULL) {
                if (arestaG1->dest == arestaAtual->dest) {
                    encontrada = true;
                    break;
                }
                arestaG1 = arestaG1->prox;
            }
            if (!encontrada)
                return false;
            arestaAtual = arestaAtual->prox;
        }
    }

    return true;
}

//Dados dois grafos g1 e g2, escreva um algoritmo que retorne um grafo g3 cujas arestas estejam
//presentes em g1 mas não em g2.
Grafo criarGrafoMenosGrafo(Grafo* g1, Grafo* g2) {
    Grafo g3;
    initializeGrafo(&g3);

    for (int i = 0; i < g1->numVertices; i++) {
        addVertice(&g3,g1->vertices[i].peso);
    }

    for(int i = 0; i < g1->numVertices; i++){
        Aresta* arestaG1 = g1->vertices[i].arestas;
        Aresta* arestaG2 = g2->vertices[i].arestas;

        while(arestaG1 != NULL){
            if(arestaG2 == NULL){
                addAresta(&g3,i,arestaG1->dest,arestaG1->peso);
            }
            arestaG1 = arestaG1->prox;
            arestaG2 = arestaG2->prox;
        }
    }
    return g3;
}

//Considere um grafo dirigido em lista de adjacências representando uma rede de troca de emails,
//onde cada vértice representa um usuário, e cada aresta representa um email enviado. Cada aresta
//possui um campo int id que representa o conteúdo da mensagem (ou seja, o campo id faz parte da
//definição do nó das listas de adjacências). Problema: circula pela rede uma mensagem x do tipo
//spam. Escreva um algoritmo para exibir os usuários suspeitos de iniciar a propagação de x. Um
//usuário é considerado suspeito se ele próprio não recebeu a mensagem x nenhuma vez.
void identificaSpammer(Grafo* grafo, int spam){
    bool spammer[grafo->numVertices];

    for(int i = 0; i < grafo->numVertices; i++){
        Aresta* aresta = grafo->vertices[i].arestas;
        while(aresta != NULL){
            if(aresta->peso == spam){
                spammer[aresta->dest] = false;
            }
            aresta = aresta->prox;
        }
    }

    for(int i = 0; i < grafo->numVertices; i++){
        if(!spammer[i] == false){
            printf(spammer);
        }
    }
}

//Considere um grafo dirigido em lista de adjacências representando uma rede de chamadas
//telefônicas entre diversas unidades de uma empresa, onde cada vértice representa uma unidade, e
//cada aresta representa uma chamada efetuada. As unidades podem estar em diferentes países,
//identificados por um campo int país definido no respectivo vértice. Problema: a conta de telefone
//global da empresa está muito alta. Escreva um algoritmo que identifique a unidade que efetua
//chamadas para o maior número de países. Havendo empate, retorne qualquer resposta possível
int telefonia(Grafo* grafo){
    int ligacoesMax = 0;
    int ligacoesAtual = 0;

    for(int i = 0; i < grafo->numVertices; i++){
        Aresta* aresta = grafo->vertices[i].arestas;
        while(aresta != NULL){
            ligacoesAtual++;
            aresta = aresta->prox;
        }
        if(ligacoesAtual>=ligacoesMax){
            ligacoesMax = ligacoesAtual;
        }
        ligacoesAtual = 0;
    }
    return ligacoesMax;
}

//Seja um grafo g e dois vértices a e b. Verifique se há um caminho qualquer entre a e b retornando true/false conforme o caso
bool verificaCaminho(Grafo* grafo, int ini, int dest, int visitado[]) {
    visitado[ini] = 1;
    printf("%d ", ini);
    int tamCaminho = 0;
    int caminho[grafo->numVertices];

    Aresta* atualAresta = grafo->vertices[ini].arestas;
    while (atualAresta != NULL) {
        if(atualAresta->dest == dest){
            printf(caminho);
            return true;
        }
        if (!visitado[atualAresta->dest]) {
            caminho[ini] = ini;
            tamCaminho++;
            verificaCaminho(grafo, atualAresta->dest, dest, visitado);
        }
        atualAresta = atualAresta->prox;
    }
    tamCaminho = 0;
    return false;
}

//Seja um grafo g não-dirigido. Escreva uma função para detectar ciclos em g, retornando true/false
bool detectaCiclos(Grafo* grafo){
    int visitado[grafo->numVertices];
    int ciclo = 0;

    for(int i = 0; i < grafo->numVertices; i++){
        ciclo = DFSCiclo(grafo,i,visitado);
    }
    if(ciclo != 0){
        return true;
    }
    return false;
}

//Um grafo não dirigido é completo se todos seus vértices são adjacentes, ou seja, existe uma aresta
//conectando cada par de vértices de um grafo. Escreva um algoritmo que, dado um grafo simples g,
//verifique se g é completo, retornando true/false conforme o caso.
bool grafoCompleto(Grafo* grafo){
    bool completo = false;
    for(int i = 0; i < grafo->numVertices; i++){
        for(int j = 0; j < grafo->numVertices; j++){
            completo = existeAresta(grafo,i,j);
            if(!completo){
                return false;
            }
        }
    }
    return true;
}

//Seja G = (V;A) um grafo simples e Ḡ = (V; V2-A) seu complemento, onde V2 é o conjunto de todos
//os pares de vértices em V . Escreva um método que, dado um grafo simples G, retorne seu complemento Ḡ.
void completaGrafo(Grafo* grafo){
    Grafo grafoComplemento;
    initializeGrafo(&grafoComplemento);

    for(int i = 0; i<grafo->numVertices; i++){
        addVertice(&grafoComplemento,grafo->vertices[i].peso);
    }

    for(int i = 0; i<grafo->numVertices; i++){
        for(int j = 0; j<grafo->numVertices;j++){
            if(!existeAresta(grafo,i,j)){
                addAresta(&grafoComplemento,i,j,0);
            }
        }
    }
}

//Seja um grafo g representando salas de aula (vértices) e suas ligações (arestas). Cada sala possui
//uma ocupação representada por um inteiro. Escreva um algoritmo que, a partir da sala atual i,
//encontre a sala vazia mais próxima, retornando o número do vértice correspondente. Havendo
//mais de uma sala que atenda estas condições, retorne a primeira que encontrar.
void salaVazia(Grafo* grafo, int ini){
    int visitado[MAX_VERTICES] = {0};
    int fila[MAX_VERTICES];
    int frent = 0, tras = -1;

    visitado[ini] = 1;
    fila[tras++] = ini;

    while(frent != tras){
        int atual = fila[frent];
        frent++;
        Aresta* arestas = grafo->vertices[atual].arestas;
        while(arestas != NULL){
            if(!visitado[arestas->dest]){
                visitado[arestas->dest] = 1;
                fila[++tras] = arestas->dest;
                if(grafo->vertices[arestas->dest].peso == 0){
                    printf("a sala %d esta vazia.", arestas->dest);
                }
            }
            arestas = arestas->prox;
        }
    }
}

//Para todos os vértices de um grafo, calcular o tamanho do caminho mais curto a partir de um vértice inicial i.
void caminhoMaisCurto(Grafo* grafo){
    for(int ini = 0; ini<grafo->numVertices; ini++){
        for(int dest = 0; dest<grafo->numVertices; dest++){
            int visitado[MAX_VERTICES] = {0};
            int fila[MAX_VERTICES];
            int frent = 0, tras = -1;
            int tam = 0;

            visitado[ini] = 1;
            fila[++tras] = ini;

            while(frent <= tras){
                int atual = fila[frent++];
                Aresta* arestas = grafo->vertices[ini].arestas;
                while(arestas != NULL){
                    if(!visitado[arestas->dest]){
                        visitado[arestas->dest] = 1;
                        fila[++tras] = arestas->dest;
                        tam++;
                        if(arestas->dest = dest){
                            printf("o tamanho do caminho de %d a %d e: %d.", ini, dest, tam);
                        }
                    }
                    arestas = arestas->prox;
                }
            }
        }
    }
}

//Seja um grafo não-dirigido representando uma rede social. Os vértices são os usuários e as arestas
//indicam relações (e.g., de amizade) entre pares de usuários. Dado um usuário i, escreva um
//algoritmo para exibir todos os usuários relacionados a i com até d graus de distância (medida em
//quantidade de arestas). Os amigos imediatos estão no grau 1, os amigos dos amigos no grau 2, e assim por diante
void redeSocial(Grafo* grafo, int graus, int nvAmizade[], int visitado[]){
    int grauMax = graus;
    for(int i; i<grafo->numVertices; i++){
        visitado[i] = 1;
        Aresta* arestas = grafo->vertices[i].arestas;
        while(arestas != NULL && graus >= 0){
            if(!visitado[arestas->dest]){
                graus--;
                nvAmizade[arestas->dest] = grauMax-graus;
                redeSocial(grafo,graus,nvAmizade,visitado);
            }
            arestas=arestas->prox;
        }
    }
}

//Seja um grafo dirigido representando trocas de email entre usuários. Os vértices são usuários e as
//arestas orientadas indicam que houve envio de mensagens na respectiva direção, incluindo um
//contador do número de mensagens enviadas. Escreva um algoritmo que, dado um usuário atual i,
//retorne uma lista ligada contendo todos os usuários que estão diretamente relacionados com i, e
//que enviaram ou receberam pelo menos k mensagens de/para i. A constante k é fornecida como
//parâmetro de entrada para a função.
typedef struct listaEmail{
    int dest;
    int peso;
    struct ListaEmail* prox;
} ListaEmail;

ListaEmail trocaTrocaEmails(Grafo* grafo, int quantidade, int usuario){
    ListaEmail listaEmail;
    for(int i = 0; i<grafo->numVertices; i++){
        Aresta* arestas = grafo->vertices[i].arestas;
        while(arestas != NULL){
            if(i==usuario || arestas->dest == usuario && arestas->peso==quantidade){
                //struct listaEmail;
            }
            arestas = arestas->prox;
        }
    }
    return listaEmail;
}

int main() {
    Grafo grafo;
    initializeGrafo(&grafo);

    addVertice(&grafo,2);
    addVertice(&grafo,4);
    addVertice(&grafo,7);
    addVertice(&grafo,0);

    addAresta(&grafo, 0, 1, 2);
    addAresta(&grafo, 1, 2, 3);
    addAresta(&grafo, 1, 1, 0);
    addAresta(&grafo, 2, 2, 0);
    addAresta(&grafo, 3, 3, 0);
    addAresta(&grafo, 0, 3, 5);
    addAresta(&grafo, 2, 3, 1);

    printf("Busca em profundidade (DFS): ");
    int visitedDFS[MAX_VERTICES] = {0};
    DFS(&grafo, 0, visitedDFS);
    printf("\n");

    printf("Busca em largura (BFS): ");
    BFS(&grafo, 0);
    printf("\n");

    Dijkstra(&grafo, 0);

    imprimeGrafo(&grafo);
    transpoeGrafo(&grafo);

    lacosNoGrafo(&grafo);
    //removeLacosNoGrafo(&grafo);
    removeTodasArestas(&grafo);

    return 0;
}
