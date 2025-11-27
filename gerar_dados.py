import random

# Configuração
NUM_URLS = 1000
NUM_ACCESSES = 500000  # 500k acessos
URLS = [f"/video/episodio_{i}.mp4" for i in range(NUM_URLS)]

print("1. Gerando manifest.txt...")
with open("manifest.txt", "w") as f:
    for url in URLS:
        f.write(url + "\n")

print("2. Gerando log_distribuido.txt (Uniforme)...")
with open("log_distribuido.txt", "w") as f:
    for _ in range(NUM_ACCESSES):
        f.write(random.choice(URLS) + "\n")

print("3. Gerando log_concorrente.txt (Hotspot)...")
# 90% dos acessos vão para as primeiras 10 URLs (Hot Content)
hot_urls = URLS[:10]
cold_urls = URLS[10:]
with open("log_concorrente.txt", "w") as f:
    for _ in range(NUM_ACCESSES):
        if random.random() < 0.9: # 90% de chance
            f.write(random.choice(hot_urls) + "\n")
        else:
            f.write(random.choice(cold_urls) + "\n")

print(">>> TUDO PRONTO! Arquivos gerados.")