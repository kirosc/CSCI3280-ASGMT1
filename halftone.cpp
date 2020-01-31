/*

CSCI 3280, Introduction to Multimedia Systems
Spring 2020

Assignment 01 Skeleton

halftone.cpp

*/

#include "bmp.h" //	Simple .bmp library
#include <cstdlib>
#include <iostream>
#include <string>
#include <unistd.h>

#define MAX_SHADES 3

#define SAFE_FREE(p)                                                           \
  {                                                                            \
    if (p) {                                                                   \
      free(p);                                                                 \
      (p) = NULL;                                                              \
    }                                                                          \
  }

using namespace std;

inline bool file_exists(const string &name);

int main(int argc, char **argv) {
  //
  //	Your code goes here ....
  //
  //	1. Open BMP file
  //  you can use the following parameters for testing,
  //  remember to modify it based on the user input.
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

  int image_size, patch_size;

  try {
    image_size = stoi(argv[2]);
    patch_size = stoi(argv[3]);
  } catch (...) {
    cout << "Invalid option" << endl;
    exit(EXIT_FAILURE);
  }

  cout << image_size << " " << patch_size;
  Bitmap image_data("monokuma.bmp");
  image_data.resize(image_size);

  //	2. Load image patches
  //

  //
  //	3. Obtain Luminance
  //

  //
  //	4. Quantization
  //

  //
  //  5. Generate bmp image and parse patches according to quantized image
  //

  //  free memory

  return 0;
}

inline bool file_exists(const string &name) {
  return (access(name.c_str(), F_OK) != -1);
}