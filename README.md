# lenv.h

Uma biblioteca *header-only* (apenas um arquivo de cabeçalho) leve, rápida e segura em C para ler e gerenciar arquivos de configuração ou variáveis de ambiente (estilo arquivos `.env`).

## ✨ Recursos

*   **Arquivo Único (`header-only`):** Basta copiar o arquivo `lenv.h` para o seu projeto e começar a usar.
*   **Inserção Eficiente:** Carregamento de dados com complexidade $O(1)$.
*   **Segurança de Memória:** Inclui rotina de liberação total para evitar *memory leaks*.
*   **Comentários:** Ignora automaticamente linhas em branco ou linhas iniciadas com `#`.

## 🚀 Como Usar

### 1. Crie seu arquivo de configuração

Crie um arquivo chamado `.env` ou `config.ini` na raiz do seu projeto:

```ini
# Configurações do Banco de Dados
DB_HOST=localhost
DB_USER=root
DB_PASS=secret123

# Configurações do Sistema
PORT=8080
DEBUG=true
```

### 2. Inclua no seu código C

Basta incluir o arquivo `"lenv.h"` no seu código-fonte. Veja o exemplo completo de uso abaixo:

```c
#include <stdio.h>
#include "lenv.h"

int main() {
    // 1. Inicializa o ambiente carregando o arquivo
    struct lenv_t* env = lenv_init(".env");
    
    if (env == NULL) {
        printf("Erro ao carregar o arquivo ou arquivo vazio/inexistente.\n");
        return 1;
    }

    // 2. Percorre e exibe todas as configurações carregadas
    printf("--- Configurações Carregadas ---\n");
    struct lenv_t* current = env;
    while (current != NULL) {
        printf("Chave: %s | Valor: %s\n", current->token, current->value);
        current = current->next;
    }
    printf("--------------------------------\n\n");

    // 3. Libera toda a memória alocada de forma segura
    // Passamos o endereço (&env) para que o ponteiro seja definido como NULL internamente
    lenvt_free(&env);

    // Verificação de segurança pós-free
    if (env == NULL) {
        printf("Memória liberada com sucesso e ponteiro limpo!\n");
    }

    return 0;
}
```

## 🛠️ Estrutura de Dados

A biblioteca utiliza uma lista encadeada simples representada pela estrutura abaixo:

```c
struct lenv_t {
    char *token;         // Nome da chave/configuração
    char *value;         // Valor associado
    struct lenv_t *next; // Ponteiro para o próximo nó
};
```

## ✒️ Assinatura das Funções

### `lenv_init`
```c
static inline struct lenv_t *lenv_init(const char* fp);
```
Abre o arquivo especificado no caminho `fp`, processa linha por linha dividindo as chaves e valores pelo caractere `=`, e retorna o ponteiro para o início da lista encadeada. Retorna `NULL` se o arquivo não puder ser aberto.

### `lenvt_free`
```c
static inline void lenvt_free(struct lenv_t** lenv);
```
Libera recursivamente a memória de todas as chaves (`token`), valores (`value`) e a própria estrutura dos nós. No final, define o ponteiro original passado por referência como `NULL` para evitar ponteiros fantasmas (*dangling pointers*).

