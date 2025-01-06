#ifndef D2_HACK_COMMON_INCLUDE_D2_HACK_COMMON_UTILS_H
#define D2_HACK_COMMON_INCLUDE_D2_HACK_COMMON_UTILS_H

#include <d2_hack/common/types.h>

namespace d2_hack
{
namespace common
{

std::string ResourceNameToString(const ResourceName& resName);

ResourceName StringToResourceName(const std::string& name);


bool IsValid(const BoundingSphere& boundingSphere);

template <typename T>
struct Hash;

template <>
struct Hash<common::BoundingSphere>
{
    size_t operator()(const common::BoundingSphere& data) const noexcept;
};

bool operator==(const BoundingSphere& left, const BoundingSphere& right);



template <>
struct Hash<Ogre::Vector3>
{
    size_t operator()(const Ogre::Vector3& data) const noexcept;
};

template <typename IntType>
IntType& GetDataFrom2D(IntType* data, std::size_t borderSize, std::size_t row, std::size_t col)
{
    return data[(row * borderSize) + col];
}

template <typename IntType>
void TransposeInplace(IntType* data, std::size_t borderSize)
{
    for (std::size_t row = 0; row != borderSize; ++row)
    {
        for (std::size_t col = row; col != borderSize; ++col)
        {
            using std::swap;
            swap(GetDataFrom2D(data, borderSize, row, col), GetDataFrom2D(data, borderSize, col, row));
        }
    }
}

template <typename IntType>
void ReverseRowInplace(IntType* data, std::size_t borderSize)
{
    for (std::size_t col = 0; col != borderSize; ++col)
    {
        for (std::size_t row = 0; row != (borderSize / 2); ++row)
        {
            using std::swap;
            swap(GetDataFrom2D(data, borderSize, row, col), GetDataFrom2D(data, borderSize, borderSize - row - 1, col));
        }
    }
}

template <typename IntType>
void ReverseColumnInplace(IntType* data, std::size_t borderSize)
{
    for (std::size_t row = 0; row != borderSize; ++row)
    {
        for (std::size_t col = 0; col != (borderSize / 2); ++col)
        {
            using std::swap;
            swap(GetDataFrom2D(data, borderSize, row, col), GetDataFrom2D(data, borderSize, row, borderSize - col - 1));
        }
    }
}

/**
 * @brief Rotate image data inplace counter-clockwise
 * 
 * @note data must have width == height
 */
template <typename IntType>
void RotateImageDataCounterClockWise(IntType* imageData, std::size_t borderSize)
{
    TransposeInplace(imageData, borderSize);
    ReverseColumnInplace(imageData, borderSize);
}

/**
 * @brief Rotate image data inplace clockwise
 *
 * @note data must have width == height
 */
template <typename IntType>
void RotateImageDataClockWise(IntType* imageData, std::size_t borderSize)
{
    TransposeInplace(imageData, borderSize);
    ReverseRowInplace(imageData, borderSize);
}


} // namespace common
} // namespace d2_hack






#endif /* D2_HACK_COMMON_INCLUDE_D2_HACK_COMMON_UTILS_H */
