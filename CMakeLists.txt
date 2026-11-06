cmake_minimum_required(VERSION 3.16)
project(MovieSearchSystem LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release) # timings are meaningless in a debug build
endif()

include(FetchContent)

# GLFW: opens the window
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
FetchContent_Declare(glfw
        GIT_REPOSITORY https://github.com/glfw/glfw.git
        GIT_TAG 3.4
        GIT_SHALLOW TRUE)

# Dear ImGui: buttons, text boxes, tables
FetchContent_Declare(imgui
        GIT_REPOSITORY https://github.com/ocornut/imgui.git
        GIT_TAG v1.91.5
        GIT_SHALLOW TRUE)

FetchContent_MakeAvailable(glfw imgui)
find_package(OpenGL REQUIRED)

add_executable(MovieSearchSystem
        main.cpp
        MovieApp.cpp
        MovieData.cpp
        RedBlackTree.cpp
        SplayTree.cpp
        ${imgui_SOURCE_DIR}/imgui.cpp
        ${imgui_SOURCE_DIR}/imgui_draw.cpp
        ${imgui_SOURCE_DIR}/imgui_tables.cpp
        ${imgui_SOURCE_DIR}/imgui_widgets.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp)

target_include_directories(MovieSearchSystem PRIVATE
        ${imgui_SOURCE_DIR}
        ${imgui_SOURCE_DIR}/backends)

target_link_libraries(MovieSearchSystem PRIVATE glfw OpenGL::GL)

target_compile_definitions(MovieSearchSystem PRIVATE
        DATA_FILE="${CMAKE_SOURCE_DIR}/tmdb_movies_100000.csv"
        FONT_FILE="${imgui_SOURCE_DIR}/misc/fonts/Roboto-Medium.ttf")

# copy the dataset next to the program so it runs from any IDE
add_custom_command(TARGET MovieSearchSystem POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_SOURCE_DIR}/tmdb_movies_100000.csv
        $<TARGET_FILE_DIR:MovieSearchSystem>/tmdb_movies_100000.csv)