#include "pdfium_option.h"

#define INIT_PDFIUM_OPTION(name)                                           \
    if (obj.Has(#name))            \
    {                                                                      \
        ops->name = (obj).Get(#name).ToNumber().Int32Value() ;                                    \
    }

namespace node_pdfium
{
std::unique_ptr<PdfiumOption> V8OptionToStruct(const Napi::Value &options)
{
    auto ops = std::make_unique<PdfiumOption>();
    if (options.IsObject())
    {
        auto obj = options.ToObject();
        auto &env = options.Env();

        INIT_PDFIUM_OPTION(dpi);
        INIT_PDFIUM_OPTION(copies);
        INIT_PDFIUM_OPTION(width);
        INIT_PDFIUM_OPTION(height);
        INIT_PDFIUM_OPTION(fit);

        ops->dpi = ops->dpi / 72;

        if (obj.Has("pageList"))
        {
            auto& pageList = obj.Get("pageList") ;
            Napi::Array& arr = pageList.As<Napi::Array>();
            for (unsigned int i = 0; i < arr.Length(); ++i)
            {
                const Napi::Array& item = (arr.Get(i)).As<Napi::Array>();
                auto pair = std::make_pair(
                    item.Get(static_cast<unsigned int>(0)).As<Napi::Number>().Int32Value(),
                    item.Get(static_cast<unsigned int>(1)).As<Napi::Number>().Int32Value());
                ops->page_list.push_back(std::move(pair));
            }
        }
    }

    return ops;
}
} // namespace node_pdfium
