#include "include/Image.hpp"

namespace Local {

Image::Image(std::string filePath) : _file_path(filePath) {
    _image_data = QImage(filePath.c_str());
    _original_image_data = QImage(filePath.c_str());
}

Image::Image(const Image &other) : _file_path(other._file_path),
                                    _original_image_data(other._original_image_data),
                                    _image_data(other._image_data){}

Image &Image::operator=(const Image &other) {
    _image_data = other._image_data;
    return *this;
}

QChart &Image::getHistogram()
{
    uint64_t buckets[256] = {0};

    uint32_t height = _image_data.height();
    uint32_t width = _image_data.width();
    uchar pixel_size = _image_data.depth()/8;

    uchar *row;

    // first we count how many pixels use each tone (assuming we are dealing with a grayscale image)
    for (uint32_t i = 0; i < height; i++) {
        row = _image_data.scanLine(i);
        for (uint32_t j = 0; j < width; j+=pixel_size) {
            buckets[*(row + j)] += 1;
        }
    }

    uint64_t greatestBucketCount = 0;

    QBarSet *set = new QBarSet("");
    for (uint64_t i = 0; i < 256 ; i++ ) {
        *set << double(buckets[i]);
        if (buckets[i] > greatestBucketCount) greatestBucketCount = buckets[i];
    }

    QBarSeries *series = new QBarSeries();
    series->append(set);

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle(QString::fromStdString(_file_path + " grayscale histogram"));
    chart->setAnimationOptions(QChart::SeriesAnimations);

    QValueAxis *axisY = new QValueAxis();
    axisY->setRange(0,greatestBucketCount);
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    return *chart;
}

Image &Image::adjustBrightness(int8_t delta)
{
    uint32_t height = _image_data.height();
    uint32_t width = _image_data.width();

    uchar pixel_size = _image_data.depth()/8;
    uchar *row;
    int pixel;

    for (uint32_t i = 0; i < height; i++) {
        row = _image_data.scanLine(i);
        for (uint32_t j = 0; j < width; j++) {
            pixel = j * pixel_size;
            row[pixel] = Image::clamp(row[pixel] + delta, 0, 255);
            row[pixel + 1] = Image::clamp(row[pixel + 1] + delta, 0, 255);
            row[pixel + 2] = Image::clamp(row[pixel + 2] + delta, 0, 255);
        }
    }

    return *this;
}

Image &Image::adjustContrast(float alpha)
{
    uint32_t height = _image_data.height();
    uint32_t width = _image_data.width();

    uchar pixel_size = _image_data.depth()/8;
    uchar *row;
    int pixel;

    for (uint32_t i = 0; i < height; i++) {
        row = _image_data.scanLine(i);
        for (uint32_t j = 0; j < width; j++) {
            pixel = j * pixel_size;
            row[pixel] = uchar(Image::clamp(float(row[pixel]) * alpha, 0.0f, 255.0f));
            row[pixel + 1] = uchar(Image::clamp(float(row[pixel]) * alpha, 0.0f, 255.0f));
            row[pixel + 2] = uchar(Image::clamp(float(row[pixel]) * alpha, 0.0f, 255.0f));
        }
    }

    return *this;
}

Image &Image::toNegative()
{
    uint32_t height = _image_data.height();
    uint32_t width = _image_data.width();

    uchar pixel_size = _image_data.depth()/8;
    uchar *row;
    int pixel;

    for (uint32_t i = 0; i < height; i++) {
        row = _image_data.scanLine(i);
        for (uint32_t j = 0; j < width; j++) {
            pixel = j * pixel_size;
            row[pixel] = Image::clamp(255 - row[pixel], 0, 255);
            row[pixel + 1] = Image::clamp(255 - row[pixel + 1], 0, 255);
            row[pixel + 2] = Image::clamp(255 - row[pixel + 2], 0, 255);
        }
    }

    return *this;
}

Image &Image::zoomOut(uint32_t sx, uint32_t sy)
{
    const uint32_t height = _image_data.height();
    const uint32_t width = _image_data.width();

    const uint32_t new_height = ceil(height / sy);
    const uint32_t new_width = ceil(width / sx);

    const uchar pixel_size = _image_data.depth()/8;
    uchar *dst_row;

    QImage result = QImage(new_width, new_height, _image_data.format());

    Pixel averagePixel;
    uint32_t pixel;


    auto getAverageFromRegion = [height, width, pixel_size](QImage& image, uint32_t i, uint32_t j, int sx, int sy){
        uint32_t pixel_counter = 0;
        uint64_t accum[3] = {0};
        uint32_t n;
        uchar *row;


        for (uint32_t k = i; k < i + sy && k < height; k++) {
            row = image.scanLine(k);
            for (uint32_t m = j; m < j + sx * pixel_size && m < width * pixel_size; m += pixel_size) {
                n = m;
                while(n - m < 3 && n < width * pixel_size){
                    accum[n - m] += row[n];
                    n++;
                }
                pixel_counter++;
            }
        }

        assert(pixel_counter != 0);

        Pixel result{
            Image::clamp(uint8_t(accum[0]/pixel_counter), uint8_t(0), uint8_t(255)),
            Image::clamp(uint8_t(accum[1]/pixel_counter), uint8_t(0), uint8_t(255)),
            Image::clamp(uint8_t(accum[2]/pixel_counter), uint8_t(0), uint8_t(255)),
            0
        };
        return result;
    };

    for (uint32_t i = 0; i < new_height; i++) {
        dst_row = result.scanLine(i);
        for (uint32_t j = 0; j < new_width; j ++) {
            pixel = j * pixel_size;
            averagePixel = getAverageFromRegion(_image_data, i * sy, j * sx * pixel_size, sx, sy);
            dst_row[pixel] = averagePixel.R;
            dst_row[pixel + 1] = averagePixel.G;
            dst_row[pixel + 2] = averagePixel.B;
        }
    }

    _image_data = result;

    return *this;
}

Image &Image::rotate90Deg(int8_t s)
{
    int real_rotation = abs(s) % 4;

    uint32_t height = _image_data.height();
    uint32_t width = _image_data.width();

    uchar pixel_size = _image_data.depth()/8;
    uchar  *src_row, *dst_row;

    real_rotation = (s < 0)? 4 - real_rotation : real_rotation; // get the complement in the case of clockwise PI/2 rotations

    switch(real_rotation){
        case 0:
            break;
        case 1:{
                QImage result = QImage(height, width, _image_data.format());
                for (uint32_t i = 0; i < width; i++) {
                    for (uint32_t j = 0; j < height; j++) {
                        src_row = _image_data.scanLine(j);
                        dst_row = result.scanLine(width - i - 1);
                        memcpy(dst_row + j * pixel_size, src_row + i * pixel_size, pixel_size);
                    }
                }
                _image_data = result;
            }break;
        case 2:{
                QImage result = QImage(width, height, _image_data.format());
                for (uint32_t i = 0; i < height; i++) {
                    for (uint32_t j = 0; j < width; j++) {
                        src_row = _image_data.scanLine(i);
                        dst_row = result.scanLine(height - 1 - i);
                        memcpy(dst_row + (width - 1 -j) * pixel_size, src_row + j * pixel_size, pixel_size);
                    }
                }
                _image_data = result;
            }break;
        case 3:{
                QImage result = QImage(height, width, _image_data.format());
                for (uint32_t i = 0; i < width; i++) {
                    for (uint32_t j = 0; j < height; j++) {
                        src_row = _image_data.scanLine(j);
                        dst_row = result.scanLine(i);
                        memcpy(dst_row + (height - 1 - j) * pixel_size, src_row + i * pixel_size, pixel_size);
                    }
                }
                _image_data = result;
            }break;
    }

    return *this;
}

Image &Image::mirrorH() {
    uint32_t height = _image_data.height();
    uint32_t width = _image_data.width();

    uchar pixel_size = _image_data.depth()/8;
    uchar *buffer = new uchar[pixel_size];
    uchar *a_buffer, *b_buffer, *row;

    for (uint32_t i = 0; i < height; i++) {
        row = _image_data.scanLine(i);
        for (uint32_t j = 0; j < width / 2; j++) {
            uint32_t b_column = width - 1 - j;
            if (j != b_column) {
                a_buffer = row + j * pixel_size;
                b_buffer = row + b_column * pixel_size;
                memcpy(buffer, a_buffer, pixel_size);   // buffer <- current_row
                memcpy(a_buffer, b_buffer, pixel_size); // current row <- opposite row
                memcpy(b_buffer, buffer, pixel_size);   // opposite row <- (buffer = previous current row)
            }
        }
    }
    return *this;
}

Image &Image::mirrorV() {
    uint32_t height = _image_data.height();
    uint32_t width = _image_data.width();

    uchar *buffer = new uchar[width];
    uchar *b_buffer, *row;
    int row_blength = _image_data.bytesPerLine();

    std::cout << width << std::endl;

    for (size_t i = 0; i < height / 2; i++) {
        row = _image_data.scanLine(i);
        int a_row = i;
        int b_row = height - 1 - i;
        if (a_row != b_row) {
            b_buffer = _image_data.scanLine(b_row);
            memcpy(buffer, row, row_blength);      // buffer <- current_row
            memcpy(row, b_buffer, row_blength);    // current row <- opposite row
            memcpy(b_buffer, buffer, row_blength); // opposite row <- (buffer = previous current row)
        }
    }
    return *this;
}

Image &Image::toGrayscale() {
    uint32_t height = _image_data.height();
    uint32_t width = _image_data.width();

    uchar *row, pixel_size = _image_data.depth()/8;
    float L;
    int offset;

    for (size_t i = 0; i < height; i++) {
        row = _image_data.scanLine(i);
        for (size_t j = 0; j < width; j++) {
            offset = j * pixel_size;
            L = 0.299 * row[offset] + 0.587 * row[offset + 1] + 0.114 * row[offset + 2];

            row[offset] = (uchar)L;
            row[offset + 1] = (uchar)L;
            row[offset + 2] = (uchar)L;
        }
    }
    return *this;
}

bool Image::saveToJPEG(std::string filePath, int quality) {
    // TODO Recap on PNG and JPEG compression techniques in order to answer the first question appropriately
    return _image_data.save(QString(filePath.c_str()), "JPG", quality);
}

const QImage &Image::underlyingContainer() {
    return _image_data;
}

Image &Image::quantize(int n_of_tones) {
    uint32_t height = _image_data.height();
    uint32_t width = _image_data.width();

    uchar *image_row, pixel_size = _image_data.depth()/8;
    int offset;
    uchar t1 = 255, t2 = 0;

    // scanning for the tone range
    for (size_t i = 0; i < height; i++) {
        image_row = _image_data.scanLine(i);
        for (size_t j = 0; j < width; j++) {
            offset = j * pixel_size;
            if (image_row[offset] > t2)
                t2 = image_row[offset];
            if (image_row[offset] < t1)
                t1 = image_row[offset];
        }
    }

    int current_tone_range = t2 - t1 + 1; // must be stored with the appropriate value range (i.e.: must prevent overflow)

    if (n_of_tones < current_tone_range) {
        float bin_size = float(current_tone_range) / n_of_tones;
        uchar quantized_value, *image_row;

        int offset;
        float alpha, bin_center;

        std::cout << height << ", " << width << ", " << int(pixel_size) << "bytes per pixel" << std::endl;

        for (size_t i = 0; i < height; i++) {
            image_row = _image_data.scanLine(i);
            for (size_t j = 0; j < width; j++) {
                offset = j * pixel_size;

                alpha = floor((image_row[offset] - (t1 - 0.5)) / bin_size);
                bin_center = alpha * bin_size + t1 - .5 + bin_size / 2;
                quantized_value = uchar(round(bin_center));

                image_row[offset] = quantized_value;
                image_row[offset + 1] = quantized_value;
                image_row[offset + 2] = quantized_value;
            }
        }
    }
    return *this;
}

std::string Image::info() {
    std::stringstream output;
    output << "FilePath: " << _file_path << std::endl;
    output << "Resolution: " << _image_data.width() << "x" << _image_data.width() << std::endl;
    output << "Pixel Size: " << _image_data.depth() / 8 << std::endl;

    return output.str();
}

std::string Image::getFilePath()
{
    return _file_path;
}

} // namespace Local
