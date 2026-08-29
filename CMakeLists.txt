cmake_minimum_required(VERSION 3.28)

project(WardAnywhere VERSION 1.0.0 LANGUAGES CXX)

find_package(CommonLibSSE REQUIRED)
find_package(SimpleIni CONFIG REQUIRED)

add_commonlibsse_plugin(${PROJECT_NAME}
    SOURCES
        src/PCH.h
        src/PCH.cpp
        src/Plugin.cpp
        src/Settings.h
        src/Settings.cpp
        src/InputHandler.h
        src/InputHandler.cpp
        src/WardManager.h
        src/WardManager.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE src)
target_precompile_headers(${PROJECT_NAME} PRIVATE src/PCH.h)
target_link_libraries(${PROJECT_NAME} PRIVATE SimpleIni::SimpleIni)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_23)

add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${CMAKE_CURRENT_SOURCE_DIR}/dist/SKSE/Plugins/WardAnywhere.ini"
    $<TARGET_FILE_DIR:${PROJECT_NAME}>
)