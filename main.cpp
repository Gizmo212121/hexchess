#include "GameEngine.h"

int main()
{
    std::string assets = "Assets.txt";

    GameEngine game(assets);

    game.run();

    return 0;
}
