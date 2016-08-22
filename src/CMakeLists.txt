cmake_minimum_required(VERSION 3.0)

add_compile_options(-std=c++11 -pipe -Wall -Wextra -Wshadow -pedantic)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2 -march=native -fomit-frame-pointer")

add_library(${PROJECT_NAME} SHARED gnudo.cpp managers.cpp objects.cpp)

# XXX
add_library(sqlite3pp SHARED IMPORTED)
set_property(TARGET sqlite3pp PROPERTY IMPORTED_LOCATION /usr/local/lib/libsqlite3pp.so)

target_link_libraries(${PROJECT_NAME} sqlite3pp)

install(TARGETS ${PROJECT_NAME} DESTINATION lib)
install(FILES dbdefs.hpp gnudo.hpp managers.hpp objects.hpp DESTINATION include/${PROJECT_NAME})


add_executable(testlib test/test.cpp)
target_link_libraries(testlib ${PROJECT_NAME})
