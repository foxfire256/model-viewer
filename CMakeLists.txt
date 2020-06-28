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
	set(GUI_LIBS ${GUI_LIBS}
		optimized "C:\\vclibs\\vclibs\\lib\\glew32.lib"
		debug "C:\\vclibs\\vclibs\\lib\\glew32d.lib")
	set(GUI_LIBS ${SDL2_LIBS} ${GUI_LIBS} ${OPENGL_LIBRARY})
	
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
	find_package(Boost COMPONENTS program_options system filesystem REQUIRED)
	include_directories(${Boost_INCLUDE_DIRS})
	find_package(GLEW REQUIRED)
	include_directories(${GLEW_INCLUDE_DIRS})
	
	set(BOOST_LIBS ${Boost_LIBRARIES})
	set(LIBS ${LIBS} -lmingw32)
	set(SDL2_LIBS -lSDL2main -lSDL2)
	set(GUI_LIBS ${SDL2_LIBS} ${OPENGL_LIBRARY} ${GLEW_LIBRARIES})
	
	set(EDITOR_LIBS ${EDITOR_LIBS} -lmingw32)
	
	include_directories("c:/msys64/mingw64/include/eigen3")
endif(MINGW OR MSYS)

# this probably only works on Arch Linux
if(UNIX AND NOT APPLE)
	set(CMAKE_PREFIX_PATH "/usr")
	set(CMAKE_INCLUDE_PATH "/usr/include")
	set(CMAKE_LIBRARY_PATH "/usr/lib")

	find_package(OpenGL REQUIRED)
	find_library(OPENGL_LIBRARY NAMES GL)
	find_package(Boost COMPONENTS program_options system filesystem REQUIRED)
	include_directories(${Boost_INCLUDE_DIRS})
	find_package(GLEW REQUIRED)
	include_directories(${GLEW_INCLUDE_DIRS})

	INCLUDE(FindPkgConfig)

	PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)
	include_directories(${SDL2_INCLUDE_DIR})

	set(BOOST_LIBS ${Boost_LIBRARIES})
	set(LIBS ${LIBS})
	set(SDL2_LIBS ${SDL2_LIBRARIES})
	set(GUI_LIBS ${SDL2_LIBS} ${OPENGL_LIBRARY} ${GLEW_LIBRARIES})

	include_directories("/usr/include")
	include_directories("/usr/include/eigen3")

endif(UNIX AND NOT APPLE)


if(NOT MSVC)
	set(CMAKE_C_FLAGS_RELEASE "-Ofast")
	set(CMAKE_CXX_FLAGS_RELEASE "-Ofast")
	
	# enable C11/C++14 for gcc and clang
	set(EXTRA_CXX_FLAGS "-std=c++17 -Wno-deprecated-declarations")
	set(EXTRA_C_FLAGS "-std=c11 -Wno-deprecated-declarations")
	
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${EXTRA_C_FLAGS}")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${EXTRA_CXX_FLAGS}")
endif(NOT MSVC)

set(MAIN_SOURCE
	main.cpp
	main_observer.hpp
	main_observer.cpp
	sdl_message_handler.hpp
	sdl_message_handler.cpp
)
set(COMMON_GUI_SOURCE
	../common-cpp/fox/counter.hpp
	../common-cpp/fox/counter.cpp
	../common-cpp/fox/gfx/eigen_opengl.hpp
	../common-cpp/fox/gfx/eigen_opengl.cpp
	../common-cpp-old/jds_obj_model.h
	../common-cpp-old/jds_obj_model.c
	../common-cpp-old/jds_shader.hpp
	../common-cpp-old/jds_shader.cpp
	../common-cpp/fox/gfx/gl_glew.h
	../common-cpp/fox/gfx/gl_glew.c
	../common-cpp/fox/gfx/opengl_error_checker.h
	../common-cpp/fox/gfx/opengl_error_checker.c
	../common-cpp/fox/gfx/model_loader_obj.hpp
	../common-cpp/fox/gfx/model_loader_obj.cpp
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

if(MSVC)
	set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
endif(MSVC)

MESSAGE( STATUS "MINGW: " ${MINGW} )
MESSAGE( STATUS "MSYS: " ${MSYS} )
MESSAGE( STATUS "MSVC: " ${MSVC} )
MESSAGE( STATUS "APPLE: " ${APPLE} )
