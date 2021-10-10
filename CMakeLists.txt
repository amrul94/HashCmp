cmake_minimum_required(VERSION 3.17)
project(ThesisWork)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS -pthread)

add_subdirectory(HashLib HashLibBuildDir)
add_subdirectory(CSV_Parser CSV_ParserBuildDir)

#set(LIBJPEG_DIR "/general/Amrulla/Programming/C++/Дипломная работа/Thesis work/libjpeg")

set(GENERATORS src/generators.h src/generators.cpp)
set(TESTS src/count_collisions.h src/count_collisions.cpp)
set(UTILITIES src/log_duration.h)
set(WRAPPERS src/hash_wrappers.h src/hash_wrappers.cpp src/hashes.h src/hashes.cpp)
set(IMG_FILES src/img.h src/img.cpp src/jpeg_image.h src/jpeg_image.cpp)

set(GENERAL_FILES ${TESTS} ${UTILITIES} ${WRAPPERS} ${IMG_FILES})

set(WORDS_TEST_MAIN src/words_test.cpp)
add_executable(words_test ${WORDS_TEST_MAIN} ${GENERATORS} ${GENERAL_FILES})
target_include_directories(words_test PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/HashLib" "${LIBJPEG_DIR}/include")
target_link_directories(words_test INTERFACE "${LIBJPEG_DIR}/lib/$<IF:$<CONFIG:Debug>,Debug,Release>")
target_link_libraries(words_test HashLib jpeg)

set(IMAGES_TESTS_MAIN src/images_tests.cpp)
add_executable(images_tests ${IMAGES_TESTS_MAIN} ${GENERAL_FILES})
target_include_directories(images_tests PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/HashLib" "${LIBJPEG_DIR}/include")
target_link_directories(images_tests INTERFACE "${LIBJPEG_DIR}/lib/$<IF:$<CONFIG:Debug>,Debug,Release>")
target_link_libraries(images_tests HashLib jpeg)

set(CHECK_DIST_MAIN src/check_distribution.cpp)
add_executable(check_distribution ${CHECK_DIST_MAIN} ${GENERATORS} ${GENERAL_FILES})
target_include_directories(check_distribution PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/HashLib" "${LIBJPEG_DIR}/include")
target_link_directories(check_distribution INTERFACE "${LIBJPEG_DIR}/lib/$<IF:$<CONFIG:Debug>,Debug,Release>")
target_link_libraries(check_distribution HashLib jpeg)



