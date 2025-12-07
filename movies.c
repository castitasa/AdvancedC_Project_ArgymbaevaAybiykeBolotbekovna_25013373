#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "movies.h"

char* read_line_wrapper(void);
char* str_dup_wrapper(const char* s);
int ci_contains_wrapper(const char* h, const char* n);
int parse_int_wrapper(const char* s, int* o);
int parse_float_wrapper(const char* s, float* o);

static int ensure_capacity(MovieCollection* c, size_t need) {
    if (c->capacity >= need) return 1;
    size_t newcap = c->capacity ? c->capacity * 2 : 4;
    while (newcap < need) newcap *= 2;

    Movie* tmp = realloc(c->items, newcap * sizeof(Movie));
    if (!tmp) return 0;

    c->items = tmp;
    c->capacity = newcap;
    return 1;
}

void init_collection(MovieCollection* c) {
    c->items = NULL;
    c->count = 0;
    c->capacity = 0;
}

void free_collection(MovieCollection* c) {
    for (size_t i = 0; i < c->count; i++) {
        free(c->items[i].title);
        free(c->items[i].genre);
    }
    free(c->items);
    c->items = NULL;
    c->count = 0;
    c->capacity = 0;
}

int add_movie_from_input(MovieCollection* c) {
    if (!ensure_capacity(c, c->count + 1)) return 0;

    printf("Enter title: ");
    char* title = read_line_wrapper();
    if (!title) return 0;

    printf("Enter genre: ");
    char* genre = read_line_wrapper();
    if (!genre) { free(title); return 0; }

    printf("Enter year: ");
    char* ys = read_line_wrapper();
    int year = 0;
    if (ys) parse_int_wrapper(ys, &year);
    free(ys);

    printf("Enter rating: ");
    char* rs = read_line_wrapper();
    float rating = 0.0f;
    if (rs) parse_float_wrapper(rs, &rating);
    free(rs);

    Movie m;
    m.title = str_dup_wrapper(title);
    m.genre = str_dup_wrapper(genre);
    m.year = year;
    m.rating = rating;

    free(title);
    free(genre);

    c->items[c->count++] = m;
    printf("Movie added.\n");
    return 1;
}

void list_movies(const MovieCollection* c) {
    if (c->count == 0) {
        printf("No movies.\n");
        return;
    }
    for (size_t i = 0; i < c->count; i++) {
        Movie* m = &c->items[i];
        printf("%zu) %s | %s | %d | %.1f\n",
            i + 1, m->title, m->genre, m->year, m->rating);
    }
}

int search_by_title(const MovieCollection* c, const char* q) {
    int found = 0;
    for (size_t i = 0; i < c->count; i++) {
        if (ci_contains_wrapper(c->items[i].title, q)) {
            Movie* m = &c->items[i];
            printf("%zu) %s | %s | %d | %.1f\n",
                i + 1, m->title, m->genre, m->year, m->rating);
            found = 1;
        }
    }
    if (!found) printf("No results.\n");
    return found;
}

int search_by_genre(const MovieCollection* c, const char* q) {
    int found = 0;
    for (size_t i = 0; i < c->count; i++) {
        if (ci_contains_wrapper(c->items[i].genre, q)) {
            Movie* m = &c->items[i];
            printf("%zu) %s | %s | %d | %.1f\n",
                i + 1, m->title, m->genre, m->year, m->rating);
            found = 1;
        }
    }
    if (!found) printf("No results.\n");
    return found;
}

int update_movie_by_index(MovieCollection* c, size_t index) {
    if (index >= c->count) return 0;
    Movie* m = &c->items[index];

    printf("Updating movie %zu...\n", index + 1);

    printf("New title (empty = skip): ");
    char* s = read_line_wrapper();
    if (s && s[0] != '\0') {
        free(m->title);
        m->title = str_dup_wrapper(s);
    }
    free(s);

    printf("New genre (empty = skip): ");
    s = read_line_wrapper();
    if (s && s[0] != '\0') {
        free(m->genre);
        m->genre = str_dup_wrapper(s);
    }
    free(s);

    printf("New year (empty = skip): ");
    s = read_line_wrapper();
    if (s && s[0] != '\0') {
        int y;
        if (parse_int_wrapper(s, &y)) m->year = y;
    }
    free(s);

    printf("New rating (empty = skip): ");
    s = read_line_wrapper();
    if (s && s[0] != '\0') {
        float r;
        if (parse_float_wrapper(s, &r)) m->rating = r;
    }
    free(s);

    printf("Updated.\n");
    return 1;
}

int delete_movie_by_index(MovieCollection* c, size_t index) {
    if (index >= c->count) return 0;

    free(c->items[index].title);
    free(c->items[index].genre);

    for (size_t i = index; i + 1 < c->count; i++)
        c->items[i] = c->items[i + 1];

    c->count--;
    printf("Movie deleted.\n");
    return 1;
}

int save_to_file(const MovieCollection* c, const char* filename) {
    FILE* f = fopen(filename, "w");
    if (!f) return 0;

    for (size_t i = 0; i < c->count; i++) {
        Movie* m = &c->items[i];
        fprintf(f, "%s\t%s\t%d\t%.2f\n",
            m->title, m->genre, m->year, m->rating);
    }

    fclose(f);
    return 1;
}

int load_from_file(MovieCollection* c, const char* filename) {
    FILE* f = fopen(filename, "r");
    if (!f) return 0;

    char buf[1024];

    while (fgets(buf, sizeof(buf), f))
    {
        size_t len = strlen(buf);
        if (len > 0 && buf[len - 1] == '\n')
            buf[len - 1] = '\0';

        char* p1 = buf;
        char* p2 = strchr(p1, '\t');
        if (!p2) continue;
        *p2++ = '\0';

        char* p3 = strchr(p2, '\t');
        if (!p3) continue;
        *p3++ = '\0';

        char* p4 = strchr(p3, '\t');
        if (!p4) continue;
        *p4++ = '\0';

        int year = atoi(p3);
        float rating = (float)atof(p4);

        if (!ensure_capacity(c, c->count + 1)) { fclose(f); return 0; }

        Movie m;
        m.title = str_dup_wrapper(p1);
        m.genre = str_dup_wrapper(p2);
        m.year = year;
        m.rating = rating;

        c->items[c->count++] = m;
    }

    fclose(f);
    return 1;
}
