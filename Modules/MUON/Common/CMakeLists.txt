set(MODULE_NAME "O2QcMUONCommon")

# ---- Files ----

set(SRCS
  src/Helpers.cxx
  src/MergeableTH1Ratio.cxx
  src/MergeableTH2Ratio.cxx
  src/HistPlotter.cxx
  src/MuonTrack.cxx
  src/TrackPlotter.cxx
  src/TracksCheck.cxx
  src/TracksTask.cxx
)

set(HEADERS
  include/MUONCommon/MergeableTH1Ratio.h
  include/MUONCommon/MergeableTH2Ratio.h
  include/MUONCommon/HistPlotter.h
  include/MUONCommon/MuonTrack.h
  include/MUONCommon/TrackCheck.h
  include/MUONCommon/TrackPlotter.h
  include/MUONCommon/TracksTask.h
)

# ---- Library ----

add_library(${MODULE_NAME} SHARED ${SRCS})

target_include_directories(
  ${MODULE_NAME}
  PUBLIC $<INSTALL_INTERFACE:include> $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(${MODULE_NAME} PUBLIC O2QualityControl O2::MCHMappingImpl4 O2::DataFormatsMCH O2::MCHTracking O2::MCHRawElecMap O2::ReconstructionDataFormats O2::MCHGeometryTransformer O2::DataFormatsGlobalTracking)

# Digit.h is moving from MCHBase to DataFormatsMCH : let's handle both
# gracefully for the moment...
get_target_property(O2_INCLUDE_DIRS O2::CommonDataFormat INTERFACE_INCLUDE_DIRECTORIES)
get_target_property(ROOT_INCLUDE_DIRS ROOT::Core INTERFACE_INCLUDE_DIRECTORIES)

set(CMAKE_REQUIRED_INCLUDES ${O2_INCLUDE_DIRS} ${ROOT_INCLUDE_DIRS})

install(
        TARGETS ${MODULE_NAME}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# ---- ROOT dictionary ----

add_root_dictionary(${MODULE_NAME}
                    HEADERS include/MUONCommon/MergeableTH1Ratio.h
                            include/MUONCommon/MergeableTH2Ratio.h
                            include/MUONCommon/TrackPlotter.h
                            include/MUONCommon/TracksCheck.h
                            include/MUONCommon/TracksTask.h
                    LINKDEF include/MUONCommon/LinkDef.h)

# ---- Tests ----

set(
  TEST_SRCS
)

foreach(test ${TEST_SRCS})
  get_filename_component(test_name ${test} NAME)
  string(REGEX REPLACE ".cxx" "" test_name ${test_name})

  add_executable(${test_name} ${test})
  target_link_libraries(${test_name} PRIVATE ${MODULE_NAME} Boost::unit_test_framework)
  add_test(NAME ${test_name} COMMAND ${test_name})
  set_tests_properties(${test_name} PROPERTIES TIMEOUT 60)
endforeach()
