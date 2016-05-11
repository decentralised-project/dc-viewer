CMAKE_MINIMUM_REQUIRED( VERSION 2.8 )
cmake_policy(SET CMP0015 NEW)

PROJECT( Decentralised )

# Setup build path.
set( EXECUTABLE_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/../build" )

# Setup some variables required for static building.
set( Boost_USE_STATIC_LIBS ON ) 
set( Boost_USE_MULTITHREADED ON )
set( OGRE_STATIC TRUE ) 
set( OGRE_SOURCE TRUE )
set( OGRE_BUILD TRUE )

set( OGRE_DEPENDENCIES_DIR "NOT-FOUND" CACHE PATH "Path to dependencies" )

# Find required packages
find_package( Boost COMPONENTS date_time thread atomic chrono system )
find_package( OIS )
find_package( ZLIB )
find_package( ZZip )
find_package( Cg )
find_package( Freetype )
find_package( FreeImage )
find_package( DirectX )
find_package( OGRE )

# Find root src files
FILE( GLOB VIEWER_ROOT_INC_FILES "${CMAKE_SOURCE_DIR}/dc/include/*.hpp" )
FILE( GLOB VIEWER_ROOT_SRC_FILES "${CMAKE_SOURCE_DIR}/dc/src/*.cpp" )

# Set up application resource files for Windows
if( WIN32 )
	# find_package( DirectX )
	# find_package( DirectX11 )

	FILE( GLOB WINRES_FILES
		"${CMAKE_SOURCE_DIR}/dc/src/Win32/resource.h"
		"${CMAKE_SOURCE_DIR}/dc/src/Win32/dcviewer.rc"
	)
endif( WIN32 )

# Organise files into project folders
source_group( "" FILES ${WINRES_FILES} )
source_group( include FILES ${VIEWER_ROOT_INC_FILES} )
source_group( src FILES ${VIEWER_ROOT_SRC_FILES} )

set( INCLUDE_DIRS
	${OGRE_INCLUDE_DIR}
	${OGRE_RenderSystem_Direct3D9_INCLUDE_DIR}
	${OGRE_RenderSystem_Direct3D11_INCLUDE_DIR}
	${OGRE_RenderSystem_GL_INCLUDE_DIR}
	${Cg_INCLUDE_DIR}
	${FREETYPE_INCLUDE_DIR}
	${OIS_INCLUDE_DIR}
	${ZZip_INCLUDE_DIR}
	${FreeImage_INCLUDE_DIR}
	${DirectX9_INCLUDE_DIR}
	${Boost_INCLUDE_DIR}
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

set( LIBS
	debug ${OIS_LIBRARY_DBG} optimized ${OIS_LIBRARY_REL}
	debug ${OGRE_LIBRARY_DBG} optimized ${OGRE_LIBRARY_REL}
	debug ${ZLIB_LIBRARY_DBG} optimized ${ZLIB_LIBRARY_REL}
	debug ${ZZip_LIBRARY_DBG} optimized ${ZZip_LIBRARY_REL}
	debug ${FREETYPE_LIBRARY_DBG} optimized ${FREETYPE_LIBRARY_REL}
	debug ${FreeImage_LIBRARY_DBG} optimized ${FreeImage_LIBRARY_REL}
	debug ${OGRE_RenderSystem_Direct3D9_LIBRARY_DBG} optimized ${OGRE_RenderSystem_Direct3D9_LIBRARY_REL}
	debug ${OGRE_RenderSystem_Direct3D11_LIBRARY_DBG} optimized ${OGRE_RenderSystem_Direct3D11_LIBRARY_REL}
	debug ${OGRE_RenderSystem_Direct3D11_LIBRARY_DBG} optimized ${OGRE_RenderSystem_Direct3D11_LIBRARY_REL}
	debug ${OGRE_Overlay_LIBRARY_DBG} optimized ${OGRE_Overlay_LIBRARY_REL}
	debug ${Boost_THREAD_LIBRARY_DEBUG} optimized ${Boost_THREAD_LIBRARY_RELEASE}
	debug ${Boost_SYSTEM_LIBRARY_DEBUG} optimized ${Boost_SYSTEM_LIBRARY_RELEASE}
	debug ${Boost_DATE_TIME_LIBRARY_DEBUG} optimized ${Boost_DATE_TIME_LIBRARY_RELEASE}
	debug ${Boost_CHRONO_LIBRARY_DEBUG} optimized ${Boost_CHRONO_LIBRARY_RELEASE}
	${DirectX9_LIBRARY}
	${DirectX9_D3DX9_LIBRARY}
	${DirectX9_DXERR_LIBRARY}
	${DirectX9_DXGUID_LIBRARY}
)

if(WIN32)
    set( LIBS ${LIBS}
	d3d11
	dxgi
    )
#else()
#    set( LIBS ${LIBS}
#    )
endif(WIN32)

include_directories( 
    ${INCLUDE_DIRS}
)

link_directories( 
	debug ${LIB_DIRS_DBG} optimized ${LIB_DIRS_REL}
)

add_executable( 
	Decentralised 
	${VIEWER_ROOT_INC_FILES} 
	${VIEWER_ROOT_SRC_FILES} 
	${WINRES_FILES}
)

target_link_libraries( 
	Decentralised 
	${LIBS}
)

if(MSVC)
    set_target_properties(Decentralised PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS")
    set_target_properties(Decentralised PROPERTIES LINK_FLAGS_DEBUG "/SUBSYSTEM:WINDOWS")
endif()

IF( WIN32 )
	ADD_DEFINITIONS( 
		-DOGRE_STATIC 
	)
ENDIF(  )

add_custom_command(TARGET Decentralised POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                       ${CMAKE_SOURCE_DIR}/data $<TARGET_FILE_DIR:Decentralised>/data)
