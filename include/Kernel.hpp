#include <cstring>
#include <string>
#include <sstream>

namespace Local{

#ifndef KERNEL_H
#define KERNEL_H

template<int h, int w, typename T>
class Kernel{
public:
    Kernel(T data[h][w]);
    Kernel();
    T _data[h][w];
    Kernel& rotate180Degrees();
    std::string getInfo();
};

template<int h, int w, typename T>
Kernel<h, w, T>::Kernel()
{

}

template<int h, int w, typename T>
Kernel<h, w, T>::Kernel(T data[h][w])
{
    for (uint32_t i =0; i < h; i++) {
        for (uint32_t j =0; j < w; j++) {
            _data[i][j] = data[i][j];
        }
    }
}

template<int h, int w, typename T>
Kernel<h, w, T> &Kernel<h, w, T>::rotate180Degrees()
{
    T buffer[w], b;
    for (int i = 0; i < h / 2; i++) {
        // first we flip vertically (swapping the rows completely)
        std::memcpy(buffer, _data[i], w*sizeof(T));
        std::memcpy(_data[i], _data[h - i - 1], w*sizeof(T));
        std::memcpy(_data[h - i - 1], buffer, w*sizeof(T));

        for (int j = 0; j < w / 2; j++) {
            // and then we do it horizontally for the values in the swaped rows
            b = _data[i][j];
            _data[i][j] = _data[i][w - j - 1];
            _data[i][w - j - 1] = b;

            b = _data[w - i - 1][j];
            _data[w - i - 1][j] = _data[w - i - 1][w - j - 1];
            _data[w - i - 1][w - j - 1] = b;
        }
    }
  return *this;
}

template<int h, int w, typename T>
std::string Kernel<h, w, T>::getInfo(){
    std::stringstream ss;
    for (int i = 0; i < h ; i++ ) {
        ss << "{";
        for (int j = 0; j < w - 1; j++ ) {
               ss << _data[i][j] << ", ";
        }
        ss << _data[i][w - 1] << "}";
    }
    return ss.str();
}

#endif // KERNEL_H

}
