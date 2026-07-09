#include <unistd.h>
#include <switch.h>
#include <curl/curl.h>

#include <borealis.hpp>
#include "gallery.h"
#include "settings.h"
#include "config.h"
#include "uploader.h"

int main(int argc, char *argv[])
{
    romfsInit();
    chdir("romfs:/");

    socketInitializeDefault();
    curl_global_init(CURL_GLOBAL_DEFAULT);

    brls::Logger::setLogLevel(brls::LogLevel::DEBUG);

    if (!brls::Application::init())
        return EXIT_FAILURE;

    brls::Application::createWindow("Mojira Gallery");

    std::string server = config_get_server();
    if (server.empty() || !check_health(server))
        brls::Application::pushActivity(new SettingsActivity(true));
    else
        brls::Application::pushActivity(new GalleryActivity());

    while (brls::Application::mainLoop())
        ;

    curl_global_cleanup();
    socketExit();
    romfsExit();
    return EXIT_SUCCESS;
}
