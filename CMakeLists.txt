#cmake -DCMAKE_BUILD_TYPE=Release .
#cmake -DCMAKE_BUILD_TYPE=Debug .

cmake_minimum_required(VERSION 3.0)

project(cursespp)

set (cursespp_VERSION_MAJOR 0)
set (cursespp_VERSION_MINOR 1)

set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-std=c++11")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2")

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/Modules)

# SET (CMAKE_BUILD_TYPE DEBUG)
set (LIBRARY_OUTPUT_PATH ${cursespp_SOURCE_DIR}/lib)

include_directories (
  "${cursespp_SOURCE_DIR}/src"
)

# "/usr/local" doesn't seem to be included by default on macOS 10.12+
if (CMAKE_SYSTEM_NAME MATCHES "Darwin")
  link_directories ("/usr/local/lib")
  include_directories("/usr/local/include")
endif (CMAKE_SYSTEM_NAME MATCHES "Darwin")

set (CURSESPP_SRCS
    ./src/App.cpp
    ./src/Checkbox.cpp
    ./src/Colors.cpp
    ./src/DialogOverlay.cpp
    ./src/LayoutBase.cpp
    ./src/ListOverlay.cpp
    ./src/ListWindow.cpp
    ./src/Message.cpp
    ./src/MessageQueue.cpp
    ./src/MultiLineEntry.cpp
    ./src/OverlayStack.cpp
    ./src/ShortcutsWindow.cpp
    ./src/Screen.cpp
    ./src/ScrollableWindow.cpp
    ./src/ScrollAdapterBase.cpp
    ./src/SimpleScrollAdapter.cpp
    ./src/SingleLineEntry.cpp
    ./src/Text.cpp
    ./src/TextInput.cpp
    ./src/TextLabel.cpp
    ./src/Window.cpp
)

add_library(cursespp STATIC ${CURSESPP_SRCS})

if (CMAKE_SYSTEM_NAME MATCHES "Linux")
    target_link_libraries(cursespp ncursesw panelw)
else (CMAKE_SYSTEM_NAME MATCHES "Linux")
    target_link_libraries(cursespp curses panel)
endif (CMAKE_SYSTEM_NAME MATCHES "Linux")

file(GLOB sdk_headers "src/*.h")

install(
    FILES ${sdk_headers}
    DESTINATION include/cursespp
)

install(
  FILES lib/libcursespp.a
  DESTINATION lib/
)
