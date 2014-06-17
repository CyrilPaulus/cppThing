#include <SFML/System.hpp>

#include <cstdlib>
#include "../server.h"
using namespace std;

void startServerThread(void* server);

/*
 * 
 */
int main(int argc, char** argv) {
  // Initialize Google's logging library.
  Server s;
  sf::Thread serverThread(&startServerThread, &s);  
  s.init();
  serverThread.launch();
  std::cout << "Press any key to stop the server" << std::endl;
  scanf("\n");
  s.stop();
  serverThread.wait();    
  return 0;
}

void startServerThread(void* server){
  (static_cast<Server*>(server))->run();
}
