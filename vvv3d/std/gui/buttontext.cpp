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

void ButtonText::onHover() { background->setColor(SILVER); }

void ButtonText::onUnhover() { background->setColor(Color(0)); }

void ButtonText::onHoverPressed(int, int) { background->setColor(ORANGE); }

void ButtonText::onContentChanged()
{
    setSize(text->getSize());
}

void ButtonText::setText(const std::string& text) {this->text->setText(text);}
const std::string& ButtonText::getText() const {return text->getText();}

}
