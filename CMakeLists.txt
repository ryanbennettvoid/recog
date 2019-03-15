
cmake_minimum_required( VERSION 3.12 )

add_library( 
  genann STATIC
  ./vendor/genann/src/genann.c
)

target_include_directories( 
  genann PUBLIC 
  ./vendor/genann/include
)

# ---

project( recog-learn )
add_executable( 
  recog-learn 
  ./src/main.learn.cpp
  ./src/util.cpp
  ./src/images_file.cpp
  ./src/labels_file.cpp
  ./src/image.cpp
  ./src/file.cpp
)

find_package( Threads )
target_compile_options( recog-learn PRIVATE -Wall -Wextra -Wpedantic -Werror -std=c++17 )
target_compile_options( recog-learn PRIVATE -std=c++17 )
target_link_libraries( recog-learn ${CMAKE_THREAD_LIBS_INIT} genann )

target_include_directories( 
  recog-learn PUBLIC 
  ./include
)

# ---

project( recog-cam )
add_executable( 
  recog-cam 
  ./src/main.cam.cpp
  ./src/util.cpp
  ./src/images_file.cpp
  ./src/labels_file.cpp
  ./src/image.cpp
  ./src/file.cpp
)

find_package( Threads )
find_package( OpenCV REQUIRED )
target_compile_options( recog-cam PRIVATE -Wall -Wextra -Wpedantic -Werror -std=c++17 )
target_compile_options( recog-cam PRIVATE -std=c++17 )
target_link_libraries( recog-cam ${CMAKE_THREAD_LIBS_INIT} genann ${OpenCV_LIBS} )

target_include_directories( 
  recog-cam PUBLIC 
  ${OpenCV_INCLUDE_DIRS}
  ./include
)