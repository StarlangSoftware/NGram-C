cmake_minimum_required(VERSION 3.23)
project(NGram C)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_FLAGS "-O3")

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_library(NGram src/NGramNode.c src/NGramNode.h src/MultipleFile.c src/MultipleFile.h src/NGram.c src/NGram.h src/NoSmoothing.c src/NoSmoothing.h src/GoodTuringSmoothing.c src/GoodTuringSmoothing.h src/LaplaceSmoothing.c src/LaplaceSmoothing.h src/NoSmoothingWithDictionary.c src/NoSmoothingWithDictionary.h src/NoSmoothingWithNonRareWords.c src/NoSmoothingWithNonRareWords.h src/TrainedSmoothing.c src/TrainedSmoothing.h src/AdditiveSmoothing.c src/AdditiveSmoothing.h src/InterpolatedSmoothing.c src/InterpolatedSmoothing.h)
target_link_libraries(NGram ${CONAN_LIBS})
