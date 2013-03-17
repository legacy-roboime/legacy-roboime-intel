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
