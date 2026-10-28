include(FetchContent)

FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.8.1
  GIT_SHALLOW    TRUE
)

FetchContent_MakeAvailable(Catch2)

# Catch2 defaults to C++14; compile it as C++17 so StringMaker specializations
# for std::string_view are linked. Otherwise macOS/AppleClang fails when tests
# compare string_view values (headers declare the specialization under C++17,
# but the Catch2 archive was built without the definition).
set_target_properties(Catch2 PROPERTIES
  CXX_STANDARD 17
  CXX_STANDARD_REQUIRED ON
)
set_target_properties(Catch2WithMain PROPERTIES
  CXX_STANDARD 17
  CXX_STANDARD_REQUIRED ON
)

list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
include(Catch)

file(GLOB TEST_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
foreach(TEST_FILENAME ${TEST_SOURCES})
  get_filename_component(TEST_NAME ${TEST_FILENAME} NAME_WE)
  add_executable(${TEST_NAME} ${TEST_FILENAME})

  target_include_directories(
    ${TEST_NAME}
    PRIVATE
      ${CMAKE_CURRENT_SOURCE_DIR}/../include
  )

  target_compile_features(
    ${TEST_NAME}
    PRIVATE
      cxx_std_17
  )

  if(MSVC)
    target_compile_options(
      ${TEST_NAME}
      PRIVATE
        /W4 /WX /utf-8
    )
  else()
    target_compile_options(
      ${TEST_NAME}
      PRIVATE
        -Wall -Werror
    )
  endif()

  target_link_libraries(
    ${TEST_NAME}
    PRIVATE
      PeeloUnicode
      Catch2::Catch2WithMain
  )

  catch_discover_tests(${TEST_NAME})
endforeach()
