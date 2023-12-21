// Théo Pariney
// Augustin Delpeut

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "player/player.h"
#include "game_area/game_area.h"
#include "target/target.h"
#include "game_instance/game_instance.h"

#define BUFSIZE 256

/**
 * Clears the game instance, then returns the exit code
 * @param exit_code The exit code to return
 * @param game_instance The game instance
 * @return The exit code
 */
int end_game(int exit_code, struct GameInstance *game_instance) {
    game_instance_destroy(game_instance);
    return exit_code;
}

/**
 * Receives a new target and replaces the previous one
 * @param game_instance The game instance
 * @param buf The buffer used to receive the target
 */
void receive_target(struct GameInstance *game_instance, char *buf) {
    if (game_instance->target != NULL) target_destroy(game_instance->target);
    fgets(buf, BUFSIZE, stdin);
    int x = (int) strtol(buf,(char**) NULL, 10);
    fgets(buf, BUFSIZE, stdin);
    int y = (int) strtol(buf,(char**) NULL, 10);
    fgets(buf, BUFSIZE, stdin);
    int w = (int) strtol(buf,(char**) NULL, 10);
    fgets(buf, BUFSIZE, stdin);
    int h = (int) strtol(buf,(char**) NULL, 10);
    game_instance->target = target_create(x, y, w, h);
}

/**
 * The main function, getting informations from the server and playing the game automatically
 * @return 0 if everything executed successfully, the error code otherwise
 */
int main() {
    setbuf(stdout, NULL);
    char buf[BUFSIZE];

    struct GameInstance gameInstance;
    gameInstance.target = NULL;

    // Get the size of the game area
    fgets(buf, BUFSIZE, stdin);
    int size = (int) strtol(buf,(char**) NULL, 10);

    // Create game area
    gameInstance.gameArea = game_area_create(size);

    // Get the tile scores
    for (int i = 0; i < size * size; ++i) {
        fgets(buf, BUFSIZE, stdin);
        int value = (int) strtol(buf,(char**) NULL, 10);
        size_t x = i % size;
        size_t y = (i - x) / size;
        struct Vector vect;
        vect.x = (int) x; vect.y = (int) y;
        *game_area_get_tile_score(gameInstance.gameArea, &vect) = value;
    }

    // Get the player starting position
    fgets(buf, BUFSIZE, stdin);
    int px = (int) strtol(buf,(char**) NULL, 10);
    fgets(buf, BUFSIZE, stdin);
    int py = (int) strtol(buf,(char**) NULL, 10);

    // Create player
    gameInstance.player = player_create(px, py);

    // Get the objective and create target
    receive_target(&gameInstance, buf);

    // Game loop
    for (;;) {
        game_instance_tick(&gameInstance);

        // Send new position
        printf("%i\n%i\n", gameInstance.player->position->x, gameInstance.player->position->y);

        // Get the response and react
        fgets(buf, BUFSIZE, stdin);
        if (strcmp(buf, "ERROR\n") == 0) {
            fprintf(stderr, "ERROR: Incorrect move!\n");
            return end_game(1, &gameInstance);
        }
        if (strcmp(buf, "FINISH\n") == 0) {
            fprintf(stderr, "FINISHED\n");
            return end_game(0, &gameInstance);
        }
        if (strcmp(buf, "CHECKPOINT\n") == 0) {
            fprintf(stderr, "CHECKPOINT\n");
            receive_target(&gameInstance, buf);
            continue;
        }
        fprintf(stderr, "OK\n");
    }
}
