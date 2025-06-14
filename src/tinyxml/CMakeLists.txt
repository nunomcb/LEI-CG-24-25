set(SOURCE_FILES tinyxml2.cpp)
set(THIS_MODULE tinyxml)

add_library(${THIS_MODULE} STATIC ${SOURCE_FILES})

target_include_directories(${THIS_MODULE} PRIVATE include/${THIS_MODULE})
target_include_directories(${THIS_MODULE} PUBLIC include)