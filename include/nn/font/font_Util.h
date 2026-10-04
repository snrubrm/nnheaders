#pragma once

namespace nn::font::detail {

class RuntimeTypeInfo {
public:
    const RuntimeTypeInfo* m_ParentTypeInfo;

    explicit RuntimeTypeInfo(const RuntimeTypeInfo* parent) : m_ParentTypeInfo(parent) {}

    // No out-of-line copy exists in the binary: the walk is inlined at the call sites.
    bool IsDerivedFrom(const RuntimeTypeInfo* other) const {
        for (const RuntimeTypeInfo* info = this; info; info = info->m_ParentTypeInfo) {
            if (info == other)
                return true;
        }
        return false;
    }
};

}  // namespace nn::font::detail

// todo: figure out where to put this
#define NN_RUNTIME_TYPEINFO_BASE()                                                                 \
    static const nn::font::detail::RuntimeTypeInfo* GetRuntimeTypeInfoStatic() {                   \
        static const nn::font::detail::RuntimeTypeInfo s_TypeInfo(nullptr);                        \
        return &s_TypeInfo;                                                                        \
    }                                                                                              \
                                                                                                   \
    virtual const nn::font::detail::RuntimeTypeInfo* GetRuntimeTypeInfo() const {                  \
        return GetRuntimeTypeInfoStatic();                                                         \
    }

#define NN_RUNTIME_TYPEINFO(BASE)                                                                  \
    static const nn::font::detail::RuntimeTypeInfo* GetRuntimeTypeInfoStatic() {                   \
        static const nn::font::detail::RuntimeTypeInfo s_TypeInfo(                                 \
            BASE::GetRuntimeTypeInfoStatic());                                                     \
        return &s_TypeInfo;                                                                        \
    }                                                                                              \
                                                                                                   \
    virtual const nn::font::detail::RuntimeTypeInfo* GetRuntimeTypeInfo() const {                  \
        return GetRuntimeTypeInfoStatic();                                                         \
    }

namespace nn::font {

// The runtime type check the library's callers inline (the target type's static info is evaluated before the
// null test; Layout::SetTagProcessor's helper 0x7100ab79a8 does it for TextBox).
template <typename T, typename U>
T* DynamicCast(U* object) {
    const detail::RuntimeTypeInfo* type = T::GetRuntimeTypeInfoStatic();
    if (object && object->GetRuntimeTypeInfo()->IsDerivedFrom(type))
        return static_cast<T*>(object);
    return nullptr;
}

}  // namespace nn::font
