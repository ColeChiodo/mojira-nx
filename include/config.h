#ifndef CONFIG_H
#define CONFIG_H

#include <string>

std::string config_get_server();
void config_set_server(const std::string &url);

#endif
