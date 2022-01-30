#include "gameClass.h"
#include <chrono>
#include <thread>

vec2i adjustPos(10, 50);

MaptrixClass MaptrixObj;
PlayersListClass PlayersListObj;
vec2i playerPos;
char playerColorR;
char playerColorG;
char playerColorB;
int playerID;
vector <string> rankingText;


void reciveACC(char * buf, int &cursor, int size){
    if(cursor >= size)
        return;
    int i;
    int maxNameLength;
    popIntFromBuffer(buf, maxNameLength, cursor);

    cout << "name: ";
    for(i = 0; i < maxNameLength; i++){
        cout << buf[cursor];
        cursor++;
    }
    int temp;
    cout << "\nID: ";
    popIntFromBuffer(buf, temp, cursor);
    cout << temp;
    playerID = temp;

    popIntFromBuffer(buf, temp, cursor);
    cout << "\nx: " << temp;
    playerPos.x = temp;
    popIntFromBuffer(buf, temp, cursor);
    cout << "\ny: " << temp;
    playerPos.y = temp;

    char r, g, b;
    r = buf[cursor];
    cursor++;
    g = buf[cursor];
    cursor++;
    b = buf[cursor];
    cursor++;
    cout << "\nrgb: " << int(r)+128 << ", " << int(g)+128 << ", " << int(b)+128 << "\n";
    playerColorR = r;
    playerColorR = g;
    playerColorR = b;
}

void loadMap(vector <vector <Cell>> & GameMap, string mapName, vec2i & mapSize){
    GameMap.clear();
    std::fstream mapFile;
    mapFile.open(mapName.c_str(), std::ios_base::in);

    string temp;
    mapFile >> temp;

    mapSize.x = atoi(temp.c_str());
    mapFile >> temp;
    mapSize.y = atoi(temp.c_str());

    vector <Cell> CellsRow;

    for(int i = 0; i < mapSize.y; i++){
        mapFile >> temp;
        for(int j = 0; j < mapSize.x; j++){
            CellsRow.push_back(Cell());
            if(temp[j] == '1')
                CellsRow.back().isWall = true;
        }
        GameMap.push_back(CellsRow);
        CellsRow.clear();
    }
    mapFile.close();
}

