/* Projeto Pratico 2 Computacão paralela Analisador de Desempenho de Cache de CDN com OpenMP
   Grupo: 
   Enzo Ponte Gamberi RA: 10389931
   João Guilherme Messias de Oliveira Santos RA: 10426110
   
   [Utilitários de Arquivo e Tempo]:
   Este arquivo contém funções auxiliares para evitar repetição de código.
   A função mais crítica aqui é 'load_log_to_memory', que carrega todo o arquivo de log para a RAM.
   Isso é feito para isolar o teste de CPU/Memória da lentidão de leitura de disco (I/O).
*/

#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include "hash_table.h"

// Remove \n ou \r do final da string lida pelo fgets
void strip_newline(char* str) {
    size_t len = strlen(str);
    if (len > 0 && str[len-1] == '\n') str[len-1] = '\0';
    if (len > 0 && str[len-1] == '\r') str[len-1] = '\0';
}

// Carrega o manifest.txt e já insere na tabela (Fase 1 - Sequencial)
void load_manifest(HashTable* ht, const char* filename) {
    FILE* f = fopen(filename, "r");
    if (!f) { perror("Erro ao abrir manifest"); exit(1); }
    
    char buffer[1024];
    while (fgets(buffer, sizeof(buffer), f)) {
        strip_newline(buffer);
        ht_insert(ht, buffer);
    }
    fclose(f);
}

// Carrega o arquivo de log inteiro para um array de strings na memória
char** load_log_to_memory(const char* filename, long* count) {
    FILE* f = fopen(filename, "r");
    if (!f) { perror("Erro ao abrir log"); exit(1); }

    long capacity = 100000; // Capacidade inicial
    long size = 0;
    char** lines = malloc(capacity * sizeof(char*));
    
    char buffer[1024];
    while (fgets(buffer, sizeof(buffer), f)) {
        if (size >= capacity) {
            capacity *= 2; // Dobra capacidade se encher
            lines = realloc(lines, capacity * sizeof(char*));
        }
        strip_newline(buffer);
        lines[size++] = strdup(buffer);
    }
    fclose(f);
    *count = size;
    return lines;
}

// Pega o tempo atual em segundos com precisão de microsegundos
double get_time() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec + tv.tv_usec * 1e-6;
}

#endif