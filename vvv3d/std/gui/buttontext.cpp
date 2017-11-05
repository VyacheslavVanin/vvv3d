#include "buttontext.h"
#include "colorrectanglewidget.h"
#include "panel.h"
#include "textwidget.h"
#include "verticallayout.h"
#include "vvv3d/core/graphics/color.h"

namespace vvv3d {

ButtonText::ButtonText(const std::string& caption)
    : text(new TextWidget(caption)), background(new ColorRectWidget(Color(0))),
      panel(new Panel(new VerticalLayout(), background))
{
    panel->addWidget(text);
    addChild(panel);
    setSize(text->getSize());
}

void ButtonText::onHover() { background->setColor(Color::SILVER); }

void ButtonText::onUnhover() { background->setColor(Color(0)); }

void ButtonText::onPressDown() { background->setColor(Color::ORANGE); }

void ButtonText::setText(const std::string& text) {this->text->setText(text);}
const std::u32string& ButtonText::getText32() const {return text->getText();}
std::string ButtonText::getText() const {return toU8(text->getText());}

}
