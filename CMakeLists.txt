cmake_minimum_required(VERSION 2.8.12)
project(model-viewer)

# http://www.cmake.org/Wiki/CMake_Useful_Variables

set(CMAKE_INCLUDE_CURRENT_DIR ON)

include_directories("../common-cpp")

# this assumes you are running cmake from a build directory in the source directory
# cmake .. -G "Visual Studio 14 2015 Win64"
if(MSVC)
	ADD_DEFINITIONS(-D_SCL_SECURE_NO_WARNINGS)
	ADD_DEFINITIONS(-D_CRT_SECURE_NO_DEPRECATE)
	
	#SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}" " /MP")
	#SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}" " /MP")
	
	find_package(OpenGL REQUIRED)
	find_library(OPENGL_LIBRARY NAMES OpenGL32 opengl32)
	
	set(LIBS ${LIBS} kernel32.lib)
	set(SDL2_LIBS
		optimized "C:\\vclibs\\vc2015libs\\lib\\SDL2main.lib"
		debug "C:\\vclibs\\vc2015libs\\lib\\debug\\SDL2main.lib")
	set(SDL2_LIBS ${SDL2_LIBS}
		optimized "C:\\vclibs\\vc2015libs\\lib\\SDL2.lib"
		debug "C:\\vclibs\\vc2015libs\\lib\\debug\\SDL2.lib")
	set(GUI_LIBS ${SDL2_LIBS}
		optimized "C:\\vclibs\\vc2015libs\\lib\\SDL2_ttf.lib"
		debug "C:\\vclibs\\vc2015libs\\lib\\debug\\SDL2_ttf.lib")
	set(GUI_LIBS ${GUI_LIBS}
		optimized "C:\\vclibs\\vc2015libs\\lib\\glew32.lib"
		debug "C:\\vclibs\\vc2015libs\\lib\\debug\\glew32d.lib")
	set(GUI_LIBS ${GUI_LIBS} ${OPENGL_LIBRARY})
	
	set(PNG_LIBRARIES
		optimized "C:\\vclibs\\vc2015libs\\lib\\libpng16.lib"
		debug "C:\\vclibs\\vc2015libs\\lib\\debug\\libpng16d.lib")
	
	set(BOOST_LIBS ${BOOST_LIBS}
		optimized "C:\\vclibs\\vc2015libs\\lib\\libboost_program_options-vc140-mt-1_60.lib"
		debug "C:\\vclibs\\vc2015libs\\lib\\debug\\libboost_program_options-vc140-mt-gd-1_60.lib")
	set(BOOST_LIBS ${BOOST_LIBS}
		optimized "C:\\vclibs\\vc2015libs\\lib\\libboost_system-vc140-mt-1_60.lib"
		debug "C:\\vclibs\\vc2015libs\\lib\\debug\\libboost_system-vc140-mt-gd-1_60.lib")
	set(BOOST_LIBS ${BOOST_LIBS}
		optimized "C:\\vclibs\\vc2015libs\\lib\\libboost_filesystem-vc140-mt-1_60.lib"
		debug "C:\\vclibs\\vc2015libs\\lib\\debug\\libboost_filesystem-vc140-mt-gd-1_60.lib")
	
	include_directories("C:\\vclibs\\vc2015libs\\include")
endif(MSVC)

if(MINGW OR MSYS)
	set(CMAKE_PREFIX_PATH "c:/msys64/mingw64")
	set(CMAKE_INCLUDE_PATH "c:/msys64/mingw64/include")
	set(CMAKE_LIBRARY_PATH "c:/msys64/mingw64/lib")
	
	find_package(OpenGL REQUIRED)
	find_library(OPENGL_LIBRARY NAMES OpenGL32 opengl32)
	find_package(Freetype REQUIRED)
	include_directories(${FREETYPE_INCLUDE_DIRS})
	find_package(PNG REQUIRED)
	include_directories(${PNG_INCLUDE_DIR})
	find_package(Boost COMPONENTS program_options system filesystem REQUIRED)
	include_directories(${Boost_INCLUDE_DIRS})
	find_package(GLEW REQUIRED)
	include_directories(${GLEW_INCLUDE_DIRS})
	#find_package(OpenAL REQUIRED)
	#include_directories(${OPENAL_INCLUDE_DIRS})
	
	set(BOOST_LIBS ${Boost_LIBRARIES})
	set(LIBS ${LIBS} -lmingw32)
	set(SDL2_LIBS -lSDL2main -lSDL2)
	set(GUI_LIBS ${SDL2_LIBS} -lSDL2_ttf ${OPENGL_LIBRARY} ${GLEW_LIBRARIES})
	set(SOUND_LIBS -lSDL2_mixer -lvorbis -logg -lz)
	
	set(EDITOR_LIBS ${EDITOR_LIBS} -lmingw32)
	
	include_directories("c:/msys64/mingw64/include/eigen3")
endif(MINGW OR MSYS)

