#include <stdlib.h>
#include "vector.h"

struct Vector *vec_create(int x, int y) {
    struct Vector *new = malloc(sizeof(struct Vector));
    new->x = x;
    new->y = y;
    return new;
}

void vec_add(struct Vector *vec1, struct Vector *vec2, struct Vector *res) {
    res->x = vec1->x + vec2->x;
    res->y = vec1->y + vec2->y;
}

void vec_substract(struct Vector *vec1, struct Vector *vec2, struct Vector *res) {
    res->x = vec1->x - vec2->x;
    res->y = vec1->y - vec2->y;
}

void vec_mult(struct Vector *self, double multiplier) {
    self->x = (int)(self->x * multiplier);
    self->y = (int)(self->y * multiplier);
}

void vec_copy(struct Vector *src, struct Vector *dest) {
    dest->x = src->x; dest->y = src->y;
}
