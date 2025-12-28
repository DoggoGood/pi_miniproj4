#include "graph.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/**
 * Wyświetla labirynt w formacie ASCII
 * @param n - rozmiar labiryntu (n x n węzłów)
 * @param t - tablica 2D reprezentująca labirynt ((2*n+1) x (2*n+1))
 */
void print_labyrynth(int n, int t[(2 * n) + 1][(2 * n) + 1]) {
    for (int i = 0; i < (2 * n) + 1; i++) {
        for (int j = 0; j < (2 * n) + 1; j++) {
            if (t[i][j] == 1) {
                // Ściana
                printf("██");
            } else if (i % 2 == 1 && j % 2 == 1) {
                // Węzeł (pokój)
                int node_i = (i - 1) / 2;
                int node_j = (j - 1) / 2;
                int node_id = node_i * n + node_j;
                if (node_id >= 100) {
                    printf(".."); // Dla dużych liczb używamy ".."
                } else {
                    printf("%2d", node_id);
                }
            } else if (i % 2 == 1 && j % 2 == 0) {
                // Poziomy korytarz
                printf("──");
            } else if (i % 2 == 0 && j % 2 == 1) {
                // Pionowy korytarz
                printf(" │");
            } else {
                // Narożnik
                printf("  ");
            }
        }
        printf("\n");
    }
}

/**
 * Generuje labirynt przy użyciu DFS (randomizowany)
 * @param n - rozmiar labiryntu
 * @param t - tablica reprezentująca labirynt
 * @param visited - tablica śledząca odwiedzone komórki
 * @param x - aktualna pozycja x
 * @param y - aktualna pozycja y
 */
void dfs_gen(int n, int t[(2 * n) + 1][(2 * n) + 1],
             int visited[(2 * n) + 1][(2 * n) + 1], int x, int y) {
    int directions[4][2] = {{0, 2}, {2, 0}, {0, -2}, {-2, 0}};
    visited[y][x] = 1;

    // Losowa kolejność eksploracji kierunków
    int random_index = rand() % 4;
    for (int i = 0; i < 4; i++) {
        int dir_x = directions[(random_index + i) % 4][0];
        int dir_y = directions[(random_index + i) % 4][1];
        int new_x = x + dir_x;
        int new_y = y + dir_y;

        // Sprawdź czy nowa pozycja jest w granicach i nieodwiedzona
        if (new_x > 0 && new_x < (2 * n) && new_y > 0 && new_y < (2 * n) &&
            visited[new_y][new_x] == 0) {
            // Usuń ścianę między obecną a nową pozycją
            t[y + dir_y / 2][x + dir_x / 2] = 0;
            dfs_gen(n, t, visited, new_x, new_y);
        }
    }
}

/**
 * Generuje labirynt z wejściem i wyjściem
 * @param n - rozmiar labiryntu
 * @param t - tablica reprezentująca labirynt
 * @param start - pozycja wejścia (kolumna w górnym rzędzie)
 * @param end - pozycja wyjścia (kolumna w dolnym rzędzie)
 */
void generate_labyrynth(int n, int t[(2 * n) + 1][(2 * n) + 1], int start,
                        int end) {
    int start_x = start;
    int end_x = end;
    int size = (2 * n) + 1;

    // Utwórz zewnętrzne ściany z wejściem na górze i wyjściem na dole
    for (int i = 0; i < size; i++) {
        if (i == start_x * 2 + 1) {
            t[0][i] = 0; // Wejście
        } else {
            t[0][i] = 1; // Górna ściana
        }
        if (i == end_x * 2 + 1) {
            t[(2 * n)][i] = 0; // Wyjście
        } else {
            t[(2 * n)][i] = 1; // Dolna ściana
        }
        t[i][0] = 1;       // Lewa ściana
        t[i][(2 * n)] = 1; // Prawa ściana
    }

    // Inicjalizuj wewnętrzne ściany (krata)
    for (int i = 1; i < (2 * n); i++) {
        for (int j = 1; j < (2 * n); j++) {
            if (i % 2 == 0 || j % 2 == 0) {
                t[i][j] = 1;
            }
        }
    }

    // Generuj labirynt za pomocą DFS
    int (*visited)[size] = calloc(size, sizeof(int[size]));
    dfs_gen(n, t, visited, (2 * start_x) + 1, 1);
    free(visited);

    // Dodaj losowe dziury aby stworzyć alternatywne ścieżki
    int max_holes = 0;
    for (int i = 1; i < size - 1; i++) {
        for (int j = 1; j < size - 1; j++) {
            if (t[i][j] == 1 && !(i % 2 == 0 && j % 2 == 0)) {
                max_holes++;
            }
        }
    }
    int holes = rand() % (max_holes + 1);
    for (int i = 0; i < holes; i++) {
        int x = rand() % size;
        int y = rand() % size;
        // Znajdź ścianę do usunięcia (nie narożnik)
        while (t[y][x] == 0 || (x == 0) || (y == 0) || (x == size - 1) ||
               (y == size - 1) || (x % 2 == 0 && y % 2 == 0)) {
            x = rand() % size;
            y = rand() % size;
        }
        t[y][x] = 0;
    }
}

