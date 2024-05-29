#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <stdbool.h>

typedef struct {
int NroUSP; // chave primária
int curso;
int estado;
int idade;
bool valido; // para exclusão lógica
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

// I-Exercícios em arquivos simples (sem índices)


//1. Reescreva um arquivo arq1 em um novo arquivo arq2 eliminado os registros inválidos.
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

    // Lendo registros de arq1 e escrevendo em arq2 apenas os registros válidos
    while (fread(&reg, sizeof(REGISTRO), 1, arq1)) {
        if (reg.valido) {
            fwrite(&reg, sizeof(REGISTRO), 1, arq2);
        }
    }
    // Fechando arquivos
    fclose(arq1);
    fclose(arq2);

    printf("Registros válidos copiados para arq2 com sucesso.\n");
}

//2. Faça uma cópia invertida de arq1 em um novo arquivo arq2, ou seja: copie o último registro (n) de
//arq1 para o início de arq2, depois copie o registro n-1 para a segunda posição etc.
void copiarInvertido(FILE *arq1, FILE *arq2, int numRegistros) {
    REGISTRO reg;

    // Itera sobre os registros em arq1, começando do último
    for (int i = numRegistros - 1; i >= 0; i--) {
        // Move o ponteiro para a posição correta no arquivo arq1
        fseek(arq1, i * sizeof(REGISTRO), SEEK_SET);
        // Lê o registro atual
        fread(&reg, sizeof(REGISTRO), 1, arq1);
        // Escreve o registro lido em arq2
        fwrite(&reg, sizeof(REGISTRO), 1, arq2);
    }
}
//3. Escreva uma função que, dada um nroUSP X, retorne o registro correspondente.
REGISTRO buscarRegistro(FILE *arquivo, int numRegistros, int numUSP) {
    REGISTRO reg;

    // Itera sobre os registros no arquivo
    for (int i = 0; i < numRegistros; i++) {
        // Move o ponteiro para a posição correta no arquivo
        fseek(arquivo, i * sizeof(REGISTRO), SEEK_SET);
        // Lê o registro atual
        fread(&reg, sizeof(REGISTRO), 1, arquivo);
        // Verifica se o número USP corresponde ao número do registro
        if (reg.NroUSP == numUSP) {
            // Retorna o registro correspondente
            return reg;
        }
    }

    // Se o registro não for encontrado, retorna um registro vazio (ou um valor de retorno indicativo)
    REGISTRO registroNaoEncontrado = { .NroUSP = -1 }; // Supondo que -1 seja um número USP inválido
    return registroNaoEncontrado;
}

//4. Escreva uma função para inserir um novo registro r no arquivo, tomando cuidado para evitar
//chaves duplicadas.
// Função para verificar se já existe um registro com a mesma chave primária
bool verificarDuplicata(FILE *arquivo, int numRegistros, int numUSP) {
    REGISTRO reg;

    // Itera sobre os registros no arquivo
    for (int i = 0; i < numRegistros; i++) {
        // Move o ponteiro para a posição correta no arquivo
        fseek(arquivo, i * sizeof(REGISTRO), SEEK_SET);
        // Lê o registro atual
        fread(&reg, sizeof(REGISTRO), 1, arquivo);
        // Verifica se a chave primária já existe
        if (reg.NroUSP == numUSP) {
            // Se existir, retorna true
            return true;
        }
    }

    // Se não existir, retorna false
    return false;
}

// Função para inserir um novo registro no arquivo
void inserirRegistro(FILE *arquivo, REGISTRO novoRegistro, int *numRegistros) {
    // Verifica se já existe um registro com a mesma chave primária
    if (verificarDuplicata(arquivo, *numRegistros, novoRegistro.NroUSP)) {
        printf("Erro: Já existe um registro com a mesma chave primária.\n");
        return;
    }

    // Posiciona o ponteiro do arquivo no final
    fseek(arquivo, 0, SEEK_END);
    // Escreve o novo registro no final do arquivo
    fwrite(&novoRegistro, sizeof(REGISTRO), 1, arquivo);
    // Atualiza o número total de registros
    (*numRegistros)++;
}