if(APPLE)
	set(CMAKE_PREFIX_PATH "/usr/local")
	set(CMAKE_INCLUDE_PATH "/usr/local/include")
	set(CMAKE_LIBRARY_PATH "/usr/local/lib")
	
	find_package(OpenGL REQUIRED)
	find_library(OPENGL_LIBRARY NAMES OpenGL)
	find_package(Freetype REQUIRED)
	include_directories(${FREETYPE_INCLUDE_DIRS})
	find_package(PNG REQUIRED)
	include_directories(${PNG_INCLUDE_DIR})
	find_package(Boost COMPONENTS program_options system filesystem REQUIRED)
	include_directories(${Boost_INCLUDE_DIRS})
	find_package(GLEW REQUIRED)
	include_directories(${GLEW_INCLUDE_DIRS})
	find_package(OpenAL REQUIRED)
	include_directories(${OPENAL_INCLUDE_DIRS})

	INCLUDE(FindPkgConfig)

	PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)
	include_directories(${SDL2_INCLUDE_DIR})

	set(BOOST_LIBS ${Boost_LIBRARIES})
	set(LIBS ${LIBS} -L/usr/local/lib)
	set(SDL2_LIBS ${SDL2_LIBRARIES})
	set(GUI_LIBS ${SDL2_LIBS} -lSDL2_ttf ${OPENGL_LIBRARY} ${GLEW_LIBRARIES})
	set(SOUND_LIBS -lSDL2_mixer -lvorbis -logg -lz)

	# If there are problems with qt5 on Apple run 'which qmake' and make sure
	# it is the correct one. Usually you have to run 'brew link qt5 --force'.
	# Anaconda causes problems too since it has qt4 and other libs.
	set(CMAKE_PREFIX_PATH "/usr/local/Cellar/qt5/5.6.1")
	set(CMAKE_INCLUDE_PATH "/usr/local/Cellar/qt5/5.6.1/include")
	set(CMAKE_LIBRARY_PATH "/usr/local/Cellar/qt5/5.6.1/lib")
	find_package(Qt5Core)
	find_package(Qt5Widgets)
	set(CMAKE_PREFIX_PATH "/usr/local")
	set(CMAKE_INCLUDE_PATH "/usr/local/include")
	set(CMAKE_LIBRARY_PATH "/usr/local/lib")
	include_directories("/usr/local/Cellar/qt5/5.6.1/include")

	set(EDITOR_LIBS -L/usr/local/Cellar/qt5/5.6.1/lib)
	
	include_directories("/usr/local/include")
	include_directories("/usr/local/include/eigen3")
endif(APPLE)

# this probably only works on Arch Linux
if(UNIX AND NOT APPLE)
	set(CMAKE_PREFIX_PATH "/usr")
	set(CMAKE_INCLUDE_PATH "/usr/include")
	set(CMAKE_LIBRARY_PATH "/usr/lib")

	find_package(OpenGL REQUIRED)
	find_library(OPENGL_LIBRARY NAMES GL)
	find_package(Freetype REQUIRED)
	include_directories(${FREETYPE_INCLUDE_DIRS})
	find_package(PNG REQUIRED)
	include_directories(${PNG_INCLUDE_DIR})
	find_package(Boost COMPONENTS program_options system filesystem REQUIRED)
	include_directories(${Boost_INCLUDE_DIRS})
	find_package(GLEW REQUIRED)
	include_directories(${GLEW_INCLUDE_DIRS})
	find_package(OpenAL REQUIRED)
	include_directories(${OPENAL_INCLUDE_DIRS})

	INCLUDE(FindPkgConfig)

	PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)
	include_directories(${SDL2_INCLUDE_DIR})

	set(BOOST_LIBS ${Boost_LIBRARIES})
	set(LIBS ${LIBS})
	set(SDL2_LIBS ${SDL2_LIBRARIES})
	set(GUI_LIBS ${SDL2_LIBS} -lSDL2_ttf ${OPENGL_LIBRARY} ${GLEW_LIBRARIES})
	set(SOUND_LIBS -lSDL2_mixer -lvorbis -logg -lz)

	include_directories("/usr/include")
	include_directories("/usr/include/eigen3")

endif(UNIX AND NOT APPLE)


if(NOT MSVC)
	# better optimizations
	# -O3 still IEEE754 compliant
	# -Ofast not IEEE754 compliant, faster than -O3
	# -ffast-math not needed with -Ofast
	# -ftree-vectorize not sure how well this does
	# -mavx
	# -mavx2
	set(CMAKE_C_FLAGS_RELEASE "-Ofast")
	set(CMAKE_CXX_FLAGS_RELEASE "-Ofast")
	
	# enable C11/C++14 for gcc and clang
	set(EXTRA_CXX_FLAGS "-std=c++14 -Wno-deprecated-declarations")
	set(EXTRA_C_FLAGS "-std=c11 -Wno-deprecated-declarations")
	
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${EXTRA_C_FLAGS}")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${EXTRA_CXX_FLAGS}")

	#set(CMAKE_BUILD_TYPE "debug")
