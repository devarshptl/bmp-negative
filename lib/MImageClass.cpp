//
// Created by Devarsh Patel on 4/3/23.
//
#include "MImageClass.h"

using namespace std;

MImageClass::MImageClass(const string& filePath) {
    ifstream inputImageFile;
    inputImageFile.open(filePath.c_str(), ios_base::in | ios_base::binary);

    // clear previous data
    for(int i=0; i<pixels.size(); i++){
        pixels[i].clear();
    }
    pixels.clear();

    if (inputImageFile.fail()) {
        cerr << filePath << " opening failed.\n";
        ::exit(FAIL);
    }
    else {
        bmp_initials initials;
        inputImageFile.read((char*)(&initials), sizeof(initials));

        if(initials.initials[0] != 'B' || initials.initials[1] != 'M') {
            cerr << "Invalid image format.\n";
            ::exit(FAIL);
        } else {

            // get headers
            bmp_header header;
            inputImageFile.read((char*)(&header), sizeof(header));

            // get dib
            bmp_dib dib;
            inputImageFile.read((char *)(&dib), sizeof(dib));

            // check flip
            bool flip = true;
            if(dib.height < 0) {
                flip = false;
                dib.height = -dib.height;
            }

            // set support for 24-bit
            if(dib.bits_per_pixel != 24) {
                cerr << "Only 24-bit formatted files are supported.\n";
                ::exit(FAIL);
            }

            // allow non-compressed files
            if (dib.cmp !=0 ) {
                cerr << "Compressed files are not supported.\n";
                ::exit(FAIL);
            }

            inputImageFile.seekg(header.offset);

            maxValue = 0;

            cout << "Image loading started...\n";

            // get data
            for(int y=0; y < dib.height; y++) {
                vector<Pixel> y_ind;
                for(int x=0; x < dib.width; x++) {
                    // pixel color are stored in reverse format
                    int b = inputImageFile.get(),
                        g = inputImageFile.get(),
                        r = inputImageFile.get();

                    maxValue = max(maxValue, max(r, max(g, b)));
                    y_ind.emplace_back(Pixel(r, g, b));
                    inputImageFile.seekg(dib.width % 4, ios::cur);
                }
                if (flip) {
                    pixels.insert(pixels.begin(), y_ind);
                }
                else {
                    pixels.push_back(y_ind);
                }
            }
            cout << "Image loading completed...\n";
        }
        inputImageFile.close();
    }
}

bool MImageClass::isValid() {
    int p_height = pixels.size();
    if (p_height == 0 || pixels[0].size() == 0) {
        return false;
    } else {
        int p_width = pixels[0].size();
        cout << "height: " << p_height << ", width: " << p_width << endl;
        for(int y=0; y < p_height ; y++) {
            if (pixels[y].size() != p_width) return false;
            for (int x = 0; x < p_width; x++) {
                Pixel cur = pixels[y][x];
                if( cur.r > MAX_RGB || cur.r < MIN_RGB) {
                    cout << "r: " << cur.r << endl;
                    return false;
                }
                else if (cur.g > MAX_RGB || cur.g < MIN_RGB) {
                    cout << "g: " << cur.g << endl;
                    return false;
                }
                else if (cur.b > MAX_RGB || cur.b < MIN_RGB ) {
                    cout << "b: " << cur.b << endl;
                    return false;
                }
            }
        }
        return true;
    }
}

MImageClass MImageClass::negative() {
    int height = pixels.size(), n_maxValue = 0;
    PixelMatrix n_pixels;
    if (height != 0) {
        int width = pixels[0].size();
        for(int y = 0; y < height; y++) {
            vector<Pixel> y_ind;
            for(int x = 0; x < width; x++) {
                auto pxl = pixels[y][x];
                int n_r = maxValue - pxl.r,
                    n_g = maxValue - pxl.g,
                    n_b = maxValue - pxl.b;
                n_maxValue = max(n_maxValue, max(n_r, max(n_g, n_b)));
                y_ind.emplace_back(Pixel(n_r, n_g, n_b));
            }
            n_pixels.emplace_back(y_ind);
        }
    }
    return MImageClass(n_maxValue, n_pixels);
}

void MImageClass::save(const string& filePath) {
    ofstream outputImageFile;
    outputImageFile.open(filePath.c_str(), ios_base::out | ios_base::binary);
    if (outputImageFile.fail()) {
        cerr << filePath << " opening failed.\n";
        ::exit(FAIL);
    }
    else if (!isValid()) {
        cerr << "Invalid image data.\n";
        ::exit(FAIL);
    }
    else {
        cout << "Image writing started...\n";
        // initials
        bmp_initials initials;
        initials.initials[0] = 'B';
        initials.initials[1] = 'M';
        outputImageFile.write((char*)(&initials), sizeof(initials));

        // header
        bmp_header header = {0};
        header.offset = sizeof(bmp_initials) + sizeof(bmp_header) + sizeof(bmp_dib);
        header.size = header.offset + (pixels.size() * 3 + pixels[0].size() % 4) * pixels.size();
        outputImageFile.write((char*)(&header), sizeof(header));

        // dib information
        bmp_dib dib = {0};
        dib.header_size = sizeof(bmp_dib);
        dib.height = pixels.size();
        dib.width = pixels[0].size();
        dib.planes = 1;
        dib.bits_per_pixel = 24;
        dib.cmp = 0;
        dib.byte_size = 0;
        dib.x_res = 2835;
        dib.y_res = 2835;
        dib.colors = 0;
        dib.imp_colors = 0;
        outputImageFile.write((char*)(&dib), sizeof(dib));

        // write data in reverse
        for(int y = pixels.size()-1 ; y>=0 ; y--) {
            for(int x=0; x < pixels[y].size() ; x++) {
                auto pix = pixels[y][x];
                outputImageFile.put((u_char)(pix.b));
                outputImageFile.put((u_char)(pix.g));
                outputImageFile.put((u_char)(pix.r));
            }

            // padding
            for(int x=0; x < (pixels[0].size() % 4); x++) {
                outputImageFile.put(0);
            }
        }
        outputImageFile.close();
        cout << "Image writing completed...\n";
    }
}