//5. Escreva uma função para excluir todos os registros do curso X.
// Função para excluir todos os registros de um curso específico
void excluirRegistrosDoCurso(FILE *arqEntrada, FILE *arqSaida, int curso) {
    REGISTRO reg;

    // Itera sobre todos os registros no arquivo de entrada
    while (fread(&reg, sizeof(REGISTRO), 1, arqEntrada)) {
        // Verifica se o registro pertence ao curso a ser excluído
        if (reg.curso != curso) {
            // Se não pertencer, escreve o registro no arquivo de saída
            fwrite(&reg, sizeof(REGISTRO), 1, arqSaida);
        }
    }
}

//6. Escreva uma função para alterar o curso de um aluno de nroUSP X para o curso Y.
void alterarCursoPorNroUSP(FILE *arquivo, int numUSP, int novoCurso) {
    REGISTRO reg;
    bool encontrado = false;

    // Posiciona o ponteiro do arquivo no início
    rewind(arquivo);

    // Itera sobre todos os registros no arquivo
    while (fread(&reg, sizeof(REGISTRO), 1, arquivo)) {
        // Verifica se o registro corresponde ao número USP especificado
        if (reg.NroUSP == numUSP) {
            // Atualiza o campo de curso do registro
            reg.curso = novoCurso;
            // Move o ponteiro de arquivo para a posição atual menos o tamanho de um registro
            fseek(arquivo, -sizeof(REGISTRO), SEEK_CUR);
            // Escreve o registro atualizado de volta no arquivo
            fwrite(&reg, sizeof(REGISTRO), 1, arquivo);
            encontrado = true;
            break;
        }
    }

    // Verifica se o número USP especificado foi encontrado no arquivo
    if (!encontrado) {
        printf("Registro com o número USP %d não encontrado.\n", numUSP);
    } else {
        printf("Curso do aluno com número USP %d alterado para %d.\n", numUSP, novoCurso);
    }
}

//7. Implemente o procedimento de ordenação KeySort, que dado um arquivo arq1 cria uma tabela
//temporária de chaves em memória (idêntica a uma tabela de índices primários) e então reescreve o
//arquivo em um novo arquivo de saída arq2, na ordem correta de chaves
// Função de comparação para o qsort
int compararChaves(const void *a, const void *b) {
    const INDICE *indiceA = (const INDICE *)a;
    const INDICE *indiceB = (const INDICE *)b;
    return (indiceA->chave - indiceB->chave);
}

// Procedimento de ordenação KeySort
void keySort(FILE *arqEntrada, FILE *arqSaida) {
    REGISTRO reg;
    INDICE *indices = NULL;
    long posicao = 0;
    int numRegistros = 0;

    // Determina o número total de registros no arquivo de entrada
    fseek(arqEntrada, 0, SEEK_END);
    numRegistros = ftell(arqEntrada) / sizeof(REGISTRO);
    rewind(arqEntrada);

    // Aloca memória para a tabela de índices
    indices = (INDICE *)malloc(numRegistros * sizeof(INDICE));
    if (indices == NULL) {
        printf("Erro: Falha na alocação de memória.\n");
        return;
    }

    // Lê os registros do arquivo de entrada e preenche a tabela de índices
    while (fread(&reg, sizeof(REGISTRO), 1, arqEntrada)) {
        indices[posicao].chave = reg.NroUSP;
        indices[posicao].posicao = posicao;
        posicao++;
    }

    // Ordena a tabela de índices com base nas chaves
    qsort(indices, numRegistros, sizeof(INDICE), compararChaves);

    // Escreve os registros na ordem correta no arquivo de saída
    for (int i = 0; i < numRegistros; i++) {
        fseek(arqEntrada, indices[i].posicao * sizeof(REGISTRO), SEEK_SET);
        fread(&reg, sizeof(REGISTRO), 1, arqEntrada);
        fwrite(&reg, sizeof(REGISTRO), 1, arqSaida);
    }

    // Libera a memória alocada para a tabela de índices
    free(indices);
}

