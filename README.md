# Camera Simulator

## How to build

### dependency
Build with CMake
Basic Install step (Windows)
- Download code
- Make build folder
- In CMake, Set "source folder" to the Downloaded code and "build folder" to create build folder
- Configure (select VS version you use) and generate
- Open VS with the administrator
- Build "INSTALL"


#### Eigen3
(https://eigen.tuxfamily.org/index.php?title=Main_Page)

#### GLEW
(http://glew.sourceforge.net/)

#### OpenCV 3.x (for build example)
(https://github.com/opencv/opencv/tree/3.4)

### build this code
Build with CMake
check the paths (GLEW_DIR, OpenCV_DIR, Eigen3_DIR)
example:
- Eigen: E:/repo/eigen-eigen-5a0156e40feb/build
- glew: C:/Program Files (x86)/glew/lib/cmake/glew
- OpenCV: C:/repo/cv_build/install

Then generate the project.
