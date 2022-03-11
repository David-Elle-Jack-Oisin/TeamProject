#include "gtest/gtest.h"
#include "../simulation/menus/MainMenu.cpp"

TEST (Control_Flow_Tests, Initial_Node_Main_Menu_Test) { 
    MainMenu menu;
    ASSERT_EQ(menu.isMainMenuFinished(), false);
    ASSERT_EQ(menu.isSinglePlayer(), false);
    ASSERT_EQ(menu.isMultiplayer(), false);
}
TEST (Control_Flow_Tests, SinglePlayer_Node_Test) { 
    MainMenu menu;
    menu.setSinglePlayer(true);
    ASSERT_EQ(menu.isMainMenuFinished(), true);
    ASSERT_EQ(menu.isSinglePlayer(), true);
    ASSERT_EQ(menu.getIsAttemptingToJoinServer(), false);
    ASSERT_EQ(menu.exitTheIpScreen(), false);
}
TEST (Control_Flow_Tests, Muiltiplayer_Node_Test) { 
    MainMenu menu;
    menu.setMultiplayer(true);
    ASSERT_EQ(menu.isMultiplayer(), true);
    ASSERT_EQ(menu.isMainMenuFinished(), false);
    ASSERT_EQ(menu.isSinglePlayer(), false);
    ASSERT_EQ(menu.getIsAttemptingToJoinServer(), false);
}
TEST (Control_Flow_Tests, JoinServer_Node_Test) { 
    MainMenu menu;
    menu.setMultiplayer(true);
    menu.setIsAttemptingToJoinServer(true);
    ASSERT_EQ(menu.isMainMenuFinished(), true);
    ASSERT_EQ(menu.isSinglePlayer(), false);
    ASSERT_EQ(menu.getIsAttemptingToJoinServer(), true);
}