#include <stdlib.h>
#include "player.h"
#include "../vector/vector.h"

struct Player *player_create(const size_t startingX, const size_t startingY) {
    struct Player *new = malloc(sizeof(struct Player));
    new->position = vec_create((int) startingX, (int) startingY);
    new->velocity = vec_create(0, 0);
    return new;
}

void player_tick_movement(struct Player *self) {
    self->position->x += self->velocity->x;
    self->position->y += self->velocity->y;
}
