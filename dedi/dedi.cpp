#include <SFML/System.hpp>

#include <cstdlib>
#include <glog/logging.h>
#include "../server.h"
using namespace std;

void startServerThread(void* server);

/*
 * 
 */
int main(int argc, char** argv) {
  // Initialize Google's logging library.
  FLAGS_logtostderr = true;
  FLAGS_minloglevel = 0;
  FLAGS_v = 0;
  google::InitGoogleLogging(argv[0]);
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
