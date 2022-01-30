#include "serverClient.h"

int charInt(char character){
    return character - 48;
}

void popCharFromBuffer(char * buf, char &value, int &cursor){
    value = buf[cursor];
    cursor++;
}
void popBoolFromBuffer(char * buf, bool &value, int &cursor){
    if(buf[cursor] == 't')
        value = true;
    else if(buf[cursor] == 'f')
        value = false;
    cursor++;
}
void popIntFromBuffer(char * buf, int &value, int &cursor){
    value = 0;
    do{
        value *= 10;
        value += buf[cursor]-48;
        cursor++;
    }while(buf[cursor] != ';');
    cursor++;
}
void popUnIntFromBuffer(char * buf, unsigned int &value, int &cursor){
    value = 0;
    do{
        value *= 10;
        value += buf[cursor]-48;
        cursor++;
    }while(buf[cursor] != ';');
    cursor++;
}
void popStringFromBuffer(char * buf, string &value, int &cursor, int bufSize, int stringSize){
    value = "";
    if(bufSize == 0){
        return;
    }
    for(int i = 0; i < stringSize; i++){
        if(cursor == bufSize)
            return;
        value += buf[cursor];
        cursor++;
    }
}
string getStringFromBuffer(char * buf, int cursor, int bufSize, int stringSize){
    if(bufSize == 0){
        return "";
    }
    string value = "";
    for(int i = 0; i < stringSize; i++){
        if(cursor == bufSize)
            break;
        value += buf[cursor];
        cursor++;
    }
    return value;
}
int writeBytes(int fd, string buf, int size){
	int bTotal = 0;
	while(bTotal < size){
		bTotal += write(fd, buf.substr(bTotal, size).c_str(), size - bTotal);
	}
	return bTotal;
}
bool request(char * buf, string command, int &cursor){
    if(sizeof(buf) < 3 || command.size() != 3)
        return false;
    for(int i = 0; i < 3; i++){
        if(buf[cursor+i] != command[i])
            return false;
    }
    cursor += 3;
    return true;
}
void putOnCout(char * buf){
    for(unsigned int i=0; i < sizeof(buf); i++){
        cout << buf[i];
    }
    cout << "\n";
}
void putOnCout(char * buf, int size){
    for(int i=0; i < size; i++){
        cout << buf[i];
    }
    cout << "\n";
}


void resetCell(Cell & one){
    one.r = char(100);
    one.g = char(100);
    one.b = char(100);
    one.isWall = false;
}



