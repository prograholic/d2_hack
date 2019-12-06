#ifndef D2_HACK_COMMON_D2_HACK_COMMON_H
#define D2_HACK_COMMON_D2_HACK_COMMON_H

#include <iterator>
#include <vector>
#include <iomanip>
#include <cstdint>

#include <OgreException.h>
#include <OgreString.h>
#include <OgreLogManager.h>


#ifndef OGRE_LOG
#define OGRE_LOG(x) Ogre::LogManager::getSingleton().stream() << #x << ": "
#endif /* OGRE_LOG */




namespace file_io
{
    typedef std::istreambuf_iterator<char> data_iterator;
    typedef std::iterator_traits<data_iterator> data_iter_traits;

    typedef std::vector<std::uint8_t> blob_t;


    class SeparatorBase
    {
    public:
        SeparatorBase() = default;
        virtual ~SeparatorBase() = default;

        virtual bool IsSeparator(char symbol) = 0;

        virtual bool Skip() = 0;
        
    private:
        SeparatorBase(const SeparatorBase&) = delete;
        SeparatorBase& operator=(const SeparatorBase& ) = delete;
    };

    class Reader
    {
    public:
        explicit Reader(std::istream& input);

    protected:

        template <typename OutputIteratorT>
        size_t ReadUntil(OutputIteratorT res, SeparatorBase& separator)
        {
            size_t count = 0;
            for ( ; ; )
            {
                if (m_begin == m_end)
                {
                    ThrowError("unexpected end of file", "Reader::readUntil");
                }
                const char data = *m_begin;
                if (separator.IsSeparator(data))
                {
                    if (separator.Skip())
                    {
                        ++m_begin;
                        ++m_offset;
                        ++count;
                    }
                    break;
                }

                *res = data;
                ++res;
                ++m_begin;
                ++m_offset;
                ++count;
            }

            return count;
        }

        void ThrowError(const Ogre::String & msg, const Ogre::String & where);

        size_t m_offset;
        data_iterator m_begin;
        data_iterator m_end;
    };

    namespace helpers
    {
        template <typename InnerSeparatorT, bool skipSeparator>
        class SkipAdapter : public SeparatorBase
        {
        public:
            virtual bool Skip()
            {
                return skipSeparator;
            }

            virtual bool IsSeparator(char symbol)
            {
                return static_cast<InnerSeparatorT *>(this)->IsSeparator(symbol);
            }
        };

        template <char sepSymbol, bool skip>
        class SymbolSeparatorBase : public SkipAdapter<SymbolSeparatorBase<sepSymbol, skip>, skip>
        {
        public:
            bool IsSeparator(char symbol)
            {
                return symbol == sepSymbol;
            }
        };

        class ReadCount : public SkipAdapter<ReadCount, false>
        {
        public:
            explicit ReadCount(size_t count);

            bool IsSeparator(char symbol);

        private:
            size_t m_count;
        };


        template <typename T>
        struct empty_container
        {
            typedef const T& const_reference;
            typedef T value_type;

            void push_back(const T& /* unused */)
            {

            }
        };


        namespace dump_type
        {
            enum Value
            {
                String,
                Data
            };
        }

        template <typename RangeT>
        std::ostream& Dump(std::ostream &outStream, const RangeT& range, dump_type::Value dumpType, size_t* size = 0)
        {
            auto begin = std::begin(range);
            auto end = std::end(range);

            size_t tmpSize = 0;

            while (begin != end)
            {
                unsigned char symbol = *begin;
                if (dumpType == dump_type::String)
                {
                    if (std::isprint(symbol))
                    {
                        outStream << symbol;
                    }
                    else
                    {
                        if (symbol != 0)
                        {
                            outStream << "\\x";
                            outStream << std::hex << std::setw(2) << std::setfill('0') << (static_cast<unsigned int>(symbol) & 0xFF);
                        }
                    }
                }
                else
                {
                    outStream << std::hex << std::setw(2) << std::setfill('0') << (static_cast<unsigned int>(symbol) & 0xFF);
                }

                ++begin;
                ++tmpSize;
            }

            if (size)
            {
                *size = tmpSize;
            }

            return outStream;
        }
    }
}


#endif /* D2_HACK_COMMON_D2_HACK_COMMON_H */
