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
 * @param gameInstance The game instance
 * @return 1 if an error occured, 0 otherwise
 */
int end_game(int exit_code, struct GameInstance *gameInstance) {
    game_instance_destroy(gameInstance);
    return exit_code;
}

/**
 * Receives a new target and replaces the previous one
 * @param gameInstance
 * @param buf
 */
void receive_target(struct GameInstance *gameInstance, char *buf) {
    if (gameInstance->target != NULL) free(gameInstance->target);
    fgets(buf, BUFSIZE, stdin);
    int x = (int) strtol(buf,(char**) NULL, 10);
    fgets(buf, BUFSIZE, stdin);
    int y = (int) strtol(buf,(char**) NULL, 10);
    fgets(buf, BUFSIZE, stdin);
    int w = (int) strtol(buf,(char**) NULL, 10);
    fgets(buf, BUFSIZE, stdin);
    int h = (int) strtol(buf,(char**) NULL, 10);
    gameInstance->target = target_create(x, y, w, h);
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
        *game_area_get_tile_score(gameInstance.gameArea, x, y) = value;
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
        printf("%i\n%i\n", px, py);

        // Get the response and react
        fgets(buf, BUFSIZE, stdin);
        if (strcmp(buf, "ERROR\n") == 0) {
            printf("ERROR: Incorrect move!\n");
            return end_game(1, &gameInstance);
        }
        if (strcmp(buf, "FINISH\n") == 0) {
            printf("FINISHED with score: %i\n", gameInstance.player->score);
            return end_game(0, &gameInstance);
        }
        if (strcmp(buf, "CHECKPOINT\n") == 0) {
            printf("CHECKPOINT\n");
            receive_target(&gameInstance, buf);
            continue;
        }
        printf("OK\n");
    }
}
