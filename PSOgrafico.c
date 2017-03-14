#include "PSOgrafico.h"
#include <math.h>
#include <time.h>
#include <SDL2/SDL.h>




struct ponto
{
    float x, y, fitness;
};
typedef struct ponto Ponto;

struct particula
{
    Ponto posicao, pb, velocidade;
};
typedef struct particula Particula;



void inicializaSDL(SDL_Window **w, SDL_Renderer **r, int tamJanela);

void finalizaSDL(SDL_Window **w, SDL_Renderer **r);

void renderizaJanela(SDL_Renderer **r, Ponto alvo,
        Particula *particula, int n, Ponto gb, int tempoDelay);

float fitness(Ponto p, Ponto alvo);

void inicializaPopulacao(Ponto *alvo, int limitePlano,
        Particula *particula, int n, Ponto *gb, float c1, float c2);

int trataEventos(SDL_Event event, int *quit, Ponto *alvo, Particula *particula, int n, Ponto *gb);



void pso(float w, float c1, float c2, int n, int limitePlano, int tempoDelay)
{
    srand((unsigned)time(NULL));

    SDL_Window *window;
    SDL_Renderer *render;
    SDL_Event event;

    int i, quit;
    Ponto gb, alvo;
    Particula *particula;
    float r1, r2;

    quit = 0;
    inicializaSDL(&window, &render, limitePlano);
    particula = (Particula*) malloc(n * sizeof(Particula));
    inicializaPopulacao(&alvo, limitePlano, particula, n, &gb, c1, c2);


    while(quit != 1)//Variável de controle sobre fechamento da janela SDL
    {
        renderizaJanela(&render, alvo, particula, n, gb, tempoDelay);

        //PSO
        for(i = 0; i < n; ++i)
        {
            r1 = ((float) (rand()%10))/10;
            r2 = ((float) (rand()%10))/10;

            //Atualiza posição e fitness
            particula[i].posicao.x += particula[i].velocidade.x;
            particula[i].posicao.y += particula[i].velocidade.y;
            particula[i].posicao.fitness = fitness(particula[i].posicao, alvo);

            //Atualiza pb se necessário
            if(particula[i].posicao.fitness < particula[i].pb.fitness)
                particula[i].pb = particula[i].posicao;

            //Atualiza gb se necessário
            if(particula[i].pb.fitness < gb.fitness)
                gb = particula[i].pb;

            //Atualiza velocidade
            particula[i].velocidade.x = w*particula[i].velocidade.x +
                    c1*r1*(particula[i].pb.x - particula[i].posicao.x) +
                    c2*r2*(gb.x - particula[i].posicao.x);

            particula[i].velocidade.y = w*particula[i].velocidade.y +
                    c1*r1*(particula[i].pb.y - particula[i].posicao.y) +
                    c2*r2*(gb.y - particula[i].posicao.y);

            //Limita velocidade

            if(particula[i].posicao.x + particula[i].velocidade.x > limitePlano - 1)
                particula[i].velocidade.x = (limitePlano - 1) - particula[i].posicao.x;

            if(particula[i].posicao.y + particula[i].velocidade.y > limitePlano - 1)
                particula[i].velocidade.y = (limitePlano - 1) - particula[i].posicao.y;

            if(particula[i].posicao.x + particula[i].velocidade.x < 0)
                particula[i].velocidade.x = particula[i].posicao.x * (-1);

            if(particula[i].posicao.y + particula[i].velocidade.y < 0)
                particula[i].velocidade.y = particula[i].posicao.y * (-1);
        }

        quit = trataEventos(event, &quit, &alvo, particula, n, &gb);
    }

    finalizaSDL(&window, &render);
    free(particula);
}

float fitness(Ponto p, Ponto alvo)
{
    return sqrt(pow(p.x - alvo.x, 2) + pow(p.y - alvo.y, 2));
}