// Exercícios em tabelas de índices primários
// Para os exercícios a seguir, considere os mesmos arquivos do tipo acima, e também a existência de uma
// tabela de índices primários mantida em memória e suas respectivas funções de manipulação.

// bool inserirÍndice(Tabela, int nroUSP, int end){} //inserção em tabela ordenada, retornando true/false
// int buscarEndereço(Tabela, int nroUSP){} // retorna -1 se end não existe
// int excluirÍndice(Tabela, int nroUSP){} // retorna o endereço exluído, ou -1 se não encontrarExercícios em tabelas de índices primários

//1. Escreva uma função para inserir um novo registro r no arquivo, tomando cuidado para evitar
//chaves duplicadas (use o índice).
// Função para inserir um novo registro no arquivo, evitando chaves duplicadas
bool inserirRegistroIndices(FILE *arquivo, TabelaIndice *tabelaIndice, REGISTRO r) {
    // Verifica se a chave já existe na tabela de índices
    if (buscarEndereço(tabelaIndice, r.NroUSP) != -1) {
        printf("Chave primária já existe.\n");
        return false;
    }
    // Move o cursor para o final do arquivo
    fseek(arquivo, 0, SEEK_END);
    // Escreve o registro no arquivo
    fwrite(&r, sizeof(REGISTRO), 1, arquivo);
    // Obtém a posição do registro recém-inserido
    long posicao = ftell(arquivo) - sizeof(REGISTRO);
    // Insere a chave e a posição na tabela de índices
    if (!inserirÍndice(tabelaIndice, r.NroUSP, posicao)) {
        // Se falhar ao inserir no índice, remove o registro do arquivo
        fseek(arquivo, posicao, SEEK_SET);
        REGISTRO vazio = {0};
        fwrite(&vazio, sizeof(REGISTRO), 1, arquivo);
        return false;
    }
    return true;
}

//2. Escreva uma função que, dada um nroUSP X, retorne o registro correspondente (use o índice).
REGISTRO buscarRegistro(FILE *arquivo, TabelaIndice tabelaIndice, int nroUSP) {
    // Busca o endereço do registro na tabela de índices
    int endereco = buscarEndereço(&tabelaIndice, nroUSP);
    if (endereco == -1) {
        printf("Registro com NroUSP %d não encontrado.\n", nroUSP);
        // Se o registro não for encontrado, retorna um registro vazio
        REGISTRO registro_vazio = {0};
        return registro_vazio;
    }
    // Move o cursor para a posição do registro no arquivo
    fseek(arquivo, tabelaIndice.indices[endereco].posicao, SEEK_SET);
    // Lê o registro do arquivo
    REGISTRO registro;
    fread(&registro, sizeof(REGISTRO), 1, arquivo);

    return registro;
}

//3. Escreva uma função para excluir todos os registros do curso X (use o índice).
void excluirRegistrosDoCurso(FILE *arquivo, TabelaIndice *tabelaIndice, int cursoX) {
    for (int i = 0; i < tabelaIndice->tamanho; i++) {
        // Verifica se o registro pertence ao curso X
        if (tabelaIndice->indices[i].chave != -1 && buscarRegistro(arquivo, *tabelaIndice, tabelaIndice->indices[i].chave).curso == cursoX) {
            // Marca o registro como inválido (excluído logicamente)
            REGISTRO registro = buscarRegistro(arquivo, *tabelaIndice, tabelaIndice->indices[i].chave);
            registro.valido = false;
            // Move o cursor para a posição do registro no arquivo
            fseek(arquivo, tabelaIndice->indices[i].posicao, SEEK_SET);
            // Escreve o registro de volta no arquivo com o campo 'valido' atualizado
            fwrite(&registro, sizeof(REGISTRO), 1, arquivo);
            // Remove o índice correspondente da tabela de índices
            excluirÍndice(tabelaIndice, tabelaIndice->indices[i].chave);
        }
    }
}

