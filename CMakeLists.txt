cmake_minimum_required(VERSION 3.22)
project(RaceProject C)

set(CMAKE_C_STANDARD 99)

add_executable(RaceProject src/main.c
        src/player/player.h
        src/player/player.c
        src/game_area/game_area.h
        src/game_area/game_area.c
        src/target/target.h
        src/target/target.c
        src/game_instance/game_instance.h
        src/game_instance/game_instance.c
        src/vector/vector.h
        src/vector/vector.c)