/**
 * Generuje graf z labiryntu (listę sąsiedztwa)
 * @param n - rozmiar labiryntu
 * @param t - tablica reprezentująca labirynt
 * @return wskaźnik do listy sąsiedztwa
 */
struct vector *graph_gen(int n, int t[(2 * n) + 1][(2 * n) + 1]) {
    struct vector *adj_list = malloc(n * n * sizeof(struct vector));

    for (int i = 0; i < n * n; i++) {
        vector_init(&adj_list[i]);
    }

    int directions[4][2] = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};

    // Dla każdego węzła w labiryncie
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            int node_id = i * n + j;
            int y = 2 * i + 1;
            int x = 2 * j + 1;

            // Sprawdź wszystkie 4 kierunki
            for (int d = 0; d < 4; d++) {
                int wall_y = y + directions[d][0];
                int wall_x = x + directions[d][1];
                int neighbor_y = y + 2 * directions[d][0];
                int neighbor_x = x + 2 * directions[d][1];

                // Jeśli nie ma ściany i sąsiad jest w granicach
                if (neighbor_x >= 1 && neighbor_x < (2 * n) &&
                    neighbor_y >= 1 && neighbor_y < (2 * n) &&
                    t[wall_y][wall_x] == 0) {
                    int neighbor_i = (neighbor_y - 1) / 2;
                    int neighbor_j = (neighbor_x - 1) / 2;
                    int neighbor_id = neighbor_i * n + neighbor_j;
                    vector_push_back(&adj_list[node_id], neighbor_id);
                }
            }
        }
    }

    return adj_list;
}

/**
 * Szuka ścieżek w grafie używając DFS
 * @param current - obecny węzeł
 * @param end - węzeł docelowy
 * @param n - rozmiar labiryntu
 * @param adj_list - lista sąsiedztwa grafu
 * @param visited - tablica odwiedzonych węzłów
 * @param path - obecna ścieżka
 * @param path_len - długość obecnej ścieżki
 * @param all_paths - tablica wszystkich znalezionych ścieżek
 * @param path_lengths - długości znalezionych ścieżek
 * @param num_paths - liczba znalezionych ścieżek
 * @param max_paths - maksymalna liczba ścieżek do znalezienia
 * @param total_count - licznik iteracji
 * @param max_depth - maksymalna głębokość ścieżki
 * @param max_iterations - maksymalna liczba iteracji
 */
void dfs_find_paths(int current, int end, int n, struct vector *adj_list,
                    int *visited, int *path, int path_len, int **all_paths,
                    int *path_lengths, long long *num_paths,
                    long long max_paths, long long *total_count, int max_depth,
                    long long max_iterations) {
    (*total_count)++;

    // Sprawdź warunki zakończenia
    if (*num_paths >= max_paths || path_len >= max_depth ||
        *total_count >= max_iterations) {
        return;
    }

    // Dodaj obecny węzeł do ścieżki
    path[path_len] = current;
    path_len++;

    // Sprawdź czy dotarliśmy do celu
    if (current == end) {
        if (*num_paths < max_paths) {
            // Zapisz ścieżkę
            all_paths[*num_paths] = malloc(path_len * sizeof(int));
            for (int i = 0; i < path_len; i++) {
                all_paths[*num_paths][i] = path[i];
            }
            path_lengths[*num_paths] = path_len;
        }
        (*num_paths)++;
        return;
    }

    visited[current] = 1;

    // Eksploruj sąsiadów
    for (int i = 0; i < adj_list[current].size; i++) {
        if (*num_paths >= max_paths || *total_count >= max_iterations) {
            break;
        }
        int neighbor = adj_list[current].neighbors[i];

        if (!visited[neighbor]) {
            dfs_find_paths(neighbor, end, n, adj_list, visited, path, path_len,
                           all_paths, path_lengths, num_paths, max_paths,
                           total_count, max_depth, max_iterations);
        }
    }

    visited[current] = 0; // Backtrack
}

/**
 * Znajduje najkrótszą ścieżkę używając BFS
 * @param n - rozmiar labiryntu
 * @param adj_list - lista sąsiedztwa grafu
 * @param start - węzeł startowy
 * @param end - węzeł końcowy
 * @param path_len - wskaźnik do zmiennej przechowującej długość ścieżki
 * @return wskaźnik do tablicy reprezentującej ścieżkę (lub NULL jeśli nie ma
 * ścieżki)
 */
