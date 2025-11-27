/* Projeto Pratico 2 Computacão paralela Analisador de Desempenho de Cache de CDN com OpenMP
   Grupo: 
   Enzo Ponte Gamberi RA: 10389931
   João Guilherme Messias de Oliveira Santos RA: 10426110
   
   [Implementação da Tabela Hash]:
   Este arquivo contém a lógica real da Tabela Hash. Implementa a função de hash (djb2),
   o tratamento de colisões por encadeamento e a lógica de salvar os resultados em CSV.
   A função ht_save_results ordena os dados antes de salvar para garantir consistência na verificação.
*/

#include "hash_table.h"

// Função de Hash djb2 (rápida e eficiente para strings)
unsigned long hash_function(const char *str) {
    unsigned long hash = 5381;
    int c;
    while ((c = *str++))
        hash = ((hash << 5) + hash) + c; 
    return hash;
}

HashTable* ht_create(int size) {
    HashTable* ht = (HashTable*)malloc(sizeof(HashTable));
    ht->size = size;
    ht->buckets = (CacheNode**)calloc(size, sizeof(CacheNode*));
    return ht;
}

// Inserção (Usada apenas na leitura do manifest, sequencial)
void ht_insert(HashTable* ht, const char* url) {
    unsigned long index = hash_function(url) % ht->size;
    
    // Verifica se já existe
    CacheNode* current = ht->buckets[index];
    while (current != NULL) {
        if (strcmp(current->url, url) == 0) return;
        current = current->next;
    }

    // Insere novo nó na cabeça da lista
    CacheNode* new_node = (CacheNode*)malloc(sizeof(CacheNode));
    new_node->url = strdup(url); 
    new_node->hit_count = 0;
    new_node->next = ht->buckets[index]; 
    ht->buckets[index] = new_node;
}

// Busca (Thread-safe para leitura: retorna o ponteiro direto para a memória)
CacheNode* ht_get(HashTable* ht, const char* url) {
    unsigned long index = hash_function(url) % ht->size;
    CacheNode* current = ht->buckets[index];
    
    while (current != NULL) {
        if (strcmp(current->url, url) == 0) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

// Função auxiliar para o qsort (ordenar alfabeticamente pela URL)
int compare_nodes(const void* a, const void* b) {
    CacheNode* nodeA = *(CacheNode**)a;
    CacheNode* nodeB = *(CacheNode**)b;
    return strcmp(nodeA->url, nodeB->url);
}

// Salva os resultados ordenados no CSV
void ht_save_results(HashTable* ht, const char* filename) {
    FILE* f = fopen(filename, "w");
    if (!f) {
        perror("Erro ao abrir arquivo de resultados");
        return;
    }

    // 1. Contar total de nós
    long total_nodes = 0;
    for (int i = 0; i < ht->size; i++) {
        CacheNode* n = ht->buckets[i];
        while (n) {
            total_nodes++;
            n = n->next;
        }
    }

    // 2. Colocar todos em um array linear
    CacheNode** linear_list = malloc(total_nodes * sizeof(CacheNode*));
    long k = 0;
    for (int i = 0; i < ht->size; i++) {
        CacheNode* n = ht->buckets[i];
        while (n) {
            linear_list[k++] = n;
            n = n->next;
        }
    }

    // 3. Ordenar
    qsort(linear_list, total_nodes, sizeof(CacheNode*), compare_nodes);

    // 4. Escrever no arquivo
    for (long i = 0; i < total_nodes; i++) {
        fprintf(f, "%s,%ld\n", linear_list[i]->url, linear_list[i]->hit_count);
    }

    free(linear_list);
    fclose(f);
}

void ht_destroy(HashTable* ht) {
    for (int i = 0; i < ht->size; i++) {
        CacheNode* current = ht->buckets[i];
        while (current != NULL) {
            CacheNode* temp = current;
            current = current->next;
            free(temp->url);
            free(temp);
        }
    }
    free(ht->buckets);
    free(ht);
}