# img: Simple C++ image library with Eigen interoperability 

**img** is a C++ library that focuses on basic image manipulation where the pixels color is accessed via an [Eigen](https://eigen.tuxfamily.org/) matrix map, and loading and saving operations are managed by the [stb](https://github.com/nothings/stb) library. 

# The Image class

The [Image](https://github.com/ThibaultLejemble/img/blob/main/src/img/Image.h) class represents a 2D RGBA single floating point image with the following properties
- images can be loaded from and saved to png files
- the top-left pixel is at coordinate (0,0)
- the storage is in row-major order
- pixel access is made through an `Eigen::Map` of `Eigen::Vector4f`
- resizing operations are not conservative

## Examples

To download, compile, and run the [examples](https://github.com/ThibaultLejemble/img/tree/main/examples) run  
```
git clone --recurse-submodules git@github.com:ThibaultLejemble/img.git
cd img
mkdir build
cd build
cmake .. 
make -j
./example1_fractal
``` 

# TODO (generic ImageT)
- cast 
- bitmap io
- png io
- Eigen on/off
- bool support
- as_matrix/as_tensor
- eval (closest,trilinear)
- colormap
- cuda