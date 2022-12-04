cmake_minimum_required(VERSION 3.6.0)
project(simple-threads
        LANGUAGES CXX C
        VERSION 0.1.0
        DESCRIPTION "Simple thread control")

option(STH_BUILD_EXAMPLES "Build ${PROJECT_NAME} examples" OFF)
option(STH_GENERATE_DOCS "Generate ${PROJECT_NAME} docs" OFF)
option(STH_BUILD_TESTS "Build ${PROJECT_NAME} tests by using Google Test framework" OFF)

if (STH_GENERATE_DOCS)
    add_subdirectory(doc)
else ()
	get_directory_property(IS_SUBPROJECT PARENT_DIRECTORY)

	add_library(${PROJECT_NAME})
	add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

	target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)

	target_include_directories(${PROJECT_NAME}
			PUBLIC
			"$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
			"$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>"
			"$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>")

	file(GLOB SRCS "${PROJECT_SOURCE_DIR}/src/*")

	target_sources(${PROJECT_NAME} PRIVATE ${SRCS})

	install(DIRECTORY include/${PROJECT_NAME} DESTINATION include)

	if (STH_BUILD_EXAMPLES)
		add_subdirectory(examples)
	endif ()

	if (STH_BUILD_TESTS)
		enable_testing()
		add_subdirectory(tests)
	endif ()
endif ()
