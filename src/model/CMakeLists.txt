set(SOURCE_FILES
        Model.cpp
        ModelBuilder.cpp
        Point.cpp
        TextureCoord.cpp
        Vector.cpp
        Vertex.cpp
        serialise/ObjSerialiser.cpp
        include/model/serialise/ModelSerialiser.h
        include/model/serialise/ObjSerialiser.h
        include/model/Face.h
        include/model/Model.h
        include/model/ModelBuilder.h
        include/model/Point.h
        include/model/TextureCoord.h
        include/model/Vector.h
        include/model/Vertex.h
)

set(THIS_MODULE model)

add_library(${THIS_MODULE} STATIC ${SOURCE_FILES})

target_include_directories(${THIS_MODULE} PRIVATE include/${THIS_MODULE})
target_include_directories(${THIS_MODULE} PUBLIC include)