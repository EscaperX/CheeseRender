# Utils
This chapter introduce the function and usages of utils used in the project. Most of them are third-party opensource tools. There are also some reasons that why this tools is
chosen instead of another.


## Math

## Image

## Display

Due to the varying requirements of rendering, several display techniques are provided. 

### SDL2

SDL2 is a cross-platform media API. 

### Tev

Tev is a open-source HDR image viewer developed by Toms. It provides a convience way to display HDR images in several formats such as OpenEXR.

Tev also provide a dynamic display mode. User can send pixels data from rendering process and send to Tev by socket, which has been used in PBRT. It's a helpful tools for the implementation of real-time ray tracer.