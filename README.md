# img: single-header-only C++ image library with Eigen interoperability  

**img** is a single-header-only C++ library that focuses on basic image manipulation where the pixels color is accessed via an [Eigen](https://eigen.tuxfamily.org/) matrix map. 

## The Image class

The [Image](https://github.com/ThibaultLejemble/img/blob/main/include/img/Image.h) class `Image<T,C>` represents a 2D image of a given type `T` (`int`,`float`, or `double`) with a given number of channels `C` (1 to 4)
- the top-left pixel is at coordinates `(0,0)`
- the storage is in **row-major** order
- pixel access is made through an `Eigen::Map`
- images can be loaded from and saved to `png` files only (thanks to [stb][https://github.com/nothings/stb])
- resizing operations are not conservative
- macro `IMG_NO_EIGEN` can be defined to avoid using Eigen
- color values are internally stored inside a `std::vector`

## Examples

To download, compile, and run the [examples](https://github.com/ThibaultLejemble/img/tree/main/examples) run  
```bash
git clone --recurse-submodules git@github.com:ThibaultLejemble/img.git
cd img
mkdir build
cd build
cmake .. 
make -j
./example0_test    # test cast operations 
./example1_fractal # generate a colored fractal 
./example2_binary  # cast to gray-scale and binary image
./example3_region  # test region growing algorithm
``` 

This project is tested using
- `Ubuntu 20.04.2 LTS`
- `g++ 9.3.0` 
- `cmake 3.16.3` 

## Limitations (TODO) 

- complete Matrix data structures (when `IMG_NO_EIGEN` is defined)
- add `bool` specialization
- add function `as_tensor()` that returns an `Eigen::TensorMap`
- add function `eval()` with nearest/linear algorithm
- cast matrix to image using colormap
- add cuda support