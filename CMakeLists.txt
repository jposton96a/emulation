cmake_minimum_required(VERSION 3.3)
project(Chip8_Emu)
set(EXECUTABLE_NAME Chip8_Emu)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp core/chip8.cpp core/chip8.h)
add_executable(Chip8_Emu ${SOURCE_FILES})

# Detect and add SFML
set(SFML_DIR "/home/jposton/Documents/Programming/C++/Libraries/SFML/SFML-2.3.2/share/SFML/cmake/Modules")
#set(CMAKE_MODULE_PATH ${SFML_DIR} ${CMAKE_MODULE_PATH})
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake_modules")
find_package(SFML REQUIRED system window graphics network audio)
if(SFML_FOUND)
    include_directories(${SFML_INCLUDE_DIR})
    target_link_libraries(${EXECUTABLE_NAME} ${SFML_LIBRARIES})
endif()