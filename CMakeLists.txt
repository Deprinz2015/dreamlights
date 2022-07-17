# !!! WARNING !!! AUTO-GENERATED FILE, PLEASE DO NOT MODIFY IT AND USE
# https://docs.platformio.org/page/projectconf/section_env_build.html#build-flags
#
# If you need to override existing CMake configuration or add extra,
# please create `CMakeListsUser.txt` in the root of project.
# The `CMakeListsUser.txt` will not be overwritten by PlatformIO.

cmake_minimum_required(VERSION 3.13)
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_C_COMPILER_WORKS 1)
set(CMAKE_CXX_COMPILER_WORKS 1)

project("MainCode" C CXX)

include(CMakeListsPrivate.txt)

if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/CMakeListsUser.txt)
include(CMakeListsUser.txt)
endif()

add_custom_target(
    Production ALL
    COMMAND platformio -c clion run "$<$<NOT:$<CONFIG:All>>:-e${CMAKE_BUILD_TYPE}>"
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

add_custom_target(
    Debug ALL
    COMMAND platformio -c clion run --target debug "$<$<NOT:$<CONFIG:All>>:-e${CMAKE_BUILD_TYPE}>"
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

add_executable(Z_DUMMY_TARGET ${SRC_LIST} lib/Config.cpp include/Config.h lib/Filehandler.cpp include/Filehandler.h include/CustomTypes/Clock.h include/CustomTypes/Effect.h include/CustomTypes/Segmented_Color.h include/CustomTypes/Color_Preset.h include/CustomTypes/Color_Preset_Key.h include/CustomTypes/Main_Config.h lib/HomeSpan/DEV_Identify.cpp include/HomeSpan/DEV_Identify.h lib/HomeSpan/DEV_LEDStrip.cpp include/HomeSpan/DEV_LEDStrip.h lib/HomeSpan/HomeSpanHandler.cpp include/HomeSpan/HomeSpanHandler.h include/Constants.h lib/LED_API.cpp include/LED_API.h lib/HTTP_API.cpp include/HTTP_API.h lib/Lua/LuaAPI.cpp include/Lua/LuaAPI.h lib/Lua/LuaMain.cpp include/Lua/LuaMain.h lib/WebServer.cpp include/WebServer.h include/Globals.h)
