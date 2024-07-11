#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <stdbool.h>

typedef struct {
int NroUSP; // chave prim�ria
int curso;
int estado;
int idade;
bool valido; // para exclus�o l�gica
} REGISTRO;

typedef struct {
    int chave;
    long posicao;
} INDICE;

typedef struct {
    INDICE *indices;
    int tamanho;
} TabelaIndice;

typedef struct No {
    int chavePrimaria;
    struct No *prox;
} NO;

// I-Exerc�cios em arquivos simples (sem �ndices)


//1. Reescreva um arquivo arq1 em um novo arquivo arq2 eliminado os registros inv�lidos.
void copiarRegistrosValidos() {
    REGISTRO reg;
    FILE *arq1, *arq2;

    // Abrindo arquivos
    arq1 = fopen("arq1.bin", "rb");
    arq2 = fopen("arq2.bin", "wb");

    if (arq1 == NULL || arq2 == NULL) {
        printf("Erro ao abrir arquivos.\n");
        return 1;
    }

    // Lendo registros de arq1 e escrevendo em arq2 apenas os registros v�lidos
    while (fread(&reg, sizeof(REGISTRO), 1, arq1)) {
        if (reg.valido) {
            fwrite(&reg, sizeof(REGISTRO), 1, arq2);
        }
    }
    // Fechando arquivos
    fclose(arq1);
    fclose(arq2);

    printf("Registros v�lidos copiados para arq2 com sucesso.\n");
}

//2. Fa�a uma c�pia invertida de arq1 em um novo arquivo arq2, ou seja: copie o �ltimo registro (n) de
//arq1 para o in�cio de arq2, depois copie o registro n-1 para a segunda posi��o etc.
void copiarInvertido(FILE *arq1, FILE *arq2, int numRegistros) {
    REGISTRO reg;

    // Itera sobre os registros em arq1, come�ando do �ltimo
    for (int i = numRegistros - 1; i >= 0; i--) {
        // Move o ponteiro para a posi��o correta no arquivo arq1
        fseek(arq1, i * sizeof(REGISTRO), SEEK_SET);
        // L� o registro atual
        fread(&reg, sizeof(REGISTRO), 1, arq1);
        // Escreve o registro lido em arq2
        fwrite(&reg, sizeof(REGISTRO), 1, arq2);
    }
}
//3. Escreva uma fun��o que, dada um nroUSP X, retorne o registro correspondente.
REGISTRO buscarRegistro(FILE *arquivo, int numRegistros, int numUSP) {
    REGISTRO reg;

    // Itera sobre os registros no arquivo
    for (int i = 0; i < numRegistros; i++) {
        // Move o ponteiro para a posi��o correta no arquivo
        fseek(arquivo, i * sizeof(REGISTRO), SEEK_SET);
        // L� o registro atual
        fread(&reg, sizeof(REGISTRO), 1, arquivo);
        // Verifica se o n�mero USP corresponde ao n�mero do registro
        if (reg.NroUSP == numUSP) {
            // Retorna o registro correspondente
            return reg;
        }
    }

    // Se o registro n�o for encontrado, retorna um registro vazio (ou um valor de retorno indicativo)
    REGISTRO registroNaoEncontrado = { .NroUSP = -1 }; // Supondo que -1 seja um n�mero USP inv�lido
    return registroNaoEncontrado;
}

//4. Escreva uma fun��o para inserir um novo registro r no arquivo, tomando cuidado para evitar
//chaves duplicadas.
// Fun��o para verificar se j� existe um registro com a mesma chave prim�ria
bool verificarDuplicata(FILE *arquivo, int numRegistros, int numUSP) {
    REGISTRO reg;

    // Itera sobre os registros no arquivo
    for (int i = 0; i < numRegistros; i++) {
        // Move o ponteiro para a posi��o correta no arquivo
        fseek(arquivo, i * sizeof(REGISTRO), SEEK_SET);
        // L� o registro atual
        fread(&reg, sizeof(REGISTRO), 1, arquivo);
        // Verifica se a chave prim�ria j� existe
        if (reg.NroUSP == numUSP) {
            // Se existir, retorna true
            return true;
        }
    }

    // Se n�o existir, retorna false
    return false;
}

