#include "settings.h"
#include "config.h"
#include "gallery.h"
#include <borealis.hpp>
#include <borealis/platforms/switch/swkbd.hpp>
#include <switch.h>
#include <string>

static bool open_server_url(const std::string &initial, std::string &out)
{
    SwkbdConfig kbd;
    swkbdCreate(&kbd, 0);
    swkbdConfigMakePresetDefault(&kbd);
    swkbdConfigSetHeaderText(&kbd, "Enter server URL");
    swkbdConfigSetSubText(&kbd, "e.g. http://192.168.1.100:8000");
    swkbdConfigSetStringLenMax(&kbd, 256);
    swkbdConfigSetInitialText(&kbd, initial.c_str());
    swkbdConfigSetBlurBackground(&kbd, true);
    swkbdConfigSetType(&kbd, SwkbdType_QWERTY);

    char buf[257];
    buf[0] = '\0';
    bool ok = R_SUCCEEDED(swkbdShow(&kbd, buf, sizeof(buf))) && buf[0] != '\0';
    swkbdClose(&kbd);

    if (ok)
        out = buf;
    return ok;
}

SettingsActivity::SettingsActivity(bool autoOpened)
    : autoOpened(autoOpened)
{
}

brls::View* SettingsActivity::createContentView()
{
    serverUrl = config_get_server();

    auto *root = new brls::Box(brls::Axis::COLUMN);

    auto *header = new brls::Box(brls::Axis::ROW);
    header->setHeight(60);
    header->setPaddingLeft(40);
    header->setPaddingRight(40);
    header->setAlignItems(brls::AlignItems::CENTER);

    auto *title = new brls::Label();
    title->setText("Server Settings");
    title->setFontSize(26);
    title->setGrow(1.0f);
    header->addView(title);

    root->addView(header);

    auto *sep = new brls::Rectangle(nvgRGB(60, 60, 60));
    sep->setHeight(1);
    root->addView(sep);

    auto *body = new brls::Box(brls::Axis::COLUMN);
    body->setGrow(1.0f);
    body->setPaddingLeft(40);
    body->setPaddingRight(40);
    body->setPaddingTop(30);
    body->setAlignItems(brls::AlignItems::CENTER);

    currentLabel = new brls::Label();
    currentLabel->setFontSize(18);
    currentLabel->setTextColor(brls::Application::getTheme()["brls/text"]);
    currentLabel->setText("Server: " + (serverUrl.empty() ? "(not set)" : serverUrl));
    currentLabel->setGrow(1.0f);
    body->addView(currentLabel);

    auto *editBtn = new brls::Box(brls::Axis::ROW);
    editBtn->setHeight(50);
    editBtn->setMarginTop(20);
    editBtn->setPaddingLeft(20);
    editBtn->setPaddingRight(20);
    editBtn->setAlignItems(brls::AlignItems::CENTER);
    editBtn->setFocusable(true);

    auto *editLabel = new brls::Label();
    editLabel->setText("Edit Server URL");
    editLabel->setFontSize(20);
    editBtn->addView(editLabel);

    editBtn->registerClickAction([this](brls::View*) {
        std::string text;
        if (open_server_url(serverUrl, text))
        {
            serverUrl = text;
            config_set_server(serverUrl);
            if (currentLabel)
                currentLabel->setText("Server: " + serverUrl);
            if (statusLabel)
                statusLabel->setText("Saved");
        }
        return true;
    });
    body->addView(editBtn);

    statusLabel = new brls::Label();
    statusLabel->setFontSize(16);
    statusLabel->setTextColor(nvgRGB(100, 200, 100));
    statusLabel->setMarginTop(16);
    body->addView(statusLabel);

    if (autoOpened)
    {
        auto *autoMsg = new brls::Label();
        autoMsg->setText("Server health check failed.\nPlease set your server address.");
        autoMsg->setFontSize(16);
        autoMsg->setTextColor(nvgRGB(200, 100, 100));
        autoMsg->setMarginTop(30);
        autoMsg->setHorizontalAlign(brls::HorizontalAlign::CENTER);
        body->addView(autoMsg);
    }

    root->addView(body);

    auto *sep2 = new brls::Rectangle(nvgRGB(60, 60, 60));
    sep2->setHeight(1);
    root->addView(sep2);

    auto *footer = new brls::Box(brls::Axis::ROW);
    footer->setHeight(50);
    footer->setPaddingLeft(32);
    footer->setPaddingRight(32);
    footer->setAlignItems(brls::AlignItems::CENTER);
    footer->setJustifyContent(brls::JustifyContent::FLEX_END);

    auto *b_hint = new brls::Label();
    b_hint->setText("B  Back");
    b_hint->setFontSize(16);
    footer->addView(b_hint);

    root->addView(footer);

    return root;
}

void SettingsActivity::onContentAvailable()
{
    registerAction("Back", brls::BUTTON_B, [this](brls::View*) {
        if (autoOpened)
            brls::Application::pushActivity(new GalleryActivity());
        else
            brls::Application::popActivity();
        return true;
    });
}