//4. Escreva uma função para alterar o curso de um registro de nroUSP X para o curso Y (use o índice).
bool alterarCursoRegistro(FILE *arquivo, TabelaIndice *tabelaIndice, int nroUSP_X, int novo_curso_Y) {
    // Busca o endereço do registro na tabela de índices
    int endereco = buscarEndereço(tabelaIndice, nroUSP_X);
    if (endereco == -1) {
        printf("Registro com NroUSP %d não encontrado.\n", nroUSP_X);
        return false;
    }

    // Move o cursor para a posição do registro no arquivo
    fseek(arquivo, tabelaIndice->indices[endereco].posicao, SEEK_SET);
    // Lê o registro do arquivo
    REGISTRO registro;
    fread(&registro, sizeof(REGISTRO), 1, arquivo);
    // Altera o curso do registro para Y
    registro.curso = novo_curso_Y;
    // Move o cursor de volta para a posição do registro no arquivo
    fseek(arquivo, tabelaIndice->indices[endereco].posicao, SEEK_SET);
    // Escreve o registro de volta no arquivo com o campo 'curso' atualizado
    fwrite(&registro, sizeof(REGISTRO), 1, arquivo);

    return true;
}

//5. Escreva uma função para alterar o registro de nroUSP X para o nroUSP Y se possível (use o índice).
bool numeroUSPExiste(TabelaIndice tabelaIndice, int nroUSP) {
    for (int i = 0; i < tabelaIndice.tamanho; i++) {
        if (tabelaIndice.indices[i].chave == nroUSP) {
            return true;
        }
    }
    return false;
}

// Função para alterar o registro de nroUSP X para o nroUSP Y, se possível
bool alterarNroUSPRegistro(FILE *arquivo, TabelaIndice *tabelaIndice, int nroUSP_X, int nroUSP_Y) {
    // Verifica se o número USP Y já existe na tabela de índices
    if (numeroUSPExiste(*tabelaIndice, nroUSP_Y)) {
        printf("Já existe um registro com o número USP %d.\n", nroUSP_Y);
        return false;
    }

    // Busca o endereço do registro com o número USP X na tabela de índices
    int endereco = buscarEndereço(tabelaIndice, nroUSP_X);
    if (endereco == -1) {
        printf("Registro com NroUSP %d não encontrado.\n", nroUSP_X);
        return false;
    }

    // Move o cursor para a posição do registro no arquivo
    fseek(arquivo, tabelaIndice->indices[endereco].posicao, SEEK_SET);
    // Lê o registro do arquivo
    REGISTRO registro;
    fread(&registro, sizeof(REGISTRO), 1, arquivo);
    // Altera o número USP do registro para Y
    registro.NroUSP = nroUSP_Y;
    // Move o cursor de volta para a posição do registro no arquivo
    fseek(arquivo, tabelaIndice->indices[endereco].posicao, SEEK_SET);
    // Escreve o registro de volta no arquivo com o campo 'NroUSP' atualizado
    fwrite(&registro, sizeof(REGISTRO), 1, arquivo);
    // Atualiza o índice correspondente na tabela de índices
    tabelaIndice->indices[endereco].chave = nroUSP_Y;

    return true;
}

//Nos exercícios a seguir considere ainda a existência de tabelas de índices secundários (e suas respectivas
//operações de inserção e exclusão) para os campos int curso e int estado, e duas funções adicionais que,
//dada uma chave secundária de interesse, retornam a lista ligada das chaves primárias a ela relacionadas:

//NO* chavesCurso(int curso)
//NO *chavesEstado(int estado)

//No caso dos exercícios de exclusão, certifique-se de que você entende porque a atualização de índices
//secundários (mas não do índice primário, que sempre precisa ser excluído) é opcional.

