#!/bin/bash
g++ -std=c++17 -Wall -Wextra -pthread -o game main.cpp engine.cpp gameClass.cpp vectors.cpp mouse.cpp usefull.cpp serverClient.cpp $(pkg-config allegro-5 allegro_ttf-5 allegro_image-5 allegro_primitives-5 --libs --cflags)
g++ -Wall -Wextra -pthread -std=c++17 -o server server.cpp serverClient.cpp
