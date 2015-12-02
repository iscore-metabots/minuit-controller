project(minuit_controller)

find_package(SDL REQUIRED)

include_directories(${SDL_INCLUDE_DIR})
set(COMMON_SRCS minuit.c  device.c UDPserver.c str_array.c)
add_library(common OBJECT ${COMMON_SRCS})

set(MAIN_SRCS main.c metabot.c)
add_executable(linker ${MAIN_SRCS} $<TARGET_OBJECTS:common>)
target_link_libraries(linker ${SDL_LIBRARY})

find_package(SDL_mixer)
if(SDL_mixer)
set(MUSIC_SRCS musicPlayer.c)

add_executable(music ${MUSIC_SRCS} $<TARGET_OBJECTS:common>)
target_link_libraries(music ${SDL_LIBRARY} ${SDLMIXER_LIBRARY})
endif()
