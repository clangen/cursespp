#cmake -DCMAKE_BUILD_TYPE=Release .
#cmake -DCMAKE_BUILD_TYPE=Debug .

cmake_minimum_required(VERSION 3.1)

project(cursespp)

set (cursespp_VERSION_MAJOR 0)
set (cursespp_VERSION_MINOR 1)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2")

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/Modules)

# SET (CMAKE_BUILD_TYPE DEBUG)
set (LIBRARY_OUTPUT_PATH ${cursespp_SOURCE_DIR}/__output)

include_directories (
  "${cursespp_SOURCE_DIR}/src"
  "${cursespp_SOURCE_DIR}/f8n/src"
  "${cursespp_SOURCE_DIR}/f8n/src/3rdparty/include"
)

if (EXISTS "/etc/arch-release" OR EXISTS "/etc/manjaro-release" OR NO_NCURSESW)
  add_definitions (-DNO_NCURSESW)
elseif(CMAKE_SYSTEM_NAME MATCHES "FreeBSD")
  add_definitions (-DNO_NCURSESW)
endif()

# "/usr/local" doesn't seem to be included by default on macOS 10.12+
if (CMAKE_SYSTEM_NAME MATCHES "Darwin")
  link_directories ("/usr/local/lib")
  include_directories("/usr/local/include")
endif (CMAKE_SYSTEM_NAME MATCHES "Darwin")

set (CURSESPP_SRCS
  ./src/App.cpp
  ./src/AppLayout.cpp
  ./src/Checkbox.cpp
  ./src/Colors.cpp
  ./src/DialogOverlay.cpp
  ./src/IMouseHandler.cpp
  ./src/InputOverlay.cpp
  ./src/LayoutBase.cpp
  ./src/ListWindow.cpp
  ./src/ListOverlay.cpp
  ./src/MultiLineEntry.cpp
  ./src/OverlayStack.cpp
  ./src/SchemaOverlay.cpp
  ./src/Screen.cpp
  ./src/Scrollbar.cpp
  ./src/ScrollAdapterBase.cpp
  ./src/ScrollableWindow.cpp
  ./src/ShortcutsWindow.cpp
  ./src/SimpleScrollAdapter.cpp
  ./src/SingleLineEntry.cpp
  ./src/Text.cpp
  ./src/TextLabel.cpp
  ./src/TextInput.cpp
  ./src/ToastOverlay.cpp
  ./src/Win32Util.cpp
  ./src/Window.cpp
)

add_library(cursespp STATIC ${CURSESPP_SRCS})

add_subdirectory("${cursespp_SOURCE_DIR}/f8n")
add_dependencies(cursespp f8n)

if (CMAKE_SYSTEM_NAME MATCHES "Linux")
  target_link_libraries(cursespp ncursesw panelw f8n)
else (CMAKE_SYSTEM_NAME MATCHES "Linux")
  target_link_libraries(cursespp curses panel f8n)
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
