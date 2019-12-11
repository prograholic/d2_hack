#include <iostream>
#include <fstream>
#include <string>

#include <d2_hack/codec/data/b3d.h>
#include <d2_hack/codec/data/b3d_reader.h>



class TracingVisitor : public d2_hack::codec::data::B3dVisitorInterface
{
public:

    virtual void VisitBlockSeparator(std::uint32_t blockSeparator) override
    {
        if (blockSeparator == 333)
        {
            std::cout << GetOffsetString() << "{" << std::endl;
        }
        else if (blockSeparator == 555)
        {
            std::cout << GetOffsetString() << "}" << std::endl;
        }
    }

    virtual void VisitBlockHeader(const d2_hack::common::ResourceName& name, std::uint32_t type) override
    {
        m_offset += 1;
        std::cout << GetOffsetString() << "name: " << name.data() << std::endl;
        std::cout << GetOffsetString() << "type: " << type << std::endl;
    }

    virtual void VisitBlock(const d2_hack::common::ResourceName& /* name */, std::uint32_t /* type */) override
    {
        m_offset -= 1;
    }

    virtual void VisitNestedBlockHeader(std::uint32_t nestedBlockCount) override
    {
        std::cout << GetOffsetString() << "nested: " << nestedBlockCount << std::endl;
    }

    virtual std::ostream* GetDebugOutStream()  override
    {
        return &std::cout;
    }

private:
    size_t m_offset = 0;

    std::string GetOffsetString() const
    {
        std::string res;
        res.resize(m_offset * 4, ' ');

        return res;
    }
};



int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        std::cerr << "usage: " << argv[0] << " <input_file>" << std::endl;
        return 1;
    }

    std::string fileName = argv[1];

    std::ifstream inputFile(fileName.c_str(), std::ios_base::binary);
    if (!inputFile)
    {
        std::cerr << "failed to open file [" << fileName << "]" << std::endl;
        return 2;
    }

    TracingVisitor visitor;
    d2_hack::codec::data::b3d::B3dReader reader;
    d2_hack::codec::data::b3d::B3d data;
    reader.Read(inputFile, data, &visitor);

    return 0;
}
