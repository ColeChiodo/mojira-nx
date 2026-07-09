#include <cstdio>
#include <cstring>
#include <string>
#include <strings.h>
#include <dirent.h>
#include <sys/stat.h>
#include <algorithm>

#include "scanner.h"

static bool has_extension(const char *name) {
    const char *exts[] = { ".jpg", ".jpeg", ".png" };
    size_t len = strlen(name);
    for (int e = 0; e < 3; e++) {
        size_t elen = strlen(exts[e]);
        if (len >= elen && strcasecmp(name + len - elen, exts[e]) == 0)
            return true;
    }
    return false;
}

static bool is_dir(const std::string &path)
{
    struct stat st;
    return stat(path.c_str(), &st) == 0 && S_ISDIR(st.st_mode);
}

void GalleryScanner::scan_dir(const std::string &dirpath)
{
    DIR *dir = opendir(dirpath.c_str());
    if (!dir)
        return;

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL && image_count < MAX_GALLERY_IMAGES)
    {
        if (entry->d_name[0] == '.')
            continue;

        std::string full = dirpath + "/" + entry->d_name;

        if (is_dir(full))
        {
            scan_dir(full);
        }
        else if (has_extension(entry->d_name))
        {
            images[image_count].filename = entry->d_name;
            images[image_count].fullpath = full;
            image_count++;
        }
    }

    closedir(dir);
}

GalleryScanner::GalleryScanner()
{
    image_count = 0;
}

int GalleryScanner::scan(const std::string &root)
{
    image_count = 0;
    std::string dir = root;
    while (!dir.empty() && dir.back() == '/')
        dir.pop_back();
    if (dir.empty()) dir = "/";
    scan_dir(dir);
    return image_count;
}

const GalleryImage& GalleryScanner::get(int index) const
{
    return images[index];
}

int GalleryScanner::count() const
{
    return image_count;
}