int *bfs_shortest_path(int n, struct vector *adj_list, int start, int end,
                       int *path_len) {
    int *parent = malloc(n * n * sizeof(int));
    int *visited = calloc(n * n, sizeof(int));
    int *queue = malloc(n * n * sizeof(int));
    int front = 0, rear = 0;

    // Inicjalizuj tablicę rodziców
    for (int i = 0; i < n * n; i++) {
        parent[i] = -1;
    }

    queue[rear++] = start;
    visited[start] = 1;

    // BFS
    while (front < rear) {
        int current = queue[front++];

        if (current == end) {
            break; // Znaleziono cel
        }

        // Eksploruj sąsiadów
        for (int i = 0; i < adj_list[current].size; i++) {
            int neighbor = adj_list[current].neighbors[i];

            if (!visited[neighbor]) {
                visited[neighbor] = 1;
                parent[neighbor] = current;
                queue[rear++] = neighbor;
            }
        }
    }

    free(visited);
    free(queue);

    // Sprawdź czy znaleziono ścieżkę
    if (parent[end] == -1 && start != end) {
        free(parent);
        *path_len = 0;
        return NULL;
    }

    // Policz długość ścieżki
    int len = 0;
    for (int v = end; v != -1; v = parent[v]) {
        len++;
    }

    // Odtwórz ścieżkę
    int *path = malloc(len * sizeof(int));
    int idx = len - 1;
    for (int v = end; v != -1; v = parent[v]) {
        path[idx--] = v;
    }

    free(parent);
    *path_len = len;
    return path;
}

/**
 * Znajduje wszystkie ścieżki od startu do końca
 * @param n - rozmiar labiryntu
 * @param adj_list - lista sąsiedztwa grafu
 * @param start - węzeł startowy
 * @param end - węzeł końcowy
 */
void find_all_paths(int n, struct vector *adj_list, int start, int end) {
    // Najpierw znajdź najkrótszą ścieżkę używając BFS
    int shortest_len = 0;
    int *shortest = bfs_shortest_path(n, adj_list, start, end, &shortest_len);

    if (shortest == NULL) {
        printf("\nBrak ścieżki od węzła %d do węzła %d\n", start, end);
        return;
    }

    // Wyświetl najkrótszą ścieżkę
    printf("\nNajkrótsza ścieżka (długość %d): ", shortest_len);
    for (int i = 0; i < shortest_len; i++) {
        printf("%d", shortest[i]);
        if (i < shortest_len - 1) printf(" -> ");
    }
    printf("\n");

    // Parametry dla DFS
    long long max_paths = 500;         // Limit ścieżek do zapisania
    long long max_iterations = 100000; // Limit iteracji DFS
    int max_depth = n * n;             // Maksymalna długość ścieżki

    int *visited = calloc(n * n, sizeof(int));
    int *path = malloc(n * n * sizeof(int));
    int **all_paths = malloc(max_paths * sizeof(int *));
    int *path_lengths = malloc(max_paths * sizeof(int));
    long long num_paths = 0;
    long long total_count = 0;

    // Znajdź dodatkowe ścieżki używając DFS
    dfs_find_paths(start, end, n, adj_list, visited, path, 0, all_paths,
                   path_lengths, &num_paths, max_paths, &total_count, max_depth,
                   max_iterations);

    if (num_paths == 0) {
        printf("Znaleziono tylko najkrótszą ścieżkę\n");
        free(all_paths);
        free(path_lengths);
        free(path);
        free(visited);
        free(shortest);
        return;
    }

    printf("Znaleziono %lld ścieżek\n", num_paths);

    long long paths_to_store = num_paths < max_paths ? num_paths : max_paths;

    // Wyświetl wszystkie ścieżki lub informację o limicie
    if (num_paths < max_paths) {
        for (long long i = 0; i < num_paths; i++) {
            printf("Ścieżka %lld (długość %d): ", i + 1, path_lengths[i]);
            for (int j = 0; j < path_lengths[i]; j++) {
                printf("%d", all_paths[i][j]);
                if (j < path_lengths[i] - 1) printf(" -> ");
            }
            printf("\n");
        }
    } else {
        printf("Zbyt wiele ścieżek do wyświetlenia (limit: %lld)\n", max_paths);
    }

    // Zwolnij pamięć
    for (long long i = 0; i < paths_to_store; i++) {
        free(all_paths[i]);
    }
    free(all_paths);
    free(path_lengths);
    free(path);
    free(visited);
    free(shortest);
}

int main() {
    srand(time(NULL));
    int n;
    printf("Proszę podać rozmiar labiryntu: ");
    if (scanf("%d", &n) != 1 || n <= 0) {
        fprintf(stderr,
                "Proszę podać poprawną liczbę całkowitą większą od 0\n");
        return 1;
    }
    int start = rand() % n;
    int end = rand() % n;
    int size = (2 * n) + 1;
    int (*t)[size] = calloc(size, sizeof *t);
    generate_labyrynth(n, t, start, end);
    print_labyrynth(n, t);
    struct vector *adj_list = graph_gen(n, t);
    printf("\n");
    print_graph_edges_from_adj_list(n * n, adj_list);

    int start_node = start;
    int end_node = (n - 1) * n + end;
    find_all_paths(n, adj_list, start_node, end_node);

    for (int i = 0; i < n * n; i++) {
        vector_free(&adj_list[i]);
    }
    free(adj_list);
    free(t);
}
