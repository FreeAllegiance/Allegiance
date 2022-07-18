#include "Exposer.h"

#include "Lib/fmtlib/fmt/include/fmt/format.h"

std::string StringDumper::dump(const UiType& value) {
    return value.typeSwitch<std::string>(
        [](std::monostate value) {
            return std::string("NULL");
        },
        [](const std::string& value) {
            return fmt::format(
                "string({:s})",
                value
            );
        },
            [](int value) {
            return std::string("int");
        },
            [](float value) {
            return std::string("float");
        },
            [](const std::vector<UiType>& value) {
            return std::string("list");
        },
            [](const TRef<ColorValue>& value) {
            auto color = value->GetValue();
            return fmt::format(
                "color({:.2f}, {:.2f}, {:.2f}, {:.2f})",
                color.R(),
                color.G(),
                color.B(),
                color.A()
            );
        }
        );
};

TRef<Number> CreateNumberFromUiType(UiType const& obj) {
    return obj.typeSwitch<TRef<Number>>(
        [](float a) {
            return (TRef<Number>)new Number(a);
        },
        [](TRef<Number> const& a) {
            return a;
        }
        );
}