#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include "movies.h"

char* read_line_wrapper(void);

static void print_menu(void) {
    puts("\n=== Movie Collection Organizer ===");
    puts("1. Add movie");
    puts("2. List all movies");
    puts("3. Search by title");
    puts("4. Search by genre");
    puts("5. Update movie (by index)");
    puts("6. Delete movie (by index)");
    puts("7. Save to file");
    puts("8. Load from file");
    puts("9. Exit");
    printf("Your choice: ");
}

int main(void) {
    MovieCollection coll;
    init_collection(&coll);

    const char* DB = "movies_db.txt";

    if (!load_from_file(&coll, DB))
        printf("No file found. Starting a new collection.\n");
    else
        printf("Loaded %zu movies.\n", coll.count);

    while (1) {
        print_menu();
        char* cs = read_line_wrapper();
        if (!cs) break;

        int choice = atoi(cs);
        free(cs);

        if (choice == 1) {
            add_movie_from_input(&coll);

        }
        else if (choice == 2) {
            list_movies(&coll);

        }
        else if (choice == 3) {
            printf("Enter title: ");
            char* q = read_line_wrapper();
            if (q) { search_by_title(&coll, q); free(q); }

        }
        else if (choice == 4) {
            printf("Enter genre: ");
            char* q = read_line_wrapper();
            if (q) { search_by_genre(&coll, q); free(q); }

        }
        else if (choice == 5) {
            printf("Index to update: ");
            char* s = read_line_wrapper();
            int idx = atoi(s);
            free(s);

            if (idx >= 1 && (size_t)idx <= coll.count)
                update_movie_by_index(&coll, (size_t)idx - 1);
            else
                printf("Invalid index.\n");

        }
        else if (choice == 6) {
            printf("Index to delete: ");
            char* s = read_line_wrapper();
            int idx = atoi(s);
            free(s);

            if (idx >= 1 && (size_t)idx <= coll.count)
                delete_movie_by_index(&coll, (size_t)idx - 1);
            else
                printf("Invalid index.\n");

        }
        else if (choice == 7) {
            if (save_to_file(&coll, DB))
                printf("Saved to '%s'.\n", DB);
            else
                printf("Save failed.\n");

        }
        else if (choice == 8) {
            free_collection(&coll);
            init_collection(&coll);

            if (load_from_file(&coll, DB))
                printf("Loaded %zu movies.\n", coll.count);
            else
                printf("Load failed.\n");

        }
        else if (choice == 9) {
            printf("Goodbye! Saving...\n");
            save_to_file(&coll, DB);
            break;

        }
        else {
            printf("Unknown option.\n");
        }
    }

    free_collection(&coll);
    return 0;
}
