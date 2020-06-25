cmake_minimum_required(VERSION 2.8.12)
project(model-viewer)

# http://www.cmake.org/Wiki/CMake_Useful_Variables

set(CMAKE_INCLUDE_CURRENT_DIR ON)

include_directories("../common-cpp")
include_directories("../common-cpp-old")

# cmake .. -G "Visual Studio 16 2019" -A x64
if(MSVC)
	set(CMAKE_PREFIX_PATH "C:\\vclibs\\vclibs")
	set(CMAKE_INCLUDE_PATH "C:\\vclibs\\vclibs\\include")
	set(CMAKE_LIBRARY_PATH "C:\\vclibs\\vclibs\\lib")
	ADD_DEFINITIONS(-D_SCL_SECURE_NO_WARNINGS)
	ADD_DEFINITIONS(-D_CRT_SECURE_NO_DEPRECATE)
	
	SET(CMAKE_C_FLAGS "/MP ${CMAKE_C_FLAGS}")
	SET(CMAKE_CXX_FLAGS "/MP /std:c++17 /Oi ${CMAKE_CXX_FLAGS}")
	
	find_package(OpenGL REQUIRED)
	find_library(OPENGL_LIBRARY NAMES OpenGL32 opengl32)
	
	set(LIBS ${LIBS} kernel32.lib)
	set(SDL2_LIBS
		optimized "C:\\vclibs\\vclibs\\lib\\SDL2main.lib"
		debug "C:\\vclibs\\vclibs\\lib\\SDL2maind.lib")
	set(SDL2_LIBS ${SDL2_LIBS}
		optimized "C:\\vclibs\\vclibs\\lib\\SDL2.lib"
		debug "C:\\vclibs\\vclibs\\lib\\SDL2d.lib")
	set(GUI_LIBS ${SDL2_LIBS}
		optimized "C:\\vclibs\\vclibs\\lib\\SDL2_ttf.lib"
		debug "C:\\vclibs\\vclibs\\lib\\SDL2_ttfd.lib")
	set(GUI_LIBS ${GUI_LIBS}
		optimized "C:\\vclibs\\vclibs\\lib\\glew32.lib"
		debug "C:\\vclibs\\vclibs\\lib\\glew32d.lib")
	set(GUI_LIBS ${GUI_LIBS} ${OPENGL_LIBRARY})
	
	set(PNG_LIBRARIES
		optimized "C:\\vclibs\\vclibs\\lib\\libpng16.lib"
		debug "C:\\vclibs\\vclibs\\lib\\libpng16d.lib")
	
	set(Boost_USE_STATIC_LIBS OFF )
	set(Boost_USE_MULTITHREADED ON)
	set(Boost_USE_STATIC_RUNTIME OFF)
	
	find_package(Boost COMPONENTS program_options system filesystem REQUIRED)
	include_directories(${Boost_INCLUDE_DIRS})
	set(BOOST_LIBS ${Boost_LIBRARIES})
	
	include_directories("C:\\vclibs\\vclibs\\include")
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
	../common-cpp/fox/counter.hpp
	../common-cpp/fox/counter.cpp
	../common-cpp-old/jds_eigen_info.hpp
	../common-cpp-old/jds_eigen_info.cpp
	../common-cpp-old/jds_eigen_opengl.hpp
	../common-cpp-old/jds_eigen_opengl.cpp
	../common-cpp-old/jds_vertex_buffer_object.hpp
	../common-cpp-old/jds_vertex_buffer_object.cpp
	../common-cpp-old/jds_font.hpp
	../common-cpp-old/jds_font.cpp
	../common-cpp-old/jds_font_sdl.hpp
	../common-cpp-old/jds_font_sdl.cpp
	../common-cpp-old/jds_texture.hpp
	../common-cpp-old/jds_texture.cpp
	../common-cpp/fox/png_loader.hpp
	../common-cpp/fox/png_loader.cpp
	../common-cpp-old/jds_obj_model.h
	../common-cpp-old/jds_obj_model.c
	../common-cpp-old/jds_mesh.hpp
	../common-cpp-old/jds_mesh.cpp
	../common-cpp-old/jds_mesh_gfx.hpp
	../common-cpp-old/jds_mesh_gfx.cpp
	../common-cpp-old/jds_mesh_obj.hpp
	../common-cpp-old/jds_mesh_obj.cpp
	../common-cpp-old/jds_shader.hpp
	../common-cpp-old/jds_shader.cpp
	../common-cpp-old/jds_gl_glew.h
	../common-cpp-old/jds_gl_glew.c
	../common-cpp-old/sdl_message_handler.hpp
	../common-cpp-old/sdl_message_handler.cpp
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
	../common-cpp/events/observer.cpp
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
