cmake_minimum_required(VERSION 2.6)

set(CMAKE_MODULE_PATH)

set(PROJECT_NAME "E-Pluribus-Unum-Launcher")
project ( ${PROJECT_NAME} )

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON) 

set(CMAKE_CXX_FLAGS "/O2")

execute_process(
	COMMAND git rev-list --count HEAD
	WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
	OUTPUT_VARIABLE VERSION
	OUTPUT_STRIP_TRAILING_WHITESPACE
)

add_definitions(-DVERSION="${VERSION}")

message(STATUS "Version = ${VERSION}")

set ( SOURCES
 main.cpp
 MainWindow.cpp
 FileDownloader.cpp
 unrar.cpp
)

set ( MOC_HEADERS  
 FileDownloader.h
)

set ( RESOURCES 
)

set(Qt5_DIR "C:\\Qt\\5.15.2\\msvc2019_64\\lib\\cmake\\Qt5")
# Next lines needed for building all Qt projects
find_package( Qt5 COMPONENTS Core Widgets Gui Network REQUIRED )
#find_package( Qt4 REQUIRED )
#include( ${QT_USE_FILE} )
add_definitions( ${QT_DEFINITIONS} )
include_directories( ${CMAKE_BINARY_DIR} )
include_directories( "C:\\Qt\\5.15.2\\msvc2019_64\\include\\" )
include_directories( "C:\\Qt\\5.15.2\\msvc2019_64\\include\\QtWidgets\\" )
include_directories( "C:\\Qt\\5.15.2\\msvc2019_64\\include\\QtCore\\" )
include_directories( "C:\\Qt\\5.15.2\\msvc2019_64\\include\\QtGui\\" )
include_directories( "C:\\Qt\\5.15.2\\msvc2019_64\\include\\QtNetwork\\" )

# compile
add_executable( ${PROJECT_NAME} ${SOURCES} ${MOC_SRCS} ${RES_SOURCES} ${UI_HEADERS} )
target_link_libraries(${PROJECT_NAME} Qt5::Gui Qt5::Core Qt5::Widgets Qt5::Network)
# or use line below instead, if you using Windows ™ Operating System.
#add_executable( PROJECT_NAME WIN32 ${SOURCES} ${MOC_SRCS} ${RES_SOURCES} ${UI_HEADERS} )

# build it (link libraries)
target_link_libraries( ${PROJECT_NAME} ${QT_LIBRARIES} ) 
target_link_libraries( ${PROJECT_NAME} UnRAR64 ) 

# generate proper GUI program on specified platform
if(XWIN32) # Check if we are on Windows
	if(MSVC) # Check if we are using the Visual Studio compiler
		set_target_properties(${PROJECT_NAME} PROPERTIES
			WIN32_EXECUTABLE YES
			LINK_FLAGS "/ENTRY:mainCRTStartup"
		) 
	endif(MSVC)
elseif(APPLE)
	set_target_properties(${PROJECT_NAME} PROPERTIES
			MACOSX_BUNDLE YES
	)
#elseif(UNIX)
	# Nothing special required
#else()
#	message(SEND_ERROR "You are on an unsupported platform! (Not Win32, Mac OS X or Unix)")
endif(WIN32)
