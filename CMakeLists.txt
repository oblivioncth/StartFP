#================= Project Setup ==========================

# CMake
cmake_minimum_required(VERSION 3.21.1)

# Project
# NOTE: DON'T USE TRAILING ZEROS IN VERSIONS
set(STARTFP_BASE_VERSION 0.1) # Required for CI/CD
project(StartFP
    VERSION ${STARTFP_BASE_VERSION}
    LANGUAGES C
    DESCRIPTION "Fancy Flashpoint starter"
)

set(FORMAL_NAME "Start Flashpoint")
string(TOLOWER ${PROJECT_NAME} PROJECT_NAME_LC)
string(TOUPPER ${PROJECT_NAME} PROJECT_NAME_UC)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

# C
set(CMAKE_C_STANDARD 99)

# Build augmentation
enable_language("RC")

# Add local modules
set(PROJ_SCRIPTS_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
list(APPEND CMAKE_MODULE_PATH "${PROJ_SCRIPTS_PATH}/module")

# General Variables
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
  set(TARGET_ARCH x64)
else()
  set(TARGET_ARCH x86)
endif()

#--------------Top Level Project Setup-------------

if(${PROJECT_IS_TOP_LEVEL})
    message(STATUS "NOTE: ${PROJECT_NAME} is being configured as a top-level project")

    # Install (override the CMake default, but not a user set value)
    if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
        set(CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_BINARY_DIR}/out/install"
               CACHE PATH "Project install path" FORCE
        )
    endif()

    # Clean install when clean target is ran
    set_directory_properties(PROPERTIES ADDITIONAL_CLEAN_FILES "${CMAKE_INSTALL_PREFIX}")
else()
    message(STATUS "NOTE: ${PROJECT_NAME} is being configured as a sub-project")

    # Keep install components out of 'all' target
    set(SUB_PROJ_EXCLUDE_FROM_ALL "EXCLUDE_FROM_ALL")
endif()

#--------------------Setup Paths-------------------

# Cmake scripts
set(FILE_TEMPLATES_PATH "${PROJ_SCRIPTS_PATH}/file_templates")

# Package
set(PACKAGE_PREFIX "${CMAKE_CURRENT_BINARY_DIR}/out/dist")

#------------Set Global Build Rules----------------

# Versioning
include(StartFP/VerboseVersioning)
setup_verbose_versioning(PROJECT_VERSION_VERBOSE)

#================= Build =========================

# Get FP launcher icon
include(StartFP/FetchFile)
fetch_file(
    NAME fp_launcher_icon
    URL "https://raw.githubusercontent.com/FlashpointProject/launcher/master/icons/icon.ico"
    PATH_VAR LAUNCHER_ICON_PATH
)

# Set app sources
set(C_SOURCES
    src/main.c
)

# Name here needs to be as unique as possible for when this project is included
# in another via FetchContent or add_subdirectory (prevent target clashes)
set(TARGET_NAME ${PROJECT_NAME_LC}_${PROJECT_NAME_LC})

# Add executable target
add_executable(${TARGET_NAME})

# Make alias target so target can be referred to with its friendly
# export name both internally and when part of another build tree
add_executable(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${TARGET_NAME})

# Add sources to target
target_sources(${TARGET_NAME}
    PRIVATE
        ${C_SOURCES}
)

# Configure target properties
set_target_properties(${TARGET_NAME} PROPERTIES
    VERSION ${PROJECT_VERSION}
    OUTPUT_NAME "${PROJECT_NAME}"
    EXPORT_NAME "${PROJECT_NAME}"
    MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>"
    # Application is designed to be as minimal as possible, so always use static C runtime library
)

# Set target exe details
include(StartFP/WinExecutableDetails)
set_win_executable_details(${TARGET_NAME}
    ICON "${LAUNCHER_ICON_PATH}"
    FILE_VER ${PROJECT_VERSION}
    PRODUCT_VER ${PROJECT_VERSION}
    COMPANY_NAME "oblivioncth"
    FILE_DESC "Persistent starter for BlueMaxima's Flashpoint Launcher"
    INTERNAL_NAME "StartFP"
    COPYRIGHT "Open Source @ 2022 oblivioncth"
    TRADEMARKS_ONE "All Rights Reserved"
    TRADEMARKS_TWO "GNU AGPL V3"
    ORIG_FILENAME "StartFP.exe"
    PRODUCT_NAME "${FORMAL_NAME}"
)

#================= Install ==========================

set(TOP_LEVEL_INSTALL_COMPONENT ${PROJECT_NAME_LC})

# Install executable
install(TARGETS ${TARGET_NAME}
    CONFIGURATIONS Release
    COMPONENT ${TARGET_NAME}
    EXPORT ${PROJECT_NAME}Targets
    ${SUB_PROJ_EXCLUDE_FROM_ALL} # "EXCLUDE_FROM_ALL" if project is not top-level
    RUNTIME DESTINATION bin   
)

# Install README and LICENSE
install(FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/README.md"
    "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE"
    COMPONENT ${TOP_LEVEL_INSTALL_COMPONENT}
    DESTINATION .
    ${SUB_PROJ_EXCLUDE_FROM_ALL} # "EXCLUDE_FROM_ALL" if project is not top-level
)

#====================== CPack ==============================

set(CPACK_PACKAGE_VENDOR "oblivioncth")
set(CPACK_PACKAGE_DIRECTORY "${PACKAGE_PREFIX}")
set(CPACK_PACKAGE_FILE_NAME "${PROJECT_NAME}_${PROJECT_VERSION_VERBOSE}_${CMAKE_SYSTEM_NAME}_${TARGET_ARCH}")
set(CPACK_GENERATOR "ZIP")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set(CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_SOURCE_DIR}/README.md")
include(CPack)
