#ifndef ABOUT_H
#define ABOUT_H

#include <borealis.hpp>

class AboutActivity : public brls::Activity {
public:
    AboutActivity();
    void onContentAvailable() override;
    brls::View* createContentView() override;

    bool isTranslucent() override { return true; }
};

#endif