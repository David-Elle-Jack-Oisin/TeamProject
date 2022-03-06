#include "gtest/gtest.h"
#ifndef _PACKETS_H
#define _PACKETS_H
    #include "../network/packets.cpp"
#endif

TEST (Packets_Output_Tests, Id_Reply_Packet_Test) { 
    Packets packet;
    int id;
    float posX, posY;
    std::string idReply = packet.createIdReplyPacket(1, 5.0f, 7.0f);
    std::tie(id, posX, posY) = packet.parseIdReplyPacket(idReply);
    ASSERT_EQ(id, 1);
    ASSERT_EQ(posX, 5.0f);
    ASSERT_EQ(posY, 7.0f);
}
TEST (Packets_Output_Tests, Player_Info_Packet_Test) { 
    Packets packet;
    int id, health;
    float posX, posY;
    std::string playerInfo = packet.createPlayerInfoPacket(1, 5.0f, 7.0f, 10);
    std::tie(id, posX, posY, health) = packet.parsePlayerInfo(playerInfo);
    ASSERT_EQ(id, 1);
    ASSERT_EQ(posX, 5.0f);
    ASSERT_EQ(posY, 7.0f);
    ASSERT_EQ(health, 10);
}
TEST (Packets_Output_Tests, Bullet_Info_Packet_Test) { 
    Packets packet;
    float dirX, dirY;
    float posX, posY;
    std::string bulletInfo = packet.createBulletPacket(1.0f, 5.0f, 7.0f, 10.0f);
    std::tie(posX, posY, dirX, dirY) = packet.parseBulletInfo(bulletInfo);
    ASSERT_EQ(posX, 1.0f);
    ASSERT_EQ(posY, 5.0f);
    ASSERT_EQ(dirX, 7.0f);
    ASSERT_EQ(dirY, 10.0f);
}
TEST (Packets_Output_Tests, Enemy_Info_Packet_Test) { 
    Packets packet;
    int id, health;
    float posX, posY;
    std::string enemyInfo = packet.createEnemyInfoPacket(1, 5.0f, 7.0f, 10);
    std::tie(id, posX, posY, health) = packet.parseEnemyInfo(enemyInfo);
    ASSERT_EQ(id, 1);
    ASSERT_EQ(posX, 5.0f);
    ASSERT_EQ(posY, 7.0f);
    ASSERT_EQ(health, 10);
}