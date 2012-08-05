RoboIME SSL Computational Project
=================================

Introduction
------------
This solution aims to provide a set of software to take full control of the
robots on an autonomous match, and to aid its development by providing
facilities such as a Simulation environment.


System Requirements
-------------------
We plan to support compilation on linux/osx on the future. But currently
it only compiles on windows with visual studio compiler.
The following are the requirements to compile and develop on this project:

- Qt SDK 4.8.x
- PhysX SDK 2.8.4 (*)

*PhysX SDK dependencies are included on the _include_ thus it will compile
without PhysX, but it's still needed to run the portions that use Simulation.

