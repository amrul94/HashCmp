cmake_minimum_required(VERSION 3.17)
project(ThesisWork)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "-pthread -lboost_json")

add_subdirectory(HashLib HashLibBuildDir)
#add_subdirectory(RandLib RandLibBuildDir)
add_subdirectory(CSV_Parser CSV_ParserBuildDir)

set(RAND_LIB_DIR RandLib)

#set(LIBJPEG_DIR "/general/Amrulla/Programming/C++/Дипломная работа/Thesis work/libjpeg")

set(GENERATORS src/generators.h src/generators.cpp)
set(TESTS src/count_collisions.h src/count_collisions.cpp)
set(UTILITIES src/log_duration.h)
set(WRAPPERS src/hash_wrappers.h src/hash_wrappers.cpp src/hashes.h src/hashes.cpp)
set(IMG_FILES src/img.h src/img.cpp src/jpeg_image.h src/jpeg_image.cpp)

set(GENERAL_FILES ${TESTS} ${UTILITIES} ${WRAPPERS} ${IMG_FILES})

set(IMAGES_TESTS_MAIN src/images_tests.cpp)
add_executable(images_tests ${IMAGES_TESTS_MAIN} ${GENERAL_FILES})
target_include_directories(images_tests PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/HashLib" "${LIBJPEG_DIR}/include")
target_link_directories(images_tests INTERFACE "${LIBJPEG_DIR}/lib/$<IF:$<CONFIG:Debug>,Debug,Release>")
target_link_libraries(images_tests HashLib jpeg)

set(MAIN src/main.cpp src/words_tests.h src/words_tests.cpp src/check_distribution.h src/check_distribution.cpp src/helper_structs.h src/helper_structs.cpp)
add_executable(tests ${MAIN} ${GENERATORS} ${GENERAL_FILES})
target_include_directories(tests PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/HashLib" "${LIBJPEG_DIR}/include" "${RAND_LIB_DIR}/PCG")
target_link_directories(tests INTERFACE "${LIBJPEG_DIR}/lib/$<IF:$<CONFIG:Debug>,Debug,Release>")
target_link_libraries(tests HashLib jpeg)