void MaptrixClass::loadMap(string mapName){
    Maptrix.clear();
    std::fstream mapFile;
    mapFile.open(mapName.c_str(), std::ios_base::in);

    string temp;
    mapFile >> temp;
    int mapSizeX = stoi(temp);
    mapFile >> temp;
    int mapSizeY = stoi(temp);

    vector <Cell> CellsRow;
    for(int j = 0; j < mapSizeX; j++){
        CellsRow.push_back(Cell());
    }
    for(int j = 0; j < mapSizeY; j++){
        Maptrix.push_back(CellsRow);
    }
    resetState();

    for(int y = 0; y < mapSizeY; y++){
        mapFile >> temp;
        for(int x = 0; x < mapSizeX; x++){
            if(temp[x] == '1')
                Maptrix[y][x].isWall = true;
            else
                Maptrix[y][x].isWall = false;
        }
    }
    mapFile.close();
}
unsigned int MaptrixClass::getMapSizeY(){
    return Maptrix.size();
}
unsigned int MaptrixClass::getMapSizeX(){
    return Maptrix[0].size();
}
void MaptrixClass::resetState(){
    for(unsigned int y = 0; y < getMapSizeY(); y++){
        for(unsigned int x = 0; x < getMapSizeX(); x++){
            resetCell(Maptrix[y][x]);
        }
    }
}
void MaptrixClass::create(unsigned int xSize,unsigned int ySize){
    Maptrix.clear();
    vector <Cell> row;
    for(unsigned int i = 0; i < xSize; i++){
        row.push_back(Cell());
    }
    for(unsigned int j = 0; j < ySize; j++){
        Maptrix.push_back(row);
    }
    resetState();
}
void MaptrixClass::coutMST(){
    for(unsigned int y = 0; y < getMapSizeY(); y++){
        for(unsigned int x = 0; x < getMapSizeX(); x++)
            cout << "(" << int(Maptrix[y][x].r)+128 << ", " << int(Maptrix[y][x].g)+128 << " " << int(Maptrix[y][x].b)+128 << ") ";
        cout << "\n";
    }
    cout << "\n";
}
void MaptrixClass::reciveMSI(char * buf, int &cursor, int size){
    if(cursor >= size)
        return;
    int xSize, ySize;
    popIntFromBuffer(buf, xSize, cursor);
    popIntFromBuffer(buf, ySize, cursor);
    //cout << "New map size: " << xSize << "x" << ySize << "\n";
    create(xSize, ySize);
}
void MaptrixClass::reciveMWL(char * buf, int &cursor, int size){
    for(unsigned int y = 0; y < getMapSizeY(); y++){
        for(unsigned int x = 0; x < getMapSizeX(); x++){
            if(cursor < size){
                popBoolFromBuffer(buf, Maptrix[y][x].isWall, cursor);
            }
        }
    }
}
void MaptrixClass::reciveMST(char * buf, int &cursor, int size){
    if(size == 0)
        return;
    for(unsigned int y = 0; y < getMapSizeY(); y++){
        for(unsigned int x = 0; x < getMapSizeX(); x++){
            popCharFromBuffer(buf, Maptrix[y][x].r, cursor);
            popCharFromBuffer(buf, Maptrix[y][x].g, cursor);
            popCharFromBuffer(buf, Maptrix[y][x].b, cursor);
        }
    }
}
string MaptrixClass::sendMSI(){ //send map size
    string message = "MSI";
    message += to_string(getMapSizeX()) + ";";
    message += to_string(getMapSizeY()) + ";";
    return message;
}
string MaptrixClass::sendMWL(){
    string message = "MWL";
    for(unsigned int y = 0; y < getMapSizeY(); y++){
        for(unsigned int x = 0; x < getMapSizeX(); x++){
            if(Maptrix[y][x].isWall)
                message += 't';
            else
                message += 'f';
        }
    }
    return message;
}
string MaptrixClass::sendMST(){
    string message = "MST";
    for(unsigned int y = 0; y < getMapSizeY(); y++){
        for(unsigned int x = 0; x < getMapSizeX(); x++){
            message += Maptrix[y][x].r;
            message += Maptrix[y][x].g;
            message += Maptrix[y][x].b;
        }
    }
    return message;
}
void MaptrixClass::updateCell(Player NewUpdate){
    if(NewUpdate.x < 0 || NewUpdate.x >= (int)getMapSizeX() || NewUpdate.y < 0 || NewUpdate.y >= (int)getMapSizeY()){
        cout << "ERR: Player out of map's bounds!\n";
        return;
    }
    if(Maptrix[NewUpdate.y][NewUpdate.x].isWall == true){
        cout << "ERR: Player is in the wall!\n";
        return;
    }
    Maptrix[NewUpdate.y][NewUpdate.x].r = NewUpdate.r;
    Maptrix[NewUpdate.y][NewUpdate.x].g = NewUpdate.g;
    Maptrix[NewUpdate.y][NewUpdate.x].b = NewUpdate.b;
}
void MaptrixClass::deleteColor(char r, char g, char b){
    for(unsigned int y = 0; y < getMapSizeY(); y++){
        for(unsigned int x = 0; x < getMapSizeX(); x++){
            if(Maptrix[y][x].r == r && Maptrix[y][x].g == g && Maptrix[y][x].b == b){
                Maptrix[y][x].r = char(100);
                Maptrix[y][x].g = char(100);
                Maptrix[y][x].b = char(100);
            }
        }
    }
}
int MaptrixClass::getNumberOfColoredCells(char r, char g, char b){
    int numberOfCells = 0;
    for(unsigned int y = 0; y < getMapSizeY(); y++){
        for(unsigned int x = 0; x < getMapSizeX(); x++){
            if(Maptrix[y][x].r == r && Maptrix[y][x].g == g && Maptrix[y][x].b == b){
                numberOfCells++;
            }
        }
    }
    return numberOfCells;
}
void MaptrixClass::randomizePositions(vector <Player> & PlayersList){
    for(auto &Ply : PlayersList){
        do{
            Ply.x = rand()%getMapSizeX();
            Ply.y = rand()%getMapSizeY();
        }while(Maptrix[Ply.y][Ply.x].isWall);

    }
}

