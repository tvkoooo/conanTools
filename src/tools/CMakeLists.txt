# target
aux_source_directory(. SRC)
file(GLOB_RECURSE INTERNAL_SRC internal/*.h internal/*.cpp )
file(GLOB_RECURSE HEADERS *.h *.hpp)
source_group("include" FILES ${HEADERS})
source_group("internal" FILES ${INTERNAL_SRC})

set(ETARGET_NAME tools)
add_library(${ETARGET_NAME} ${TARGET_TYPE} ${SRC} ${HEADERS} ${INTERNAL_SRC})
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

#message(FATAL_ERROR  ${PROJECT_BINARY_DIR}/bin)

# definitions
IF (CMAKE_SYSTEM_NAME MATCHES "Windows" AND TARGET_TYPE MATCHES "SHARED")
	target_compile_definitions(${ETARGET_NAME} PUBLIC "FRAMEWORK_TOOL_API=__declspec(dllimport)")
	target_compile_definitions(${ETARGET_NAME} PRIVATE "FRAMEWORK_TOOL_API=__declspec(dllexport)")
ELSE ()
	target_compile_definitions(${ETARGET_NAME} PUBLIC "FRAMEWORK_TOOL_API=")
ENDIF()

# includes
#target_include_directories(${ETARGET_NAME} PUBLIC ${ETOOL_LIBRARY_PATH}/oatpp/include)

# libs
#target_link_directories(${ETARGET_NAME} PUBLIC ${ETOOL_LIBRARY_PATH}/oatpp/lib/${CMAKE_SYSTEM_NAME}/${CMAKE_BUILD_TYPE})


IF (CMAKE_SYSTEM_NAME MATCHES "Windows")
 	target_link_libraries(${ETARGET_NAME} PUBLIC ws2_32)
ELSEIF (CMAKE_SYSTEM_NAME MATCHES "QNX") 
        target_link_libraries(${ETARGET_NAME}  -Wl,--start-group slog2 socket -Wl,--end-group)
ELSEIF (CMAKE_SYSTEM_NAME MATCHES "Linux") 
        target_link_libraries(${ETARGET_NAME}  -Wl,--start-group slog2 socket -Wl,--end-group)
ENDIF()
# installs
install(TARGETS ${ETARGET_NAME} 
	LIBRARY DESTINATION ${ETARGET_NAME}/lib/${CMAKE_SYSTEM_NAME}/${CMAKE_BUILD_TYPE}
	ARCHIVE DESTINATION ${ETARGET_NAME}/lib/${CMAKE_SYSTEM_NAME}/${CMAKE_BUILD_TYPE}
	RUNTIME DESTINATION ${PROJECT_BINARY_DIR}/output
	)
install(TARGETS ${ETARGET_NAME} 
	LIBRARY DESTINATION ${PROJECT_BINARY_DIR}/output
	RUNTIME DESTINATION ${ETARGET_NAME}/lib/${CMAKE_SYSTEM_NAME}/${CMAKE_BUILD_TYPE}
	)
install(FILES ${HEADERS} DESTINATION  ${ETARGET_NAME}/include)
