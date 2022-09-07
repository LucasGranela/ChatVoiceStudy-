cmake_minimum_required(VERSION 3.10)

# set the project name and version
project(RPGStudioServer VERSION 0.0.1)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/../bin)
set(SOURCES main.cpp 
            controlerServer/sources/controler.cpp) 
            #server/sources/server.cpp)

add_executable(RPGStudioServer ${SOURCES})