//--------------------------------------------------------------
// Heitor Lopes Bianchi
// Sergio Guilherme Rodrigues da Rocha
//--------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <stdbool.h>

// INFORME O NRO DO GRUPO CONFORME PLANILHA DIVULGADA
int grupo() {
  return 24;
}

// ######### ESCREVA O NROUSP AQUI
int nroUSP1() {
    return 10258730;
}

// E O SEGUNDO NROUSP AQUI, SE HOUVER
int nroUSP2() {
    return 11270906;
}

// elemento da lista de resposta - NAO ALTERE ESTA DEFINICAO
typedef struct estr {
      int adj; // elemento do caminho
      struct estr *prox;
} NO;

// estrutura para representar uma aresta
typedef struct {
    int destino;
    int peso;
    struct Aresta* prox;
} Aresta;

// estrutura para representar um vértice
typedef struct {
    Aresta* arestas;
    int ganho;
    bool visitado;
} Vertice;

// estrutura para representar o grafo
typedef struct {
    Vertice *vertices;
    int numVertices;
} Grafo;

// Função para adicionar uma aresta ao grafo
void adicionarAresta(Grafo *grafo, int origem, int destino, int peso) {
    Aresta *novaAresta = malloc(sizeof(Aresta));
    novaAresta->destino = destino;
    novaAresta->peso = peso;
    novaAresta->prox = grafo->vertices[origem].arestas;
    grafo->vertices[origem].arestas = novaAresta;
}

// Função para criar e inicializar o grafo
Grafo *criarGrafo(int N, int A, int ijpeso[], int ganhos[]) {
    Grafo *grafo = malloc(sizeof(Grafo));
    grafo->numVertices = N;
    grafo->vertices = malloc((N + 1) * sizeof(Vertice)); // +1 para evitar o uso do índice 0

    // Inicializa os vértices
    for (int i = 1; i <= N; i++) {
        grafo->vertices[i].ganho = ganhos[i];
        grafo->vertices[i].visitado = false;
        grafo->vertices[i].arestas = NULL;
    }

    // Adiciona as arestas ao grafo
    for (int i = 0; i < 3 * A; i += 3) {
        int origem = ijpeso[i];
        int destino = ijpeso[i + 1];
        int peso = ijpeso[i + 2];
        adicionarAresta(grafo, origem, destino, peso);
        adicionarAresta(grafo, destino, origem, peso); // Considera o grafo não-dirigido
    }

    return grafo;
}

// Função auxiliar para imprimir o caminho encontrado
void imprimirCaminho(NO *caminho) {
    if (caminho == NULL) {
        printf("Nao ha caminho valido.\n");
        return;
    }

    printf("Caminho encontrado: ");
    while (caminho != NULL) {
        printf("%d ", caminho->adj);
        caminho = caminho->prox;
    }
    printf("\n");
}

// Função principal
NO *caminhoRecursivo(Grafo *grafo, int atual, int destino, int *dinheiro) {
    if (atual == destino) {
        // Chegou ao destino, retorna o último vértice como caminho válido
        NO *ultimoVertice = malloc(sizeof(NO));
        ultimoVertice->adj = destino;
        ultimoVertice->prox = NULL;
        return ultimoVertice;
    }

    // Marca o vértice atual como visitado
    grafo->vertices[atual].visitado = true;

    // Itera sobre as arestas do vértice atual
    Aresta *arestaAtual = grafo->vertices[atual].arestas;
    while (arestaAtual != NULL) {
        int proximoVertice = arestaAtual->destino;
        int custo = arestaAtual->peso;

        // Verifica se o próximo vértice já foi visitado
        if (!grafo->vertices[proximoVertice].visitado && *dinheiro >= custo) {
            // Subtrai o custo do dinheiro disponível
            *dinheiro -= custo;

            // Acrescenta o dinheiro ganho na cidade visitada
            *dinheiro += grafo->vertices[proximoVertice].ganho;

            // Realiza a chamada recursiva para o próximo vértice
            NO *caminhoRestante = caminhoRecursivo(grafo, proximoVertice, destino, dinheiro);

            // Verifica se foi encontrado um caminho válido a partir do próximo vértice
            if (caminhoRestante != NULL) {
                // Adiciona o vértice atual ao caminho válido encontrado
                NO *novoNo = malloc(sizeof(NO));
                novoNo->adj = atual;
                novoNo->prox = caminhoRestante;
                return novoNo;
            }

            // Caso não tenha sido encontrado um caminho válido, restaura o dinheiro disponível
            *dinheiro += custo;
        }

        arestaAtual = arestaAtual->prox;
    }

    // Marca o vértice atual como não visitado para permitir novas tentativas
    grafo->vertices[atual].visitado = false;

    // Se não encontrar nenhum caminho válido a partir deste vértice, retorna NULL
    return NULL;
}

// funcao principal
NO *caminho(int N, int A, int ijpeso[], int ganhos[], int inicio, int fim, int* dinheiro){
	NO* resp = NULL;

    // Cria e inicializa o grafo
    Grafo *grafo = criarGrafo(N, A, ijpeso, ganhos);

    // Realiza a busca pelo caminho válido
    resp = caminhoRecursivo(grafo, inicio, fim, dinheiro);

	return resp;
}

int main() {
	// exemplo de teste trivial

    int N = 9;
    int A = 11;
    int ijpeso[] = {0,0,0,1,2,30,1,3,20,2,6,20,2,7,30,3,7,80,3,4,20,4,9,80,5,8,10,6,7,10,7,8,10,7,9,80};
    int ganhos[] = {0, 20, 10, 20, 200, 200, 200, 0, 40, 20};
    int inicio = 7;
    int fim = 4;
    int dinheiro = 50;

    NO *caminhoEncontrado = caminho(N, A, ijpeso, ganhos, inicio, fim, &dinheiro);
    imprimirCaminho(caminhoEncontrado);

    return 0;

}