//1. Escreva uma função para inserir um novo registro r no arquivo, tomando cuidado para evitar
//chaves duplicadas (verifique quais índices precisam ser atualizados).
// Função para inserir um novo registro no arquivo, evitando chaves duplicadas e atualizando índices secundários
bool inserirRegistro(FILE *arquivo, TabelaIndice *tabelaIndicePrimario, TabelaIndice *tabelaIndiceCurso, TabelaIndice *tabelaIndiceEstado, REGISTRO r) {
    // Verifica se a chave primária já existe na tabela de índices primários
    if (buscarEndereço(tabelaIndicePrimario, r.NroUSP) != -1) {
        printf("Chave primária já existe.\n");
        return false;
    }
    // Move o cursor para o final do arquivo
    fseek(arquivo, 0, SEEK_END);
    // Escreve o registro no arquivo
    fwrite(&r, sizeof(REGISTRO), 1, arquivo);
    // Obtém a posição do registro recém-inserido
    long posicao = ftell(arquivo) - sizeof(REGISTRO);
    // Insere a chave primária na tabela de índices primários
    if (!inserirÍndice(tabelaIndicePrimario, r.NroUSP, posicao)) {
        // Se falhar ao inserir no índice primário, remove o registro do arquivo
        fseek(arquivo, posicao, SEEK_SET);
        REGISTRO vazio = {0};
        fwrite(&vazio, sizeof(REGISTRO), 1, arquivo);
        return false;
    }

    // Insere a chave primária na tabela de índices secundários (curso)
    if (!inserirÍndice(tabelaIndiceCurso, r.curso, r.NroUSP)) {
        // Se falhar ao inserir no índice secundário (curso), remove o registro do arquivo e do índice primário
        fseek(arquivo, posicao, SEEK_SET);
        REGISTRO vazio = {0};
        fwrite(&vazio, sizeof(REGISTRO), 1, arquivo);
        excluirÍndice(tabelaIndicePrimario, r.NroUSP);
        return false;
    }

    // Insere a chave primária na tabela de índices secundários (estado)
    if (!inserirÍndice(tabelaIndiceEstado, r.estado, r.NroUSP)) {
        // Se falhar ao inserir no índice secundário (estado), remove o registro do arquivo e dos índices primário e secundário (curso)
        fseek(arquivo, posicao, SEEK_SET);
        REGISTRO vazio = {0};
        fwrite(&vazio, sizeof(REGISTRO), 1, arquivo);
        excluirÍndice(tabelaIndicePrimario, r.NroUSP);
        excluirÍndice(tabelaIndiceCurso, r.curso, r.NroUSP);
        return false;
    }
    return true;
}

//2. Escreva uma função para exibir todos os registros do curso X.
// Função para exibir todos os registros do curso X
void exibirRegistrosDoCurso(FILE *arquivo, TabelaIndice *tabelaIndiceCurso, int cursoX) {
    NO *chaves = chavesCurso(cursoX); // Obtém a lista ligada das chaves primárias do curso X

    printf("Registros do curso %d:\n", cursoX);
    // Percorre a lista ligada de chaves primárias
    NO *atual = chaves;
    while (atual != NULL) {
        // Obtém a posição do registro no arquivo a partir do índice primário
        long posicao = buscarEndereço(tabelaIndicePrimario, atual->chavePrimaria);
        // Move o cursor para a posição do registro no arquivo
        fseek(arquivo, posicao, SEEK_SET);
        // Lê o registro do arquivo
        REGISTRO registro;
        fread(&registro, sizeof(REGISTRO), 1, arquivo);
        // Exibe os dados do registro
        printf("NroUSP: %d, Curso: %d, Estado: %d, Idade: %d\n", registro.NroUSP, registro.curso, registro.estado, registro.idade);
        // Avança para o próximo nó na lista
        atual = atual->prox;
    }
}

