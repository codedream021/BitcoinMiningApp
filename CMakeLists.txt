# Set windows env var: set QT5_DIR=C:\Qt\5.15.2\msvc2019_64\
# Run cmake: cmake.exe --build . -j 4 --config Release
cmake_minimum_required(VERSION 3.13.0)

add_subdirectory(xmr-stak)
if(WIN32)
add_subdirectory(launcher)
endif()

set(GUI_VERSION 1)
set(PROJECT_NAME "envirosoft")
project ( ${PROJECT_NAME}-v${GUI_VERSION} )

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON) 

#execute_process(COMMAND git rev-list --count HEAD WORKING_DIRECTORY ${CMAKE_SOURCE_DIR} OUTPUT_VARIABLE GUI_VERSION OUTPUT_STRIP_TRAILING_WHITESPACE)
message(STATUS "Version = ${GUI_VERSION}")
add_definitions(-DGUI_VERSION="${GUI_VERSION}")
#add_definitions(-DXMRSTAK_VERSION="${XMRSTAK_VERSION}")
if(WIN32)
add_definitions(-DWIN32=1)
endif(WIN32)

set(QML_QRC qml.qrc)
set(APP_ICON_RESOURCE_WINDOWS "${CMAKE_SOURCE_DIR}/appIcon.rc")
set(SOURCES main.cpp ProjectManager.cpp InfoDialog.cpp SettingsDialog.cpp FileDownloader.cpp)
set(MOC_HEADERS InfoDialog.h FileDownloader.h)
set(RESOURCES)
 
# Next lines needed for building all Qt projects
find_package( Qt5 COMPONENTS Core Qml Quick QuickControls2 Widgets Gui Network REQUIRED )

#find_package( Qt4 REQUIRED )
#include( ${QT_USE_FILE} )
add_definitions( ${QT_DEFINITIONS} )
include_directories( ${CMAKE_BINARY_DIR} )

# using Qt meta-system (precompiler)
#QT4_ADD_RESOURCES( RES_SOURCES ${RESOURCES} )
#QT4_WRAP_UI( UI_HEADERS ${UIS} )
#QT4_WRAP_CPP( MOC_SRCS ${MOC_HEADERS} )

# compile
add_executable( ${PROJECT_NAME} ${QML_QRC} ${APP_ICON_RESOURCE_WINDOWS} ${SOURCES} ${MOC_SRCS} ${RES_SOURCES} ${UI_HEADERS} )
target_link_libraries(${PROJECT_NAME} Qt5::Gui Qt5::Core Qt5::Widgets Qt5::Network)
# or use line below instead, if you using Windows ™ Operating System.
#add_executable( PROJECT_NAME WIN32 ${SOURCES} ${MOC_SRCS} ${RES_SOURCES} ${UI_HEADERS} )

# build it (link libraries)
target_link_libraries( ${PROJECT_NAME} ${QT_LIBRARIES} Qt${QT_VERSION_MAJOR}::Qml Qt${QT_VERSION_MAJOR}::Quick Qt${QT_VERSION_MAJOR}::QuickControls2)
target_link_libraries( ${PROJECT_NAME} xmr-stak-rx )

# generate proper GUI program on specified platform
if(XWIN32) # Check if we are on Windows
    set(CMAKE_CXX_FLAGS "/O2")
    if(MSVC) # Check if we are using the Visual Studio compiler
                set_target_properties(${PROJECT_NAME} PROPERTIES WIN32_EXECUTABLE YES LINK_FLAGS "/ENTRY:mainCRTStartup")
    endif(MSVC)
elseif(APPLE)
  set(CMAKE_CXX_FLAGS "-O3")
  set_target_properties(${PROJECT_NAME} PROPERTIES MACOSX_BUNDLE YES)
#elseif(UNIX)
	# Nothing special required
#else()
#	message(SEND_ERROR "You are on an unsupported platform! (Not Win32, Mac OS X or Unix)")
endif()
