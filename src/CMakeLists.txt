cmake_minimum_required(VERSION 3.0)

add_compile_options(-std=c++11 -pipe -Wall -Wextra -Wshadow -pedantic)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2 -march=native -fomit-frame-pointer")


add_library(${PROJECT_NAME}-static STATIC gnudo.cpp managers.cpp objects.cpp)
target_link_libraries(${PROJECT_NAME}-static sqlite3pp-static)

add_executable(testlib test/test.cpp)
target_link_libraries(testlib ${PROJECT_NAME})

install(TARGETS ${PROJECT_NAME}-static DESTINATION lib)
install(FILES dbdefs.hpp gnudo.hpp managers.hpp objects.hpp DESTINATION include/${PROJECT_NAME})