// Fun��o para inserir um novo registro no arquivo
void inserirRegistro(FILE *arquivo, REGISTRO novoRegistro, int *numRegistros) {
    // Verifica se j� existe um registro com a mesma chave prim�ria
    if (verificarDuplicata(arquivo, *numRegistros, novoRegistro.NroUSP)) {
        printf("Erro: J� existe um registro com a mesma chave prim�ria.\n");
        return;
    }

    // Posiciona o ponteiro do arquivo no final
    fseek(arquivo, 0, SEEK_END);
    // Escreve o novo registro no final do arquivo
    fwrite(&novoRegistro, sizeof(REGISTRO), 1, arquivo);
    // Atualiza o n�mero total de registros
    (*numRegistros)++;
}

//5. Escreva uma fun��o para excluir todos os registros do curso X.
// Fun��o para excluir todos os registros de um curso espec�fico
void excluirRegistrosDoCurso(FILE *arqEntrada, FILE *arqSaida, int curso) {
    REGISTRO reg;

    // Itera sobre todos os registros no arquivo de entrada
    while (fread(&reg, sizeof(REGISTRO), 1, arqEntrada)) {
        // Verifica se o registro pertence ao curso a ser exclu�do
        if (reg.curso != curso) {
            // Se n�o pertencer, escreve o registro no arquivo de sa�da
            fwrite(&reg, sizeof(REGISTRO), 1, arqSaida);
        }
    }
}

//6. Escreva uma fun��o para alterar o curso de um aluno de nroUSP X para o curso Y.
void alterarCursoPorNroUSP(FILE *arquivo, int numUSP, int novoCurso) {
    REGISTRO reg;
    bool encontrado = false;

    // Posiciona o ponteiro do arquivo no in�cio
    rewind(arquivo);

    // Itera sobre todos os registros no arquivo
    while (fread(&reg, sizeof(REGISTRO), 1, arquivo)) {
        // Verifica se o registro corresponde ao n�mero USP especificado
        if (reg.NroUSP == numUSP) {
            // Atualiza o campo de curso do registro
            reg.curso = novoCurso;
            // Move o ponteiro de arquivo para a posi��o atual menos o tamanho de um registro
            fseek(arquivo, -sizeof(REGISTRO), SEEK_CUR);
            // Escreve o registro atualizado de volta no arquivo
            fwrite(&reg, sizeof(REGISTRO), 1, arquivo);
            encontrado = true;
            break;
        }
    }

    // Verifica se o n�mero USP especificado foi encontrado no arquivo
    if (!encontrado) {
        printf("Registro com o n�mero USP %d n�o encontrado.\n", numUSP);
    } else {
        printf("Curso do aluno com n�mero USP %d alterado para %d.\n", numUSP, novoCurso);
    }
}

//7. Implemente o procedimento de ordena��o KeySort, que dado um arquivo arq1 cria uma tabela
//tempor�ria de chaves em mem�ria (id�ntica a uma tabela de �ndices prim�rios) e ent�o reescreve o
//arquivo em um novo arquivo de sa�da arq2, na ordem correta de chaves
// Fun��o de compara��o para o qsort
int compararChaves(const void *a, const void *b) {
    const INDICE *indiceA = (const INDICE *)a;
    const INDICE *indiceB = (const INDICE *)b;
    return (indiceA->chave - indiceB->chave);
}

// Procedimento de ordena��o KeySort
void keySort(FILE *arqEntrada, FILE *arqSaida) {
    REGISTRO reg;
    INDICE *indices = NULL;
    long posicao = 0;
    int numRegistros = 0;

    // Determina o n�mero total de registros no arquivo de entrada
    fseek(arqEntrada, 0, SEEK_END);
    numRegistros = ftell(arqEntrada) / sizeof(REGISTRO);
    rewind(arqEntrada);

    // Aloca mem�ria para a tabela de �ndices
    indices = (INDICE *)malloc(numRegistros * sizeof(INDICE));
    if (indices == NULL) {
        printf("Erro: Falha na aloca��o de mem�ria.\n");
        return;
    }

    // L� os registros do arquivo de entrada e preenche a tabela de �ndices
    while (fread(&reg, sizeof(REGISTRO), 1, arqEntrada)) {
        indices[posicao].chave = reg.NroUSP;
        indices[posicao].posicao = posicao;
        posicao++;
    }

    // Ordena a tabela de �ndices com base nas chaves
    qsort(indices, numRegistros, sizeof(INDICE), compararChaves);

    // Escreve os registros na ordem correta no arquivo de sa�da
    for (int i = 0; i < numRegistros; i++) {
        fseek(arqEntrada, indices[i].posicao * sizeof(REGISTRO), SEEK_SET);
        fread(&reg, sizeof(REGISTRO), 1, arqEntrada);
        fwrite(&reg, sizeof(REGISTRO), 1, arqSaida);
    }

    // Libera a mem�ria alocada para a tabela de �ndices
    free(indices);
}

