project(minuit_controller)

find_package(SDL REQUIRED)
find_package(SDL_mixer REQUIRED)

include_directories(${SDL_INCLUDE_DIR})
file(GLOB_RECURSE COMMON_SRCS
		minuit.c  device.c UDPserver.c str_array.c)
file(GLOB_RECURSE MAIN_SRCS
		main.c metabot.c)
file(GLOB_RECURSE MUSIC_SRCS
		musicPlayer.c
)
add_library(common OBJECT ${COMMON_SRCS})
add_executable(linker ${MAIN_SRCS} $<TARGET_OBJECTS:common>)
target_link_libraries(linker ${SDL_LIBRARY})


add_executable(music ${MUSIC_SRCS} $<TARGET_OBJECTS:common>)
target_link_libraries(music ${SDL_LIBRARY} ${SDLMIXER_LIBRARY})
