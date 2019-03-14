
cmake_minimum_required( VERSION 3.12 )

set(CMAKE_BUILD_TYPE Debug)

add_library( 
  genann STATIC
  ./vendor/genann/src/genann.c
)

target_include_directories( 
  genann PUBLIC 
  ./vendor/genann/include
)

# ---

project( recog )
add_executable( 
  recog 
  ./src/main.cpp
  ./src/util.cpp
  ./src/images_file.cpp
  ./src/labels_file.cpp
  ./src/image.cpp
  ./src/file.cpp
)

target_compile_options( recog PRIVATE -Wall -Wextra -Wpedantic -Werror -std=c++17 )
target_compile_options( recog PRIVATE -std=c++17 )
target_link_libraries( recog genann )

target_include_directories( 
  recog PUBLIC 
  ./include
)