#ifndef UPLOADER_H
#define UPLOADER_H

#include <string>

enum class UploadResult
{
    SUCCESS,
    FAILED,
};

struct UploadResponse
{
    UploadResult result;
    std::string message;
};

UploadResponse upload_image(const std::string &server, const std::string &filepath);
bool check_health(const std::string &server);

#endif
