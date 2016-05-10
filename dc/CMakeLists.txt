CMAKE_MINIMUM_REQUIRED( VERSION 2.8 )
cmake_policy(SET CMP0015 NEW)

PROJECT( Decentralised )

set( EXECUTABLE_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/../build" )

set( Boost_USE_STATIC_LIBS ON ) 
set( Boost_USE_MULTITHREADED ON )
set( OGRE_STATIC TRUE ) 

# find_package( Boost COMPONENTS date_time thread atomic )

# set( OGRE_DEPENDENCIES_DIR "${CMAKE_SOURCE_DIR}/build/ogredeps/ogredeps" CACHE PATH "Path to Ogre deps directory" FORCE )
# find_package( OGRE )

# set( CEGUI_STATIC TRUE ) 
# find_package( CEGUI )

# Set up some path variables
set( ROOT_OGRE "${CMAKE_SOURCE_DIR}/../libs/ogre" CACHE PATH "Path to Ogre SDK directory" FORCE )
set( ROOT_DEPS "${CMAKE_SOURCE_DIR}/../libs/deps" CACHE PATH "Path to Ogre dependencies directory" FORCE )
set( ROOT_BOOST "${CMAKE_SOURCE_DIR}/../libs/boost" CACHE PATH "Path to boost directory" FORCE )

# Find root src files
FILE( GLOB VIEWER_ROOT_INC_FILES "${CMAKE_SOURCE_DIR}/dc/include/*.hpp" )
FILE( GLOB VIEWER_ROOT_SRC_FILES "${CMAKE_SOURCE_DIR}/dc/src/*.cpp" )

# Set up application resource files for Windows
if(WIN32)

if (ROOT_DirectX9 STREQUAL "")
set( ROOT_DirectX9 "C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)" CACHE PATH "Path to DirectX June 2010 SDK directory" FORCE )
endif()

FILE( GLOB WINRES_FILES
    "${CMAKE_SOURCE_DIR}/dc/src/Win32/resource.h"
    "${CMAKE_SOURCE_DIR}/dc/src/Win32/dcviewer.rc"
)
endif(WIN32)

# Organise files into project folders
source_group( "" FILES ${WINRES_FILES} )
source_group( include FILES ${VIEWER_ROOT_INC_FILES} )
source_group( src FILES ${VIEWER_ROOT_SRC_FILES} )

find_package( DirectX )
# find_package( DirectX11 )

set( INCLUDE_DIRS
	${ROOT_OGRE}/include/OGRE
	${ROOT_OGRE}/include/OGRE/RenderSystems/Direct3D9
	${ROOT_OGRE}/include/OGRE/RenderSystems/Direct3D11
	${ROOT_OGRE}/include/OGRE/RenderSystems/GL
	${ROOT_DEPS}/include/Cg
	${ROOT_DEPS}/include/freetype
	${ROOT_DEPS}/include/GL
	${ROOT_DEPS}/include/glm
	${ROOT_DEPS}/include/loaders
	${ROOT_DEPS}/include/OIS
	${ROOT_DEPS}/include/OIS/win32
	${ROOT_DEPS}/include/rapidjson
	${ROOT_DEPS}/include/SDL2
	${ROOT_DEPS}/include/zzip
	${ROOT_BOOST}
	${DirectX9_INCLUDE_DIR}
	"${CMAKE_SOURCE_DIR}/dc/include"
	"${CMAKE_SOURCE_DIR}/dc/src/Win32"
)

set( LIB_DIRS_REL
	${ROOT_OGRE}/lib
	${ROOT_OGRE}/lib/Release
	${ROOT_OGRE}/lib/Release/opt
	${ROOT_BOOST}/lib
	${ROOT_DirectX9}/Lib/x86
)

set( LIB_DIRS_DBG
	${ROOT_OGRE}/lib
	${ROOT_OGRE}/lib/Debug
	${ROOT_OGRE}/lib/Debug/opt
	${ROOT_BOOST}/lib
)

if(WIN32)
    set( LIBS
	debug OIS_d optimized OIS
	debug OgreMainStatic_d optimized OgreMainStatic
	debug zlib_d optimized zlib
	debug zziplib_d optimized zziplib
	debug freetype_d optimized freetype
	debug FreeImage_d optimized FreeImage
	debug RenderSystem_Direct3D9Static_d optimized RenderSystem_Direct3D9Static
	debug RenderSystem_Direct3D11Static_d optimized RenderSystem_Direct3D11Static
	debug OgreOverlayStatic_d optimized OgreOverlayStatic
	${DirectX9_LIBRARY}
	d3dx9
	d3d11
	dxerr
	dxgi
	dxguid
    )
else()

    set( LIBS
    )
endif(WIN32)

include_directories( 
    ${INCLUDE_DIRS}
)

link_directories( 
	debug ${LIB_DIRS_DBG} optimized ${LIB_DIRS_REL}
)

add_executable( 
	dcviewer 
	${VIEWER_ROOT_INC_FILES} 
	${VIEWER_ROOT_SRC_FILES} 
	${WINRES_FILES}
)

target_link_libraries( 
	dcviewer 
	${LIBS}
)

if(MSVC)
    set_target_properties(dcviewer PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS")
    set_target_properties(dcviewer PROPERTIES LINK_FLAGS_DEBUG "/SUBSYSTEM:WINDOWS")
endif()

IF( WIN32 )
	ADD_DEFINITIONS( 
		-DOGRE_STATIC 
	)
ENDIF(  )

add_custom_command(TARGET dcviewer POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                       ${CMAKE_SOURCE_DIR}/data $<TARGET_FILE_DIR:dcviewer>/data)