void GameClass::prepareGameObjects(){
    MaptrixObj.create(5, 5);
    ClientPointersObj = new ClientPointersStruct;
    ClientPointersObj->tryToConnect = false;
    ClientPointersObj->connectionFailed = false;
    ClientPointersObj->nameIsWrong = false;
    ClientPointersObj->nameExists = false;
    ClientPointersObj->serverIsFull = false;
    ClientPointersObj->connected = false;
    ClientPointersObj->typedName = "";
    ClientPointersObj->disconnecting = false;
    pthread_t clientThreadID;
    pthread_create(&clientThreadID, NULL, clientThread, ClientPointersObj);
    pthread_detach(clientThreadID);

    moveCooldown = 0;
    int randFace = 1+rand()%6;
    string faceFile = "img/face" + std::to_string(randFace) + ".png";
    player = al_load_bitmap(faceFile.c_str());
    floor1 = al_load_bitmap("img/floor1.png");
    wallBitmap1 = al_load_bitmap("img/wall1.png");
    floorColorBitmap = al_create_bitmap(50, 50);
    fontSans = al_load_ttf_font("fonts/FreeSansBold.ttf",24, 0);
    fontLittleSans = al_load_ttf_font("fonts/FreeSansBold.ttf",14, 0);


    MaptrixObj.resetState();
    mapSize.set(MaptrixObj.getMapSizeX(), MaptrixObj.getMapSizeY());

    do{
        playerPos.set(rand()%mapSize.x, rand()%mapSize.y);
    }while(MaptrixObj.Maptrix[playerPos.y][playerPos.x].isWall);

    playerColorR = char(rand()%256);
    playerColorG = char(rand()%256);
    playerColorB = char(rand()%256);
}
void GameClass::destroyObjects(){
    al_destroy_font(fontSans);
    al_destroy_font(fontLittleSans);

    al_destroy_bitmap(floorColorBitmap);
    al_destroy_bitmap(player);
    al_destroy_bitmap(wallBitmap1);

    al_destroy_bitmap(floor1);
}
void GameClass::drawGame(ALLEGRO_DISPLAY *window){
    al_clear_to_color(al_map_rgb_f(0.5,0.5,0.5));

    for(unsigned int y = 0; y < MaptrixObj.getMapSizeY(); y++){
        for(unsigned int x = 0; x < MaptrixObj.getMapSizeX(); x++){
            al_set_target_bitmap(floorColorBitmap);
            int r = int(MaptrixObj.Maptrix[y][x].r)+128;
            int g = int(MaptrixObj.Maptrix[y][x].g)+128;
            int b = int(MaptrixObj.Maptrix[y][x].b)+128;
            al_clear_to_color(al_map_rgb(r, g, b));
            al_set_target_bitmap(al_get_backbuffer(window));
            al_draw_bitmap(floorColorBitmap, adjustPos.x+x*50, adjustPos.y+y*50, 0);

        }
    }
    al_set_target_bitmap(al_get_backbuffer(window));

    for(unsigned int y = 0; y < MaptrixObj.getMapSizeY(); y++){
        for(unsigned int x = 0; x < MaptrixObj.getMapSizeX(); x++){
            al_draw_bitmap(floor1, adjustPos.x+x*50, adjustPos.y+y*50, 0);
            if(MaptrixObj.Maptrix[y][x].isWall){
                al_draw_bitmap(wallBitmap1, adjustPos.x+x*50, adjustPos.y+y*50, 0);
            }
        }
    }
    for(auto ply : PlayersListObj.PlayersList){
        int startX = adjustPos.x + ply.x*50;
        int startY = adjustPos.y + ply.y*50;
        al_draw_bitmap(player, startX, startY, 0);
        int textWidth = al_get_text_width(fontLittleSans, ply.name.c_str());
        al_draw_filled_rectangle(startX-5, startY-20, startX+textWidth-5, startY-6, al_map_rgba(0, 0, 0, 200));
        al_draw_textf(fontLittleSans, al_map_rgb(255, 255, 255), startX-5, startY-20, 0, ply.name.c_str(), 0);
    }

    for(unsigned int i = 0; i < PlayersListObj.PlayersList.size(); i++){
        string scoreLine = PlayersListObj.PlayersList[i].name + ": " + to_string(PlayersListObj.PlayersList[i].score);
        al_draw_textf(fontLittleSans, al_map_rgb(255,255,0), al_get_display_width(window)-140, 10+i*16, 0, scoreLine.c_str(), 0);
    }


}
void GameClass::moveThePlayer(vector <short> pressedKeys){
    if(moveCooldown > 0){
        moveCooldown--;
        return;
    }
    vec2i newPos(playerPos);

    mapSize.set(MaptrixObj.getMapSizeX(), MaptrixObj.getMapSizeY());

    for(unsigned int i = 0; i < pressedKeys.size(); i++){
        if(pressedKeys[i] == ALLEGRO_KEY_UP && playerPos.y > 0){
            newPos.y -= 1;
        }
        if(pressedKeys[i] == ALLEGRO_KEY_RIGHT && playerPos.x < mapSize.x-1){
            newPos.x += 1;
        }
        if(pressedKeys[i] == ALLEGRO_KEY_DOWN && playerPos.y < mapSize.y-1){
            newPos.y += 1;
        }
        if(pressedKeys[i] == ALLEGRO_KEY_LEFT && playerPos.x > 0){
            newPos.x -= 1;
        }
    }

    if(!MaptrixObj.Maptrix[newPos.y][newPos.x].isWall){
        moveCooldown = 10;
        playerPos.set(newPos);
        /*MaptrixObj.Maptrix[playerPos.y][playerPos.x].r = playerColorR;
        MaptrixObj.Maptrix[playerPos.y][playerPos.x].g = playerColorG;
        MaptrixObj.Maptrix[playerPos.y][playerPos.x].b = playerColorB;*/
    }
}
void countRanking(){
    bool isInRanking;
    vector <unsigned int> ranking;
    for(unsigned int i = 0; i < PlayersListObj.PlayersList.size(); i++){
        if(i == 10)
            break;
        ranking.push_back(0);
        //excluding previously found scores
        for(unsigned int rr = 0; rr < ranking.size()-1; rr++){
            if(ranking.back() == ranking[rr]){
                ranking.back()++;
                rr = -1;
            }
        }
        //finding next highest value
        for(unsigned int j = 0; j < PlayersListObj.PlayersList.size(); j++){
            isInRanking = false;
            //excluding previously found scores
            for(unsigned int r = 0; r < ranking.size(); r++){
                if(ranking[r] == j){
                    isInRanking = true;
                    break;
                }
            }
            if(isInRanking)
                continue;
            if(PlayersListObj.PlayersList[ranking.back()].score < PlayersListObj.PlayersList[j].score){
                ranking.back() = PlayersListObj.PlayersList[j].score;
            }
        }
    }
    string rankingLine;
    rankingText.clear();
    for(unsigned int i = 0; i < ranking.size(); i++){
        rankingLine = to_string(i+1) + ". " + PlayersListObj.PlayersList[ranking[i]].name;
        rankingLine += to_string(PlayersListObj.PlayersList[ranking[i]].score);
        rankingText.push_back(rankingLine);
    }
}



