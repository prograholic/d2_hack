#include "B3dReader.h"

#include <iostream>
#include <vector>
#include <stdexcept>

#include <boost/cstdint.hpp>
#include <boost/range.hpp>
#include <boost/range/iterator.hpp>


namespace detail
{
  template <typename RangeT>
  std::ostream& dump(std::ostream &outStream, RangeT range)
  {
    typedef typename boost::range_iterator<const RangeT>::type iterator_t;

    iterator_t begin = boost::begin(range);
    iterator_t end = boost::end(range);

    while (begin != end)
    {
      unsigned char symbol = *begin;
      if (std::isprint(symbol))
      {
        outStream << symbol;
      }
      else
      {
        outStream << "\\x";
        outStream << std::hex << std::setw(2) << std::setfill('0') << static_cast<unsigned int>(symbol);
      }

      ++begin;
    }

    return outStream;
  }
}



B3dReader::B3dReader(std::istream &input)
  : file_io::Reader(input)
{
}

void B3dReader::dump(std::ostream &outStream)
{
  detail::b3d_file b3d;
  read(b3d);

  outStream << "signature [" << detail::dump(outStream, b3d.signature) << "]" << std::endl;
}

void B3dReader::read(detail::b3d_file& b3d)
{
  readUntil(b3d.signature, file_io::helpers::ReadCount(4), false);
  readUntil(b3d.unknown_1, file_io::helpers::ReadCount(20), false);

  boost::uint8_t countBuff[sizeof(boost::uint32_t)];
  readUntil(countBuff, file_io::helpers::ReadCount(4), false);
  b3d.res_name_count = *reinterpret_cast<boost::uint32_t *>(countBuff);
  b3d.res_names.resize(b3d.res_name_count);

  for (boost::uint32_t resNo = 0; resNo != b3d.res_name_count; ++resNo)
  {
    readUntil(b3d.res_names[resNo].elems, file_io::helpers::ReadCount(32), false);
  }
}