// Exerc�cios em tabelas de �ndices prim�rios
// Para os exerc�cios a seguir, considere os mesmos arquivos do tipo acima, e tamb�m a exist�ncia de uma
// tabela de �ndices prim�rios mantida em mem�ria e suas respectivas fun��es de manipula��o.

// bool inserir�ndice(Tabela, int nroUSP, int end){} //inser��o em tabela ordenada, retornando true/false
// int buscarEndere�o(Tabela, int nroUSP){} // retorna -1 se end n�o existe
// int excluir�ndice(Tabela, int nroUSP){} // retorna o endere�o exlu�do, ou -1 se n�o encontrarExerc�cios em tabelas de �ndices prim�rios

//1. Escreva uma fun��o para inserir um novo registro r no arquivo, tomando cuidado para evitar
//chaves duplicadas (use o �ndice).
// Fun��o para inserir um novo registro no arquivo, evitando chaves duplicadas
bool inserirRegistroIndices(FILE *arquivo, TabelaIndice *tabelaIndice, REGISTRO r) {
    // Verifica se a chave j� existe na tabela de �ndices
    if (buscarEndere�o(tabelaIndice, r.NroUSP) != -1) {
        printf("Chave prim�ria j� existe.\n");
        return false;
    }
    // Move o cursor para o final do arquivo
    fseek(arquivo, 0, SEEK_END);
    // Escreve o registro no arquivo
    fwrite(&r, sizeof(REGISTRO), 1, arquivo);
    // Obt�m a posi��o do registro rec�m-inserido
    long posicao = ftell(arquivo) - sizeof(REGISTRO);
    // Insere a chave e a posi��o na tabela de �ndices
    if (!inserir�ndice(tabelaIndice, r.NroUSP, posicao)) {
        // Se falhar ao inserir no �ndice, remove o registro do arquivo
        fseek(arquivo, posicao, SEEK_SET);
        REGISTRO vazio = {0};
        fwrite(&vazio, sizeof(REGISTRO), 1, arquivo);
        return false;
    }
    return true;
}

//2. Escreva uma fun��o que, dada um nroUSP X, retorne o registro correspondente (use o �ndice).
REGISTRO buscarRegistro(FILE *arquivo, TabelaIndice tabelaIndice, int nroUSP) {
    // Busca o endere�o do registro na tabela de �ndices
    int endereco = buscarEndere�o(&tabelaIndice, nroUSP);
    if (endereco == -1) {
        printf("Registro com NroUSP %d n�o encontrado.\n", nroUSP);
        // Se o registro n�o for encontrado, retorna um registro vazio
        REGISTRO registro_vazio = {0};
        return registro_vazio;
    }
    // Move o cursor para a posi��o do registro no arquivo
    fseek(arquivo, tabelaIndice.indices[endereco].posicao, SEEK_SET);
    // L� o registro do arquivo
    REGISTRO registro;
    fread(&registro, sizeof(REGISTRO), 1, arquivo);

    return registro;
}

//3. Escreva uma fun��o para excluir todos os registros do curso X (use o �ndice).
void excluirRegistrosDoCurso(FILE *arquivo, TabelaIndice *tabelaIndice, int cursoX) {
    for (int i = 0; i < tabelaIndice->tamanho; i++) {
        // Verifica se o registro pertence ao curso X
        if (tabelaIndice->indices[i].chave != -1 && buscarRegistro(arquivo, *tabelaIndice, tabelaIndice->indices[i].chave).curso == cursoX) {
            // Marca o registro como inv�lido (exclu�do logicamente)
            REGISTRO registro = buscarRegistro(arquivo, *tabelaIndice, tabelaIndice->indices[i].chave);
            registro.valido = false;
            // Move o cursor para a posi��o do registro no arquivo
            fseek(arquivo, tabelaIndice->indices[i].posicao, SEEK_SET);
            // Escreve o registro de volta no arquivo com o campo 'valido' atualizado
            fwrite(&registro, sizeof(REGISTRO), 1, arquivo);
            // Remove o �ndice correspondente da tabela de �ndices
            excluir�ndice(tabelaIndice, tabelaIndice->indices[i].chave);
        }
    }
}

