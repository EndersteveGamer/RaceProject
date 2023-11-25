#include <stdlib.h>
#include "target.h"

struct Target *target_create(size_t x, size_t y, size_t width, size_t height) {
    struct Target *new = malloc(sizeof(struct Target));
    new->x = x;
    new->y = y;
    new->width = width;
    new->height = height;
    return new;
}