void* clientThread(void* arg){
    struct ClientPointersStruct * ClientPointersObj = (struct ClientPointersStruct*)arg;

    int socketFD = 0;
    struct sockaddr_in serv_addr;

    if ((socketFD = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return EXIT_SUCCESS;
    }
    struct timeval tv;
    tv.tv_sec = 1;
    tv.tv_usec = 0;

    int on = 1;
    //socket
    setsockopt(socketFD, SOL_SOCKET, SO_RCVTIMEO, (char*)&tv, sizeof(tv));
    setsockopt(socketFD, SOL_SOCKET, SO_REUSEADDR, (char*)&on, sizeof(on));



    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(ClientPointersObj->port);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if(inet_pton(AF_INET, ClientPointersObj->ipAdress.c_str(), &serv_addr.sin_addr)<=0)
    {
        printf("\nInvalid address/ Address not supported \n");
        return EXIT_SUCCESS;
    }

    bool clientIsRunning = true;
    do{
        if(ClientPointersObj->tryToConnect == false){

            continue;
        }
        cout << "\ntrying! \n";
        if (connect(socketFD, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        {
            ClientPointersObj->connectionFailed = true;
            ClientPointersObj->tryToConnect = false;
            continue;
        }

        char buf[1200];
        for(;;){

            if(ClientPointersObj->connected == false && ClientPointersObj->tryToConnect){
                ClientPointersObj->tryToConnect = false;
                string connectMessage = "CON" + ClientPointersObj->typedName;
                writeBytes(socketFD, connectMessage, connectMessage.size());
            }
            if(ClientPointersObj->disconnecting){
                write(socketFD, "DIS", sizeof("DIS"));
                return EXIT_SUCCESS;
            }
            string hello;
            //std::cin >> hello;
            //sleep(1);
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
            string message = "POS" + to_string(playerPos.x) + ";" + to_string(playerPos.y) + ";";
            writeBytes(socketFD, message, message.size());

            buf[0] = ' ';
            int ss = read(socketFD , buf, 3000);
            int cursor = 0;
            do{
                //cout << "<" << getStringFromBuffer(buf, cursor, ss, 3)<< ">";
                if(request(buf, "ACC", cursor)){
                    reciveACC(buf, cursor, ss);
                    ClientPointersObj->connected = true;
                }
                else if(request(buf, "MSI", cursor)){
                    MaptrixObj.reciveMSI(buf, cursor, ss);
                }
                else if(request(buf, "MWL", cursor)){
                    MaptrixObj.reciveMWL(buf, cursor, ss);
                }
                else if(request(buf, "MST", cursor)){
                    MaptrixObj.reciveMST(buf, cursor, ss);
                }
                else if(request(buf, "PLY", cursor)){
                    PlayersListObj.recivePLY(buf, cursor, ss);
                    for(auto ply : PlayersListObj.PlayersList){
                        if(ply.ID == playerID){
                            playerPos.set(ply.x, ply.y);
                        }
                    }
                }
                else if(request(buf, "RAN", cursor)){
                    //NEW ROUND!
                }
                else if(request(buf, "ERN", cursor)){
                    cout << "Wrong name!\n";
                    ClientPointersObj->nameIsWrong = true;
                    break;
                }
                else if(request(buf, "ERN", cursor)){
                    cout << "Name already in use!\n";
                    ClientPointersObj->nameExists = true;
                }
                else if(request(buf, "ERR", cursor)){
                    cout << "Wrong request!\n";
                }
                else if(request(buf, "ESF", cursor)){
                    cout << "Server is full!\n";
                    ClientPointersObj->serverIsFull = true;
                }
                else if(request(buf, "AFK", cursor)){
                    cout << "You have been kicked for being AFK for too long!\n";
                }
                else if(request(buf, "BYE", cursor)){
                    cout << "Server said goodbye!\n";
                }
                else if(request(buf, "SSD", cursor)){
                    cout << "Server has been shut down!\n";
                }
                else{
                    cout << ".";
                    cursor++;
                }
            }while(cursor < ss);


            //sleep(1);
        }
    }while(clientIsRunning);


    return EXIT_SUCCESS;
}
