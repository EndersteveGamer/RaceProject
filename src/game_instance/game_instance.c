#include <stdlib.h>
#include "game_instance.h"
#include "../player/player.h"
#include "../target/target.h"
#include "../game_area/game_area.h"

void game_instance_destroy(struct GameInstance *self) {
    free(self->player);
    game_area_destroy(self->gameArea);
    target_destroy(self->target);
    free(self);
}

void change_velocity(struct GameInstance *self) {

}

void game_instance_tick(struct GameInstance *self) {
    change_velocity(self);
    player_tick_movement(self->player);
}
