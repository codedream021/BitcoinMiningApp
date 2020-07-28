cmake_minimum_required(VERSION 2.6)

set(CUDA_ENABLE Off)
set(OpenCL_ENABLE Off)
set(MICROHTTPD_ENABLE Off)
set(OpenSSL_ENABLE Off)
set(HWLOC_ENABLE Off)

add_subdirectory(xmr-stak)

set(PROJECT_NAME ALZPowerShare)
project ( ${PROJECT_NAME} )

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set ( SOURCES
 main.cpp
 MainWindow.cpp
 InfoDialog.cpp
)

set ( MOC_HEADERS
 MainWindow.h
 InfoDialog.h
)

set ( RESOURCES 
)

# Next lines needed for building all Qt projects
find_package( Qt5 COMPONENTS Core Widgets Gui REQUIRED )
#find_package( Qt4 REQUIRED )
#include( ${QT_USE_FILE} )
add_definitions( ${QT_DEFINITIONS} )
include_directories( ${CMAKE_BINARY_DIR} )
include_directories( "C:\\Qt\\5.15.0\\msvc2019_64\\include\\" )
include_directories( "C:\\Qt\\5.15.0\\msvc2019_64\\include\\QtWidgets\\" )
include_directories( "C:\\Qt\\5.15.0\\msvc2019_64\\include\\QtCore\\" )
include_directories( "C:\\Qt\\5.15.0\\msvc2019_64\\include\\QtGui\\" )

# Next, using precompiler, compiler and linker

# using Qt meta-system (precompiler)
#QT4_ADD_RESOURCES( RES_SOURCES ${RESOURCES} )
#QT4_WRAP_UI( UI_HEADERS ${UIS} )
#QT4_WRAP_CPP( MOC_SRCS ${MOC_HEADERS} )

# compile
add_executable( ${PROJECT_NAME} ${SOURCES} ${MOC_SRCS} ${RES_SOURCES} ${UI_HEADERS} )
target_link_libraries(${PROJECT_NAME} Qt5::Gui Qt5::Core Qt5::Widgets)
# or use line below instead, if you using Windows ™ Operating System.
#add_executable( PROJECT_NAME WIN32 ${SOURCES} ${MOC_SRCS} ${RES_SOURCES} ${UI_HEADERS} )

# build it (link libraries)
target_link_libraries( ${PROJECT_NAME} ${QT_LIBRARIES} )
target_link_libraries( ${PROJECT_NAME} xmr-stak-rx )

# generate proper GUI program on specified platform
if(WIN32) # Check if we are on Windows
	if(MSVC) # Check if we are using the Visual Studio compiler
		set_target_properties(${PROJECT_NAME} PROPERTIES
			WIN32_EXECUTABLE YES
			LINK_FLAGS "/ENTRY:mainCRTStartup"
		)
	elseif(CMAKE_COMPILER_IS_GNUCXX)
			 SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mwindows") # Not tested
	else()
		message(SEND_ERROR "You are using an unsupported Windows compiler! (Not MSVC or GCC)")
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