void inicializaPopulacao(Ponto *alvo, int limitePlano,
        Particula *particula, int n, Ponto *gb, float c1, float c2)
{

    float r1, r2;
    int i;

    alvo->x = rand()%limitePlano;
    alvo->y = rand()%limitePlano;

    gb->fitness = 99999;

    //Inicializa posição e pb de cada partícula e define o gb
    for(i = 0; i < n; ++i)
    {
        //Randomiza posição
        particula[i].posicao.x = rand()%(limitePlano - 1) + ((float) (rand()%10))/10;
        particula[i].posicao.y = rand()%(limitePlano - 1) + ((float) (rand()%10))/10;

        //Define fitness da posição atual e atualiza pb
        particula[i].posicao.fitness = fitness(particula[i].posicao, *alvo);
        particula[i].pb = particula[i].posicao;

        //Checa se pb da particula é o gb
        if(particula[i].pb.fitness < gb->fitness)
            *gb = particula[i].pb;
    }

    //Inicializa velocidade de cada partícula
    for(i = 0; i < n; ++i)
    {
        r1 = ((float) (rand()%10))/10;
        r2 = ((float) (rand()%10))/10;

        particula[i].velocidade.fitness = -1;
        particula[i].velocidade.x = c1*r1*(particula[i].pb.x - particula[i].posicao.x)
                + c2*r2*(gb->x - particula[i].posicao.x);
        particula[i].velocidade.y = c1*r1*(particula[i].pb.y - particula[i].posicao.y)
                + c2*r2*(gb->y - particula[i].posicao.y);
    }
}

void reinicializaPopulacao(Particula *particula, int n, Ponto *gb, Ponto alvo){
    int i;
    gb->fitness = 99999;

    for(i = 0; i < n; ++i)
    {
        particula[i].posicao.fitness = fitness(particula[i].posicao, alvo);
        particula[i].pb = particula[i].posicao;
    }
}

void inicializaSDL(SDL_Window **w, SDL_Renderer **r, int tamJanela){
    SDL_Init(SDL_INIT_VIDEO);
    *w = SDL_CreateWindow("Particle Swarm Optimization", 50, 50,
            tamJanela * 10, tamJanela * 10, SDL_WINDOW_SHOWN);
    *r = SDL_CreateRenderer(*w, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
}

void finalizaSDL(SDL_Window **w, SDL_Renderer **r){
    SDL_DestroyRenderer(*r);
    SDL_DestroyWindow (*w);
    SDL_Quit();
}

void renderizaJanela(SDL_Renderer **r, Ponto alvo,
        Particula *particula, int n, Ponto gb, int tempoDelay)
{
    int i, j;
    SDL_Rect rect;

    for(i = 0; i < 10; ++i)
    {
        //delay entre cada atualização da tela
        SDL_Delay(tempoDelay);

        //Limpa tela
        SDL_SetRenderDrawColor(*r, 0, 0, 0, 255);
        SDL_RenderClear(*r);

        //Desenha partículas
        SDL_SetRenderDrawColor(*r, 255, 255, 255, 255);
        for(j = 0; j < n; ++j)
        {
            rect.x = particula[j].posicao.x * 10 +  i*particula[j].velocidade.x;
            rect.y = particula[j].posicao.y * 10 +  i*particula[j].velocidade.y;
            rect.w = 10;
            rect.h = 10;
            SDL_RenderFillRect(*r, &rect);
        }

        //Desenha alvo
        SDL_SetRenderDrawColor(*r, 255, 0, 0, 255);
        rect.x = alvo.x * 10;
        rect.y = alvo.y * 10;
        rect.w = 10;
        rect.h = 10;
        SDL_RenderFillRect(*r, &rect);

        //Desenha gb
        SDL_SetRenderDrawColor(*r, 255, 255, 0, 255);
        rect.x = gb.x * 10;
        rect.y = gb.y * 10;
        rect.w = 10;
        rect.h = 10;
        SDL_RenderFillRect(*r, &rect);

        //Atualiza tela
        SDL_RenderPresent(*r);
    }
}

int trataEventos(SDL_Event event, int *quit, Ponto *alvo,
        Particula *particula, int n, Ponto *gb)
{
    while(SDL_PollEvent(&event))
    {
        if(event.type == SDL_QUIT)
            *quit = 1;
        if(event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT)
        {
            int x, y;
            SDL_GetMouseState(&x, &y);
            alvo->x = floor((float)x)/10;
            alvo->y = floor((float)y)/10;
            reinicializaPopulacao(particula, n, gb, *alvo);
        }
        if(event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE)
            return 1;
    }
    return 0;
}

