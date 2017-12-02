#include "buttontext.hpp"
#include "colorrectanglewidget.hpp"
#include "panel.hpp"
#include "textwidget.hpp"
#include "verticallayout.hpp"
#include "vvv3d/core/graphics/color.hpp"

namespace vvv3d {

ButtonText::ButtonText(const std::string& caption)
    : text(new TextWidget(caption)), background(new ColorRectWidget(Color(0))),
      panel(new Panel(new VerticalLayout(), background))
{
    panel->addWidget(text);
    addChild(panel);
    setSize(text->getSize());
}

void ButtonText::onHover() { background->setColor(SILVER); }

void ButtonText::onUnhover() { background->setColor(Color(0)); }

void ButtonText::onHoverPressed(int, int) { background->setColor(ORANGE); }

void ButtonText::onContentChanged() { setSize(text->getSize()); }
void ButtonText::onResize(const vvv::vector2i& oldsize,
                          const vvv::vector2i& newsize)
{
    text->setSize(newsize);
}

void ButtonText::setText(const std::string& text) { this->text->setText(text); }
const std::string& ButtonText::getText() const { return text->getText(); }

} // namespace vvv3d
