CMAKE_MINIMUM_REQUIRED( VERSION 2.8 )
cmake_policy(SET CMP0015 NEW)

PROJECT( dcviewer )

set(EXECUTABLE_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/build/bin")

set( Boost_USE_STATIC_LIBS ON ) 
set( Boost_USE_MULTITHREADED ON )
find_package( Boost COMPONENTS date_time thread atomic )

set( OGRE_STATIC TRUE ) 
find_package( OGRE )

set( OGRE_DEPENDENCIES_DIR "${CMAKE_SOURCE_DIR}/build/ogredeps/ogredeps" CACHE PATH "Path to Ogre deps directory" FORCE )

set( EXECUTABLE_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/build")

FILE( GLOB VIEWER_ROOT_INC_FILES "${CMAKE_SOURCE_DIR}/src/*.hpp" )
FILE( GLOB VIEWER_ROOT_SRC_FILES "${CMAKE_SOURCE_DIR}/src/*.cpp" )

source_group( src FILES ${VIEWER_ROOT_INC_FILES} ${VIEWER_ROOT_SRC_FILES} )

if(WIN32)
    set( INCLUDE_DIRS
	${OGRE_RenderSystem_Direct3D9_INCLUDE_DIR} 
	${OGRE_RenderSystem_Direct3D11_INCLUDE_DIR} 
	${OGRE_RenderSystem_GL_INCLUDE_DIR} 
    )
    set( LIBS
	debug ${OGRE_RenderSystem_Direct3D9_LIBRARY_DBG} optimized ${OGRE_RenderSystem_Direct3D9_LIBRARY_REL}
	debug ${OGRE_RenderSystem_Direct3D11_LIBRARY_DBG} optimized ${OGRE_RenderSystem_Direct3D11_LIBRARY_REL}
	debug ${OGRE_RenderSystem_GL_LIBRARY_DBG} optimized ${OGRE_RenderSystem_GL_LIBRARY_REL}
    )
else()
    set( INCLUDE_DIRS
	${OGRE_RenderSystem_GL_INCLUDE_DIR} 
    )

    set( LIBS
	debug ${OGRE_RenderSystem_GL_LIBRARY_DBG} optimized ${OGRE_RenderSystem_GL_LIBRARY_REL}
    )
endif(WIN32)

include_directories( 
    ${OGRE_INCLUDE_DIR} 
    ${VIEWER_ROOT_INC_FILES} 
    ${OIS_INCLUDE_DIR} 
    ${Boost_INCLUDE_DIRS}
    ${INCLUDE_DIRS}
)

link_directories( 
	debug ${OGRE_LIB_OPT_DIR_D} optimized ${OGRE_LIB_OPT_DIR} 
	${OGRE_DEPENDENCIES_DIR}/lib
	debug ${Boost_LIBRARY_DIR_DEBUG} optimized ${Boost_LIBRARY_DIR_RELEASE}
)

add_executable( 
	dcviewer 
	${VIEWER_ROOT_INC_FILES} 
	${VIEWER_ROOT_SRC_FILES} 
)

target_link_libraries( 
	dcviewer 
	debug ${OGRE_LIBRARY_DBG} optimized ${OGRE_LIBRARY_REL}
	${LIBS}
	debug FreeImage_d optimized FreeImage
	debug OIS_d optimized OIS
	debug zlib_d optimized zlib
	debug zziplib_d optimized zziplib
	${DirectX9_LIBRARY}
	${DirectX9_D3DX9_LIBRARY}
	${DirectX9_DXERR_LIBRARY}
	${DirectX9_DXGUID_LIBRARY}
	d3d11
	dxgi
	opengl32 
	glu32 
)

IF( WIN32 )
	ADD_DEFINITIONS( 
		-DOGRE_STATIC 
	)
ENDIF(  )