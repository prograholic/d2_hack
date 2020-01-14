#include <d2_hack/common/resource_mgmt.h>


namespace d2_hack
{
namespace common
{

std::string GetResourceName(const std::string& fileBaseName, const std::string& resourceName)
{
    return fileBaseName + "_" + resourceName;
}

} // namespace common
} // namespace d2_hack
