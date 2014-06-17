#include "game.h"

#include <string.h>

int main (int argc, char *argv[]) {

  // Initialize Google's logging library.
  Game game;
  int game_type = Game::LOCAL;

  for(int i = 0; i < argc; i++){
    if(strcmp(argv[i], "--server") == 0)
      game_type = Game::SERVER;
    else if (strcmp(argv[i], "--connect") == 0) {
      game_type = Game::CLIENT;
      game.setIp(argv[i+1]);
    } else if(strcmp(argv[i], "--port") == 0) {
      game.setPort(atoi(argv[i+1]));
    } else if(strcmp(argv[i], "--pseudo") == 0) {
      game.setPseudo(argv[i+1]);
    } else if(strcmp(argv[i], "--help") == 0) {
      std::cout << "\t --server \t : Create a server" << std::endl;
      std::cout << "\t --connect ip \t : Connect to ip" << std::endl;
      std::cout << "\t --port port \t : Specify port" << std::endl;
      std::cout << "\t --pseudo pseudo\t : Specify pseudo" << std::endl;
      return EXIT_SUCCESS;
    } 
  }

  LOG(INFO) << "GAMETYPE :" << game_type;
  game.run(game_type);
  return EXIT_SUCCESS;
}
