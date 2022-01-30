//serwer
#include <pthread.h>
#include "serverClient.h"

//Global objects
bool shutDownServer = false;
MaptrixClass MaptrixObj;
PlayersListClass PlayersListObj;

struct MapInfo{
    string file;
    int importance;
};
class GameClass{
public:
    //values from config file
    unsigned int max_player_count;
    int round_length;
    vector <MapInfo> list_of_maps;
    int min_name_length;
    int max_name_length;
    int afk_time_limit;

    int timeToEndRound;
    unsigned int maxNumberOfPlayerDuringRound; //max number

    void loadConfigFile();
    GameClass();
    string chooseNextFile();
    void operateRound();
};
void GameClass::loadConfigFile(){
    list_of_maps.clear();
    std::fstream configFile;
    configFile.open("config", std::ios_base::in);
    string line;
    while(configFile>>line){
        if(line == "MAX_PLAYER_COUNT:"){
            configFile>>line;
            max_player_count = stoi(line);
        }
        else if(line == "ROUND_LENGTH:"){
            configFile>>line;
            round_length = stoi(line);
        }
        else if(line == "LIST_OF_MAPS:"){
            while(configFile>>line){
                if(line != "NO_MORE_MAPS"){
                    list_of_maps.push_back(MapInfo());
                    list_of_maps.back().file = line;
                    configFile>>line;
                    list_of_maps.back().importance = stoi(line);
                }
                else
                    break;
            }
        }
        else if(line == "MIN_NAME_LENGTH:"){
            configFile>>line;
            min_name_length = stoi(line);
        }
        else if(line == "MAX_NAME_LENGTH:"){
            configFile>>line;
            max_name_length = stoi(line);
        }
        else if(line == "AFK_TIME_LIMIT:"){
            configFile>>line;
            afk_time_limit = stoi(line);
        }
    }
    configFile.close();
}
GameClass::GameClass(){
    timeToEndRound = 0;
    maxNumberOfPlayerDuringRound = 0;
    loadConfigFile();
}
string GameClass::chooseNextFile(){
    int range = 0;
    for(auto map : list_of_maps){
        range += map.importance;
    }
    int randomNumber = rand()%range;
    range = 0;
    for(auto map : list_of_maps){
        range += map.importance;
        if(randomNumber < range){
            return "maps/" + map.file;
        }
    }
    return "";
}
void GameClass::operateRound(){
    if(timeToEndRound <= 0){
        int newPoints;
        timeToEndRound = round_length;
        for(auto &Ply : PlayersListObj.PlayersList){
            newPoints = MaptrixObj.getNumberOfColoredCells(Ply.r, Ply.g, Ply.b);
            newPoints *= maxNumberOfPlayerDuringRound;
            Ply.score += newPoints;
            cout << Ply.name << ": " << Ply.score << "\n";
            Ply.newRound = true;
        }
        string aaa = chooseNextFile();
        MaptrixObj.loadMap(aaa);

        maxNumberOfPlayerDuringRound = 0;
        cout << "New round!\n";
        return;
    }
    if(maxNumberOfPlayerDuringRound < PlayersListObj.PlayersList.size())
        maxNumberOfPlayerDuringRound = PlayersListObj.PlayersList.size();
    timeToEndRound--;
}
GameClass GameState;

struct cln{
    int cfd;
    struct sockaddr_in caddr;
};



void* roundThread(void*){
    do{
        GameState.operateRound();
        sleep(1);
    }while(shutDownServer == false);
    return EXIT_SUCCESS;
}
void* serverCommandsThread(void*){
    string command;
    do{
        command = "";
        std::cin >> command;
        if(command == "SSD"){
            shutDownServer = true;
        }
        if(command == "MST"){
            //MaptrixObj.coutMST();
        }
        if(command == "RTI"){
            cout << "Round ends in: " << GameState.timeToEndRound << " sec left\n";
        }
        if(command == "RELOAD_CONFIG"){
            GameState.loadConfigFile();
            cout << "Config file has been reloaded!\n";
        }
    }while(shutDownServer == false);
    return EXIT_SUCCESS;
}

string sendACC(char * buf, int nameLength, int &playerID){

    if(PlayersListObj.addPlayer(buf, GameState.max_player_count, nameLength, GameState.max_name_length, MaptrixObj.getMapSizeX(), MaptrixObj.getMapSizeY())){
        string command;
        playerID = PlayersListObj.PlayersList.back().ID;
        command = "ACC";
        command += to_string(GameState.max_name_length) + ";";
        command += PlayersListObj.PlayersList.back().name;
        command += to_string(PlayersListObj.PlayersList.back().ID) + ";";
        command += to_string(PlayersListObj.PlayersList.back().x) + ";";
        command += to_string(PlayersListObj.PlayersList.back().y) + ";";
        command += PlayersListObj.PlayersList.back().r;
        command += PlayersListObj.PlayersList.back().g;
        command += PlayersListObj.PlayersList.back().b;
        cout << "Player '" << PlayersListObj.PlayersList.back().name <<"' has joined the server!\n";
        return command;
    }

    return "ERR";
}

