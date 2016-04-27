CMAKE_MINIMUM_REQUIRED( VERSION 2.8 )
cmake_policy(SET CMP0015 NEW)

PROJECT( dc-viewer )

set( Boost_USE_STATIC_LIBS ON ) 
set( Boost_USE_MULTITHREADED ON )
find_package( Boost COMPONENTS date_time thread atomic )

set( OGRE_INC_DIR "${CMAKE_SOURCE_DIR}/build/ogre/sdk/include/OGRE" CACHE PATH "Path to Ogre sdk include directory" FORCE
 )
set( OIS_INC_DIR "${CMAKE_SOURCE_DIR}/build/ogre/sdk/include/OIS" CACHE PATH "Path to Ogre sdk OIS include directory" FORCE
 )
set( D3D9_INC_DIR "${CMAKE_SOURCE_DIR}/build/ogre/sdk/include/OGRE/RenderSystems/Direct3D9" CACHE PATH "Path to Ogre sdk OIS include directory" FORCE
 )
set( D3D11_INC_DIR "${CMAKE_SOURCE_DIR}/build/ogre/sdk/include/OGRE/RenderSystems/Direct3D11" CACHE PATH "Path to Ogre sdk OIS include directory" FORCE
 )
set( OPENGL_INC_DIR "${CMAKE_SOURCE_DIR}/build/ogre/sdk/include/OGRE/RenderSystems/GL" CACHE PATH "Path to Ogre sdk OIS include directory" FORCE
 )

set( OGRE_LIB_DIR "${CMAKE_SOURCE_DIR}/build/ogre/sdk/lib" CACHE PATH "Path to Ogre sdk lib directory" FORCE
 )
set( OGRE_LIB_OPT_DIR "${CMAKE_SOURCE_DIR}/build/ogre/sdk/lib/Release/opt" CACHE PATH "Path to Ogre sdk lib/opt directory" FORCE
 )
set( OGRE_LIB_OPT_DIR_D "${CMAKE_SOURCE_DIR}/build/ogre/sdk/lib/Debug/opt" CACHE PATH "Path to Ogre sdk lib/opt directory" FORCE
 )
set( OGREDEPS_LIB_DIR "${CMAKE_SOURCE_DIR}/build/ogredeps/ogredeps/lib" CACHE PATH "Path to Ogre sdk lib directory" FORCE
 )
set( DX11_LIB_DIR "C:/Program Files (x86)/Windows Kits/8.0/Lib/win8/um/x86" CACHE PATH "Path to DX11 Windows SDK" FORCE
 )
set( DX9_LIB_DIR "E:/Programs (x86)/Microsoft DirectX SDK (June 2010)/Lib/x86" CACHE PATH "Path to DX9 SDK" FORCE
 )

set( EXECUTABLE_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/build")

FILE( GLOB VIEWER_ROOT_INC_FILES "${CMAKE_SOURCE_DIR}/src/*.hpp" )
FILE( GLOB VIEWER_ROOT_SRC_FILES "${CMAKE_SOURCE_DIR}/src/*.cpp" )

source_group( src FILES ${VIEWER_ROOT_INC_FILES} ${VIEWER_ROOT_SRC_FILES} )

include_directories( 
	${OGRE_INC_DIR} 
	${VIEWER_ROOT_INC_FILES} 
	${OIS_INC_DIR} 
	${Boost_INCLUDE_DIRS}
	${D3D9_INC_DIR} 
	${D3D11_INC_DIR} 
	${OPENGL_INC_DIR} 
)

link_directories( 
	${OGRE_LIB_DIR} 
	debug ${OGRE_LIB_OPT_DIR_D} optimized ${OGRE_LIB_OPT_DIR} 
	${DX11_LIB_DIR} 
	${DX9_LIB_DIR} 
	${OGREDEPS_LIB_DIR}
	debug ${Boost_LIBRARY_DIR_DEBUG} optimized ${Boost_LIBRARY_DIR_RELEASE}
)

add_executable( 
	dc-viewer 
	${VIEWER_ROOT_INC_FILES} 
	${VIEWER_ROOT_SRC_FILES} 
)

target_link_libraries( 
	dc-viewer 
	debug OgreMainStatic_d optimized OgreMainStatic 
	debug RenderSystem_Direct3D9Static_d optimized RenderSystem_Direct3D9Static
	debug RenderSystem_Direct3D11Static_d optimized RenderSystem_Direct3D11Static 
	d3d9
	d3dx9 
	DxErr
	dxguid
	d3d11
	dxgi
	debug RenderSystem_GLStatic_d optimized RenderSystem_GLStatic
	opengl32 
	glu32 
	debug FreeImage_d optimized FreeImage
	debug OIS_d optimized OIS
	debug zlib_d optimized zlib
	debug zziplib_d optimized zziplib
)

IF( WIN32 )
	ADD_DEFINITIONS( 
		-DOGRE_STATIC 
	)
ENDIF(  )