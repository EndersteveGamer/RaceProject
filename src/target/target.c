#include <stdlib.h>
#include "target.h"
#include "../vector/vector.h"

struct Target *target_create(size_t x, size_t y, size_t width, size_t height) {
    struct Target *new = malloc(sizeof(struct Target));
    new->position = vec_create((int) x, (int) y);
    new->size = vec_create((int) width, (int) height);
    return new;
}

void target_destroy(struct Target *self) {
    free(self->position);
    free(self->size);
    free(self);
}
