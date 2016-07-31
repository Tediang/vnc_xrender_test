cmake_minimum_required(VERSION 3.5)
project(xrender_xlib_resize_demo)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include_directories(
        /usr/include/cairo
        /usr/include/glib-2.0
        /usr/lib64/glib-2.0/include
        /usr/include/pixman-1
        /usr/include/freetype2
        /usr/include/libpng15
        /usr/include/libdrm
)

set(SOURCE_FILES
    xx_resize_demo.c
    )

add_executable(xrender_xlib_resize_demo ${SOURCE_FILES})
target_link_libraries(xrender_xlib_resize_demo
        /usr/lib64/libX11.so
        /usr/lib64/libXrender.so
        /usr/lib64/libcairo.so)
