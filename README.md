Geometric Modeling - Mesh Processing
======================================

This package contains exercise code for the lecture "Geometric Modeling" by Prof. Dr. Mario Botsch, TU Dortmund.


Documentation
-------------

A pre-built HTML documentation can be found in `doc/index.html` and can be opened via any web browser.


Prerequisites
-------------

We use [CMake](www.cmake.org) for setting up build environments. You can download it [here](https://cmake.org/download/) or by using your favorite package manager.
You will also need a C++ compiler that supports C++17. Depending on your OS, the typical choices are:

  * Linux - GCC/G++, usually pre-installed (terminal command: `sudo apt install build-essential`)
  * MacOS - Clang, comes with the IDE "XCode" (terminal command: `xcode-select --install`)
  * Windows - MSVC, can be installed alongside the IDE "Visual Studio Community" (see below) or separately [here](https://visualstudio.microsoft.com/downloads/#other) under "All Downloads" --> "Tools for Visual Studio" --> "Build Tools for Visual Studio 2019"

We highly recommend to use some kind of IDE. Prominent examples are:
 
 - [XCode](https://developer.apple.com/xcode/) (MacOS) 
 - [Visual Studio Community](https://visualstudio.microsoft.com/de/vs/community/) (Windows)
 - [Visual Studio Code](https://code.visualstudio.com/) (Linux, Windows, MacOS)
 - [Jetbrains CLion](https://www.jetbrains.com/de-de/clion/) (Linux, Windows, MacOS)
 
Below, we provide examples for setting up, compiling and running the project via command line, XCode, VSCommunity, and VSCode.


Building on Linux via Command Line (no IDE)
-------------------------------------------

Execute the following commands in the exercise's top-level directory:

    mkdir build
    cd build
    cmake ..
    make

The last command (`make`) compiles the application. Re-run it whenever you have added/changed code in order to re-compile.

For running the code via command line use

    ./mesh-processing


Building on MacOS (XCode)
-------------------------

Execute the following commands in the exercise's top-level directory:

    mkdir xcode
    cd xcode
    cmake -G Xcode ..
    open mesh-processing.xcodeproj

Within XCode, select the target `mesh-processing` in the top bar. You can compile and run the code by pressing the play button. You can specify command line arguments by again opening the menu next to the stop button from the top bar and selecting "Edit Scheme". In the popup window, select the "Run" option left and open the "Arguments" tab.


Building on Windows (Visual Studio Community) 
---------------------------------------------

* In order to get the MSVC C++ compiler, make sure that you check "Desktop development with C++" during installation of [VSCommunity](https://visualstudio.microsoft.com/de/vs/community/)
* Create an empty build folder inside the project's top-level directory
* Start cmake-gui.exe (located in cmake's bin folder)
* Specify the top-level directory as source directory (button Browse source...)
* Specify the previously created build folder as build directory (button Browse build...)
* Select "Configure" using your Visual Studio Version as option.
* When configuration is finished, select "Generate".
* Start Visual Studio Community
* Open the project via File -> open -> project -> .sln in build folder
* In the project explorer window on the right, right-click the project (SkeletalAnimation) and set it as startup-project
* Switch to release mode
* You can specify command line arguments via project -> properties -> debugging -> command arguments
* Hit CTRL + F5 to build and run (or CTRL + SHIFT + B to build)


Building via VSCode
-------------------

There are a lot of useful extensions for VSCode to shape it the way you like.
The required extensions for C++ development and Cmake support are "C/C++" and "Cmake Tools". Extensions can be found in the extensions tab on the left.
Once this is done, you can set up the project:

 * Start VSCode
 * Open the project via File --> Open Folder and select the exercise's top-level directory containing the `CMakeLists.txt` file and accept by clicking `OK`
 * In the bottom bar, click on `CMake`, choose your compiler and select `Debug` or `Release` mode
 * In the bottom bar, select `[mesh-processing]` as default build target.
 * Still in the bottom bar, there are buttons for building (Build) and running (play symbol)
 * You can specify command line arguments by using the terminal inside of VSCode

Have a look at the VSCode [documentation](https://code.visualstudio.com/docs/cpp/introvideos-cpp) for further details. 



Running 
-------

You can either start an empty viewer and load a mesh via the dropdown-menu

    ./mesh-processing

Or you provide a specific file (mesh or point cloud) as an argument. Included are a couple of meshes and point clouds -- some with color information, some without. Try the following to see the [Stanford bunny](http://graphics.stanford.edu/data/3Dscanrep/#bunny):

    ./mesh-processing ../data/pointsets/bunny.xyz

You can rotate the point cloud by holding the left mouse button and dragging. Move it by holding the middle mouse button and dragging. Zoom in/out using the mouse wheel (or Shift and left mouse).


Code Overview
-------------

There is quite some code that comes with this exercise, but you can focus just on the files in the directory `src/`.

For this exercise you will use [Surface Mesh](https://ls7-gv.cs.tu-dortmund.de/publications/2011/imr11/imr11.html), so take a look at the [accompanying documentation](surface_mesh_module.html) or our [quick start guide](surface_mesh_intro.pdf).


To-Do
-----

1. Reconstruct a triangle mesh from a point set
    - Find the code in the directory `src/01-reconstruction`.
    - Implement Hoppe's method in `src/01-reconstruction/reconstruction-hoppe.cpp`. 
    - Try to use a kD-tree to accelerate the closest point query 
      (see class kDTree in file `src/01-reconstruction/kd-tree.h`). 
    - Poisson Surface Reconstruction is already implemented. Compare how the methods differ in speed and result quality.


License
-------

The code examples in directory `src` are copyright of Computer Graphics Group, TU Dortmund University.

We use the following external libraries (thanks to their authors!)
- [Eigen](https://gitlab.com/libeigen/eigen) for solving linear systems 
- [GLEW](https://glew.sourceforge.net/) for managing OpenGL extensions
- [GLFW](https://www.glfw.org/) for window handling and keyboard/mouse interaction
- [ImGui](https://github.com/ocornut/imgui) for the user interface
- [PMP](https://www.pmp-library.org/) for dealing with polygon meshes
- [Poisson](https://github.com/mkazhdan/PoissonRecon) for surface reconstruction
- [RPLY](https://w3.impa.br/~diego/software/rply/) for PLY file format
- [STB](https://github.com/nothings/stb) for reading/writing images