//4. Escreva uma fun��o para alterar o curso de um registro de nroUSP X para o curso Y (use o �ndice).
bool alterarCursoRegistro(FILE *arquivo, TabelaIndice *tabelaIndice, int nroUSP_X, int novo_curso_Y) {
    // Busca o endere�o do registro na tabela de �ndices
    int endereco = buscarEndere�o(tabelaIndice, nroUSP_X);
    if (endereco == -1) {
        printf("Registro com NroUSP %d n�o encontrado.\n", nroUSP_X);
        return false;
    }

    // Move o cursor para a posi��o do registro no arquivo
    fseek(arquivo, tabelaIndice->indices[endereco].posicao, SEEK_SET);
    // L� o registro do arquivo
    REGISTRO registro;
    fread(&registro, sizeof(REGISTRO), 1, arquivo);
    // Altera o curso do registro para Y
    registro.curso = novo_curso_Y;
    // Move o cursor de volta para a posi��o do registro no arquivo
    fseek(arquivo, tabelaIndice->indices[endereco].posicao, SEEK_SET);
    // Escreve o registro de volta no arquivo com o campo 'curso' atualizado
    fwrite(&registro, sizeof(REGISTRO), 1, arquivo);

    return true;
}

//5. Escreva uma fun��o para alterar o registro de nroUSP X para o nroUSP Y se poss�vel (use o �ndice).
bool numeroUSPExiste(TabelaIndice tabelaIndice, int nroUSP) {
    for (int i = 0; i < tabelaIndice.tamanho; i++) {
        if (tabelaIndice.indices[i].chave == nroUSP) {
            return true;
        }
    }
    return false;
}

// Fun��o para alterar o registro de nroUSP X para o nroUSP Y, se poss�vel
bool alterarNroUSPRegistro(FILE *arquivo, TabelaIndice *tabelaIndice, int nroUSP_X, int nroUSP_Y) {
    // Verifica se o n�mero USP Y j� existe na tabela de �ndices
    if (numeroUSPExiste(*tabelaIndice, nroUSP_Y)) {
        printf("J� existe um registro com o n�mero USP %d.\n", nroUSP_Y);
        return false;
    }

    // Busca o endere�o do registro com o n�mero USP X na tabela de �ndices
    int endereco = buscarEndere�o(tabelaIndice, nroUSP_X);
    if (endereco == -1) {
        printf("Registro com NroUSP %d n�o encontrado.\n", nroUSP_X);
        return false;
    }

    // Move o cursor para a posi��o do registro no arquivo
    fseek(arquivo, tabelaIndice->indices[endereco].posicao, SEEK_SET);
    // L� o registro do arquivo
    REGISTRO registro;
    fread(&registro, sizeof(REGISTRO), 1, arquivo);
    // Altera o n�mero USP do registro para Y
    registro.NroUSP = nroUSP_Y;
    // Move o cursor de volta para a posi��o do registro no arquivo
    fseek(arquivo, tabelaIndice->indices[endereco].posicao, SEEK_SET);
    // Escreve o registro de volta no arquivo com o campo 'NroUSP' atualizado
    fwrite(&registro, sizeof(REGISTRO), 1, arquivo);
    // Atualiza o �ndice correspondente na tabela de �ndices
    tabelaIndice->indices[endereco].chave = nroUSP_Y;

    return true;
}

//Nos exerc�cios a seguir considere ainda a exist�ncia de tabelas de �ndices secund�rios (e suas respectivas
//opera��es de inser��o e exclus�o) para os campos int curso e int estado, e duas fun��es adicionais que,
//dada uma chave secund�ria de interesse, retornam a lista ligada das chaves prim�rias a ela relacionadas:

//NO* chavesCurso(int curso)
//NO *chavesEstado(int estado)

//No caso dos exerc�cios de exclus�o, certifique-se de que voc� entende porque a atualiza��o de �ndices
//secund�rios (mas n�o do �ndice prim�rio, que sempre precisa ser exclu�do) � opcional.

