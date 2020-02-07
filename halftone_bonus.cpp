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
#include <vector>

#define MAX_SHADES 3

#define SAFE_FREE(p)                                                           \
  {                                                                            \
    if (p) {                                                                   \
      free(p);                                                                 \
      (p) = NULL;                                                              \
    }                                                                          \
  }

using namespace std;

void pre_check(int argc, char **argv);

inline bool file_exists(const std::string &name);

inline unsigned char to_grayscale(unsigned char r, unsigned char g,
                                  unsigned char b);

inline unsigned char quantize(unsigned char gray_value, int n);

void transform_image(Bitmap &image);

void halftone(Bitmap source, Bitmap &destination);

void set_patch_image(int x, int y, unsigned char quantized_val, Bitmap &destination);

void save(const char *source_file_name, Bitmap &image);

void rotate_image(Bitmap &image, int degree);

template<typename T>
void rotate(vector<vector<T>> &v);

Bitmap patch1, patch2, patch3;

int main(int argc, char **argv) {
    pre_check(argc, argv);

    int image_size, patch_size, rotation_degree;

    try {
        image_size = stoi(argv[2]);
        patch_size = stoi(argv[3]);
        rotation_degree = stoi(argv[4]);

        if (rotation_degree % 90 != 0) {
            cout << "Rotation degree must be a multiple of 90" << endl;
            exit(EXIT_FAILURE);
        }
    } catch (...) {
        cout << "Invalid option" << endl;
        exit(EXIT_FAILURE);
    }

    Bitmap image;

    // Loading images
    image.create(argv[1]);
    patch1.create("patch/0.bmp");
    patch2.create("patch/1.bmp");
    patch3.create("patch/2.bmp");

    // Size down the image and patch
    image.resize(image_size);
    patch1.resize(patch_size);
    patch2.resize(patch_size);
    patch3.resize(patch_size);

    // Rotate the image
    rotate_image(image, rotation_degree);

    // Transform the image to grayscale and quantize it
    transform_image(image);

    // The Bitmap that contains the final halftone image
    Bitmap halftone_image(image._width * patch_size, image._height * patch_size);

    halftone(image, halftone_image);

    save(argv[1], halftone_image);

    return 0;
}

// Arguments and file checking
void pre_check(int argc, char **argv) {
    if (argc != 5) {
        cout << "Invalid input!" << endl;
        cout << "Usage: halftone_bonus <input.bmp> <size1> <size2> <rotation_degree>" << endl;
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
}

// Check if a file exists
inline bool file_exists(const std::string &name) {
    ifstream f(name.c_str());
    return f.good();
}

inline unsigned char to_grayscale(unsigned char r, unsigned char g,
                                  unsigned char b) {
    return (unsigned char) floor(0.299 * r + 0.587 * g + 0.114 * b);
}

// Turn the gray_value to a number with value 0 to n - 1
inline unsigned char quantize(unsigned char gray_value, int n) {
    return (unsigned char) floor(gray_value / (256.0 / n));
}

// Transform the image to grayscale and quantize it
void transform_image(Bitmap &image) {
    for (int i = 0; i < image._height; i++) {
        for (int j = 0; j < image._width; j++) {
            unsigned char r, g, b, val;
            image.getColor(j, i, r, g, b);
            val = to_grayscale(r, g, b);
            val = quantize(val, 3);
            image.setColor(j, i, val, val, val);
        }
    }
}

// Turn an image from source to a halftone image and store it in &destination
void halftone(Bitmap source, Bitmap &destination) {
    int patch_size = patch1._width;

    for (int i = 0; i < destination._height; i += patch_size) {
        for (int j = 0; j < destination._width; j += patch_size) {
            unsigned char val;
            source.getColor(j / patch_size, i / patch_size, val, val, val);
            set_patch_image(j, i, val, destination);
        }
    }
}

// Set a patch image on destination image starting at position (x, y)
void set_patch_image(int x, int y, unsigned char quantized_val, Bitmap &destination) {
    Bitmap *patch;
    int patch_size = patch1._width;

    if (quantized_val == 0) {
        patch = &patch1;
    } else if (quantized_val == 1) {
        patch = &patch2;
    } else {
        patch = &patch3;
    }

    // Set the patch image to destination image pixel by pixel
    for (int i = 0; i < patch_size; ++i) {
        for (int j = 0; j < patch_size; ++j) {
            unsigned char r, g, b;
            patch->getColor(j, i, r, g, b);
            destination.setColor(x + j, y + i, r, g, b);
        }
    }
}

// Save the halftone image to "halftone-{file_name}.bmp"
void save(const char *source_file_name, Bitmap &image) {
    const char * file_name = (string("halftone-") + source_file_name).c_str();
    if (image.save(file_name)) {
        cout << "Saved to " << file_name << endl;
    } else {
        cout << "Cannot save to " << file_name << endl;
        exit(EXIT_FAILURE);
    }
}

// Rotate an image anti-clockwise with provided degree.
void rotate_image(Bitmap &image, int degree) {
    vector<vector<Color>> image_vector;

    // Turn the image to a 2D vector
    for (int i = 0; i < image._height; i++) {
        vector<Color> row;
        for (int j = 0; j < image._width; j++) {
            Color color;
            unsigned char r, g, b;

            image.getColor(j, i, r, g, b);
            color.R = r;
            color.G = g;
            color.B = b;

            row.push_back(color);
        }
        image_vector.push_back(row);
    }

    // Rotate according to the degree
    for (int i = 0; i < degree / 90 % 4; ++i) {
        rotate<Color>(image_vector);
    }

    // Turn the vector back to an image
    for (int i = 0; i < image_vector.size(); i++) {
        for (int j = 0; j < image_vector[0].size(); j++) {
            unsigned char r, g, b;
            r = image_vector[i][j].R;
            g = image_vector[i][j].G;
            b = image_vector[i][j].B;
            image.setColor(j, i, r, g, b);
        }
    }
}

// Rotate a 2D array anti-clockwise
template<typename T>
void rotate(vector<vector<T>> &v)
{
    vector<vector<T>>rotated;
    for (size_t i = 0; i < v[0].size(); i++) {
        vector<T> newRow;
        for (int j = v.size() - 1; j >=0; j--) {
            newRow.push_back(v[j][i]);
        }
        rotated.push_back(newRow);
    }
    v = rotated;
}