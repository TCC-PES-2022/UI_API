# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "C:/Users/afons/source/repos/TCC_Embraer_UIAPI/p2/teste/build/_deps/gtest-src"
  "C:/Users/afons/source/repos/TCC_Embraer_UIAPI/p2/teste/build/_deps/gtest-build"
  "C:/Users/afons/source/repos/TCC_Embraer_UIAPI/p2/teste/build/_deps/gtest-subbuild/gtest-populate-prefix"
  "C:/Users/afons/source/repos/TCC_Embraer_UIAPI/p2/teste/build/_deps/gtest-subbuild/gtest-populate-prefix/tmp"
  "C:/Users/afons/source/repos/TCC_Embraer_UIAPI/p2/teste/build/_deps/gtest-subbuild/gtest-populate-prefix/src/gtest-populate-stamp"
  "C:/Users/afons/source/repos/TCC_Embraer_UIAPI/p2/teste/build/_deps/gtest-subbuild/gtest-populate-prefix/src"
  "C:/Users/afons/source/repos/TCC_Embraer_UIAPI/p2/teste/build/_deps/gtest-subbuild/gtest-populate-prefix/src/gtest-populate-stamp"
)

set(configSubDirs Debug;Release;MinSizeRel;RelWithDebInfo)
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "C:/Users/afons/source/repos/TCC_Embraer_UIAPI/p2/teste/build/_deps/gtest-subbuild/gtest-populate-prefix/src/gtest-populate-stamp/${subDir}")
endforeach()
