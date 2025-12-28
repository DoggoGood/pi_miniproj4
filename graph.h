
#ifndef GRAPH_H
#define GRAPH_H

struct vector {
    int *neighbors;
    int size;
    int capacity;
};

void vector_init(struct vector *vec);
void vector_push_back(struct vector *vec, int value);
void vector_free(struct vector *vec);
void print_graph_edges_from_adj_list(int n, struct vector adj_list[n]);

#endif // GRAPH_H
