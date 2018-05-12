PROJECT(itlpExamples C)
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
SET(CMAKE_BUILD_TYPE Debug)

SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Werror -Wpedantic -std=gnu99")

ADD_EXECUTABLE(listSongs listsongs.c)
TARGET_LINK_LIBRARIES(listSongs itlp)
TARGET_LINK_LIBRARIES(listSongs crypto)
TARGET_LINK_LIBRARIES(listSongs z)

ADD_EXECUTABLE(listPlaylistsAndContent listPlaylistsAndContent.c)
TARGET_LINK_LIBRARIES(listPlaylistsAndContent itlp)
TARGET_LINK_LIBRARIES(listPlaylistsAndContent crypto)
TARGET_LINK_LIBRARIES(listPlaylistsAndContent z)
