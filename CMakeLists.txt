cmake_minimum_required( VERSION 3.10 )

project(game)	

add_executable( game main.cpp map.h map.cpp textures.h textures.cpp
													gamelogic.h gamelogic.cpp entity.h entity.cpp)

target_link_libraries( game PRIVATE sfml-window sfml-system sfml-graphics )
