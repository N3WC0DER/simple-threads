find_package(Doxygen)

if (Doxygen_FOUND)
    if (NOT STH_DOXYGEN_LANGUAGE)
        set(STH_DOXYGEN_LANGUAGE English)
    endif()
	set(DOXYGEN_INPUT ${CMAKE_SOURCE_DIR})
	set(DOXYGEN_OUTPUT ${CMAKE_SOURCE_DIR}/doc)

	set(DOXYFILE_IN ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in)
	set(DOXYFILE_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

	configure_file(${DOXYFILE_IN} ${DOXYFILE_OUT} @ONLY)
	message(STATUS "Doxygen build started")

	add_custom_target(docs ALL
		COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYFILE_OUT}
		WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
		COMMENT "Generating API documentation with Doxygen"
		VERBATIM )
else ()
	message(STATUS "WARNING: Doxygen not found")
endif ()
