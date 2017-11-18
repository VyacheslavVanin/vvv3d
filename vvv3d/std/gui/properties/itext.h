#ifndef VVV_ITEXT_H
#define VVV_ITEXT_H
#include <string>

namespace vvv3d {

class ITextProperty {
public:
    virtual void setText(const std::string& text) = 0;
    virtual const std::string& getText() const = 0;
    virtual ~ITextProperty() = default;
};

} // namespace vvv3d

#endif
