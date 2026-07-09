#include "config.h"
#include <cstdio>
#include <cstring>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>

static const char *CONFIG_DIR = "sdmc:/switch/mojira-nx";
static const char *CONFIG_FILE = "sdmc:/switch/mojira-nx/config.txt";

std::string config_get_server()
{
    mkdir(CONFIG_DIR, 0777);

    FILE *f = fopen(CONFIG_FILE, "r");
    if (!f)
        return "";

    char buf[512];
    if (!fgets(buf, sizeof(buf), f))
    {
        fclose(f);
        return "";
    }

    fclose(f);

    std::string url(buf);
    while (!url.empty() && (url.back() == '\n' || url.back() == '\r'))
        url.pop_back();
    return url;
}

void config_set_server(const std::string &url)
{
    mkdir(CONFIG_DIR, 0777);

    FILE *f = fopen(CONFIG_FILE, "w");
    if (!f)
        return;

    fputs(url.c_str(), f);
    fputc('\n', f);
    fclose(f);
}