endif(NOT MSVC)

set(MAIN_SOURCE
	main.cpp
	main_observer.hpp
	main_observer.cpp
)
set(COMMON_GUI_SOURCE
	../common-cpp/jds_counter.hpp
	../common-cpp/jds_counter.cpp
	../common-cpp/jds_eigen_info.hpp
	../common-cpp/jds_eigen_info.cpp
	../common-cpp/jds_eigen_opengl.hpp
	../common-cpp/jds_eigen_opengl.cpp
	../common-cpp/jds_vertex_buffer_object.hpp
	../common-cpp/jds_vertex_buffer_object.cpp
	../common-cpp/jds_font.hpp
	../common-cpp/jds_font.cpp
	../common-cpp/jds_font_sdl.hpp
	../common-cpp/jds_font_sdl.cpp
	../common-cpp/jds_texture.hpp
	../common-cpp/jds_texture.cpp
	../common-cpp/jds_png.hpp
	../common-cpp/jds_png.cpp
	../common-cpp/jds_obj_model.h
	../common-cpp/jds_obj_model.c
	../common-cpp/jds_mesh.hpp
	../common-cpp/jds_mesh.cpp
	../common-cpp/jds_mesh_gfx.hpp
	../common-cpp/jds_mesh_gfx.cpp
	../common-cpp/jds_mesh_obj.hpp
	../common-cpp/jds_mesh_obj.cpp
	../common-cpp/jds_shader.hpp
	../common-cpp/jds_shader.cpp
	../common-cpp/jds_gl_glew.h
	../common-cpp/jds_gl_glew.c
	../common-cpp/sdl_message_handler.hpp
	../common-cpp/sdl_message_handler.cpp
)
set(COMMON_EVENTS_SOURCE
	../common-cpp/events/console_writer.hpp
	../common-cpp/events/console_writer.cpp
	../common-cpp/events/message_base.hpp
	../common-cpp/events/message_info.hpp
	../common-cpp/events/message_error.hpp
	../common-cpp/events/manager_interface.hpp
	../common-cpp/events/manager.hpp
	../common-cpp/events/manager.cpp
	../common-cpp/events/message_framerate.hpp
	../common-cpp/events/message_keydown.hpp
	../common-cpp/events/message_keyup.hpp
	../common-cpp/events/message_mousebutton.hpp
	../common-cpp/events/message_mousemotion.hpp
	../common-cpp/events/message_mousewheel.hpp
	../common-cpp/events/message_resize.hpp
	../common-cpp/events/observer.hpp
	../common-cpp/events/subject.hpp
	../common-cpp/events/subject.cpp
)

set(GUI_SOURCE
	gui/gfx_sdl.hpp
	gui/gfx_sdl.cpp
)

add_executable(${PROJECT_NAME} ${MAIN_SOURCE} ${COMMON_EVENTS_SOURCE} ${COMMON_GUI_SOURCE} ${GUI_SOURCE})
target_link_libraries(${PROJECT_NAME} ${LIBS} ${GUI_LIBS} ${BOOST_LIBS} ${PNG_LIBRARIES})

# Instruct CMake to run Qt moc automatically when needed.
#set(CMAKE_AUTOMOC ON)
#add_executable(${PROJECT_NAME}_config_editor ${CONFIG_EDITOR_SOURCE})
#target_link_libraries(${PROJECT_NAME}_config_editor
#	${LBIS} ${EDITOR_LIBS} Qt5::Core Qt5::Widgets gcore)
#set(CMAKE_AUTOMOC OFF)


MESSAGE( STATUS "MINGW: " ${MINGW} )
MESSAGE( STATUS "MSYS: " ${MSYS} )
MESSAGE( STATUS "MSVC: " ${MSVC} )
MESSAGE( STATUS "APPLE: " ${APPLE} )
MESSAGE( STATUS "INCLUDE_DIRECTORIES: " ${INCLUDE_DIRECTORIES} )
MESSAGE( STATUS "CMAKE_C_FLAGS: " ${CMAKE_C_FLAGS} )
MESSAGE( STATUS "CMAKE_C_FLAGS_DEBUG: " ${CMAKE_C_FLAGS_DEBUG} )
MESSAGE( STATUS "CMAKE_C_FLAGS_RELEASE: " ${CMAKE_C_FLAGS_RELEASE} )
MESSAGE( STATUS "CMAKE_CXX_FLAGS: " ${CMAKE_CXX_FLAGS} )
MESSAGE( STATUS "CMAKE_CXX_FLAGS_DEBUG: " ${CMAKE_CXX_FLAGS_DEBUG} )
MESSAGE( STATUS "CMAKE_CXX_FLAGS_RELEASE: " ${CMAKE_CXX_FLAGS_RELEASE} )
