if($ENV{GCC_TOOLCHAIN_INCLUDED})
    return()
endif()
set(ENV{GCC_TOOLCHAIN_INCLUDED} true)

set(CMAKE_C_COMPILER   gcc)
set(CMAKE_CXX_COMPILER g++)
set(CMAKE_AR           gcc-ar)
