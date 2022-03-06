#ifndef IMAGE_HPP
#define IMAGE_HPP

#pragma once

#include <math.h>
#include <iostream>
#include <sstream>
#include <cmath>
#include <assert.h>

#include <QImage>
#include <QtCharts>

#include "include/Kernel.hpp"

namespace Local {

struct Pixel{
    uint8_t R, G, B, A;
};

class Image {

private:
    std::string _file_path;
    QImage _original_image_data;
    QImage _image_data;

public:
    Image(std::string filePath);

    //! The copy constructor
    Image(const Image &other);

    //! The copy-assignment operator
    Image &operator=(const Image &other);

    //! Get the grayscale image histogram
    QChart& getHistogram(); // TODO

    //! Tweak the brightness of each picture element by clamping its sum with delta inside [0, 255]
    Image& adjustBrightness(int8_t delta);

    //! Tweak the contrast of each picture element by clamping its product by alpha inside [0, 255]
    Image& adjustContrast(float alpha);

    //! Gets the negative of the image
    Image& toNegative();

    //! Equalizes the image`s histogram
    Image& equalizeHistogram(); // TODO

    //! Matches the image`s histogram with the one of a given Image
    Image& matchHistogram(); // TODO

    //! Zooms the image in (magnification)
    Image& zoomIn(float sx, float sy); // TODO

    //! Zooms the image out (magnification)
    Image& zoomOut(uint32_t sx, uint32_t sy);

    //! Rotates the image 90 degrees *s* times using the sign to get clockwise direction (trigonometric convention is applied)
    Image& rotate90Deg(int8_t s);

    //! Applies a 2D convolution over the image
    template<int h, int w, typename T>
    Image& apply2DConv(Kernel<h, w, T>& filter); // TODO

    //! Mirrors the image horizontally
    Image &mirrorH();

    //! Mirrors the image vertically
    Image &mirrorV();

    //! Converts the image to grayscale
    Image &toGrayscale();

    //! Quantizes the grayscale version into the given amount of tones
    Image &quantize(int noTones);

    const QImage &underlyingContainer();

    //! Saves the current image state to a JPEG file using the given quality parameter
    bool saveToJPEG(std::string filePath, int quality);

    //! Returns some info (must be replaced by <<)
    std::string info();

    //! Returns the file path from where the image was originally loaded
    std::string getFilePath();

    //! Clamps the value to the interval bounded below by min and above by max
    template<typename T>
    static T clamp(T value, T min, T max)
    {
        return (value < min) ? min : ((value > max) ? max : value);
    }
};

template<int h, int w, typename T>
Image &Image::apply2DConv(Kernel<h, w, T> &filter)
{
    assert(h == w && w == 3); // the kernel is assumed to be 3x3

    const uint32_t height = _image_data.height();
    const uint32_t width = _image_data.width();

    const uchar pixel_size = _image_data.depth()/8;
    uchar *dst_row;

    QImage result = QImage(width, height, _image_data.format());
    uint32_t pixel;
    Pixel convolutedPixel;

    auto getConvolutedPixel = [height, width, pixel_size, filter](QImage& image, uint32_t y, uint32_t x){
        uchar *row;
        uint32_t pixel;

        for (uint32_t i = y; i < y + 3 && i < height - 1; i++) {
            row = image.scanLine(i);
            for (uint32_t j = x; j < x + 3 && j < width - 1; j ++) {
                pixel = j * pixel_size;
                convolutedPixel = getConvolutedPixel(_image_data, i, j * pixel_size);
                dst_row[pixel] = convolutedPixel.R;
                dst_row[pixel + 1] = convolutedPixel.G;
                dst_row[pixel + 2] = convolutedPixel.B;
            }
        }
    };

    for (uint32_t i = 1; i < height - 1; i++) {
        dst_row = result.scanLine(i);
        for (uint32_t j = 1; j < width - 1; j ++) {
            pixel = j * pixel_size;
            convolutedPixel = getConvolutedPixel(_image_data, i, j * pixel_size);
            dst_row[pixel] = convolutedPixel.R;
            dst_row[pixel + 1] = convolutedPixel.G;
            dst_row[pixel + 2] = convolutedPixel.B;
        }
    }

    return *this;
}

} // namespace Local

#endif // IMAGE_HPP
