#ifndef ENGINE_H_INCLUDED
#define ENGINE_H_INCLUDED

#include "mouse.h"
#include "gameClass.h"

#define KEY_SEEN     1
#define KEY_RELEASED 2

const vec2i windowSize(1280, 720);
const int FPS = 60;


class Engine{
public:
    ALLEGRO_DISPLAY *window;
    ALLEGRO_MOUSE_STATE mouseState;
    ALLEGRO_TIMER * timer;
    ALLEGRO_EVENT_QUEUE * eventQueue;
    ALLEGRO_EVENT event;
    ALLEGRO_FONT * fontMenu, * fontError;
    unsigned char key[ALLEGRO_KEY_MAX];
    vector <short> pressedKeys;
    vector <short> releasedKeys;
    MouseClass Mouse;
    bool closeGame;
    bool isMenuScreen;
    bool isGameRunning;
    void initAllegro();
    void gameLoop(GameClass & Game);
    void destroyEngine();

//Menu screen
    string typedName;
    bool areYouTyping;
    int blinkingTime;
    int refreshErrors;
    void drawMenuScreen(ClientPointersStruct * ClientPointersOb);

};

#endif
