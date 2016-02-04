include(CMakeGenericSystem)

find_program(GRAPHVIZ_DOT_EXECUTABLE
  NAMES dot dot.exe
  HINTS
    "${CMAKE_GENERIC_PROGRAM_FILES}/Graphviz/bin"
  DOC "Graphviz Dot tool for using Doxygen")

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Graphviz_dot DEFAULT_MSG GRAPHVIZ_DOT_EXECUTABLE)
get_filename_component(GRAPHVIZ_DOT_PATH "${GRAPHVIZ_DOT_EXECUTABLE}" PATH CACHE)

mark_as_advanced(
  GRAPHVIZ_DOT_EXECUTABLE
  GRAPHVIZ_DOT_PATH)
