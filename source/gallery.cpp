#include "gallery.h"
#include "about.h"
#include "scanner.h"
#include "config.h"
#include "settings.h"
#include "uploader.h"
#include <string>

class AlbumImage : public brls::Image
{
public:
    AlbumImage(brls::Label *label, const std::string &name) : status_label(label), filename(name) {}
    void onFocusGained() override
    {
        status_label->setText(filename);
        brls::Image::onFocusGained();
    }
private:
    brls::Label *status_label;
    std::string filename;
};

GalleryGridRow::GalleryGridRow()
    : brls::Box(brls::Axis::ROW)
{
    setAlignItems(brls::AlignItems::STRETCH);
}

brls::View* GalleryGridRow::getNextFocus(brls::FocusDirection direction, brls::View* currentView)
{
    if (direction != brls::FocusDirection::LEFT && direction != brls::FocusDirection::RIGHT)
        return brls::Box::getNextFocus(direction, currentView);

    brls::View* realFocus = brls::Application::getCurrentFocus();
    if (!realFocus) return nullptr;

    brls::View* col = realFocus;
    while (col)
    {
        brls::Box* parent = col->getParent();
        if (!parent) break;
        if (parent == this) break;
        col = parent;
    }
    if (!col) return nullptr;

    auto& children = getChildren();
    int colIndex = -1;
    for (size_t i = 0; i < children.size(); i++)
    {
        if (children[i] == col)
        {
            colIndex = i;
            break;
        }
    }
    if (colIndex < 0) return nullptr;

    auto& colChildren = static_cast<brls::Box*>(col)->getChildren();
    int vertIndex = -1;
    for (size_t i = 0; i < colChildren.size(); i++)
    {
        if (colChildren[i] == realFocus)
        {
            vertIndex = i;
            break;
        }
    }
    if (vertIndex < 0) return nullptr;

    int nextColIndex = colIndex + (direction == brls::FocusDirection::RIGHT ? 1 : -1);
    if (nextColIndex < 0 || nextColIndex >= (int)children.size())
        return nullptr;

    brls::Box* nextCol = static_cast<brls::Box*>(children[nextColIndex]);
    auto& nextChildren = nextCol->getChildren();

    int targetIndex = std::min(vertIndex, (int)nextChildren.size() - 1);
    return nextChildren[targetIndex];
}

GalleryGrid::GalleryGrid()
    : brls::Box(brls::Axis::COLUMN)
{
    setGrow(1.0f);
    setPadding(8, 8, 8, 8);
    setMarginTop(20);
    setMarginLeft(16);
}

void GalleryGrid::setContentView(brls::View* view)
{
    if (contentView)
    {
        Box::removeView(contentView);
        contentView = nullptr;
    }
    if (!view) return;
    contentView = view;
    contentView->detach();
    contentView->setCulled(false);
    contentView->setDetachedPosition(8, 8);
    contentView->setMaxWidth(brls::View::AUTO);
    contentView->setMaxHeight(brls::View::AUTO);
    Box::addView(view);
}

void GalleryGrid::onLayout()
{
    if (contentView)
    {
        contentView->setDetachedPosition(getX() + 8, getY() + 8);
        contentView->setHeight(getHeight() - 16);
        contentView->invalidate();
    }
    Box::onLayout();
}

void GalleryGrid::draw(NVGcontext* vg, float x, float y, float width, float height, brls::Style style, brls::FrameContext* ctx)
{
    nvgSave(vg);
    nvgIntersectScissor(vg, x, y, width, height);
    Box::draw(vg, x, y, width, height, style, ctx);
    nvgRestore(vg);
}

void GalleryGrid::scrollTo(float offset)
{
    if (contentView)
    {
        scrollOffset = offset;
        contentView->setTranslationX(-scrollOffset);
        invalidate();
    }
}

float GalleryGrid::getContentWidth()
{
    if (contentView)
        return contentView->getWidth();
    return 0;
}

void GalleryGrid::onChildFocusGained(brls::View* directChild, brls::View* focusedView)
{
    if (contentView && focusedView)
    {
        float focusCenter = focusedView->getX() + focusedView->getWidth() / 2;
        float ourCenter = getX() + getWidth() / 2;
        float diff = focusCenter - ourCenter;

        float newOffset = scrollOffset + diff;
        float maxS = std::max(0.0f, getContentWidth() - getWidth());
        if (newOffset > maxS) newOffset = maxS;
        if (newOffset < 0) newOffset = 0;

        if (std::abs(diff) > 10.0f)
            scrollTo(newOffset);
    }
    Box::onChildFocusGained(directChild, focusedView);
}

GalleryActivity::GalleryActivity()
{
}

