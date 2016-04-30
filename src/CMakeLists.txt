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

if(WIN32)
FILE( GLOB WINRES_FILES
    "${CMAKE_SOURCE_DIR}/src/resource.h"
    "${CMAKE_SOURCE_DIR}/src/dcviewer.rc"
)
endif(WIN32)

source_group( "" FILES ${WINRES_FILES} )
source_group( src FILES ${VIEWER_ROOT_INC_FILES} ${VIEWER_ROOT_SRC_FILES} )

if(WIN32)
    set( INCLUDE_DIRS
		${OGRE_RenderSystem_Direct3D9_INCLUDE_DIR} 
		${OGRE_RenderSystem_Direct3D11_INCLUDE_DIR} 
		${OGRE_RenderSystem_GL_INCLUDE_DIR} 
    )
    set( LIBS
	${DirectX9_LIBRARY}
	${DirectX9_D3DX9_LIBRARY}
	${DirectX9_DXERR_LIBRARY}
	${DirectX9_DXGUID_LIBRARY}
	debug ${OGRE_RenderSystem_Direct3D9_LIBRARY_DBG} optimized ${OGRE_RenderSystem_Direct3D9_LIBRARY_REL}
	debug ${OGRE_RenderSystem_Direct3D11_LIBRARY_DBG} optimized ${OGRE_RenderSystem_Direct3D11_LIBRARY_REL}
	debug ${OGRE_RenderSystem_GL_LIBRARY_DBG} optimized ${OGRE_RenderSystem_GL_LIBRARY_REL}
	debug ${OGRE_LIBRARY_DBG} optimized ${OGRE_LIBRARY_REL}
	${OPENGL_gl_LIBRARY} 
	${OPENGL_glu_LIBRARY} 
	debug ${Boost_SYSTEM_LIBRARY_DEBUG} optimized ${Boost_SYSTEM_LIBRARY_RELEASE}
	debug ${Boost_THREAD_LIBRARY_DEBUG} optimized ${Boost_THREAD_LIBRARY_RELEASE}
	debug ${Boost_ATOMIC_LIBRARY_DEBUG} optimized ${Boost_ATOMIC_LIBRARY_RELEASE}
	debug ${Boost_CHRONO_LIBRARY_DEBUG} optimized ${Boost_CHRONO_LIBRARY_RELEASE}
	debug ${Boost_DATE_TIME_LIBRARY_DEBUG} optimized ${Boost_DATE_TIME_LIBRARY_RELEASE}
	debug ${FreeImage_LIBRARY_DBG} optimized ${FreeImage_LIBRARY_REL}
	debug OIS_d optimized OIS
	debug ${ZLIB_LIBRARY_DBG} optimized ${ZLIB_LIBRARY_REL}
	debug ${ZZip_LIBRARY_DBG} optimized ${ZZip_LIBRARY_REL}
    )
else()
    set( INCLUDE_DIRS
		${OGRE_RenderSystem_GL_INCLUDE_DIR} 
    )

    set( LIBS
	${DirectX9_LIBRARY}
	${DirectX9_D3DX9_LIBRARY}
	${DirectX9_DXERR_LIBRARY}
	${DirectX9_DXGUID_LIBRARY}
	debug ${OGRE_RenderSystem_GL_LIBRARY_DBG} optimized ${OGRE_RenderSystem_GL_LIBRARY_REL}
	${X11_X11_LIB}
	debug ${OGRE_LIBRARY_DBG} optimized ${OGRE_LIBRARY_REL}
	dl
	${OPENGL_gl_LIBRARY} 
	${OPENGL_glu_LIBRARY} 
	pthread
	debug ${Boost_SYSTEM_LIBRARY_DEBUG} optimized ${Boost_SYSTEM_LIBRARY_RELEASE}
	debug ${Boost_THREAD_LIBRARY_DEBUG} optimized ${Boost_THREAD_LIBRARY_RELEASE}
	debug ${Boost_ATOMIC_LIBRARY_DEBUG} optimized ${Boost_ATOMIC_LIBRARY_RELEASE}
	debug ${Boost_CHRONO_LIBRARY_DEBUG} optimized ${Boost_CHRONO_LIBRARY_RELEASE}
	debug ${Boost_DATE_TIME_LIBRARY_DEBUG} optimized ${Boost_DATE_TIME_LIBRARY_RELEASE}
	${X11_Xt_LIB}
	debug ${FreeImage_LIBRARY_DBG} optimized ${FreeImage_LIBRARY_REL}
	debug OIS_d optimized OIS
	debug ${ZLIB_LIBRARY_DBG} optimized ${ZLIB_LIBRARY_REL}
	debug ${ZZip_LIBRARY_DBG} optimized ${ZZip_LIBRARY_REL}
	${X11_ICE_LIB}
	${X11_SM_LIB}
	${X11_Xext_LIB}
	${X11_Xau_LIB}
	${X11_Xdamage_LIB}
	${X11_Xdmcp_LIB}
	${X11_Xfixes_LIB}
	${X11_Xmu_LIB}
	${X11_Xpm_LIB}
	${X11_Xrandr_LIB}
	${X11_Xrender_LIB}
	${X11_Xxf86vm_LIB}
	${XAW_LIBRARY}
    )
endif(WIN32)

include_directories( 
    ${OGRE_INCLUDE_DIR} 
    ${VIEWER_ROOT_INC_FILES} 
    ${OIS_INCLUDE_DIR} 
    ${Boost_INCLUDE_DIR}
    ${INCLUDE_DIRS}
)

link_directories( 
	${OGRE_DEPENDENCIES_DIR}/lib
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

IF( WIN32 )
	ADD_DEFINITIONS( 
		-DOGRE_STATIC 
	)
ENDIF(  )