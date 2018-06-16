ADD_LIBRARY(
  peelocpp_unicode
  convert.cpp
  test.cpp
  utf8.cpp
)
INSTALL(
  DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../include/peelo
  DESTINATION include
  FILES_MATCHING PATTERN "*.hpp"
)
INSTALL(
  TARGETS peelocpp_unicode
  ARCHIVE
  DESTINATION lib
)
