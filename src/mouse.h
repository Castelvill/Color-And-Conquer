#ifndef MOUSE_H_INCLUDED
#define MOUSE_H_INCLUDED

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <vector>
#include "vectors.h"
#include "usefull.h"

using std::vector;

vector <short> getPressedKeys(unsigned char key[]);
vector <short> getReleasedKeys(unsigned char key[], vector <short> pressedKeys);


class MouseClass{
private:
    vec2f pos;
    vec2f pressedPos;
    bool pressed[5];
    bool released[5];
public:
    bool doButtonsCancelEachOther;
    void reset();
    void resetReleased();
    void setUp(bool newDoButtonsCancelEachOther);
    void getPressed(bool tempArr[]);
    void getReleased(bool tempArr[]);
    vec2f getPos();
    void updateAxes(ALLEGRO_EVENT event);
    void updateButtons(ALLEGRO_EVENT event);
    void updateButtons1(ALLEGRO_EVENT event);
    bool inRectangle(vec2f rPos, vec2f rSize);
    bool pressedInRectangle(vec2f rPos, vec2f rSize, short button);
    bool firstPositionInRectangle(vec2f rPos, vec2f rSize, short button);
    bool releasedInRectangle(vec2f rPos, vec2f rSize, short button);
    bool inRadius(vec2f rPos, double radius);
    bool pressedInRadius(vec2f rPos, double radius, short button);
    bool releasedInRadius(vec2f rPos, double radius, short button);
    bool wasMousePressed();
};


#endif // MOUSE_H_INCLUDED
