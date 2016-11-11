
# 
# Configuration for all sub-projects
# 

# Generate version-header
string(TOUPPER ${META_PROJECT_NAME} META_PROJECT_NAME_UPPER)
configure_file(version.h.in ${CMAKE_CURRENT_BINARY_DIR}/include/${META_PROJECT_NAME}/${META_PROJECT_NAME}-version.h)


# 
# Sub-projects
# 

# Libraries
set(IDE_FOLDER "Libs")
add_subdirectory(common)


# Apps
set(IDE_FOLDER "Apps")
add_subdirectory(sample_app)

# Apps
set(IDE_FOLDER "Apps")
add_subdirectory(lattice)


# Apps
set(IDE_FOLDER "Apps")
add_subdirectory(boolean_poly)


# 
# Deployment
# 

# Deploy generated headers
install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/include/${META_PROJECT_NAME} DESTINATION include COMPONENT dev)
