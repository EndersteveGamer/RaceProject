#include <stdlib.h>
#include "player.h"

struct Player *player_create(const size_t startingX, const size_t startingY) {
    struct Player *new = malloc(sizeof(struct Player));
    new->x = startingX;
    new->y = startingY;
    new->xVelocity = 0;
    new->yVelocity = 0;
    new ->score = 0;
    return new;
}

void player_tick_movement(struct Player *self) {
    self->x += self->xVelocity;
    self->y += self->yVelocity;
}
