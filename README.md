RoboIME Artificial Intelligence
===============================


This project aims to provide a set of software to take full control of the
robots on an autonomous match, and to aid its development by providing
facilities such as a Simulation environment.

There is a proposal to strip the simulator out of the project and make it either
standalone or merge it with grSim.


Requirements
------------

- Git
- CMake
- Qt SDK 4.8.x
- OpenGL
- Glut
- Protobuf
- PhysX SDK 2.8.4 (Required only to build the simulator)

On Ubuntu 12.04 the following should suffice:

    apt-get install git cmake qt libgl1-mesa-dev freeglut3-dev protobuf-compiler libprotobuf-dev libxmu-dev libxi-dev

You're recommended to use QtCreator, but not mandatory, use your preferred IDE,
or no IDE at all, plain vim, make and gdb will do just fine.

TODO: add packages for other distros

