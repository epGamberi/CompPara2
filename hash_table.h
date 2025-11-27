/* Projeto Pratico 2 Computacão paralela Analisador de Desempenho de Cache de CDN com OpenMP
   Grupo: 
   Enzo Ponte Gamberi RA: 10389931
   João Guilherme Messias de Oliveira Santos RA: 10426110
   
   [Definição da Tabela Hash]:
   Este arquivo define a estrutura de dados (structs) para os nós de cache e para a tabela hash em si.
   Ele serve como um contrato (header) que expõe as funções de inserção, busca e salvamento de resultados
   para serem usadas pelos analisadores.
*/

#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Definição do Nó (Lista encadeada para colisões)
typedef struct CacheNode {
    char* url;
    long hit_count;
    struct CacheNode* next; 
} CacheNode;

// Definição da Tabela
typedef struct {
    CacheNode** buckets;
    int size;
} HashTable;

// Protótipos das funções
HashTable* ht_create(int size);
void ht_insert(HashTable* ht, const char* url);
CacheNode* ht_get(HashTable* ht, const char* url);
void ht_save_results(HashTable* ht, const char* filename);
void ht_destroy(HashTable* ht);

#endif