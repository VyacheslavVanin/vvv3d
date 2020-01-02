#include "buttontext.hpp"
#include <vvv3d/core/graphics/color.hpp>
#include <vvv3d/gui/widgets/images/colorrectanglewidget.hpp>
#include <vvv3d/gui/widgets/layouts/verticallayout.hpp>
#include <vvv3d/gui/widgets/panel.hpp>
#include <vvv3d/gui/widgets/text/textwidget.hpp>

namespace vvv3d {

ButtonText::ButtonText(const std::string& caption)
    : text(new TextWidget(caption)), background(new ColorRectWidget(Color(0))),
      panel(new Panel(new VerticalLayout(), background))
{
    panel->setBorder(0);
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
    std::ignore = oldsize;
    text->setSize(newsize);
}

void ButtonText::setText(const std::string& text) { this->text->setText(text); }
const std::string& ButtonText::getText() const { return text->getText(); }

} // namespace vvv3d