//1. Escreva uma fun��o para inserir um novo registro r no arquivo, tomando cuidado para evitar
//chaves duplicadas (verifique quais �ndices precisam ser atualizados).
// Fun��o para inserir um novo registro no arquivo, evitando chaves duplicadas e atualizando �ndices secund�rios
bool inserirRegistro(FILE *arquivo, TabelaIndice *tabelaIndicePrimario, TabelaIndice *tabelaIndiceCurso, TabelaIndice *tabelaIndiceEstado, REGISTRO r) {
    // Verifica se a chave prim�ria j� existe na tabela de �ndices prim�rios
    if (buscarEndere�o(tabelaIndicePrimario, r.NroUSP) != -1) {
        printf("Chave prim�ria j� existe.\n");
        return false;
    }
    // Move o cursor para o final do arquivo
    fseek(arquivo, 0, SEEK_END);
    // Escreve o registro no arquivo
    fwrite(&r, sizeof(REGISTRO), 1, arquivo);
    // Obt�m a posi��o do registro rec�m-inserido
    long posicao = ftell(arquivo) - sizeof(REGISTRO);
    // Insere a chave prim�ria na tabela de �ndices prim�rios
    if (!inserir�ndice(tabelaIndicePrimario, r.NroUSP, posicao)) {
        // Se falhar ao inserir no �ndice prim�rio, remove o registro do arquivo
        fseek(arquivo, posicao, SEEK_SET);
        REGISTRO vazio = {0};
        fwrite(&vazio, sizeof(REGISTRO), 1, arquivo);
        return false;
    }

    // Insere a chave prim�ria na tabela de �ndices secund�rios (curso)
    if (!inserir�ndice(tabelaIndiceCurso, r.curso, r.NroUSP)) {
        // Se falhar ao inserir no �ndice secund�rio (curso), remove o registro do arquivo e do �ndice prim�rio
        fseek(arquivo, posicao, SEEK_SET);
        REGISTRO vazio = {0};
        fwrite(&vazio, sizeof(REGISTRO), 1, arquivo);
        excluir�ndice(tabelaIndicePrimario, r.NroUSP);
        return false;
    }

    // Insere a chave prim�ria na tabela de �ndices secund�rios (estado)
    if (!inserir�ndice(tabelaIndiceEstado, r.estado, r.NroUSP)) {
        // Se falhar ao inserir no �ndice secund�rio (estado), remove o registro do arquivo e dos �ndices prim�rio e secund�rio (curso)
        fseek(arquivo, posicao, SEEK_SET);
        REGISTRO vazio = {0};
        fwrite(&vazio, sizeof(REGISTRO), 1, arquivo);
        excluir�ndice(tabelaIndicePrimario, r.NroUSP);
        excluir�ndice(tabelaIndiceCurso, r.curso, r.NroUSP);
        return false;
    }
    return true;
}

//2. Escreva uma fun��o para exibir todos os registros do curso X.
// Fun��o para exibir todos os registros do curso X
void exibirRegistrosDoCurso(FILE *arquivo, TabelaIndice *tabelaIndiceCurso, int cursoX) {
    NO *chaves = chavesCurso(cursoX); // Obt�m a lista ligada das chaves prim�rias do curso X

    printf("Registros do curso %d:\n", cursoX);
    // Percorre a lista ligada de chaves prim�rias
    NO *atual = chaves;
    while (atual != NULL) {
        // Obt�m a posi��o do registro no arquivo a partir do �ndice prim�rio
        long posicao = buscarEndere�o(tabelaIndicePrimario, atual->chavePrimaria);
        // Move o cursor para a posi��o do registro no arquivo
        fseek(arquivo, posicao, SEEK_SET);
        // L� o registro do arquivo
        REGISTRO registro;
        fread(&registro, sizeof(REGISTRO), 1, arquivo);
        // Exibe os dados do registro
        printf("NroUSP: %d, Curso: %d, Estado: %d, Idade: %d\n", registro.NroUSP, registro.curso, registro.estado, registro.idade);
        // Avan�a para o pr�ximo n� na lista
        atual = atual->prox;
    }
}

