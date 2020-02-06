/*

CSCI 3280, Introduction to Multimedia Systems
Spring 2020

Assignment 01 Skeleton

halftone.cpp

*/

#include "bmp.h" //	Simple .bmp library
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <string>
#include <fstream>

#define MAX_SHADES 3

#define SAFE_FREE(p)                                                           \
  {                                                                            \
    if (p) {                                                                   \
      free(p);                                                                 \
      (p) = NULL;                                                              \
    }                                                                          \
  }

using namespace std;

inline bool file_exists(const std::string& name);

inline unsigned char to_grayscale(unsigned char r, unsigned char g,
                                  unsigned char b);

inline unsigned char quantize(unsigned char gray_value, int n);

bool set_patch_image(int x, int y, Bitmap &destination);

Bitmap image, patch1, patch2, patch3;

int main(int argc, char **argv) {
    cout << "===================================" << endl;
    // Arguments and file checking
    if (argc != 4) {
        cout << "Invalid input!" << endl;
        cout << "Usage: halftone <input.bmp> <size1> <size2>" << endl;
        exit(EXIT_FAILURE);
    }

    if (!file_exists(argv[1])) {
        cout << "Cannot access '" << argv[1] << "': No such file or directory"
             << endl;
        exit(EXIT_FAILURE);
    }

    if (!(file_exists("patch/0.bmp") && file_exists("patch/1.bmp") &&
          file_exists("patch/2.bmp"))) {
        cout << "Cannot access patch images" << endl;
        exit(EXIT_FAILURE);
    }

    int image_size, patch_size;

    try {
        image_size = stoi(argv[2]);
        patch_size = stoi(argv[3]);
    } catch (...) {
        cout << "Invalid option" << endl;
        exit(EXIT_FAILURE);
    }

    // Loading images
    image.create(argv[1]);
    patch1.create("patch/0.bmp");
    patch2.create("patch/1.bmp");
    patch3.create("patch/2.bmp");

    cout << image._height << " " << image._width << endl;
    image.resize(image_size);
    patch1.resize(patch_size);
    patch2.resize(patch_size);
    patch3.resize(patch_size);
    cout << image._height << " " << image._width << endl;
    Bitmap output_image(image._width * patch_size, image._height * patch_size);
    cout << output_image._height << " " << output_image._width << endl;
    unsigned char r, g, b;
    set_patch_image(0, 0, output_image);
    output_image.getColor(0, 0, r, g, b);
    cout << (int)r << " " << (int)g << " " << (int)b << endl;

    // Transform the image to grayscale
    for (int i = 0; i < image._height; i++) {
        for (int j = 0; j < image._width; j++) {
            unsigned char r, g, b, val;
            image.getColor(j, i, r, g, b);
            val = to_grayscale(r, g, b);
            val = quantize(val, 3);
            image.setColor(j, i, val, val, val);
        }
    }
    image.save("test.bmp");

    //
    //  5. Generate bmp image and parse patches according to quantized image
    //

    //  free memory

    return 0;
}

// Check if a file exists
inline bool file_exists(const std::string& name) {
    ifstream f(name.c_str());
    return f.good();
}


inline unsigned char to_grayscale(unsigned char r, unsigned char g,
                                  unsigned char b) {
    return (unsigned char)round(0.299 * r + 0.587 * g + 0.114 * b);
}

inline unsigned char quantize(unsigned char gray_value, int n) {
    return (unsigned char)floor(gray_value / (255 / n));
}

bool set_patch_image(int x, int y, Bitmap &destination) {
    int patch_size = patch1._width;
    destination.setColor(x, y, 255, ' ', '_');
    return true;
}