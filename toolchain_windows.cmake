SET( CMAKE_SYSTEM_NAME Windows )
SET( CMAKE_SYSTEM_VERSION 7.0.0 )
SET( LIBRARY_PREFIX )
SET( LIBRARY_SUFFIX .dll)

# TODO: remove. these belong to kanzi find module.
IF (TARGET_TYPE MATCHES "SHARED")
	add_definitions("-DKANZI_API_IMPORT")
	add_definitions("-DKANZI_UI_API_IMPORT")
ENDIF()
add_definitions("-DWIN32")
add_definitions("-DKANZI_FEATURE_3D")
add_definitions("-DKZ_DLOAD_GL_SYMBOLS=12")
add_definitions("-DKZ_RUNTIME_GRAPHICS_BACKEND=1")
add_definitions("-DKZ_SUPPORT_GRAPHICS_API_OPENGL=1")
add_definitions("-DKZ_SUPPORT_GRAPHICS_API_GLES=1")
add_definitions("-DKZ_SUPPORT_GRAPHICS_CONTEXT_API_WGL=1")
add_definitions("-DKZ_SUPPORT_GRAPHICS_CONTEXT_API_EGL=1")
add_definitions("-DKZ_DLOAD_GLES_SYMBOLS=32")
add_definitions("-DKZ_DLOAD_EGL_SYMBOLS=1")
add_definitions("-D_CRT_SECURE_NO_WARNINGS")
add_definitions("-D_CONSOLE")
add_definitions("-DKZ_INLINE=inline")
add_definitions("-DKANZI_${CMAKE_BUILD_TYPE}")
add_definitions("-DUNICODE")
add_definitions("-D_UNICODE")
add_definitions("-D_WINDLL")
add_compile_options(/MP /Gm-)
