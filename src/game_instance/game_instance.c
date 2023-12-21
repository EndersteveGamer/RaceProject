#include <stdlib.h>
#include <stdbool.h>
#include "game_instance.h"
#include "../player/player.h"
#include "../target/target.h"
#include "../game_area/game_area.h"

/* A multiplier for braking earlier than normal. When calculating the player position, the player velocity is added
 * BRAKE_MULT times. With this, the player starts braking earlier when approaching the target, as it is considered as
 * behind the target when it is not already
 */
#define BRAKE_MULT 1.4

/* Used in the can_accelerate_target function. If the player speed is lower than this value, the player is allowed
 * to accelerate. This is used to prevent the player to be stuck with 0 velocity
 */
#define BRAKE_SAFETY 3

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
    // Make n the highest value between m and n
    if (m > n) {int temp = n; n = m; m = temp;}
    // Calculate the sum
    return sum(n) - sum(m - 1);
}

/**
 * Checks if a position is in the target
 * @param self The game instance
 * @param pos The position to check
 * @return true if the position is in the target, false otherwise
 */
bool is_in_target(const struct GameInstance *self, const struct Vector *pos) {
    const struct Vector *targetPos = self->target->position;
    const struct Vector *size = self->target->size;
    return (pos->x >= targetPos->x && pos->x < targetPos->x + size->x)
        && (pos->y >= targetPos->y && pos->y < targetPos->y + size->y);
}

/**
 * If the next tile is in the target, change the velocity to choose the tile with the lowest score
 * @param self The game instance
 * @return true if the target was approached (no further action is necessary), false otherwise
 */
bool approach_target(struct GameInstance *self) {
    struct Vector nextTile;
    vec_copy(self->player->position, &nextTile);
    vec_add(&nextTile, self->player->velocity, &nextTile);
    if (!is_in_target(self, &nextTile)) return false;
    struct Vector selectedMove;
    selectedMove.x = selectedMove.y = 0;
    for (int y = -1; y <= 1; y++) {
        for (int x = -1; x <= 1; x++) {
            struct Vector delta; delta.x = x; delta.y = y;
            struct Vector temp;
            vec_add(&nextTile, &delta, &temp);
            if (temp.x < 0 || temp.x >= self->gameArea->width
                || temp.y < 0 || temp.y >= self->gameArea->width
                || !is_in_target(self, &temp)) continue;
            int newScore = *game_area_get_tile_score(self->gameArea, &temp);
            vec_add(&nextTile, &selectedMove, &temp);
            int currScore = *game_area_get_tile_score(self->gameArea, &temp);
            if (newScore < currScore) selectedMove = delta;
        }
    }
    vec_add(self->player->velocity, &selectedMove, self->player->velocity);
    return true;
}

/**
 * Get the tile from the target with the lowest score. If two tiles have the same score, the closest one from the
 * player will be chosen
 * @param self The game instance
 * @param tile A pointer to a vector the result will be put in
 */
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
            else if (curr == min) {
                if (vec_distance(&currTile, self->player->position)
                    < vec_distance(tile, self->player->position)) {
                    min = curr;
                    tile->x = currTile.x; tile->y = currTile.y;
                }
            }
        }
    }
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

/**
 * Checks if the player can accelerate in this axis without hitting the wall
 * @param self The game instance
 * @param x_axis true if the axis is the x axis, false if the axis is the y axis
 * @return true if the player can accelerate, false otherwise
 */
bool can_accelerate(struct GameInstance *self, bool x_axis, bool accelerate) {
    int pos = x_axis ? self->player->position->x : self->player->position->y;
    int speed = x_axis ? self->player->velocity->x : self->player->velocity->y;
    if (speed != 0 && accelerate) speed += (speed > 0 ? 1 : -1);
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
bool can_accelerate_target(struct GameInstance *self, bool x_axis, bool accelerate) {
    int pos = x_axis ? self->player->position->x : self->player->position->y;
    int speed = x_axis ? self->player->velocity->x : self->player->velocity->y;
    int targetWidth = x_axis ? self->target->size->x : self->target->size->y;
    int targetPos = x_axis ? self->target->position->x : self->target->position->y;
    if (speed != 0 && accelerate) speed += (speed > 0 ? 1 : -1);
    if (speed == 0) return true;
    if ((speed >= 0 ? speed : -speed) <= BRAKE_SAFETY) return true;
    if (speed > 0) return pos + sum_from(targetWidth, speed) < targetPos + targetWidth;
    return pos - sum_from(-speed, -targetWidth) >= targetPos;
}

/**
 * Brakes if necessary (if there is a risk of crossing the map border or missing the target)\n
 * If braking was necessary, true is returned (no further action is required for this axis)
 * @param self The game instance
 * @param x_axis true if the axis considered is the x axis, false otherwise
 * @return true if braking was necessary (braking was done, no further action is required),
 * false otherwise (moving to the target is required)
 */
bool brake_if_necessary(struct GameInstance *self, bool x_axis) {
    struct Player *player = self->player;
    if (!can_accelerate(self, x_axis, true) || !can_accelerate_target(self, x_axis, true)) {
        if (x_axis) player->velocity->x += player->velocity->x == 0 ? 0 : (player->velocity->x > 0 ? -1 : 1);
        else player->velocity->y += player->velocity->y == 0 ? 0 : (player->velocity->y > 0 ? -1 : 1);
        return true;
    }
    return !can_accelerate(self, x_axis, false) || !can_accelerate_target(self, x_axis, false);
}

/**
 * Changes the velocity of the player to reach the target
 * @param self The game instance
 */
void change_velocity(struct GameInstance *self) {
    struct Player *player = self->player;
    struct Vector closest;
    get_lowest_score_target_tile(self, &closest);
    // Calculate the future position of the player using BRAKE_MULT
    struct Vector futurePos;
    struct Vector futureVelocity;
    vec_copy(player->velocity, &futureVelocity);
    vec_mult(&futureVelocity, BRAKE_MULT);
    vec_add(player->position, &futureVelocity, &futurePos);
    struct Vector targetDirection;
    vec_substract(&closest, &futurePos, &targetDirection);
    // Brake if necessary
    bool brake_x = brake_if_necessary(self, true);
    bool brake_y = brake_if_necessary(self, false);
    // If braking is not necessary, check if the target is about to be reached, and react accordingly
    bool approaching = (!brake_x && !brake_y) ? approach_target(self) : false;
    // If not braking, and not approaching target, accelerate towards the target
    if (!brake_x && !approaching) {
        if (player->velocity->x > targetDirection.x) player->velocity->x--;
        else if (player->velocity->x < targetDirection.x) player->velocity->x++;
    }
    if (!brake_y && !approaching) {
        if (player->velocity->y > targetDirection.y) player->velocity->y--;
        else if (player->velocity->y < targetDirection.y) player->velocity->y++;
    }
}

/**
 * Ticks the game instance, changing the velocity of the player and updating its position
 * @param self The game instance
 */
void game_instance_tick(struct GameInstance *self) {
    change_velocity(self);
    player_tick_movement(self->player);
}

// Cool seed: 1114082393
// Another cool seed: 2849285029
// Third cool seed: 422580649
// Big map: 2937334609
// Big map with a lot of targets: 2077811858
// Seed where we beat hi: 2084127614
