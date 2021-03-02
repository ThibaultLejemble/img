#pragma once

namespace img {

class Image;
class BinaryImage;
class GrayScaleImage;
class LabelImage;

void convert(const Image& rgb, BinaryImage& binary);
void convert(const Image& rgb, GrayScaleImage& gray);
void convert(const Image& rgb, LabelImage& labels);
// -----------------------------------------------------------------------------
void convert(const BinaryImage& binary, Image& rgb);
void convert(const BinaryImage& binary, GrayScaleImage& gray);
void convert(const BinaryImage& binary, LabelImage& labels);
// -----------------------------------------------------------------------------
void convert(const GrayScaleImage& gray, Image& rgb);
void convert(const GrayScaleImage& gray, BinaryImage& binary);
void convert(const GrayScaleImage& gray, LabelImage& labels);
// -----------------------------------------------------------------------------
void convert(const LabelImage& labels, Image& rgb);
void convert(const LabelImage& labels, BinaryImage& binary);
void convert(const LabelImage& labels, GrayScaleImage& gray);

} // namespace img
