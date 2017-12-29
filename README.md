# CamGrab

Qt + OpenCV based camera tool.

![screenshot_2017-12-29_14-50-00](https://user-images.githubusercontent.com/803299/34442356-d7f6a2c4-ecc1-11e7-8e56-8eba3d662d9e.png)

## Prerequisites

 * [Qt 5.10+](https://download.qt.io/archive/qt/)
 * [CUDA Toolkit 9.1+](https://developer.nvidia.com/cuda-downloads)
 * [OpenCV 3.4.0+](https://github.com/opencv/opencv) (needs to be compiled with CUDA support)

## Building

### Building OpenCV

```
git clone https://github.com/opencv/opencv.git
cd opencv
mkdir build
cd build
cmake -D CMAKE_BUILD_TYPE=RELEASE -D CMAKE_INSTALL_PREFIX=/usr/local -D FORCE_VTK=ON -D WITH_TBB=ON -D WITH_V4L=ON -D WITH_QT=ON -D WITH_OPENGL=ON -D WITH_CUBLAS=ON -D CUDA_NVCC_FLAGS="-D_FORCE_INLINES --expt-relaxed-constexpr" -D WITH_GDAL=ON -D WITH_XINE=ON -D BUILD_EXAMPLES=ON ..
make -j8
make install

```

### Building the application

```
git clone git://github.com/zaak/camgrab.git
```

Open `CamGrab.pro` in Qt Creator or:

```
cd camgrab
qmake
make
```
