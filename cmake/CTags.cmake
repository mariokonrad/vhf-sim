find_program(CTAGS_PATH ctags)
find_program(CSCOPE_PATH cscope)
if(CTAGS_PATH AND CSCOPE_PATH)
	message(STATUS "Found ctags: ${CTAGS_PATH}")
	message(STATUS "Found cscope: ${CSCOPE_PATH}")

	set_source_files_properties(cscope.files PROPERTIES GENERATED true)
	set_source_files_properties(cscope.out PROPERTIES GENERATED true)
	set_source_files_properties(tags PROPERTIES GENERATED true)

	file(GLOB_RECURSE SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.?pp")
	string(REPLACE ";" "\n" SOURCE_FILES "${SOURCE_FILES}")
	file(WRITE "${CMAKE_BINARY_DIR}/cscope.files.in" "${SOURCE_FILES}")
	configure_file("${CMAKE_BINARY_DIR}/cscope.files.in" "${CMAKE_BINARY_DIR}/cscope.files" COPYONLY)

	add_custom_target(tags
		COMMAND ${CTAGS_PATH} --c++-kinds=+p --fields=+iaS --extra=+q -L ${CMAKE_BINARY_DIR}/cscope.files
		COMMAND ${CSCOPE_PATH} -b
		WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
		)
endif()
