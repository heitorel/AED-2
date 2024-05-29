#define MAXNUMVERTICES 100
#define AN -1
#define VERTICE_INVALIDO -1

#include <stdbool.h>

typedef int TipoPeso;
typedef struct {
    TipoPeso mat[MAXNUMVERTICES+1][MAXNUMVERTICES+1];
} TipoGrafo;
