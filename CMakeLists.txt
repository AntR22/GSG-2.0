cmake_minimum_required(VERSION 3.10)
project(GSG-2.0)
set(CMAKE_CXX_STANDARD 20)

add_compile_options(-std=c++11)
add_compile_options(-Wno-deprecated)
add_compile_options(-DBOOST_ALL_NO_LIB=ON)

# Find and include the Boost libraries
find_package(Boost 1.74.0 REQUIRED COMPONENTS system)
include_directories(${Boost_INCLUDE_DIRS})

# Find and include the OpenSSL libraries
find_package(OpenSSL REQUIRED)
include_directories(${OPENSSL_INCLUDE_DIR})

# Add your source files here
set(SOURCES main.cpp indicators.cpp strategies.cpp)

# Create the executable
add_executable(GSG-2.0 ${SOURCES})

# Link the dependencies
target_link_libraries(GSG-2.0 Boost::system)
target_link_libraries(GSG-2.0 ${OPENSSL_LIBRARIES})
target_link_libraries(GSG-2.0 OpenSSL::SSL OpenSSL::Crypto)
