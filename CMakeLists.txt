
cmake_minimum_required( VERSION 3.12 )

set(CMAKE_BUILD_TYPE Debug)

project( recog )
add_executable( 
  recog 
  ./src/main.cpp
  ./src/util.cpp
  ./src/imagesfile.cpp
  ./src/image.cpp
  ./src/file.cpp
)

target_compile_options( recog PRIVATE -Wall -Wextra -Wpedantic -Werror -std=c++17 )
target_compile_options( recog PRIVATE -std=c++17 )

target_include_directories( 
  recog PUBLIC 
  ./include
)