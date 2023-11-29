#include <stdlib.h>
#include "game_instance.h"
#include "../player/player.h"
#include "../target/target.h"
#include "../game_area/game_area.h"

// A multiplier for braking earlier than normal
#define BRAKE_MULT 1.4

void game_instance_destroy(struct GameInstance *self) {
    free(self->player);
    game_area_destroy(self->gameArea);
    target_destroy(self->target);
    free(self);
}

/**
 * Returns the closest target tile from the player
 * @param self The game instance
 * @return A newly allocated vector containing the position of the closest tile
 */
void get_closest_target_tile(struct GameInstance *self, struct Vector *tile) {
    struct Player *player = self->player;
    struct Target *target = self->target;
    if (player->position->x > target->position->x + target->size->x - 1) {
        tile->x = target->position->x + target->size->x - 1;
    }
    else if (player->position->x < target->position->x) tile->x = target->position->x;
    else tile->x = player->position->x;
    if (player->position->y > target->position->y + target->size->y - 1) {
        tile->y = target->position->y + target->size->y - 1;
    }
    else if (player->position->y < target->position->y) tile->y = target->position->y;
    else tile->y = player->position->y;
}

void change_velocity(struct GameInstance *self) {
    struct Player *player = self->player;
    struct Vector closest;
    get_closest_target_tile(self, &closest);
    struct Vector futurePos;
    struct Vector futureVelocity;
    vec_copy(player->velocity, &futureVelocity);
    vec_mult(&futureVelocity, BRAKE_MULT);
    vec_add(player->position, &futureVelocity, &futurePos);
    struct Vector targetDirection;
    vec_substract(&closest, &futurePos, &targetDirection);
    if (player->velocity->x > targetDirection.x) player->velocity->x--;
    else if (player->velocity->x < targetDirection.x) player->velocity->x++;
    if (player->velocity->y > targetDirection.y) player->velocity->y--;
    else if (player->velocity->y < targetDirection.y) player->velocity->y++;
}

void game_instance_tick(struct GameInstance *self) {
    change_velocity(self);
    player_tick_movement(self->player);
}
