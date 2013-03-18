# Roboime Intel Tests

## Google Test

We are using google test for unittest. Read this: <https://code.google.com/p/googletest/wiki/Samples>.

### Ubuntu 13.04

(source: <http://www.thebigblob.com/getting-started-with-google-test-on-ubuntu/>)

Note that this package only install source files.
You have to compile the code yourself to create the necessary library files.
These source files should be located at /usr/src/gtest.
Browse to this folder and use cmake to compile the library:

    sudo apt-get install libgtest-dev

    cd /usr/src/gtest
    sudo cmake CMakeLists.txt
    sudo make

    # copy or symlink libgtest.a and libgtest_main.a to your /usr/lib folder
    sudo cp *.a /usr/lib

	
### Visual Studio GTests Config

(source: <http://stackoverflow.com/questions/2728649/error-lnk2005-xxx-already-defined-in-msvcrt-libmsvcr100-dllc-something-libc>)

In order to fully integrate the "intel-tests" project with Visual Studio versions, you need to build the release
and debug versions of "gtest-md" solution (<gtest-1.6.0/msvc/gtest-md.sln>) instead of the standard <gtest.sln>.
Otherwise, there will be library conflicts, such as MT and MD, and GTest won't work.
Do not forget to update the cmake references and paths properly.





