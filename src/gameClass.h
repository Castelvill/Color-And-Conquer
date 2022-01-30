#ifndef GAMECLASS_H_INCLUDED
#define GAMECLASS_H_INCLUDED

#include "mouse.h"
#include "serverClient.h"

#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <fstream>
#include <pthread.h>





void* clientThread(void* arg);
void reciveACC(char * buf, int &cursor, int size);
void countRanking();

struct ClientPointersStruct{
    int port;
    string ipAdress;
    bool tryToConnect;
    bool connectionFailed;
    bool nameIsWrong;
    bool nameExists;
    bool serverIsFull;
    bool connected;
    bool disconnecting;
    string typedName;
};

class GameClass{
public:
    vec2i mapSize;
    double moveCooldown;
    ClientPointersStruct * ClientPointersObj;

    ALLEGRO_BITMAP *player;
    ALLEGRO_BITMAP *floor1;
    ALLEGRO_BITMAP *floorColorBitmap;
    ALLEGRO_BITMAP *wallBitmap1;
    ALLEGRO_FONT * fontSans, *fontLittleSans;

    void prepareGameObjects();
    void destroyObjects();
    void drawGame(ALLEGRO_DISPLAY *window);
    void moveThePlayer(vector <short> pressedKeys);






};

#endif
