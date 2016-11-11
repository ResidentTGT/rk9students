
# 
# External dependencies
# 

# find_package(THIRDPARTY REQUIRED)


# 
# Executable name and options
# 

# Target name
set(target lattice)

# Exit here if required dependencies are not met
message(STATUS "App: ${target}")


# 
# Sources
# 

set(sources
    main.cpp
    fparser.cpp
    RectLattice.cpp
    MarchingCubes.cpp
)

set(headers
    fparser.h
	RectLattice.h
	PointWithDensity.h
	MarchingCubes.h
	LookUpTable.h
	glm/glm.hpp
)

#add_subdirectory(glm)

# 
# Create executable
# 

message (STATUS ${ROOT_HEADER})

# Build executable
add_executable(${target}
    MACOSX_BUNDLE
    ${sources}
    ${headers}
)



# Create namespaced alias
add_executable(${META_PROJECT_NAME}::${target} ALIAS ${target})


# 
# Project options
# 

set_target_properties(${target}
    PROPERTIES
    ${DEFAULT_PROJECT_OPTIONS}
    FOLDER "${IDE_FOLDER}"
)


# 
# Include directories
# 

#target_include_directories(${target}
#    PRIVATE
#    #${DEFAULT_INCLUDE_DIRECTORIES}
#    ${PROJECT_BINARY_DIR}/src/include
#)


# 
# Libraries
# 

target_link_libraries(${target}
    PRIVATE
    ${DEFAULT_LIBRARIES}
    ${META_PROJECT_NAME}::common
)


# 
# Compile definitions
# 

target_compile_definitions(${target}
    PRIVATE
    ${DEFAULT_COMPILE_DEFINITIONS}
)


# 
# Compile options
# 

target_compile_options(${target}
    PRIVATE
    ${DEFAULT_COMPILE_OPTIONS}
)


# 
# Linker options
# 

target_link_libraries(${target}
    PRIVATE
    ${DEFAULT_LINKER_OPTIONS}
)



# 
# Deployment
# 

# Executable
install(TARGETS ${target}
    RUNTIME DESTINATION ${INSTALL_BIN} COMPONENT apps
    BUNDLE  DESTINATION ${INSTALL_BIN} COMPONENT apps
)