//3. Escreva uma fun��o para excluir o registro de nroUSP X (verifique quais �ndices precisam ser
//atualizados).
bool excluirRegistro(FILE *arquivo, TabelaIndice *tabelaIndicePrimario, TabelaIndice *tabelaIndiceCurso, TabelaIndice *tabelaIndiceEstado, int nroUSP_X) {
    // Busca o endere�o do registro na tabela de �ndices prim�rios
    int endereco = buscarEndere�o(tabelaIndicePrimario, nroUSP_X);
    if (endereco == -1) {
        printf("Registro com NroUSP %d n�o encontrado.\n", nroUSP_X);
        return false;
    }
    // Move o cursor para a posi��o do registro no arquivo
    fseek(arquivo, tabelaIndicePrimario->indices[endereco].posicao, SEEK_SET);
    // L� o registro do arquivo
    REGISTRO registro;
    fread(&registro, sizeof(REGISTRO), 1, arquivo);
    // Marca o registro como inv�lido (exclu�do logicamente)
    registro.valido = false;
    // Move o cursor de volta para a posi��o do registro no arquivo
    fseek(arquivo, tabelaIndicePrimario->indices[endereco].posicao, SEEK_SET);
    // Escreve o registro de volta no arquivo com o campo 'valido' atualizado
    fwrite(&registro, sizeof(REGISTRO), 1, arquivo);
    // Remove a chave prim�ria da tabela de �ndices prim�rios
    excluir�ndice(tabelaIndicePrimario, nroUSP_X);
    // Remove a chave prim�ria da tabela de �ndices secund�rios (curso)
    excluir�ndice(tabelaIndiceCurso, registro.curso, nroUSP_X);
    // Remove a chave prim�ria da tabela de �ndices secund�rios (estado)
    excluir�ndice(tabelaIndiceEstado, registro.estado, nroUSP_X);

    return true;
}

//4. Escreva uma fun��o para excluir todos os registros do curso X (verifique quais �ndices precisam ser
//atualizados).
void excluirRegistrosDoCurso(FILE *arquivo, TabelaIndice *tabelaIndicePrimario, TabelaIndice *tabelaIndiceCurso, TabelaIndice *tabelaIndiceEstado, int cursoX) {
    NO *chaves = chavesCurso(cursoX); // Obt�m a lista ligada das chaves prim�rias do curso X

    // Percorre a lista ligada de chaves prim�rias
    NO *atual = chaves;
    while (atual != NULL) {
        // Obt�m a posi��o do registro no arquivo a partir do �ndice prim�rio
        long posicao = buscarEndere�o(tabelaIndicePrimario, atual->chavePrimaria);
        // Move o cursor para a posi��o do registro no arquivo
        fseek(arquivo, posicao, SEEK_SET);
        // L� o registro do arquivo
        REGISTRO registro;
        fread(&registro, sizeof(REGISTRO), 1, arquivo);
        // Marca o registro como inv�lido (exclu�do logicamente)
        registro.valido = false;
        // Move o cursor de volta para a posi��o do registro no arquivo
        fseek(arquivo, posicao, SEEK_SET);
        // Escreve o registro de volta no arquivo com o campo 'valido' atualizado
        fwrite(&registro, sizeof(REGISTRO), 1, arquivo);
        // Remove a chave prim�ria da tabela de �ndices prim�rios
        excluir�ndice(tabelaIndicePrimario, atual->chavePrimaria);
        // Remove a chave prim�ria da tabela de �ndices secund�rios (curso)
        excluir�ndice(tabelaIndiceCurso, registro.curso, atual->chavePrimaria);
        // Remove a chave prim�ria da tabela de �ndices secund�rios (estado)
        excluir�ndice(tabelaIndiceEstado, registro.estado, atual->chavePrimaria);
        // Avan�a para o pr�ximo n� na lista
        atual = atual->prox;
    }
}

//5. Escreva uma fun��o para alterar a idade atual de um registro de nroUSP X para a idade Y (verifique
//quais �ndices precisam ser atualizados).
bool alterarIdadeRegistro(FILE *arquivo, TabelaIndice *tabelaIndicePrimario, int nroUSP_X, int novaIdade_Y) {
    // Busca o endere�o do registro na tabela de �ndices prim�rios
    int endereco = buscarEndere�o(tabelaIndicePrimario, nroUSP_X);
    if (endereco == -1) {
        printf("Registro com NroUSP %d n�o encontrado.\n", nroUSP_X);
        return false;
    }
    // Move o cursor para a posi��o do registro no arquivo
    fseek(arquivo, tabelaIndicePrimario->indices[endereco].posicao, SEEK_SET);
    // L� o registro do arquivo
    REGISTRO registro;
    fread(&registro, sizeof(REGISTRO), 1, arquivo);
    // Altera a idade do registro para Y
    registro.idade = novaIdade_Y;
    // Move o cursor de volta para a posi��o do registro no arquivo
    fseek(arquivo, tabelaIndicePrimario->indices[endereco].posicao, SEEK_SET);
    // Escreve o registro de volta no arquivo com o campo 'idade' atualizado
    fwrite(&registro, sizeof(REGISTRO), 1, arquivo);
    return true;
}

