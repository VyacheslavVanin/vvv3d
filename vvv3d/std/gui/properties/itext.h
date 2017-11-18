#ifndef VVV_ITEXT_H
#define VVV_ITEXT_H
#include <string>
#include <vvv3d/std/gui/widget.h>

namespace vvv3d {

class ITextProperty : public virtual Widget {
public:
    virtual void setText(const std::string& text) = 0;
    virtual const std::string& getText() const = 0;
};

} // namespace vvv3d

#endif
