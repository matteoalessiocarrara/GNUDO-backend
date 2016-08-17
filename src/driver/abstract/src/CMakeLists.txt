cmake_minimum_required(VERSION 3.0)

add_compile_options(-std=c++11)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")

add_library(${PROJECT_NAME} SHARED managers.cpp objects.cpp)

install(TARGETS ${PROJECT_NAME} DESTINATION lib)
install(FILES gnudo.hpp managers.hpp objects.hpp DESTINATION include/${PROJECT_NAME})