brls::View* GalleryActivity::createContentView()
{
    status_label = new brls::Label();
    status_label->setFontSize(14);
    status_label->setTextColor(brls::Application::getTheme()["brls/text"]);

    scanner = std::make_shared<GalleryScanner>();
    image_count = scanner->scan("sdmc:/emuMMC/RAW2/Nintendo/Album");

    auto *root = new brls::Box(brls::Axis::COLUMN);

    auto *header = new brls::Box(brls::Axis::ROW);
    header->setHeight(60);
    header->setPaddingLeft(40);
    header->setPaddingRight(40);
    header->setAlignItems(brls::AlignItems::CENTER);

    auto *icon = new brls::Image();
    icon->setImageFromFile("romfs:/img/icon.jpg");
    icon->setWidth(36);
    icon->setHeight(36);
    header->addView(icon);

    auto *title = new brls::Label();
    title->setText("Mojira");
    title->setFontSize(26);
    title->setMarginLeft(12);
    title->setGrow(1.0f);
    header->addView(title);

    auto *countLabel = new brls::Label();
    countLabel->setText("All Images (" + std::to_string(image_count) + ")");
    countLabel->setFontSize(18);
    countLabel->setTextColor(brls::Application::getTheme()["brls/text"]);
    countLabel->setMarginRight(8);
    header->addView(countLabel);

    root->addView(header);

    auto *sep = new brls::Rectangle(nvgRGB(60, 60, 60));
    sep->setHeight(1);
    root->addView(sep);

    galleryGrid = new GalleryGrid();
    root->addView(galleryGrid);

    if (image_count > 0)
    {
        auto *grid = new GalleryGridRow();

        const float columnWidth = 340.0f;
        brls::Box *currentCol = nullptr;

        for (int i = image_count - 1; i >= 0; i--)
        {
            int colStart = image_count - 1 - i;
            if (colStart % 3 == 0)
            {
                currentCol = new brls::Box(brls::Axis::COLUMN);
                currentCol->setWidth(columnWidth);
                currentCol->setPadding(4, 4, 4, 4);
                currentCol->setJustifyContent(brls::JustifyContent::FLEX_START);
                grid->addView(currentCol);
            }

            const GalleryImage &img = scanner->get(i);

            std::string fname = img.filename;
            std::string fpath = img.fullpath;

            auto *imageView = new AlbumImage(status_label, fname);
            imageView->setImageFromFile(img.fullpath);
            imageView->setScalingType(brls::ImageScalingType::FIT);
            imageView->setFocusable(true);
            imageView->setMargins(4, 4, 4, 4);
            YGNodeStyleSetAspectRatio(imageView->getYGNode(), 16.0f / 9.0f);

            imageView->registerClickAction([this, fname, fpath](brls::View*) {
                status_label->setText("Uploading " + fname + "...");
                std::string server = config_get_server();
                UploadResponse resp = upload_image(server, fpath);
                if (resp.result == UploadResult::SUCCESS)
                    status_label->setText("Uploaded " + fname);
                else
                    status_label->setText("Failed to upload " + fname);
                return true;
            });

            currentCol->addView(imageView);
        }

        galleryGrid->setContentView(grid);
    }
    else
    {
        auto *emptyLabel = new brls::Label();
        emptyLabel->setText("No images found in sdmc:/emuMMC/RAW2/Nintendo/Album");
        emptyLabel->setFontSize(20);
        emptyLabel->setHorizontalAlign(brls::HorizontalAlign::CENTER);
        emptyLabel->setGrow(1.0f);
        galleryGrid->addView(emptyLabel);
    }

    auto *sep2 = new brls::Rectangle(nvgRGB(60, 60, 60));
    sep2->setHeight(1);
    root->addView(sep2);

    auto *footer = new brls::Box(brls::Axis::ROW);
    footer->setHeight(50);
    footer->setPaddingLeft(32);
    footer->setPaddingRight(32);
    footer->setAlignItems(brls::AlignItems::CENTER);

    status_label->setGrow(1.0f);
    footer->addView(status_label);

    auto *hints = new brls::Box(brls::Axis::ROW);
    hints->setAlignItems(brls::AlignItems::CENTER);

    auto *a_hint = new brls::Label();
    a_hint->setText("A  Upload");
    a_hint->setFontSize(16);
    a_hint->setMarginRight(24);
    hints->addView(a_hint);

    auto *y_hint = new brls::Label();
    y_hint->setText("Y  About");
    y_hint->setFontSize(16);
    y_hint->setMarginRight(24);
    hints->addView(y_hint);

    auto *x_hint = new brls::Label();
    x_hint->setText("X  Settings");
    x_hint->setFontSize(16);
    x_hint->setMarginRight(24);
    hints->addView(x_hint);

    auto *plus_hint = new brls::Label();
    plus_hint->setText("+  Exit");
    plus_hint->setFontSize(16);
    hints->addView(plus_hint);

    footer->addView(hints);
    root->addView(footer);

    return root;
}

void GalleryActivity::onContentAvailable()
{
    registerAction("Exit", brls::BUTTON_START, [](brls::View*) {
        brls::Application::quit();
        return true;
    });

    registerAction("About", brls::BUTTON_Y, [](brls::View*) {
        brls::Application::pushActivity(new AboutActivity());
        return true;
    });

    registerAction("Settings", brls::BUTTON_X, [](brls::View*) {
        brls::Application::pushActivity(new SettingsActivity());
        return true;
    });
}
