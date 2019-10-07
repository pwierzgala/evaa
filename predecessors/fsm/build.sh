#!/bin/bash
gcc -Wall -Wextra -pedantic -g -c fsm.c main.c
gcc -o fsm fsm.o main.o
