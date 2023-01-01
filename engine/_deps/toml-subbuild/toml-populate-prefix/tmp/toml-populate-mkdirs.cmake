# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "C:/Users/sylva/Desktop/Code/bellecour-gamebook/engine/_deps/toml-src"
  "C:/Users/sylva/Desktop/Code/bellecour-gamebook/engine/_deps/toml-build"
  "C:/Users/sylva/Desktop/Code/bellecour-gamebook/engine/_deps/toml-subbuild/toml-populate-prefix"
  "C:/Users/sylva/Desktop/Code/bellecour-gamebook/engine/_deps/toml-subbuild/toml-populate-prefix/tmp"
  "C:/Users/sylva/Desktop/Code/bellecour-gamebook/engine/_deps/toml-subbuild/toml-populate-prefix/src/toml-populate-stamp"
  "C:/Users/sylva/Desktop/Code/bellecour-gamebook/engine/_deps/toml-subbuild/toml-populate-prefix/src"
  "C:/Users/sylva/Desktop/Code/bellecour-gamebook/engine/_deps/toml-subbuild/toml-populate-prefix/src/toml-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "C:/Users/sylva/Desktop/Code/bellecour-gamebook/engine/_deps/toml-subbuild/toml-populate-prefix/src/toml-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "C:/Users/sylva/Desktop/Code/bellecour-gamebook/engine/_deps/toml-subbuild/toml-populate-prefix/src/toml-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
