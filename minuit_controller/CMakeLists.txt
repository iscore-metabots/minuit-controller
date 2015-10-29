project(minuit_controller)

find_package(SDL REQUIRED)
find_package(SDL_mixer REQUIRED)

include_directories(${SDL_INCLUDE_DIR})
file(GLOB_RECURSE SRCS "*.h" "*.hpp" "*.c" "*.cpp")

add_executable(metabots_minuit ${SRCS})

target_link_libraries(metabots_minuit ${SDL_LIBRARY} ${SDLMIXER_LIBRARY})
