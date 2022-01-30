#include "mouse.h"

vector <short> getPressedKeys(unsigned char key[]){
    vector <short> pressedKeys;
    for(int i = 1; i < ALLEGRO_KEY_MAX; i++){
        if(key[i]){
            pressedKeys.push_back(short());
            pressedKeys[pressedKeys.size()-1] = i;
        }
    }
    return pressedKeys;
}

vector <short> getReleasedKeys(unsigned char key[], vector <short> pressedKeys){
    vector <short> releasedKeys;
    for(unsigned int i = 0; i < pressedKeys.size(); i++){
        if(!key[pressedKeys[i]]){
            releasedKeys.push_back(short());
            releasedKeys[releasedKeys.size()-1] = pressedKeys[i];
        }
    }
    return releasedKeys;
}


void MouseClass::reset(){
    for(int i = 0; i < 5; i++){
        pressed[i] = false;
        released[i] = false;
    }
}
void MouseClass::resetReleased(){
    for(int i = 0; i < 5; i++){
        released[i] = false;
    }
}
void MouseClass::setUp(bool newDoButtonsCancelEachOther){
    reset();
    doButtonsCancelEachOther = newDoButtonsCancelEachOther;
}
void MouseClass::getPressed(bool tempArr[]){
    for(int i = 0; i < 5; i++){
        tempArr[i] = pressed[i];
    }
}
void MouseClass::getReleased(bool tempArr[]){
    for(int i = 0; i < 5; i++){
        tempArr[i] = released[i];
    }
}
vec2f MouseClass::getPos(){
    return pos;
}
void MouseClass::updateAxes(ALLEGRO_EVENT event){
    pos.set(event.mouse.x, event.mouse.y);
}
void MouseClass::updateButtons(ALLEGRO_EVENT event){
    reset();
    if((event.mouse.button & 1) && (event.mouse.button & 2) && (event.mouse.button & 3)){
        pressedPos.set(pos);
        pressed[2] = true;
    }
    else if((event.mouse.button & 1) && (event.mouse.button & 3) && (event.mouse.button & 4)){
        pressedPos.set(pos);
        pressed[3] = true;
    }
    else if(event.mouse.button & 1){
        pressedPos.set(pos);
        pressed[0] = true;
    }
    else if(event.mouse.button & 2){
        pressedPos.set(pos);
        pressed[1] = true;
    }
    else if(event.mouse.button & 4){
        pressedPos.set(pos);
        pressed[4] = true;
    }


    /*for(i = 0; i < 5; i++){
        if(pressed[i]){
            pressed[i] = false;
            released[i] = true;
        }
    }*/
    /*if(doButtonsCancelEachOther){
        for(i = 0; i < 5; i++){
            if(pressed[i]){
                for(int j = 0; j < 5; j++){
                    if(i == j)
                        continue;
                    released[j] = false;
                }
                break;
            }
        }
    }*/
}
void MouseClass::updateButtons1(ALLEGRO_EVENT event){
    reset();
    if((event.mouse.button & 1) && (event.mouse.button & 2) && (event.mouse.button & 3)){
        released[2] = true;
    }
    else if((event.mouse.button & 1) && (event.mouse.button & 3) && (event.mouse.button & 4)){
        released[3] = true;
    }
    else if(event.mouse.button & 1){
        released[0] = true;
    }
    else if(event.mouse.button & 2){
        released[1] = true;
    }
    else if(event.mouse.button & 4){
        released[4] = true;
    }
}
bool MouseClass::inRectangle(vec2f rPos, vec2f rSize){
    if(pos.x >= rPos.x && pos.x <= rPos.x + rSize.x && pos.y >= rPos.y && pos.y <= rPos.y + rSize.y){
        return true;
    }
    return false;
}
bool MouseClass::pressedInRectangle(vec2f rPos, vec2f rSize, short button){
    if(pressed[button]){
        if(pos.x >= rPos.x && pos.x <= rPos.x + rSize.x && pos.y >= rPos.y && pos.y <= rPos.y + rSize.y){
            return true;
        }
    }
    return false;
}
bool MouseClass::firstPositionInRectangle(vec2f rPos, vec2f rSize, short button){
    if(pressed[button]){
        if(pressedPos.x >= rPos.x && pressedPos.x <= rPos.x + rSize.x && pressedPos.y >= rPos.y && pressedPos.y <= rPos.y + rSize.y){
            return true;
        }
    }
    return false;
}
bool MouseClass::releasedInRectangle(vec2f rPos, vec2f rSize, short button){
    if(released[button]){
        if(pressedPos.x >= rPos.x && pressedPos.x <= rPos.x + rSize.x && pressedPos.y >= rPos.y && pressedPos.y <= rPos.y + rSize.y){
            if(pos.x >= rPos.x && pos.x <= rPos.x + rSize.x && pos.y >= rPos.y && pos.y <= rPos.y + rSize.y){
                return true;
            }
        }
    }
    return false;
}
bool MouseClass::inRadius(vec2f rPos, double radius){
    if(countDistance2(pos.x, pos.y,rPos.x, rPos.y) <= radius){
        return true;
    }
    return false;
}
bool MouseClass::pressedInRadius(vec2f rPos, double radius, short button){
    if(pressed[button]){
        if(countDistance2(pos.x, pos.y, rPos.x, rPos.y) <= radius){
            return true;
        }
    }
    return false;
}
bool MouseClass::releasedInRadius(vec2f rPos, double radius, short button){
    if(released[button]){
        if(countDistance2(pressedPos.x, pressedPos.y, rPos.x, rPos.y) <= radius){
            if(countDistance2(pos.x, pos.y, rPos.x, rPos.y) <= radius){
                return true;
            }
        }
    }
    return false;
}
bool MouseClass::wasMousePressed(){
    for(unsigned int i = 0; i < 5; i++){
        if(pressed[i])
            return true;
    }
    return false;
}
