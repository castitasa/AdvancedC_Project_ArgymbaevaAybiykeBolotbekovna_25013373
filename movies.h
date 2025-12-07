#ifndef MOVIES_H
#define MOVIES_H

#include <stddef.h>

typedef struct {
    char* title;
    char* genre;
    int year;
    float rating;
} Movie;

typedef struct {
    Movie* items;
    size_t count;
    size_t capacity;
} MovieCollection;

void init_collection(MovieCollection* c);
void free_collection(MovieCollection* c);

int add_movie_from_input(MovieCollection* c);
void list_movies(const MovieCollection* c);
int search_by_title(const MovieCollection* c, const char* query);
int search_by_genre(const MovieCollection* c, const char* query);
int update_movie_by_index(MovieCollection* c, size_t index);
int delete_movie_by_index(MovieCollection* c, size_t index);

int save_to_file(const MovieCollection* c, const char* filename);
int load_from_file(MovieCollection* c, const char* filename);

#endif
