#include "PSOgrafico.h"

int main(int argc, char *argv[])
{
    float w, c1, c2;
    int limitePlano, n, tempoDelay;

    w = 0.95;
    c1 = 0.5;//cognitivo
    c2 = 0.4;//social
    tempoDelay = 10;
    limitePlano = 70;
    n = 200;

    printf("Informe o numero de particulas: ");
    scanf("%d", &n);

    pso(w, c1, c2, n, limitePlano, tempoDelay);

    return 0;
}


