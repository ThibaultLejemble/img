#pragma once

namespace img {

class Image;
class BinaryImage;
class GrayScaleImage;

void convert(const Image& rgb, BinaryImage& binary);
void convert(const Image& rgb, GrayScaleImage& gray);

void convert(const BinaryImage& binary, Image& rgb);
void convert(const BinaryImage& binary, GrayScaleImage& gray);

void convert(const GrayScaleImage& gray, Image& rgb);
void convert(const GrayScaleImage& gray, BinaryImage& binary);

} // namespace img
