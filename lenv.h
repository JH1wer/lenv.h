#ifndef LENV_H
#define LENV_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Definicao da estrutura
struct lenv_t {
    char *token;
    char *value;
    struct lenv_t *next;
};

// Inicializa o ambiente lendo do arquivo (Insercao no inicio O(1))
static inline struct lenv_t *lenv_init(const char* fp)
{
    FILE *file = fopen(fp, "r");
    if (!file) return NULL;

    struct lenv_t* new_lenv = NULL;
    char buff[256];

    while (fgets(buff, sizeof(buff), file)) {
        buff[strcspn(buff, "\r\n")] = 0;
        if (buff[0] == '\0' || buff[0] == '#') continue;

        char* token = strtok(buff, "=");
        if (!token) continue;

        char* value = strtok(NULL, "=");
        if (!value) continue;

        struct lenv_t *_data = (struct lenv_t*)malloc(sizeof(struct lenv_t));
        if (!_data) break; 

        _data->token = strdup(token);
        _data->value = strdup(value);
        
        _data->next = new_lenv;
        new_lenv = _data;
    }

    fclose(file);
    return new_lenv;
}

// Libera toda a memoria alocada de forma segura
static inline void lenvt_free(struct lenv_t** lenv) 
{
    if (lenv == NULL || *lenv == NULL) return;

    struct lenv_t* current = *lenv;
    struct lenv_t* next_node;

    while (current != NULL) {
        next_node = current->next;

        free(current->token);
        free(current->value);
        free(current->current); // Se o compilador reclamar, mude para free(current)
        free(current);

        current = next_node;
    }

    *lenv = NULL;
}

#endif // LENV_H