void* playerThread(void* arg){
    bool isAccepted = false;
    int playerID = -1;
    int AfkStarted = GameState.timeToEndRound;
    struct cln* c = (struct cln*)arg;
    printf("new connection: %s\n", inet_ntoa((struct in_addr)c->caddr.sin_addr));
    bool forced_disconnect = false;
    char buf[100];
    string message;
    int cursor;
    do{
        buf[0] = ' ';
        int a = read(c->cfd, buf, 100);
        cursor = 0;
        if(isAccepted == false){
            if(request(buf, "CON", cursor)){
                if(a <= 2+GameState.min_name_length || a > GameState.max_name_length+3)
                    write(c->cfd, "ERN", sizeof("ERN"));
                else if(PlayersListObj.PlayersList.size() == GameState.max_player_count){
                    write(c->cfd, "ESF", sizeof("ESF"));
                }
                else if(PlayersListObj.doesNameExist(buf+3, a-3, GameState.max_name_length)){
                    write(c->cfd, "ERE", sizeof("ERE"));
                }
                else{
                    isAccepted = true;
                    AfkStarted = GameState.timeToEndRound;
                    //Server adds a new player to the list and sends the accept message
                    message = sendACC(buf+3, a-3, playerID);
                    message += MaptrixObj.sendMSI();
                    message += MaptrixObj.sendMWL();
                    message += MaptrixObj.sendMST();
                    message += PlayersListObj.sendPLY(GameState.max_name_length);
                    //cout << "sending: " << message << "\n";
                    writeBytes(c->cfd, message, message.size());
                }
            }
        }
        else{

            if(request(buf, "DIS", cursor)){
                write(c->cfd,"BYE\n",sizeof("BYE"));
                forced_disconnect = true;
            }
            else if(request(buf, "POS", cursor)){
                //reseting length of the AFK state
                AfkStarted = GameState.timeToEndRound;
                int currentIndex = PlayersListObj.getIndex(playerID);
                if(currentIndex < 0){
                    break;
                }
                //cout << "<POS:" << playerID << ":" << currentIndex << ">";
                PlayersListObj.recivePOS(buf, cursor, a, currentIndex);

                //NEW ROUND
                if(PlayersListObj.PlayersList[currentIndex].newRound){
                    PlayersListObj.PlayersList[currentIndex].newRound = false;
                    //update map
                    MaptrixObj.randomizePositions(PlayersListObj.PlayersList);
                    message = "RAN";
                    message += MaptrixObj.sendMSI();
                    message += MaptrixObj.sendMWL();
                    message += PlayersListObj.sendPLY(GameState.max_name_length);
                    //cout << "[new round]: " << message << "\n";
                    writeBytes(c->cfd, message, message.size());

                }
                else{ //ROUND IN PROGRESS
                    //Update map with player's color on current position
                    MaptrixObj.updateCell(PlayersListObj.PlayersList[currentIndex]);
                    message = MaptrixObj.sendMST();
                    message += PlayersListObj.sendPLY(GameState.max_name_length);
                    //cout << "[update]: " << message << "\n";
                    writeBytes(c->cfd, message, message.size());
                }
            }
            else if(a > 0){
                write(c->cfd, "ERR", sizeof("ERR"));
            }
            if(AfkStarted - GameState.timeToEndRound > GameState.afk_time_limit){
                write(c->cfd, "AFK", sizeof("AFK"));
                cout << "Player was kicked for being AFK!\n";
                forced_disconnect = true;
            }
        }

        /*else if(a < 0){
            cout << "nothing\n";
            forced_disconnect = true;
        }*/
    }while(forced_disconnect == false && shutDownServer == false);

    int index = PlayersListObj.getIndex(playerID);
    int r = PlayersListObj.PlayersList[index].r;
    int g = PlayersListObj.PlayersList[index].g;
    int b = PlayersListObj.PlayersList[index].b;
    MaptrixObj.deleteColor(r, g, b);
    PlayersListObj.delPlayer(playerID);

    if(shutDownServer){
        write(c->cfd, "SSD", sizeof("SSD"));
    }


    close(c->cfd);
    free(c);
    return EXIT_SUCCESS;

}

int main(int argc, char* argv[]){
    srand(time(NULL));
    pthread_t rtid, sctid;
    pthread_create(&rtid, NULL, roundThread, NULL);
    pthread_detach(rtid);
    pthread_create(&sctid, NULL, serverCommandsThread, NULL);
    pthread_detach(sctid);

    struct timeval tv;
    tv.tv_sec = 1;
    tv.tv_usec = 0;

    MaptrixObj.create(5, 5);



    int on = 1;
    pthread_t tid;
    socklen_t slt;
    //socket
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, (char*)&tv, sizeof(tv));
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (char*)&on, sizeof(on));
    //sockaddr_in saddr
    int port = 8000;
    if(argc > 1)
        port = atoi(argv[1]);
    struct sockaddr_in saddr;
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(port);
    saddr.sin_addr.s_addr = INADDR_ANY;
    //bind
    bind(fd, (struct sockaddr*)&saddr, sizeof(saddr));
    //listen
    listen(fd, 256);

    do{
    //accept
        cln* c = new cln;
        slt = sizeof(c->caddr);
        c->cfd = accept(fd, (struct sockaddr*)&c->caddr, &slt);
        if(c->cfd > 0){
            pthread_create(&tid, NULL, playerThread, c);
            pthread_detach(tid);
        }
    }while(shutDownServer == false);

    cout << "Server has been shut down!\n";

    close(fd);
    return 0;
}
