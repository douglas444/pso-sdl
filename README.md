# pso-sdl

Particle Swarm Optimization Graphical Simulation.
The code is in portuguese.

## Requirements
* gcc
* SDL2

How to install SDL2:

http://lazyfoo.net/tutorials/SDL/01_hello_SDL/linux/ 

If the link is broken, try this other one:

https://web.archive.org/web/20220706232201/http://lazyfoo.net/tutorials/SDL/01_hello_SDL/linux/

## Compilation

To compile, execute each one of the following commands from the root of the project:

```
gcc -Wall -O2  -c ./main.c -o ./main.o
```
```
gcc -Wall -O2  -c ./PSOgrafico.c -o ./PSOgrafico.o
```
```
gcc  -o ./PSOgrafico ./main.o ./PSOgrafico.o  -lSDL2 -lm -s  
```

## How to run

```
./PSOgrafico
```

The program will ask for the numbers of particles you want. I think 2000 is a good start, as it's interesting enough but don't consume a lot of processing power.
