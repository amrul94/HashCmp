cmake_minimum_required(VERSION 3.17)
project(ThesisWork)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS -pthread)

add_subdirectory(HashLib HashLibBuildDir)
add_subdirectory(CSV_Parser CSV_ParserBuildDir)

set(MAIN src/main.cpp)
set(GENERATORS src/generators.h src/generators.cpp)
set(UTILITIES src/log_duration.h)
set(WRAPPERS src/hash_wrappers.h src/hash_wrappers.cpp src/hashes.h src/hashes.cpp)
set(TESTS src/count_collisions.h src/count_collisions.cpp)
set(IMG_FILES src/img.h src/img.cpp src/jpeg_image.h src/jpeg_image.cpp)

set(RANDOM_WORD_TEST_MAIN src/random_words_test.cpp )
add_executable(random_words_test ${RANDOM_WORD_TEST_MAIN} ${GENERATORS} ${UTILITIES} ${WRAPPERS} ${TESTS})
target_link_libraries(random_words_test HashLib)
target_include_directories(random_words_test PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/HashLib")

set(IMAGES_TESTS_MAIN src/images_tests.cpp)
add_executable(images_tests ${IMAGES_TESTS_MAIN} ${IMG_FILES} ${WRAPPERS} ${UTILITIES} ${TESTS})

if(CMAKE_SYSTEM_NAME MATCHES "^MINGW")
    set(SYSTEM_LIBS -lstdc++)
else()
    set(SYSTEM_LIBS)
endif()

#set(LIBJPEG_DIR "/general/Amrulla/Programming/C++/Дипломная работа/Thesis work/libjpeg")

target_include_directories(images_tests PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/HashLib" "${LIBJPEG_DIR}/include")
target_link_directories(images_tests INTERFACE "${LIBJPEG_DIR}/lib/$<IF:$<CONFIG:Debug>,Debug,Release>")
target_link_libraries(images_tests HashLib jpeg "${SYSTEM_LIBS}" )





