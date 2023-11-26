#include <stdlib.h>
#include "vector.h"

struct Vector *vec_create(int x, int y) {
    struct Vector *new = malloc(sizeof(struct Vector));
    new->x = x;
    new->y = y;
    return new;
}
