#include "about.h"

AboutActivity::AboutActivity()
{
}

brls::View* AboutActivity::createContentView()
{
    auto *box = new brls::Box(brls::Axis::COLUMN);
    box->setJustifyContent(brls::JustifyContent::CENTER);
    box->setAlignItems(brls::AlignItems::CENTER);
    box->setPadding(60);
    box->setBackgroundColor(nvgRGB(25, 25, 35));

    auto *icon = new brls::Image();
    icon->setImageFromFile("romfs:/img/icon.jpg");
    icon->setWidth(128);
    icon->setHeight(128);
    icon->setCornerRadius(24);
    icon->setMarginBottom(24);
    box->addView(icon);

    icon->setFocusable(true);

    auto *title = new brls::Label();
    title->setText("Mojira");
    title->setFontSize(36);
    title->setMarginBottom(8);
    box->addView(title);

    auto *version = new brls::Label();
    version->setText("v1.0.0");
    version->setFontSize(18);
    version->setTextColor(brls::Application::getTheme()["brls/text"]);
    version->setMarginBottom(24);
    box->addView(version);

    auto *desc = new brls::Label();
    desc->setText("Switch screenshot uploader\n\nUpload album images to a local\nnetwork server with the press of a button.");
    desc->setFontSize(16);
    desc->setTextColor(brls::Application::getTheme()["brls/text"]);
    desc->setHorizontalAlign(brls::HorizontalAlign::CENTER);
    desc->setMarginBottom(32);
    box->addView(desc);

    auto *hint = new brls::Label();
    hint->setText("B  Back");
    hint->setFontSize(16);
    hint->setTextColor(brls::Application::getTheme()["brls/text"]);
    hint->setFocusable(true);
    box->addView(hint);

    return box;
}

void AboutActivity::onContentAvailable()
{
    registerAction("Back", brls::BUTTON_B, [](brls::View*) {
        brls::Application::popActivity();
        return true;
    });
}