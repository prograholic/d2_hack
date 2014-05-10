#ifndef D2_HACK_TOOLS_B3D_READER_B3D_READER_HEADER
#define D2_HACK_TOOLS_B3D_READER_B3D_READER_HEADER

#include "D2HackCommon.h"

#include <boost/array.hpp>

namespace detail
{
  typedef boost::array<char, 32> res_name_t;

  struct b3d_file
  {
    char signature[4];
    unsigned char unknown_1[20];
    boost::uint32_t res_name_count;
    std::vector<res_name_t> res_names;
    unsigned char unknown_2[100];

    /// Unknown stuff
  };
}

class B3dReader : public file_io::Reader
{
public:
  explicit B3dReader(std::istream& input);

  void dump(std::ostream& outStream);

private:

  void read(detail::b3d_file& b3d);
};

#endif /* D2_HACK_TOOLS_B3D_READER_B3D_READER_HEADER */
