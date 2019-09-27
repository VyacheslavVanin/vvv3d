#pragma once
#include <functional>
#include <string>
#include <vector>

namespace vvv3d {

void invokeAction(const std::string& name);
void invokeAction(const std::string& name, int value);
void invokeAction(const std::string& name, const std::string& value);
void invokeAction(const std::vector<std::string>& name);
void invokeAction(const std::vector<std::string>& name, int value);
void invokeAction(const std::vector<std::string>& name,
                  const std::string& value);

void addAction(const std::string& name, const std::function<void(void)>& f);
void addAction(const std::string& name, const std::function<void(int)>& f);
void addAction(const std::string& name,
               const std::function<void(const std::string&)>& f);

class IActionProperty {
public:
    virtual void setAction(const std::string& name) = 0;
    virtual ~IActionProperty() = default;
};

class IOnEnterProperty {
public:
    virtual void setOnEnterAction(const std::string& name) = 0;
    virtual ~IOnEnterProperty() = default;
};

class IOnValueChangeProperty {
public:
    virtual void setOnValueChangedAction(const std::string& name) = 0;
    virtual ~IOnValueChangeProperty() = default;
};

} // namespace vvv3d
