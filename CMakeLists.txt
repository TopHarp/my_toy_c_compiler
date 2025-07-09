cmake_minimum_required(VERSION 3.5)
project(toy_compiler)

set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR})
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR})

add_compile_options(-fPIC -g -std=c++14 -O0 -w)     # -Wall


list(APPEND IncludePath 
)

list(APPEND mylib  
    pthread     
)

#获取所有cpp文件
file(GLOB_RECURSE SRC  "src/*.cpp")

#获取所有h文件路径
file(GLOB_RECURSE INCLUDE  "src/*.h")
foreach(src ${INCLUDE})
    string(REGEX REPLACE "(.*)/.*h$" "\\1" include_path ${src})
    list(APPEND my_include_dirs ${include_path})
endforeach()
list(REMOVE_DUPLICATES my_include_dirs)


message(STATUS "SRC: ${SRC}")
message(STATUS "INCLUDE ${INCLUDE}")
message(STATUS "my_include_dirs ${my_include_dirs}")
message(STATUS "include ${include_path}")

include_directories(${IncludePath} ${my_include_dirs})
# link_directories(/opt/xxxx/lib )

add_executable(toy_compiler ${SRC} ./src/main.cpp)
target_link_libraries(toy_compiler ${mylib} )


