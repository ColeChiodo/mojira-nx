#ifndef GALLERY_H
#define GALLERY_H

#include <borealis.hpp>
#include "scanner.h"

class GalleryGridRow : public brls::Box
{
public:
    GalleryGridRow();
    brls::View* getNextFocus(brls::FocusDirection direction, brls::View* currentView) override;
};

class GalleryGrid : public brls::Box
{
public:
    GalleryGrid();

    void setContentView(brls::View* view);
    void onLayout() override;
    void draw(NVGcontext* vg, float x, float y, float width, float height, brls::Style style, brls::FrameContext* ctx) override;
    void scrollTo(float offset);
    float getContentWidth();
    float getScrollOffset() const { return scrollOffset; }
    void onChildFocusGained(brls::View* directChild, brls::View* focusedView) override;

private:
    brls::View* contentView = nullptr;
    float scrollOffset = 0.0f;
};

class GalleryActivity : public brls::Activity {
public:
    GalleryActivity();
    void onContentAvailable() override;
    brls::View* createContentView() override;
    bool onCancel() { brls::Application::quit(); return true; }

private:
    brls::Label *status_label = nullptr;
    GalleryGrid *galleryGrid = nullptr;
    std::shared_ptr<GalleryScanner> scanner;
    int image_count = 0;
};

#endif
