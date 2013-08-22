#include "game.h"
#include <glog/logging.h>

#include <string.h>

int main (int argc, char *argv[]) {

  // Initialize Google's logging library.
  FLAGS_logtostderr = true;
  FLAGS_minloglevel = 0;
  FLAGS_v = 0;
  google::InitGoogleLogging(argv[0]);
  Game game;
  int gameType = Game::LOCAL;

  for(int i = 0; i < argc; i++){
    if(strcmp(argv[i], "--server") == 0)
      gameType = Game::SERVER;
    else if (strcmp(argv[i], "--connect") == 0) {
      gameType = Game::CLIENT;
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

  LOG(INFO) << "GAMETYPE :" << gameType;
  game.run(gameType);
  return EXIT_SUCCESS;
}
