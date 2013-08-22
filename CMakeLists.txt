cmake_minimum_required(VERSION 2.8)

#add_definitions(
#  -DSFML_STATIC
#)

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

add_library(Core
  network/ClientToken.cpp
  network/packet.cpp
  network/PlayerUpdate.cpp
  network/CubeUpdate.cpp     
  network/PlayerAdd.cpp     
  network/UserMessage.cpp
  network/NetworkClient.cpp  
  network/PlayerDelete.cpp	
  network/TextMessage.cpp
  entity/cube.cpp
  entity/entity.cpp 
  entity/player.cpp 
  entity/world.cpp
  ui/cubeDisplay.cpp	
  ui/layerDisplay.cpp  
  ui/menuItem.cpp  
  ui/uiElement.cpp
  ui/ChatBox.cpp
  menu/mainMenu.cpp
  menu/connectMenu.cpp
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

TARGET_LINK_LIBRARIES(cppThing Core ${SFML_WINDOW_LIBRARY} ${SFML_NETWORK_LIBRARY} ${SFML_GRAPHICS_LIBRARY} ${SFML_SYSTEM_LIBRARY} ${ENet_LIBRARY})
TARGET_LINK_LIBRARIES(ds Core ${SFML_NETWORK_LIBRARY} ${SFML_SYSTEM_LIBRARY} ${ENet_LIBRARY})