//6. Escreva uma fun��o para alterar o curso de um registro de nroUSP X para o curso Y (verifique quais
//�ndices precisam ser atualizados).
bool alterarCursoRegistro(FILE *arquivo, TabelaIndice *tabelaIndicePrimario, TabelaIndice *tabelaIndiceCurso, int nroUSP_X, int novoCurso_Y) {
    // Busca o endere�o do registro na tabela de �ndices prim�rios
    int endereco = buscarEndere�o(tabelaIndicePrimario, nroUSP_X);
    if (endereco == -1) {
        printf("Registro com NroUSP %d n�o encontrado.\n", nroUSP_X);
        return false;
    }
    // Move o cursor para a posi��o do registro no arquivo
    fseek(arquivo, tabelaIndicePrimario->indices[endereco].posicao, SEEK_SET);
    // L� o registro do arquivo
    REGISTRO registro;
    fread(&registro, sizeof(REGISTRO), 1, arquivo);
    // Altera o curso do registro para Y
    registro.curso = novoCurso_Y;
    // Move o cursor de volta para a posi��o do registro no arquivo
    fseek(arquivo, tabelaIndicePrimario->indices[endereco].posicao, SEEK_SET);
    // Escreve o registro de volta no arquivo com o campo 'curso' atualizado
    fwrite(&registro, sizeof(REGISTRO), 1, arquivo);
    // Atualiza o �ndice correspondente na tabela de �ndices secund�rios (curso)
    excluir�ndice(tabelaIndiceCurso, registro.curso, nroUSP_X); // Exclui o �ndice antigo
    inserir�ndice(tabelaIndiceCurso, novoCurso_Y, nroUSP_X); // Insere o novo �ndice

    return true;
}

//7. Escreva uma fun��o para alterar o registro de nroUSP X para o nroUSP Y se poss�vel (verifique quais
//�ndices precisam ser atualizados).
bool numeroUSPExiste(TabelaIndice tabelaIndice, int nroUSP) {
    for (int i = 0; i < tabelaIndice.tamanho; i++) {
        if (tabelaIndice.indices[i].chave == nroUSP) {
            return true;
        }
    }
    return false;
}

// Fun��o para alterar o registro de nroUSP X para o nroUSP Y, se poss�vel
bool alterarNroUSPRegistro(FILE *arquivo, TabelaIndice *tabelaIndicePrimario, int nroUSP_X, int nroUSP_Y) {
    // Verifica se o n�mero USP Y j� existe na tabela de �ndices prim�rios
    if (numeroUSPExiste(*tabelaIndicePrimario, nroUSP_Y)) {
        printf("J� existe um registro com o n�mero USP %d.\n", nroUSP_Y);
        return false;
    }
    // Busca o endere�o do registro com o n�mero USP X na tabela de �ndices prim�rios
    int endereco = buscarEndere�o(tabelaIndicePrimario, nroUSP_X);
    if (endereco == -1) {
        printf("Registro com NroUSP %d n�o encontrado.\n", nroUSP_X);
        return false;
    }

    // Move o cursor para a posi��o do registro no arquivo
    fseek(arquivo, tabelaIndicePrimario->indices[endereco].posicao, SEEK_SET);
    // L� o registro do arquivo
    REGISTRO registro;
    fread(&registro, sizeof(REGISTRO), 1, arquivo);
    // Altera o n�mero USP do registro para Y
    registro.NroUSP = nroUSP_Y;
    // Move o cursor de volta para a posi��o do registro no arquivo
    fseek(arquivo, tabelaIndicePrimario->indices[endereco].posicao, SEEK_SET);
    // Escreve o registro de volta no arquivo com o campo 'NroUSP' atualizado
    fwrite(&registro, sizeof(REGISTRO), 1, arquivo);
    // Atualiza o �ndice correspondente na tabela de �ndices prim�rios
    tabelaIndicePrimario->indices[endereco].chave = nroUSP_Y;
    return true;
}

int main() {
    return 0;

}
