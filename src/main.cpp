#include "engine.h"
#include <stdlib.h>
#include <iostream>



using std::string, std::cout;


int main(int argc, char* argv[])
{
    Engine GameEngine;
    GameEngine.initAllegro();
    GameClass Game;
    Game.prepareGameObjects();

    Game.ClientPointersObj->port = 8000;
    Game.ClientPointersObj->ipAdress = "127.0.0.1";
    if(argc >= 3){
        Game.ClientPointersObj->ipAdress = argv[1];
        if(strcmp(argv[1], "0") == 0 || strcmp(argv[1], "localhost") == 0)
            Game.ClientPointersObj->ipAdress = "127.0.0.1";
        Game.ClientPointersObj->port = atoi(argv[2]);
    }
    cout << "Connecting to " << Game.ClientPointersObj->ipAdress << ":" << Game.ClientPointersObj->port << "\n";



    do{
        GameEngine.gameLoop(Game);
    }while(!GameEngine.closeGame);

    //close(socketFD);

    Game.destroyObjects();
    GameEngine.destroyEngine();
    return 0;
}
