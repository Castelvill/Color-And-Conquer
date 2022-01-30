#include "engine.h"


void Engine::initAllegro(){
    closeGame = false;
    isMenuScreen = true;
    isGameRunning = false;

    srand(time(NULL));
    al_init();

    al_install_keyboard();
    al_init_ttf_addon();
    al_init_image_addon();
    al_install_mouse();

    window = al_create_display(windowSize.x, windowSize.y);
    al_set_window_title( window,"Color and Conquer");
    al_set_system_mouse_cursor(window, ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT);
    al_set_new_bitmap_flags(ALLEGRO_MIN_LINEAR);
    fontMenu = al_load_ttf_font("fonts/FreeSansBold.ttf",50, 0);
    fontError = al_load_ttf_font("fonts/FreeSansBold.ttf",26, 0);

    memset(key, 0, sizeof(key));

    eventQueue = al_create_event_queue();
    timer = al_create_timer(1.0 / FPS);
    al_register_event_source(eventQueue, al_get_keyboard_event_source());
    al_register_event_source(eventQueue, al_get_display_event_source(window));
    al_register_event_source(eventQueue, al_get_timer_event_source(timer));
    al_register_event_source(eventQueue, al_get_mouse_event_source());
    al_start_timer(timer);
    Mouse.setUp(false);

//Menu screen
    typedName = "";
    areYouTyping = true;
    blinkingTime = 100;
    refreshErrors = 0;
}
void Engine::gameLoop(GameClass & Game){

    bool redraw = false;
    do{
       al_wait_for_event(eventQueue , &event);
       switch(event.type){
            case ALLEGRO_EVENT_TIMER:
                //Keyboard input is gathered here
                releasedKeys.clear();
                releasedKeys = getReleasedKeys(key, pressedKeys);
                pressedKeys.clear();
                pressedKeys = getPressedKeys(key);

                if(Game.ClientPointersObj->connected){
                    //Game.ClientPointersObj->connectionFailed = false;
                    isMenuScreen = false;
                    isGameRunning = true;
                }
                if(isGameRunning){
                    Game.moveThePlayer(pressedKeys);
                }
                if(isMenuScreen){
                    if(blinkingTime <= 0){
                        blinkingTime = 100;
                    }
                    else{
                        blinkingTime--;
                    }
                    if(Game.ClientPointersObj->connectionFailed || Game.ClientPointersObj->nameIsWrong
                        || Game.ClientPointersObj->nameExists || Game.ClientPointersObj->serverIsFull){
                        if(refreshErrors == 0){
                            refreshErrors = 200;
                        }
                    }
                    if(refreshErrors > 0)
                        refreshErrors--;
                    if(refreshErrors == 0){
                        Game.ClientPointersObj->connectionFailed = false;
                        Game.ClientPointersObj->nameIsWrong = false;
                        Game.ClientPointersObj->nameExists = false;
                        Game.ClientPointersObj->serverIsFull = false;
                    }
                    if(Mouse.releasedInRectangle(vec2f(10, 170), vec2f(200, 50), 0)){
                        Game.ClientPointersObj->tryToConnect = true;
                        Game.ClientPointersObj->typedName = typedName;
                        cout << "Connecting...\n";
                    }
                    int special_key = 0;
                    for(unsigned int i = 0; i < pressedKeys.size(); i++){
                        if(pressedKeys[i] == ALLEGRO_KEY_RSHIFT)
                            special_key = pressedKeys[i];
                    }
                    unsigned int previousLength = typedName.size();
                    for(unsigned int i = 0; i < releasedKeys.size(); i++){
                        if(releasedKeys[i] == ALLEGRO_KEY_BACKSPACE && typedName.size()>0){
                            typedName = typedName.substr(0, typedName.size()-1);
                        }
                        else if(releasedKeys[i] >= 1 && releasedKeys[i] <= 26 && special_key != ALLEGRO_KEY_RSHIFT)
                            typedName += 'a'+releasedKeys[i]-1;
                        else if(releasedKeys[i] >= 1 && releasedKeys[i] <= 26)
                            typedName += 'A'+releasedKeys[i]-1;
                        if(releasedKeys[i] == 61 && special_key == ALLEGRO_KEY_RSHIFT)
                            typedName += '_';
                        else if(releasedKeys[i] >= 27 && releasedKeys[i] <= 36)
                            typedName += releasedKeys[i]+21;
                    }
                    if(previousLength != typedName.size())
                        blinkingTime = 100;

                }

                if(key[ALLEGRO_KEY_ESCAPE])
                    closeGame = true;
                for(int i = 0; i < ALLEGRO_KEY_MAX; i++)
                    key[i] &= KEY_SEEN;
                redraw = true;
                Mouse.resetReleased();
                break;
            case ALLEGRO_EVENT_MOUSE_AXES:
                Mouse.updateAxes(event);
                break;
            case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
                Mouse.updateButtons(event);
                break;
            case ALLEGRO_EVENT_MOUSE_BUTTON_UP:
                Mouse.updateButtons1(event);

                break;
            case ALLEGRO_EVENT_KEY_DOWN:
                key[event.keyboard.keycode] = KEY_SEEN | KEY_RELEASED;
                break;
            case ALLEGRO_EVENT_KEY_UP:
                key[event.keyboard.keycode] &= KEY_RELEASED;
                break;

            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                closeGame = true;
                break;
        }
    } while (!al_is_event_queue_empty(eventQueue));


    if(redraw){
        if(isMenuScreen)
            drawMenuScreen(Game.ClientPointersObj);

        if(isGameRunning)
            Game.drawGame(window);

        al_flip_display();
        al_clear_to_color(al_map_rgb_f(0.0, 0.0, 0.0));

        redraw = false;
    }

    if(closeGame){
        Game.ClientPointersObj->disconnecting = true;
    }

}
void Engine::destroyEngine(){
    al_destroy_display(window);
    al_destroy_timer(timer);
    al_destroy_event_queue(eventQueue);
    al_destroy_font(fontMenu);
    al_destroy_font(fontError);
    releasedKeys.clear();
    pressedKeys.clear();
}
void Engine::drawMenuScreen(ClientPointersStruct * ClientPointersObj){
    al_clear_to_color(al_map_rgb(100, 230, 230));
    al_draw_textf(fontMenu, al_map_rgb(0, 130, 130), 10, 10, 0, "Enter name: ");
    al_draw_textf(fontMenu, al_map_rgb(0, 130, 130), 10, 80, 0, typedName.c_str(), 0);
    int text_width = al_get_text_width(fontMenu, typedName.c_str());
    if(blinkingTime < 50)
        al_draw_filled_rectangle(10+text_width, 80, 30+text_width, 130, al_map_rgb(0, 130, 130));
    if(Mouse.inRectangle(vec2f(10, 170), vec2f(200, 50)))
        al_draw_textf(fontMenu, al_map_rgb(255, 255, 255), 10, 170, 0, "Connect");
    else
        al_draw_textf(fontMenu, al_map_rgb(0, 130, 130), 10, 170, 0, "Connect");

    if(ClientPointersObj->connectionFailed)
        al_draw_textf(fontError, al_map_rgb(200, 0, 100), 10, 180+50, 0, "Connection failed!");
    if(ClientPointersObj->nameIsWrong)
        al_draw_textf(fontError, al_map_rgb(200, 0, 100), 10, 180+50*2, 0, "Name is somehow wrong!");
    if(ClientPointersObj->nameExists)
        al_draw_textf(fontError, al_map_rgb(200, 0, 100), 10, 180+50*3, 0, "Name is already in use!");
    if(ClientPointersObj->serverIsFull)
        al_draw_textf(fontError, al_map_rgb(200, 0, 100), 10, 180+50*4, 0, "Server is full!");
}
