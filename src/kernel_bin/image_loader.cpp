//
// Created by zhangyongchao on 2020/2/26.
//
#include "image_loader.h"
#include "jpeglib.h"

namespace duerVision {

void jpegErrorExit (j_common_ptr cinfo)
{
    return;
}

ImageLoader::ImageLoader(const char* filename) {

}

ImageType ImageLoader::read_image(const char *filename) {
    jpeg_decompress_struct cinfo;
    jpeg_error_mgr jerr;

    cinfo.err = jpeg_std_error(&jerr);

    jerr.error_exit = jpegErrorExit;


    ImageType image_type = read_type(filename);
    if (image_type != ImageType::JPG) {
        // currently only implement read jpeg
        return image_type;
    }

    FILE* file = fopen(filename, "rb");
    assert(!file);

    jpeg_create_decompress(&cinfo);

    jpeg_stdio_src(&cinfo, file);
    jpeg_read_header(&cinfo, TRUE);
    jpeg_start_decompress(&cinfo);

    height_ = cinfo.output_height;
    width_ = cinfo.output_width;

    if (cinfo.output_components != 3) {
        // currently only support rgb image
        jpeg_destroy_decompress(&cinfo);
        throw std::runtime_error(filename + std::string(" is not rgb image"));
    }

    output_components_ = cinfo.output_components;
    std::vector<unsigned char*> rows;
    rows.resize(height_);

    // size the image buffer
    data.resize(height_*width_*output_components_);

    // setup pointers to each row
    for (unsigned long i = 0; i < rows.size(); ++i)
        rows[i] = &data[i*width_*output_components_];

    // read the data into the buffer
    while (cinfo.output_scanline < cinfo.output_height)
    {
        jpeg_read_scanlines(&cinfo, &rows[cinfo.output_scanline], 100);
    }

    jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);

    fclose(file);

    return image_type;
}

}
