cmake_minimum_required(VERSION 3.10)
project(Websocket)
set(CMAKE_CXX_STANDARD 20)

add_compile_options(-std=c++11)

# Find and include the Boost libraries
find_package(Boost 1.74.0 REQUIRED COMPONENTS system)
include_directories(${Boost_INCLUDE_DIRS})

# Find and include the OpenSSL libraries
find_package(OpenSSL REQUIRED)
include_directories(${OPENSSL_INCLUDE_DIR})


# Add your source files here
set(SOURCES main.cpp)

# Create the executable
add_executable(websocket ${SOURCES})

# Link the dependencies
target_link_libraries(websocket Boost::system)
target_link_libraries(websocket ${OPENSSL_LIBRARIES})
target_link_libraries(websocket OpenSSL::SSL OpenSSL::Crypto)
