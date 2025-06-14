set(THIS_MODULE generator)

set(SOURCE_FILES
        main.cpp
        BezierPatch.cpp
        BezierPatch.h
        ImprovedPrimitiveGenerator.cpp
        ImprovedPrimitiveGenerator.h
        PatchFileParser.cpp
        PatchFileParser.h
)

add_executable(${THIS_MODULE} ${SOURCE_FILES})

target_link_libraries(${THIS_MODULE} model tinyxml)