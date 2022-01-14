cmake_minimum_required(VERSION 3.17)
project(ThesisWork)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_C_FLAGS "-march=native -O3 -pipe -pthread")
set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -lboost_json")

set(LIBS libs)
add_subdirectory(${LIBS}/HashLib HashLibBuildDir)
set(LIBJPEG_DIR ${LIBS}/libjpeg)

set(SRC "src/cpp")
set(UTILITIES ${SRC}/log_duration.h ${SRC}/log_duration.cpp ${SRC}/blocks_generator.h ${SRC}/blocks_generator.cpp)
set(WRAPPERS ${SRC}/hash_wrappers.h ${SRC}/hash_wrappers.cpp ${SRC}/hashes.h ${SRC}/hashes.cpp)
set(HELP_STRUCTS ${SRC}/test_parameters.h ${SRC}/test_parameters.cpp)
set(IMG_FILES ${SRC}/img.h ${SRC}/img.cpp ${SRC}/jpeg_image.h ${SRC}/jpeg_image.cpp)
set(GENERAL_FILES ${UTILITIES} ${WRAPPERS} ${HELP_STRUCTS} ${IMG_FILES})

set(DIST_TESTS ${SRC}/distribution_tests.h ${SRC}/distribution_tests.cpp)
set(GEN_TESTS ${SRC}/generated_tests.h ${SRC}/generated_tests.cpp)
set(ENG_TESTS ${SRC}/english_tests.h ${SRC}/english_tests.cpp)
set(SPEED_TESTS ${SRC}/speed_tests.h ${SRC}/speed_tests.cpp)
SET(TESTS ${WORDS_TESTS} ${DIST_TESTS} ${GEN_TESTS} ${ENG_TESTS} ${SPEED_TESTS})

set(MAIN ${SRC}/main.cpp)
add_executable(tests ${MAIN} ${TESTS} ${GENERAL_FILES})
target_include_directories(tests PUBLIC "${LIBS}/HashLib" "${LIBJPEG_DIR}/include" "${LIBS}/PCG")
target_link_directories(tests INTERFACE "${LIBJPEG_DIR}/lib/$<IF:$<CONFIG:Debug>,Debug,Release>")
target_link_libraries(tests HashLib jpeg)

add_executable(local_tests ${SRC}/local_tests.cpp)

set(IMAGES_TESTS_MAIN ${SRC}/images_tests.cpp)
add_executable(images_tests ${IMAGES_TESTS_MAIN} ${GENERAL_FILES})
target_include_directories(images_tests PUBLIC "libs/HashLib" "${LIBJPEG_DIR}/include")
target_link_directories(images_tests INTERFACE "${LIBJPEG_DIR}/lib/$<IF:$<CONFIG:Debug>,Debug,Release>")
target_link_libraries(images_tests HashLib jpeg)