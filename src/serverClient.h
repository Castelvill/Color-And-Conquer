#ifndef SERVERCLIENT_H_INCLUDED
#define SERVERCLIENT_H_INCLUDED

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/wait.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sstream>
#include <vector>
#include <iostream>
#include <fstream>

using std::string, std::vector, std::cout, std::to_string;


struct Player{
    string name;
    int ID;
    int x, y;
    int score;
    char r, g, b;
    bool newRound;
};
class PlayersListClass{
public:
    vector <Player> PlayersList;
    bool changesInTheList;

    PlayersListClass();
    int getIndex(int ID);
    bool addPlayer(char * name, unsigned int maxPlayerCount, int nameLength, int maxNameLength, int mapSizeX, int mapSizeY);
    void delPlayer(int playerID);
    string sendPOS(int ID);
    string sendPLY(int maxNameLength);
    void recivePOS(char * buf, int &cursor, int size, unsigned int ID);
    void recivePLY(char * buf, int &cursor, int size);
    bool doesNameExist(char * buf, int size, int max_name_length);
};


struct Cell{
    char r, g, b;
    bool isWall;
};
void resetCell(Cell & one);

class MaptrixClass{
public:
    vector <vector <Cell>> Maptrix;
    void loadMap(string mapName);
    unsigned int getMapSizeX();
    unsigned int getMapSizeY();
    void resetState();
    void create(unsigned int xSize,unsigned int ySize);
    void updateCell(Player NewUpdate);
    void deleteColor(char r, char g, char b);
    int getNumberOfColoredCells(char r, char g, char b);
    void randomizePositions(vector <Player> & PlayersList);

    void reciveMSI(char * buf, int &cursor, int size);
    void reciveMWL(char * buf, int &cursor, int size);
    void reciveMST(char * buf, int &cursor, int size);
    string sendMSI();
    string sendMWL();
    string sendMST();
    void coutMST();

};




int charInt(char character);

void popCharFromBuffer(char * buf, char &value, int &cursor);
void popBoolFromBuffer(char * buf, bool &value, int &cursor);
void popIntFromBuffer(char * buf, int &value, int &cursor);
void popUnIntFromBuffer(char * buf, unsigned int &value, int &cursor);
void popStringFromBuffer(char * buf, string &value, int &cursor, int bufSize, int stringSize);
string getStringFromBuffer(char * buf, int cursor, int bufSize, int stringSize);
int writeBytes(int fd, string buf, int size);
bool request(char * buf, string command, int &cursor);
void putOnCout(char * buf);
void putOnCout(char * buf, int size);

bool isMoveLegal(int orginalX, int originalY, int newX, int newY);


#endif
