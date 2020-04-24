
cmake_minimum_required ( VERSION 3.13 )

project ( vos )
set ( CMAKE_CXX_STANDARD 11 )

include ( GNUInstallDirs )

include ( TestBigEndian )

TEST_BIG_ENDIAN ( XF_BIG_ENDIAN )

# 检测大小端序
add_definitions ( -DXF_BIG_ENDIAN=${XF_BIG_ENDIAN} )
add_compile_options ( -Wall -nostdlib -fno-builtin -fno-exceptions -fno-pic )

set ( CMAKE_DEBUG_POSTFIX "d" )
set ( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin/ )
set ( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin/ )
set ( CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/bin/debug/ )
set ( CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/bin/debug/ )
set ( CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/bin/release/ )
set ( CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/bin/release/ )

##################################################

#add_library ( loader
#  src/boot/loader.c
#  include/vos/fs/fat.h )
#
#target_include_directories ( loader
#  PUBLIC
#  ${PROJECT_SOURCE_DIR}/include
#  )

##################################################

add_library ( kernel
  include/vos/bochs.h
  include/vos/amd.h
  include/vos/x86_64.h
  include/vos/assert.h
  include/vos/fs.h
  include/vos/fs/exfat.h
  include/vos/fs/fat.h
  include/vos/guest.h
  include/vos/host.h
  include/vos/intel.h
  include/vos/memory.h
  include/vos/stdio.h
  include/vos/string.h
  include/vos/svm.h
  include/vos/types.h
  include/vos/vmx.h
  include/vos/vos.h
  include/vos/x86.h
  src/vos/amd.c
  src/vos/intel.c
  src/vos/vmx.c
  src/vos/kernel.c
  src/vos/memory.c
  src/vos/stdio.c
  src/vos/string.c
  )

target_include_directories ( kernel
  PUBLIC
  ${PROJECT_SOURCE_DIR}/include
  )

##################################################


