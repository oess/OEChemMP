cmake_minimum_required(VERSION 2.8.9)

project(OpenEyeMP)

ENABLE_TESTING()

set (OPENEYE_EXAMPLES_DIR "${PROJECT_SOURCE_DIR}/openeye/toolkits/examples")

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin/)
add_executable(text2hex ${OPENEYE_EXAMPLES_DIR}/text2hex.cpp)

set (OE_LIBS 
  oechem
  oesystem
  oeplatform
)
set (LIBS "${OMPLIBS};${OE_LIBS};${SYSLIBS};${SYSZLIB}")

set (SRCS
    "molgrep molgrep.cpp molgrep.itf"
    "memsearch memsearch.cpp"
    "nxnlingos nxnlingos.cpp"
)

add_definitions("${OMPCFLAGS}")
include (${OPENEYE_EXAMPLES_DIR}/ExamplesTemplate.cmake)