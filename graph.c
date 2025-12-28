#include "graph.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * Inicjalizuje wektor z początkową pojemnością 4.
 * vec - wskaźnik do wektora do inicjalizacji
 */
void vector_init(struct vector *vec) {
    vec->size = 0;
    vec->capacity = 4;
    vec->neighbors = malloc(vec->capacity * sizeof(int));
}

/**
 * Dodaje element na koniec wektora, zwiększając pojemność jeśli potrzeba.
 * vec - wskaźnik do wektora
 * value - wartość do dodania
 */
void vector_push_back(struct vector *vec, int value) {
    // zwiększanie pojemności, jeśli potrzeba
    if (vec->size >= vec->capacity) {
        vec->capacity *= 2;
        vec->neighbors = realloc(vec->neighbors, vec->capacity * sizeof(int));
    }
    vec->neighbors[vec->size++] = value;
}

/**
 * Zwalnia pamięć zaalokowaną dla wektora.
 * vec - wskaźnik do wektora
 */
void vector_free(struct vector *vec) { free(vec->neighbors); }

/**
 * Wypisuje krawędzie grafu w formacie: u -> v lub u -> v, multikrawędź * x.
 * n - liczba wierzchołków
 * adj_list - tablica list sąsiedztwa
 */
void print_graph_edges_from_adj_list(int n, struct vector adj_list[n]) {
    printf("Krawędzie grafu:\n");
    for (int i = 0; i < n; i++) {
        int j = 0;
        while (j < adj_list[i].size) {
            int target = adj_list[i].neighbors[j];
            int count = 1;
            j++;
            // zliczanie multikrawędzi
            while (j < adj_list[i].size && adj_list[i].neighbors[j] == target) {
                count++;
                j++;
            }
            if (count == 1) {
                printf("%d -> %d\n", i, target);
            } else {
                printf("%d -> %d, multikrawędź * %d\n", i, target, count);
            }
        }
    }
}
