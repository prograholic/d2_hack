#include <d2_hack/common/utils.h>


namespace d2_hack
{
namespace common
{

std::string ResourceNameToString(const common::ResourceName& resName)
{
    const char* data = reinterpret_cast<const char*>(resName.data());
    return std::string(data, data + strnlen(data, resName.size()));
}

} // namespace common
} // namespace d2_hack
