#ifndef SCANNER_H
#define SCANNER_H

#include <string>
#include <vector>

#define MAX_GALLERY_IMAGES 512

struct GalleryImage {
    std::string filename;
    std::string fullpath;
    int w = 0;
    int h = 0;
};

class GalleryScanner {
public:
    GalleryScanner();
    int scan(const std::string &directory);
    const GalleryImage& get(int index) const;
    int count() const;

private:
    void scan_dir(const std::string &dirpath);
    GalleryImage images[MAX_GALLERY_IMAGES];
    int image_count = 0;
};

#endif