bool isMoveLegal(int originalX, int originalY, int newX, int newY){
    if(newX < originalX-1 || newX > originalX+1)
        return false;
    if(newY < originalY-1 || newY > originalY+1)
        return false;
    return true;
}
PlayersListClass::PlayersListClass(){
    changesInTheList = false;
}
bool PlayersListClass::addPlayer(char * name, unsigned int maxPlayerCount, int nameLength, int maxNameLength, int mapSizeX, int mapSizeY){
    if(PlayersList.size() >= maxPlayerCount)
        return false;
    int ID = rand()%maxPlayerCount;
    for(unsigned int i = 0; i < PlayersList.size(); i++){
        if(ID == PlayersList[i].ID){
            i = -1;
            ID = rand()%maxPlayerCount;
        }
    }
    char r = char(rand()%256);
    char g = char(rand()%256);
    char b = char(rand()%256);
    for(unsigned int i = 0; i < PlayersList.size(); i++){
        if(r == PlayersList[i].r && g == PlayersList[i].g && b == PlayersList[i].b){
            i = -1;
            r = char(rand()%256);
            g = char(rand()%256);
            b = char(rand()%256);
        }
    }
    PlayersList.push_back(Player());
    PlayersList.back().name = "";
    for(int i = 0; i < maxNameLength; i++){
        if(i < nameLength)
            PlayersList.back().name += name[i];
        else
            PlayersList.back().name += '_';
    }

    PlayersList.back().ID = ID;
    PlayersList.back().x = rand()%mapSizeX;
    PlayersList.back().y = rand()%mapSizeY;
    PlayersList.back().r = r;
    PlayersList.back().g = g;
    PlayersList.back().b = b;
    PlayersList.back().score = 0;
    PlayersList.back().newRound = false;

    return true;
}
void PlayersListClass::delPlayer(int playerID){
    int playerIndex = getIndex(playerID);
    if(playerIndex < 0){
        cout << "ERR: Attempt of deleting a non-existing player!\n";
        return;
    }
    cout << "Player '" << PlayersList[playerIndex].name << "' has been deleted!\n";
    PlayersList.erase(PlayersList.begin()+playerIndex);
    changesInTheList = true;
}
string PlayersListClass::sendPOS(int ID){
    int index = getIndex(ID);
    string message = "POS";
    message += to_string(ID) + ";";
    message += to_string(PlayersList[index].x) + ";";
    message += to_string(PlayersList[index].y) + ";";
    return message;
}
int PlayersListClass::getIndex(int ID){
    if(PlayersList.size() == 0)
        return -1;
    for(unsigned int i = 0; i < PlayersList.size(); i++){
        if(PlayersList[i].ID == ID){
            return i;
        }

    }
    return -2;
}
string PlayersListClass::sendPLY(int maxNameLength){
    if(PlayersList.size() == 0){
        return "";
    }
    string message = "PLY";
    if(changesInTheList)
        message += "t";
    else
        message += "f";
    message += to_string(PlayersList.size()) + ";";
    message += to_string(maxNameLength) + ";";
    for(unsigned i = 0; i < PlayersList.size(); i++){ //send list of current players
        message += PlayersList[i].name;
        message += to_string(PlayersList[i].ID) + ";";
        message += to_string(PlayersList[i].x) + ";";
        message += to_string(PlayersList[i].y) + ";";
        message += to_string(PlayersList[i].score) + ";";
        message += PlayersList[i].r;
        message += PlayersList[i].g;
        message += PlayersList[i].b;
    }
    return message;
}
void PlayersListClass::recivePOS(char * buf, int &cursor, int size, unsigned int playerIndex){
    //Function updates player's position if the move was legal
    if(size == 0)
        return;
    if(PlayersList.size() == 0){
        cout << "ERR: Player count: 0\n";
        return;
    }
    if(playerIndex >= PlayersList.size()){
        cout << "ERR: Player's ID not found\n";
        return;
    }
    int newX, newY;
    popIntFromBuffer(buf, newX, cursor);
    popIntFromBuffer(buf, newY, cursor);
    if(isMoveLegal(PlayersList[playerIndex].x, PlayersList[playerIndex].y, newX, newY)){
        PlayersList[playerIndex].x = newX;
        PlayersList[playerIndex].y = newY;
    }
}
void PlayersListClass::recivePLY(char * buf, int &cursor, int size){
    bool changes;
    if(buf[cursor] == 't')
        changes = true;
    else if(buf[cursor] == 'f')
        changes = true;
    cursor++;
    unsigned int listSize;
    popUnIntFromBuffer(buf, listSize, cursor);
    if(changes || listSize != PlayersList.size()){
        PlayersList.clear();
        for(unsigned int i = 0; i < listSize; i++){
            PlayersList.push_back(Player());
        }
    }
    int maxNameLength = 8;
    popIntFromBuffer(buf, maxNameLength, cursor);
    for(unsigned int i = 0; i < listSize; i++){
        popStringFromBuffer(buf, PlayersList[i].name, cursor, size, maxNameLength);
        popIntFromBuffer(buf, PlayersList[i].ID, cursor);
        popIntFromBuffer(buf, PlayersList[i].x, cursor);
        popIntFromBuffer(buf, PlayersList[i].y, cursor);
        popIntFromBuffer(buf, PlayersList[i].score, cursor);
        popCharFromBuffer(buf, PlayersList[i].r, cursor);
        popCharFromBuffer(buf, PlayersList[i].g, cursor);
        popCharFromBuffer(buf, PlayersList[i].b, cursor);
    }
}
bool PlayersListClass::doesNameExist(char * buf, int size, int max_name_length){
    string checkingName = getStringFromBuffer(buf, 0, size, max_name_length);
    for(int i = checkingName.size(); i < max_name_length; i++){
        checkingName += "_";
    }
    for(auto Ply : PlayersList){
        if(Ply.name == checkingName)
            return true;
    }
    return false;
}