//3. Escreva uma função para excluir o registro de nroUSP X (verifique quais índices precisam ser
//atualizados).
bool excluirRegistro(FILE *arquivo, TabelaIndice *tabelaIndicePrimario, TabelaIndice *tabelaIndiceCurso, TabelaIndice *tabelaIndiceEstado, int nroUSP_X) {
    // Busca o endereço do registro na tabela de índices primários
    int endereco = buscarEndereço(tabelaIndicePrimario, nroUSP_X);
    if (endereco == -1) {
        printf("Registro com NroUSP %d não encontrado.\n", nroUSP_X);
        return false;
    }
    // Move o cursor para a posição do registro no arquivo
    fseek(arquivo, tabelaIndicePrimario->indices[endereco].posicao, SEEK_SET);
    // Lê o registro do arquivo
    REGISTRO registro;
    fread(&registro, sizeof(REGISTRO), 1, arquivo);
    // Marca o registro como inválido (excluído logicamente)
    registro.valido = false;
    // Move o cursor de volta para a posição do registro no arquivo
    fseek(arquivo, tabelaIndicePrimario->indices[endereco].posicao, SEEK_SET);
    // Escreve o registro de volta no arquivo com o campo 'valido' atualizado
    fwrite(&registro, sizeof(REGISTRO), 1, arquivo);
    // Remove a chave primária da tabela de índices primários
    excluirÍndice(tabelaIndicePrimario, nroUSP_X);
    // Remove a chave primária da tabela de índices secundários (curso)
    excluirÍndice(tabelaIndiceCurso, registro.curso, nroUSP_X);
    // Remove a chave primária da tabela de índices secundários (estado)
    excluirÍndice(tabelaIndiceEstado, registro.estado, nroUSP_X);

    return true;
}

//4. Escreva uma função para excluir todos os registros do curso X (verifique quais índices precisam ser
//atualizados).
void excluirRegistrosDoCurso(FILE *arquivo, TabelaIndice *tabelaIndicePrimario, TabelaIndice *tabelaIndiceCurso, TabelaIndice *tabelaIndiceEstado, int cursoX) {
    NO *chaves = chavesCurso(cursoX); // Obtém a lista ligada das chaves primárias do curso X

    // Percorre a lista ligada de chaves primárias
    NO *atual = chaves;
    while (atual != NULL) {
        // Obtém a posição do registro no arquivo a partir do índice primário
        long posicao = buscarEndereço(tabelaIndicePrimario, atual->chavePrimaria);
        // Move o cursor para a posição do registro no arquivo
        fseek(arquivo, posicao, SEEK_SET);
        // Lê o registro do arquivo
        REGISTRO registro;
        fread(&registro, sizeof(REGISTRO), 1, arquivo);
        // Marca o registro como inválido (excluído logicamente)
        registro.valido = false;
        // Move o cursor de volta para a posição do registro no arquivo
        fseek(arquivo, posicao, SEEK_SET);
        // Escreve o registro de volta no arquivo com o campo 'valido' atualizado
        fwrite(&registro, sizeof(REGISTRO), 1, arquivo);
        // Remove a chave primária da tabela de índices primários
        excluirÍndice(tabelaIndicePrimario, atual->chavePrimaria);
        // Remove a chave primária da tabela de índices secundários (curso)
        excluirÍndice(tabelaIndiceCurso, registro.curso, atual->chavePrimaria);
        // Remove a chave primária da tabela de índices secundários (estado)
        excluirÍndice(tabelaIndiceEstado, registro.estado, atual->chavePrimaria);
        // Avança para o próximo nó na lista
        atual = atual->prox;
    }
}

//5. Escreva uma função para alterar a idade atual de um registro de nroUSP X para a idade Y (verifique
//quais índices precisam ser atualizados).
bool alterarIdadeRegistro(FILE *arquivo, TabelaIndice *tabelaIndicePrimario, int nroUSP_X, int novaIdade_Y) {
    // Busca o endereço do registro na tabela de índices primários
    int endereco = buscarEndereço(tabelaIndicePrimario, nroUSP_X);
    if (endereco == -1) {
        printf("Registro com NroUSP %d não encontrado.\n", nroUSP_X);
        return false;
    }
    // Move o cursor para a posição do registro no arquivo
    fseek(arquivo, tabelaIndicePrimario->indices[endereco].posicao, SEEK_SET);
    // Lê o registro do arquivo
    REGISTRO registro;
    fread(&registro, sizeof(REGISTRO), 1, arquivo);
    // Altera a idade do registro para Y
    registro.idade = novaIdade_Y;
    // Move o cursor de volta para a posição do registro no arquivo
    fseek(arquivo, tabelaIndicePrimario->indices[endereco].posicao, SEEK_SET);
    // Escreve o registro de volta no arquivo com o campo 'idade' atualizado
    fwrite(&registro, sizeof(REGISTRO), 1, arquivo);
    return true;
}

