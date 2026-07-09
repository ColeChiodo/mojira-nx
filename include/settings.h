#ifndef SETTINGS_H
#define SETTINGS_H

#include <borealis.hpp>

class SettingsActivity : public brls::Activity
{
public:
    SettingsActivity(bool autoOpened = false);
    void onContentAvailable() override;
    brls::View* createContentView() override;

private:
    bool autoOpened;
    brls::Label *currentLabel = nullptr;
    brls::Label *statusLabel = nullptr;
    std::string serverUrl;
};

#endif
