#pragma once

namespace vvv3d {

class IExpandProperty {
public:
    virtual void setExpandToContent(bool expand) = 0;
    virtual bool isExpandToContent() const = 0;
    virtual ~IExpandProperty() = default;
};

} // namespace vvv3d
