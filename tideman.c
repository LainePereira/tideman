#include "cs50.h"
#include "stdio.h"
#include "string.h"

#define MAX_CANDIDATES 9

typedef struct
{
    string nome;
    int votos;
    bool eliminado;
} Candidato;

typedef struct
{
    string primeiro;
    string segundo;
    string terceiro;
} VotoClassificado;

int encontrarIndiceCandidato(Candidato candidatos[], int numCandidatos, string nome);
void processarVotos(Candidato candidatos[], int numCandidatos, VotoClassificado votos[], int numEleitores);
bool todosEliminados(Candidato candidatos[], int numCandidatos);
void eliminarCandidato(Candidato candidatos[], int numCandidatos, int indice);

int main(int argc, string argv[])
{
    if (argc < 2)
    {
        printf("Uso: %s candidato1 candidato2 candidato3 ... candidatoN\n", argv[0]);
        return 1;
    }

    int numCandidatos = argc - 1;

    if (numCandidatos > MAX_CANDIDATES)
    {
        printf("Número máximo de candidatos é %d\n", MAX_CANDIDATES);
        return 2;
    }

    Candidato candidatos[MAX_CANDIDATES];

    for (int i = 0; i < numCandidatos; i++)
    {
        candidatos[i].nome = argv[i + 1];
        candidatos[i].votos = 0;
        candidatos[i].eliminado = false;
    }

    int numEleitores;

    do
    {
        numEleitores = get_int("Número de eleitores: ");
    } while (numEleitores < 1);

    VotoClassificado votos[numEleitores];
    for (int i = 0; i < numEleitores; i++)
    {
        printf("\nVoto %d:\n", i + 1);
        printf("Rank 1: ");
        votos[i].primeiro = get_string(NULL);
        printf("Rank 2: ");
        votos[i].segundo = get_string(NULL);
        printf("Rank 3: ");
        votos[i].terceiro = get_string(NULL);
    }

    processarVotos(candidatos, numCandidatos, votos, numEleitores);

    // Encontrar e imprimir o vencedor
    for (int i = 0; i < numCandidatos; i++)
    {
        if (!candidatos[i].eliminado)
        {
            printf("%s\n", candidatos[i].nome);
            break;
        }
    }

    return 0;
}

int encontrarIndiceCandidato(Candidato candidatos[], int numCandidatos, string nome)
{
    for (int i = 0; i < numCandidatos; i++)
    {
        if (strcmp(candidatos[i].nome, nome) == 0)
        {
            return i;
        }
    }
    return -1;
}

void processarVotos(Candidato candidatos[], int numCandidatos, VotoClassificado votos[], int numEleitores)
{
    while (!todosEliminados(candidatos, numCandidatos))
    {
        for (int i = 0; i < numEleitores; i++)
        {
            int indice1 = encontrarIndiceCandidato(candidatos, numCandidatos, votos[i].primeiro);
            int indice2 = encontrarIndiceCandidato(candidatos, numCandidatos, votos[i].segundo);
            int indice3 = encontrarIndiceCandidato(candidatos, numCandidatos, votos[i].terceiro);

            // Verificar se o candidato no Rank 1 não está eliminado
            if (!candidatos[indice1].eliminado)
            {
                candidatos[indice1].votos++;
            }
            // Se o candidato no Rank 1 estiver eliminado, verificar o Rank 2
            else if (!candidatos[indice2].eliminado)
            {
                candidatos[indice2].votos++;
            }
            // Se os candidatos no Rank 1 e Rank 2 estiverem eliminados, verificar o Rank 3
            else if (!candidatos[indice3].eliminado)
            {
                candidatos[indice3].votos++;
            }
        }

        int menorVotos = numEleitores + 1;

        // Encontrar o candidato com o menor número de votos
        for (int i = 0; i < numCandidatos; i++)
        {
            if (!candidatos[i].eliminado && candidatos[i].votos < menorVotos)
            {
                menorVotos = candidatos[i].votos;
            }
        }

        // Eliminar todos os candidatos com o menor número de votos
        for (int i = 0; i < numCandidatos; i++)
        {
            if (candidatos[i].votos == menorVotos)
            {
                eliminarCandidato(candidatos, numCandidatos, i);
            }
        }
    }
}

bool todosEliminados(Candidato candidatos[], int numCandidatos)
{
    for (int i = 0; i < numCandidatos; i++)
    {
        if (!candidatos[i].eliminado)
        {
            return false;
        }
    }
    return true;
}

void eliminarCandidato(Candidato candidatos[], int numCandidatos, int indice)
{
    candidatos[indice].eliminado = true;
}
