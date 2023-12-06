#include <stdlib.h>
#include <stdbool.h>
#include "game_instance.h"
#include "../player/player.h"
#include "../target/target.h"
#include "../game_area/game_area.h"

// A multiplier for braking earlier than normal
#define BRAKE_MULT 1.4

// The speed at which the braking to avoid jumping target deactivates (to prevent the player from being stuck with 0 speed)
#define TARGET_JUMP_SAFETY 2

void game_instance_destroy(struct GameInstance *self) {
    player_destroy(self->player);
    game_area_destroy(self->gameArea);
    target_destroy(self->target);
}

/**
 * Returns the sum of i from 1 to n
 * @param n n
 * @return The sum of i from 1 to n
 */
int sum(int n) {return (n * (n + 1)) / 2;}

/**
 * Returns the sum of i from m to n
 * @param m m
 * @param n n
 * @return The sum of i from m to n
 */
int sum_from(int m, int n) {
    if (m > n) {int temp = n; n = m; m = temp;}
    int sum = m; for (int i = m + 1; i <= n; i++) sum += i; return sum;
}

/**
 * Checks if the player can accelerate in this axis without hitting the wall
 * @param self The game instance
 * @param x_axis true if the axis is the x axis, false if the axis is the y axis
 * @return true if the player can accelerate, false otherwise
 */
bool can_accelerate(struct GameInstance *self, bool x_axis) {
    int pos = x_axis ? self->player->position->x : self->player->position->y;
    int speed = x_axis ? self->player->velocity->x : self->player->velocity->y;
    if (speed != 0) speed += (speed > 0 ? 1 : -1);
    if (speed == 0) return true;
    if (speed > 0) return pos + sum(speed) < self->gameArea->width;
    return pos - sum(-speed) >= 0;
}

/**
 * Checks the player can accelerate without having its speed exceed the target width (to avoid jumping over the target)
 * @param self The game instance
 * @param x_axis true if the axis checked is the x axis, false otherwise
 * @return true if the player can accelerate, false otherwise
 */
bool can_accelerate_target(struct GameInstance *self, bool x_axis) {
    int targetPos = x_axis ? self->target->position->x : self->target->position->y;
    int targetSize = x_axis ? self->target->size->x : self->target->size->y;
    targetSize--;
    int selfPos = x_axis ? self->player->position->x : self->player->position->y;
    int speed = x_axis ? self->player->velocity->x : self->player->velocity->y;
    if ((targetPos < selfPos && speed > 0) || (targetPos > selfPos && speed < 0)) return true;
    speed > 0 ? speed-- : speed++;
    if ((speed > 0 ? speed : -speed) < TARGET_JUMP_SAFETY) return true;
    if (speed < 0) return selfPos - sum_from(targetSize, speed) > targetPos;
    else if (speed > 0) return selfPos + sum_from(targetSize, speed) < targetPos;
    else return true;
}

/**
 * Returns the closest target tile from the player
 * @param self The game instance
 * @return A newly allocated vector containing the position of the closest tile
 */
void get_closest_target_tile(const struct GameInstance *self, struct Vector *tile) {
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

void get_lowest_score_target_tile(const struct GameInstance *self, struct Vector *tile) {
    const struct Target *target = self->target;
    tile->x = target->position->x;
    tile->y = target->position->y;
    int min = *game_area_get_tile_score(self->gameArea, tile);
    for (size_t x = 0; x < target->size->x; x++) {
        for (size_t y = 0; y < target->size->y; y++) {
            struct Vector currTile;
            currTile.x = (int) (target->position->x + x);
            currTile.y = (int) (target->position->y + y);
            int curr = *game_area_get_tile_score(self->gameArea, &currTile);
            if (curr < min) {
                min = curr;
                tile->x = currTile.x; tile->y = currTile.y;
            }
        }
    }
}

void change_velocity(struct GameInstance *self) {
    struct Player *player = self->player;
    struct Vector closest;
    get_lowest_score_target_tile(self, &closest);
    struct Vector futurePos;
    struct Vector futureVelocity;
    vec_copy(player->velocity, &futureVelocity);
    vec_mult(&futureVelocity, BRAKE_MULT);
    vec_add(player->position, &futureVelocity, &futurePos);
    struct Vector targetDirection;
    vec_substract(&closest, &futurePos, &targetDirection);
    if (!can_accelerate(self, true) || !can_accelerate_target(self, true)) {
        player->velocity->x += player->velocity->x == 0 ? 0 : (player->velocity->x > 0 ? -1 : 1);
    } else {
        if (player->velocity->x > targetDirection.x) player->velocity->x--;
        else if (player->velocity->x < targetDirection.x) player->velocity->x++;
    }
    if (!can_accelerate(self, false) || !can_accelerate_target(self, false)) {
        player->velocity->y += player->velocity->y == 0 ? 0 : (player->velocity->y > 0 ? -1 : 1);
    } else {
        if (player->velocity->y > targetDirection.y) player->velocity->y--;
        else if (player->velocity->y < targetDirection.y) player->velocity->y++;
    }
}

void game_instance_tick(struct GameInstance *self) {
    change_velocity(self);
    player_tick_movement(self->player);
}

// Cool seed: 1114082393
// Another cool seed: 2849285029
// Third cool seed: 422580649
// Big map: 2937334609
