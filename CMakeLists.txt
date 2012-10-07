cmake_minimum_required(VERSION 2.8)

project(cppThing)
include(CheckIncludeFiles)
SET(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

find_package(SFML 2 REQUIRED system window graphics network)
if(SFML_FOUND)
  include_directories(${SFML_INCLUDE_DIR})
endif()

find_package(ENet REQUIRED)
if(ENet_FOUND)
  message(ERROR ${ENet_LIBRARIES})
endif()

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/res DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if (${CMAKE_COMPILER_IS_GNUCXX})
  set(CMAKE_C_FLAGS "-Wall -Wno-long-long -Wno-implicit-function-declaration")  
  # --std=c99 makes libcurl not to compile
  # -pedantic gives a lot of warnings on OpenSSL 
  set(CMAKE_CXX_FLAGS "-Wall -pedantic -Wno-long-long -Wno-variadic-macros")
endif()

add_library(Core
  network/ClientToken.cpp
  network/packet.cpp
  network/PlayerUpdate.cpp
  network/CubeUpdate.cpp     
  network/PlayerAdd.cpp     
  network/UserMessage.cpp
  network/NetworkClient.cpp  
  network/PlayerDelete.cpp	
  entity/cube.cpp
  entity/entity.cpp 
  entity/player.cpp 
  entity/world.cpp
  ui/cubeDisplay.cpp	
  ui/layerDisplay.cpp  
  ui/menuItem.cpp  
  ui/uiElement.cpp
  menu/mainMenu.cpp
  mouse.cpp
  config.cpp
  screen.cpp
  ticker.cpp
  utils/quadtree.cpp
  utils/vectorutils.cpp
  server.cpp
)

add_executable(cppThing
  main.cpp
  game.cpp
  imageManager.cpp
  Renderer.cpp
  client.cpp
)

add_executable(ds
  dedi/dedi.cpp
)

TARGET_LINK_LIBRARIES(cppThing ${SFML_WINDOW_LIBRARY} ${SFML_NETWORK_LIBRARY} ${SFML_GRAPHICS_LIBRARY} ${SFML_SYSTEM_LIBRARY} ${ENet_LIBRARY} Core)
TARGET_LINK_LIBRARIES(ds ${SFML_NETWORK_LIBRARY} ${SFML_SYSTEM_LIBRARY} ${ENet_LIBRARY} Core)
