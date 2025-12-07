#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "movies.h"

char* read_line(void) {
    size_t size = 64;
    size_t len = 0;
    char* buf = malloc(size);
    if (!buf) return NULL;

    int ch;
    while ((ch = getchar()) != EOF && ch != '\n') {
        buf[len++] = (char)ch;
        if (len + 1 >= size) {
            size *= 2;
            char* temp = realloc(buf, size);
            if (!temp) { free(buf); return NULL; }
            buf = temp;
        }
    }

    if (len == 0 && ch == EOF) {
        free(buf);
        return NULL;
    }

    buf[len] = '\0';
    return buf;
}

char* read_line_wrapper(void) { return read_line(); }

char* str_dup(const char* s) {
    if (!s) return NULL;
    size_t n = strlen(s);
    char* copy = malloc(n + 1);
    if (!copy) return NULL;
    memcpy(copy, s, n + 1);
    return copy;
}

char* str_dup_wrapper(const char* s) { return str_dup(s); }

int ci_contains(const char* h, const char* n) {
    if (!h || !n) return 0;
    size_t hl = strlen(h), nl = strlen(n);
    if (nl == 0) return 1;

    for (size_t i = 0; i + nl <= hl; i++) {
        size_t j;
        for (j = 0; j < nl; j++) {
            char a = (char)tolower((unsigned char)h[i + j]);
            char b = (char)tolower((unsigned char)n[j]);
            if (a != b) break;
        }
        if (j == nl) return 1;
    }
    return 0;
}

int ci_contains_wrapper(const char* h, const char* n) {
    return ci_contains(h, n);
}

int parse_int_wrapper(const char* s, int* out) {
    if (!s || !out) return 0;
    *out = atoi(s);
    return 1;
}

int parse_float_wrapper(const char* s, float* out) {
    if (!s || !out) return 0;
    *out = (float)atof(s);
    return 1;
}
