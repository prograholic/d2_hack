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

#define B3D_TRACING

class B3dReader : public file_io::Reader
{
public:
  B3dReader(std::istream& input, std::ostream& output);

  void read();

private:

  std::ostream& m_output;

  void read(detail::b3d_file& b3d
#if defined(B3D_TRACING)
            , std::ostream& outStream
#endif /* B3D_TRACING */
            );

  template <typename TypeT>
  void dump(std::ostream& outStream,
            const std::string& field,
            const TypeT& data,
            size_t offset,
            file_io::helpers::dump_type::Value dumpType);
};

#endif /* D2_HACK_TOOLS_B3D_READER_B3D_READER_HEADER */
