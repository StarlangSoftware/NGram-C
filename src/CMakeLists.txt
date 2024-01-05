cmake_minimum_required(VERSION 3.23)
project(NGram)
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_FLAGS "-O3")

find_package(sampling_c REQUIRED)
find_package(math_c REQUIRED)

add_library(NGram NGramNode.c NGramNode.h MultipleFile.c MultipleFile.h NGram.c NGram.h NoSmoothing.c NoSmoothing.h GoodTuringSmoothing.c GoodTuringSmoothing.h LaplaceSmoothing.c LaplaceSmoothing.h NoSmoothingWithDictionary.c NoSmoothingWithDictionary.h NoSmoothingWithNonRareWords.c NoSmoothingWithNonRareWords.h TrainedSmoothing.c TrainedSmoothing.h AdditiveSmoothing.c AdditiveSmoothing.h InterpolatedSmoothing.c InterpolatedSmoothing.h)
target_link_libraries(NGram sampling_c::sampling_c math_c::math_c)
