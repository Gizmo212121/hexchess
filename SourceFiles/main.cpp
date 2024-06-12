#include "GameEngine.h"

int main()
{
    std::string assets = "ResourceFiles/Assets.txt";

    GameEngine game(assets);

    game.run();

    return 0;
}
