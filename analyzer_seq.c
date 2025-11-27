/* Projeto Pratico 2 Computacão paralela Analisador de Desempenho de Cache de CDN com OpenMP
   Grupo: 
   Enzo Ponte Gamberi RA: 10389931
   João Guilherme Messias de Oliveira Santos RA: 10426110
   
   [Analisador Sequencial]:
   Esta é a versão de referência (baseline) do projeto.
   Ela processa os logs linha por linha em uma única thread (sem OpenMP).
   O tempo de execução deste arquivo será usado para calcular o Speedup das versões paralelas.
*/

#include "hash_table.h"
#include "utils.h"

// Tamanho fixo da tabela (primo para evitar colisões excessivas)
#define TABLE_SIZE 50000 

int main() {
    printf("=== CDN Analyzer SEQUENTIAL ===\n");

    // 1. Criar Tabela Hash
    HashTable* ht = ht_create(TABLE_SIZE);

    // 2. Carregar Manifest (Fase 1)
    printf("Carregando manifest...\n");
    load_manifest(ht, "manifest.txt");

    // 3. Carregar Log para Memória (Isolando I/O)
    long log_count = 0;
    printf("Carregando log para memoria...\n");
    // O arquivo access_log.txt deve ser um link para o log que queremos testar
    char** log_lines = load_log_to_memory("access_log.txt", &log_count);
    printf("Total de acessos a processar: %ld\n", log_count);

    // 4. Processamento (Fase 2 - Cronometrada)
    printf("Iniciando processamento sequencial...\n");
    double start = get_time();

    for (long i = 0; i < log_count; i++) {
        CacheNode* node = ht_get(ht, log_lines[i]);
        if (node) {
            node->hit_count++;
        }
    }

    double end = get_time();
    printf("Tempo de processamento: %.4f segundos\n", end - start);

    // 5. Salvar Resultados
    ht_save_results(ht, "results.csv");

    // Limpeza
    for (long i = 0; i < log_count; i++) free(log_lines[i]);
    free(log_lines);
    ht_destroy(ht);

    return 0;
}