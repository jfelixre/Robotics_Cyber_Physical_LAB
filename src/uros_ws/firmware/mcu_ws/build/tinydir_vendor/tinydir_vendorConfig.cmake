# prevent multiple inclusion
if(_tinydir_vendor_CONFIG_INCLUDED)
  return()
endif()
set(_tinydir_vendor_CONFIG_INCLUDED TRUE)

# output package information
if(NOT tinydir_vendor_FIND_QUIETLY)
  message(STATUS "Found tinydir_vendor:  (${tinydir_vendor_DIR})")
endif()

set(tinydir_vendor_INCLUDE_DIRS "${tinydir_vendor_DIR}/../../../include")
