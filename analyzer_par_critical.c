/* Projeto Pratico 2 Computacão paralela Analisador de Desempenho de Cache de CDN com OpenMP
   Grupo: 
   Enzo Ponte Gamberi RA: 10389931
   João Guilherme Messias de Oliveira Santos RA: 10426110
   
   [Analisador Paralelo - Critical]:
   Esta versão implementa o paralelismo usando OpenMP com a estratégia de sincronização #pragma omp critical.
   Essa estratégia bloqueia todas as threads exceto uma na hora da escrita.
   Espera-se que essa versão tenha baixo desempenho, especialmente em cenários de alta contenção.
*/

#include <omp.h>
#include "hash_table.h"
#include "utils.h"

#define TABLE_SIZE 50000

int main() {
    printf("=== CDN Analyzer PARALLEL (CRITICAL) ===\n");
    
    HashTable* ht = ht_create(TABLE_SIZE);
    
    printf("Carregando manifest...\n");
    load_manifest(ht, "manifest.txt");

    long log_count = 0;
    printf("Carregando log para memoria...\n");
    char** log_lines = load_log_to_memory("access_log.txt", &log_count);
    printf("Total de acessos: %ld\n", log_count);

    printf("Iniciando processamento paralelo (CRITICAL). Threads: %d\n", omp_get_max_threads());
    double start = get_time();

    // Loop Paralelo
    #pragma omp parallel for
    for (long i = 0; i < log_count; i++) {
        CacheNode* node = ht_get(ht, log_lines[i]);
        if (node) {
            // Sincronização Grosseira: Bloqueia globalmente
            #pragma omp critical
            {
                node->hit_count++;
            }
        }
    }

    double end = get_time();
    printf("Tempo de processamento: %.4f segundos\n", end - start);

    ht_save_results(ht, "results.csv");

    for (long i = 0; i < log_count; i++) free(log_lines[i]);
    free(log_lines);
    ht_destroy(ht);

    return 0;
}