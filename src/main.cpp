#include "game.h"

#include <string.h>

int main (int argc, char *argv[]) {
  Game game;
  int gameType = Game::LOCAL;

  for(int i = 0; i < argc; i++){
    if(strcmp(argv[i], "-server") == 0)
      gameType = Game::SERVER;
    else if (strcmp(argv[i], "-connect") == 0){
      gameType = Game::CLIENT;
      game.SetIp(argv[i+1]);
    }
    else if(strcmp(argv[i], "-port") == 0){
      game.SetPort(atoi(argv[i+1]));
    }
  }
  printf("GAMETYPE : %d\n", gameType);
  game.Run(gameType);
  return 0;
}
