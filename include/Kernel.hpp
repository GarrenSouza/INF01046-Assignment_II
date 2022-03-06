#ifndef KERNEL_H
#define KERNEL_H

#include <cstring>
#include <string>
#include <sstream>

template<int h, int w, typename T>
class Kernel{
public:
    T data[h][w];
    Kernel& rotate180Degrees();
    std::string getInfo();
};

template<int h, int w, typename T>
Kernel<h, w, T> &Kernel<h, w, T>::rotate180Degrees()
{
    T buffer[w], b;
    for (int i = 0; i < h / 2; i++) {
        // first we flip vertically (swapping the rows completely)
        std::memcpy(buffer, data[i], w*sizeof(T));
        std::memcpy(data[i], data[h - i - 1], w*sizeof(T));
        std::memcpy(data[h - i - 1], buffer, w*sizeof(T));

        for (int j = 0; j < w / 2; j++) {
            // and then we do it horizontally for the values in the swaped rows
            b = data[i][j];
            data[i][j] = data[i][w - j - 1];
            data[i][w - j - 1] = b;

            b = data[w - i - 1][j];
            data[w - i - 1][j] = data[w - i - 1][w - j - 1];
            data[w - i - 1][w - j - 1] = b;
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
               ss << data[i][j] << ", ";
        }
        ss << data[i][w - 1] << "}";
        if (h - i > 1) ss << "\n";
    }
    return ss.str();
}

#endif // KERNEL_H
