#include <string.h>
#include <string>
#include <tuple>

class Packets{
    public:
    std::string createIdRequestPacket(){
        std::string packet = "0";
        return packet;
    }
    std::string createIdReplyPacket(int id, float enemyPosX, float enemyPosY, int health){
        std::string packet = "0\n" + std::to_string(id) + "," +
        std::to_string(enemyPosX) + ":" + std::to_string(enemyPosY) + "," + std::to_string(health);
        return packet;
    }
    std::tuple<int, float, float, int> parseIdReplyPacket(std::string packet){
        unsigned firstComma = packet.find(",") + 1;
        unsigned colon = packet.find(":") + 1;
        unsigned secondComma = packet.find_last_of(",");
        unsigned newline = packet.find("\n") + 1;
        int id = std::stoi(packet.substr(newline, firstComma - newline));
        const char *firstFloatCharArray = packet.substr(firstComma, colon - firstComma - 1).c_str();
        float posX = std::atof(firstFloatCharArray);
		const char *secondFloatCharArray = packet.substr(colon, secondComma - colon).c_str();
		float posY = std::atof(secondFloatCharArray);
        int health = std::stoi(packet.substr(secondComma+ 1));
        return {id, posX, posY, health};
    }
    std::string createPlayerInfoPacket(int id, float posX, float posY, int health){
        std::string packet = "1\n" + 
            std::to_string(id) + "," +
            std::to_string(posX) + ":" + std::to_string(posY) + "," +
            std::to_string(health);
        return packet;
    }

    std::tuple<int, float, float, int> parsePlayerInfo(std::string packet){
        unsigned firstComma = packet.find(",") + 1;
        unsigned colon = packet.find(":") + 1;
        unsigned secondComma = packet.find_last_of(",");
        unsigned newline = packet.find("\n") + 1;
        int id = std::stoi(packet.substr(newline, firstComma - newline));
        const char *firstFloatCharArray = packet.substr(firstComma, colon - firstComma - 1).c_str();
        float posX = std::atof(firstFloatCharArray);
		const char *secondFloatCharArray = packet.substr(colon, secondComma - colon).c_str();
		float posY = std::atof(secondFloatCharArray);
        int health = std::stoi(packet.substr(secondComma+ 1));
        return {id, posX, posY, health};
    }

    std::string createPlayerDeathPacket(int id){
        std::string packet = "2:" + std::to_string(id);
        return packet;
    }
    std::string createPlayerDisconnectPacket(int id){
        std::string packet = "3:" + std::to_string(id);
        return packet;
    }
    std::string createBulletPacket(float posX, float posY, float dirX, float dirY){
        std::string packet = "4\n" + 
            std::to_string(posX) + "," +
            std::to_string(posY) + ":" + std::to_string(dirX) + "," +
            std::to_string(dirY);
        return packet;
    }
    std::tuple<float, float, float, float> parseBulletInfo(std::string packet){   
        unsigned firstComma = packet.find(",") + 1;
        unsigned colon = packet.find(":") + 1;
        unsigned secondComma = packet.find_last_of(",");
        unsigned newline = packet.find("\n") + 1;
        const char *firstFloatCharArray = packet.substr(newline, firstComma - newline).c_str();
        float posX = std::atof(firstFloatCharArray);
        const char *secondFloatCharArray = packet.substr(firstComma, colon - firstComma - 1).c_str();
        float posY = std::atof(secondFloatCharArray);
		const char *thirdFloatCharArray = packet.substr(colon, secondComma - colon).c_str();
		float dirX = std::atof(thirdFloatCharArray);
        const char *fourthFloatCharArray = packet.substr(secondComma+ 1).c_str();
        float dirY = std::atof(fourthFloatCharArray);
        return {posX, posY, dirX, dirY};
    }
    std::string createGameOverPacket(int score){
        std::string packet = "5:" + std::to_string(score);
        return packet;
    }

    std::string createEnemyInfoPacket(int id, float posX, float posY, int health){
        std::string packet = "6\n" + 
            std::to_string(id) + "," +
            std::to_string(posX) + ":" + std::to_string(posY) + "," +
            std::to_string(health);
        return packet;
    }
    std::tuple<int, float, float, int> parseEnemyInfo(std::string packet){   
        unsigned firstComma = packet.find(",") + 1;
        unsigned colon = packet.find(":") + 1;
        unsigned secondComma = packet.find_last_of(",");
        unsigned newline = packet.find("\n") + 1;
        int id = std::stoi(packet.substr(newline, firstComma - newline));
        const char *firstFloatCharArray = packet.substr(firstComma, colon - firstComma - 1).c_str();
        float posX = std::atof(firstFloatCharArray);
		const char *secondFloatCharArray = packet.substr(colon, secondComma - colon).c_str();
		float posY = std::atof(secondFloatCharArray);
        int health = std::stoi(packet.substr(secondComma+ 1));
        return {id, posX, posY, health};
    }
    
    private:

};