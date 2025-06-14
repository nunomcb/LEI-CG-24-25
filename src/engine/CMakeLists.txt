set(SOURCE_FILES
        main.cpp
        config.cpp
        stb_image.h
        opengl/Frustum.cpp
        opengl/Frustum.h
        opengl/Mat4x4.cpp
        opengl/Mat4x4.h
        opengl/OpenGlCamera.cpp
        opengl/OpenGlCamera.h
        opengl/OpenGlScene.cpp
        opengl/OpenGlScene.h
        opengl/OpenGlShaderProgram.cpp
        opengl/OpenGlShaderProgram.h
        opengl/OpenGlSkybox.cpp
        opengl/OpenGlSkybox.h
        opengl/light/OpenGlDirectionalLight.cpp
        opengl/light/OpenGlDirectionalLight.h
        opengl/light/OpenGlLight.h
        opengl/light/OpenGlPointLight.cpp
        opengl/light/OpenGlPointLight.h
        opengl/light/OpenGlSpotLight.cpp
        opengl/light/OpenGlSpotLight.h
        opengl/BoundingSphere.cpp
        opengl/BoundingSphere.h
        opengl/model/OpenGlModelBuffer.h
        opengl/model/OpenGlModelColour.cpp
        opengl/model/OpenGlModelColour.h
        opengl/model/OpenGlModelDrawer.h
        opengl/model/OpenGlModelTexture.cpp
        opengl/model/OpenGlModelTexture.h
        opengl/model/OpenGlTriangleModelDrawer.cpp
        opengl/model/OpenGlTriangleModelDrawer.h
        opengl/model/OpenGlTriangleModelNormalDrawer.cpp
        opengl/model/OpenGlTriangleModelNormalDrawer.h
        opengl/model/OpenGlTriangleModelBuffer.cpp
        opengl/model/OpenGlTriangleModelBuffer.h
        opengl/model/OpenGlLineModelBuffer.cpp
        opengl/model/OpenGlLineModelBuffer.h
        opengl/transform/OpenGlTransform.h
        opengl/transform/OpenGlTranslate.cpp
        opengl/transform/OpenGlTranslate.h
        opengl/OpenGlGroup.cpp
        opengl/OpenGlGroup.h
        opengl/transform/OpenGlRotate.cpp
        opengl/transform/OpenGlRotate.h
        opengl/transform/OpenGlScale.cpp
        opengl/transform/OpenGlScale.h
        Color.h
        opengl/OpenGlWindow.cpp
        opengl/OpenGlWindow.h
        opengl/transform/OpenGlCatmullRomTranslate.cpp
        opengl/transform/OpenGlCatmullRomTranslate.h
)

set(THIS_MODULE engine)

add_executable(${THIS_MODULE} ${SOURCE_FILES})

find_package(OpenGL REQUIRED)
include_directories(${OpenGL_INCLUDE_DIRS})
link_directories(${OpenGL_LIBRARY_DIRS})
add_definitions(${OpenGL_DEFINITIONS})

if (NOT OPENGL_FOUND)
    message(ERROR " OPENGL not found!")
endif (NOT OPENGL_FOUND)

find_package(GLUT REQUIRED)
include_directories(${GLUT_INCLUDE_DIR})
link_directories(${GLUT_LIBRARY_DIRS})
add_definitions(${GLUT_DEFINITIONS})

if (NOT GLUT_FOUND)
    message(ERROR ": GLUT not found!")
endif (NOT GLUT_FOUND)

find_package(GLEW REQUIRED)
include_directories(${GLEW_INCLUDE_DIRS})
link_directories(${GLEW_LIBRARY_DIRS})
add_definitions(${GLEW_DEFINITIONS})

if (NOT GLEW_FOUND)
    message(ERROR ": GLEW not found!")
endif (NOT GLEW_FOUND)

set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} Wno-deprecated")

target_link_libraries(${THIS_MODULE} ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} ${GLEW_LIBRARIES} model tinyxml)