//6. Escreva uma função para alterar o curso de um registro de nroUSP X para o curso Y (verifique quais
//índices precisam ser atualizados).
bool alterarCursoRegistro(FILE *arquivo, TabelaIndice *tabelaIndicePrimario, TabelaIndice *tabelaIndiceCurso, int nroUSP_X, int novoCurso_Y) {
    // Busca o endereço do registro na tabela de índices primários
    int endereco = buscarEndereço(tabelaIndicePrimario, nroUSP_X);
    if (endereco == -1) {
        printf("Registro com NroUSP %d não encontrado.\n", nroUSP_X);
        return false;
    }
    // Move o cursor para a posição do registro no arquivo
    fseek(arquivo, tabelaIndicePrimario->indices[endereco].posicao, SEEK_SET);
    // Lê o registro do arquivo
    REGISTRO registro;
    fread(&registro, sizeof(REGISTRO), 1, arquivo);
    // Altera o curso do registro para Y
    registro.curso = novoCurso_Y;
    // Move o cursor de volta para a posição do registro no arquivo
    fseek(arquivo, tabelaIndicePrimario->indices[endereco].posicao, SEEK_SET);
    // Escreve o registro de volta no arquivo com o campo 'curso' atualizado
    fwrite(&registro, sizeof(REGISTRO), 1, arquivo);
    // Atualiza o índice correspondente na tabela de índices secundários (curso)
    excluirÍndice(tabelaIndiceCurso, registro.curso, nroUSP_X); // Exclui o índice antigo
    inserirÍndice(tabelaIndiceCurso, novoCurso_Y, nroUSP_X); // Insere o novo índice

    return true;
}

//7. Escreva uma função para alterar o registro de nroUSP X para o nroUSP Y se possível (verifique quais
//índices precisam ser atualizados).
bool numeroUSPExiste(TabelaIndice tabelaIndice, int nroUSP) {
    for (int i = 0; i < tabelaIndice.tamanho; i++) {
        if (tabelaIndice.indices[i].chave == nroUSP) {
            return true;
        }
    }
    return false;
}

// Função para alterar o registro de nroUSP X para o nroUSP Y, se possível
bool alterarNroUSPRegistro(FILE *arquivo, TabelaIndice *tabelaIndicePrimario, int nroUSP_X, int nroUSP_Y) {
    // Verifica se o número USP Y já existe na tabela de índices primários
    if (numeroUSPExiste(*tabelaIndicePrimario, nroUSP_Y)) {
        printf("Já existe um registro com o número USP %d.\n", nroUSP_Y);
        return false;
    }
    // Busca o endereço do registro com o número USP X na tabela de índices primários
    int endereco = buscarEndereço(tabelaIndicePrimario, nroUSP_X);
    if (endereco == -1) {
        printf("Registro com NroUSP %d não encontrado.\n", nroUSP_X);
        return false;
    }

    // Move o cursor para a posição do registro no arquivo
    fseek(arquivo, tabelaIndicePrimario->indices[endereco].posicao, SEEK_SET);
    // Lê o registro do arquivo
    REGISTRO registro;
    fread(&registro, sizeof(REGISTRO), 1, arquivo);
    // Altera o número USP do registro para Y
    registro.NroUSP = nroUSP_Y;
    // Move o cursor de volta para a posição do registro no arquivo
    fseek(arquivo, tabelaIndicePrimario->indices[endereco].posicao, SEEK_SET);
    // Escreve o registro de volta no arquivo com o campo 'NroUSP' atualizado
    fwrite(&registro, sizeof(REGISTRO), 1, arquivo);
    // Atualiza o índice correspondente na tabela de índices primários
    tabelaIndicePrimario->indices[endereco].chave = nroUSP_Y;
    return true;
}

int main() {
    return 0;

}
