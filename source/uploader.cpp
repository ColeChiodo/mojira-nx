#include "uploader.h"
#include <cstdio>
#include <cstring>
#include <string>
#include <curl/curl.h>
#include <sys/stat.h>

static size_t write_null(void *ptr, size_t size, size_t nmemb, void *userdata)
{
    (void)ptr;
    (void)userdata;
    return size * nmemb;
}

struct string_buf {
    char *data;
    size_t len;
};

static size_t write_string(void *ptr, size_t size, size_t nmemb, void *userdata)
{
    size_t total = size * nmemb;
    auto *buf = (string_buf *)userdata;
    char *new_data = (char *)realloc(buf->data, buf->len + total + 1);
    if (!new_data) return 0;
    memcpy(new_data + buf->len, ptr, total);
    buf->data = new_data;
    buf->len += total;
    buf->data[buf->len] = '\0';
    return total;
}

bool check_health(const std::string &server)
{
    if (server.empty()) return false;

    std::string url = server;
    if (url.back() == '/') url.pop_back();
    url += "/health";

    CURL *curl = curl_easy_init();
    if (!curl) return false;

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_null);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 5L);
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 3L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);

    long http_code = 0;
    CURLcode res = curl_easy_perform(curl);
    if (res == CURLE_OK)
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);

    curl_easy_cleanup(curl);
    return http_code == 200;
}

UploadResponse upload_image(const std::string &server, const std::string &filepath)
{
    if (server.empty())
        return { UploadResult::FAILED, "No server configured" };

    struct stat st;
    if (stat(filepath.c_str(), &st) != 0)
        return { UploadResult::FAILED, "File not found" };

    std::string url = server;
    if (url.back() == '/') url.pop_back();
    url += "/captures";

    CURL *curl = curl_easy_init();
    if (!curl)
        return { UploadResult::FAILED, "Failed to init HTTP" };

    curl_mime *mime = curl_mime_init(curl);
    curl_mimepart *part = curl_mime_addpart(mime);
    curl_mime_name(part, "image");
    curl_mime_filedata(part, filepath.c_str());

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_MIMEPOST, mime);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 30L);
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 5L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

    string_buf response = { nullptr, 0 };
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_string);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

    long http_code = 0;
    CURLcode res = curl_easy_perform(curl);
    std::string msg;

    if (res != CURLE_OK)
    {
        msg = curl_easy_strerror(res);
    }
    else
    {
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
        if (http_code == 200 || http_code == 201 || http_code == 302)
        {
            msg = "Uploaded";
        }
        else
        {
            msg = "HTTP " + std::to_string(http_code);
        }
    }

    curl_mime_free(mime);
    curl_easy_cleanup(curl);
    free(response.data);

    return {
        (http_code == 200 || http_code == 201 || http_code == 302) ? UploadResult::SUCCESS : UploadResult::FAILED,
        msg
    };
}
