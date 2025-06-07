#include <string_view>
#include <iostream>
#include <format>

#include <boost/preprocessor/stringize.hpp>

#include <res_material_parser.h>

namespace d2_hack
{
namespace resource
{
namespace archive
{
namespace res
{

static const std::string_view samples[] = 
{
    "col 35 noz",
    "tex 14 env 0.07 0.07 notile coord 3",
    "col 1",
    "col 2",
    "col 3",
    "col 4",
    "col 5",
    "col 6",
    "col 7",
    "col 8",
    "col 9",
    "col 10",
    "col 10 transp 0.75",
    "col 11",
    "col 12",
    "col 13",
    "col 14",
    "col 15",
    "col 16",
    "col 17",
    "col 18",
    "col 19",
    "col 20",
    "col 21",
    "col 22",
    "col 23",
    "col 24",
    "col 25",
    "col 26",
    "col 27",
    "col 28",
    "col 29",
    "col 30",
    "col 31",
    "col 32",
    "col 33",
    "col 34",
    "col 36 noz",
    "col 37 noz",
    "col 38 noz",
    "col 39",
    "col 40",
    "tex 1 nof",
    "ttx 2 col 256",
    "ttx 3 col 256",
    "ttx 4 col 256",
    "ttx 5 col 256",
    "ttx 6 col 256",
    "ttx 7 col 256",
    "ttx 8 col 256",
    "ttx 9 col 256",
    "ttx 10 col 256",
    "ttx 11 col 256",
    "tex 12",
    "tex 13",
    "tex 15 coord 2 att 55",
    "tex 16 att 56 \"wave\"",
    "tex 17",
    "tex 18",
    "tex 19",
    "tex 20",
    "tex 21",
    "ttx 22",
    "ttx 23",
    "tex 24",
    "tex 25",
    "ttx 26",
    "tex 27",
    "tex 28",
    "tex 29",
    "tex 30",
    "tex 31",
    "tex 32",
    "tex 33",
    "tex 34",
    "tex 35",
    "tex 36 notile",
    "tex 37",
    "tex 38",
    "ttx 39",
    "tex 40",
    "tex 41",
    "tex 42",
    "tex 43",
    "tex 44 notile",
    "tex 45",
    "ttx 46",
    "tex 47",
    "tex 48",
    "tex 49",
    "tex 50",
    "ttx 51",
    "tex 52",
    "tex 53",
    "tex 54",
    "tex 55",
    "tex 56",
    "tex 57 notile",
    "tex 58",
    "tex 59",
    "tex 60",
    "tex 61",
    "tex 62",
    "tex 63",
    "tex 64",
    "tex 65",
    "tex 66",
    "ttx 67",
    "tex 68",
    "tex 69",
    "tex 70",
    "ttx 71",
    "ttx 72",
    "ttx 73",
    "tex 1",
    "tex 2",
    "tex 3",
    "tex 4",
    "tex 5",
    "tex 6",
    "tex 7",
    "tex 8",
    "tex 9",
    "tex 10",
    "tex 11",
    "tex 1 env 0.07 0.07 notile coord 2",
    "tex 2 col 5 usecol att 42",
    "tex 3 col 5 usecol att 42",
    "ttx 4",
    "ttx 5",
    "tex 14",
    "tex 15 notile",
    "tex 16",
    "tex 21 notile",
    "tex 22",
    "tex 23",
    "ttx 24",
    "tex 26",
    "tex 29 notile",
    "ttx 31",
    "ttx 36",
    "ttx 37 notile",
    "ttx 38 notile",
    "ttx 39 notile",
    "ttx 40 notile",
    "ttx 41 notile",
    "tex 44",
    "ttx 9 col 256 notile",
    "ttx 10 col 256 notile",
    "ttx 11 col 256 notile",
    "ttx 12 col 256 notile",
    "tex 13 env 0.07 0.07 notile coord 2",
    "tex 14 col 5 usecol att 54",
    "tex 15 col 5 usecol att 54",
    "tex 16 col 5 usecol att 54",
    "tex 17 col 5 usecol att 54",
    "ttx 21 col 256 notile",
    "ttx 22 col 256",
    "ttx 23 col 256 notile",
    "ttx 25",
    "ttx 27",
    "tex 30 notile",
    "tex 36",
    "tex 39",
    "ttx 45",
    "tex 46",
    "tex 51 notile",
    "ttx 52 notile",
    "ttx 53 notile",
    "tex 57",
    "ttx 67 notile",
    "ttx 68 notile",
    "ttx 1 col 256 notile",
    "ttx 2 col 256 notile",
    "ttx 3 col 256 notile",
    "ttx 4 col 256 notile",
    "ttx 5 col 256 notile",
    "tex 6 env 0.07 0.07 notile coord 2",
    "tex 7 col 5 usecol att 47",
    "tex 8 col 5 usecol att 47",
    "tex 9 col 5 usecol att 47",
    "tex 10 env 0.07 0.07 notile coord 2",
    "tex 11 att 51",
    "tex 15",
    "ttx 21 col 256",
    "ttx 23 col 256",
    "ttx 24 col 256",
    "tex 25 notile",
    "ttx 28",
    "tex 33 notile",
    "ttx 35",
    "ttx 40",
    "ttx 42 notile",
    "ttx 43 notile",
    "ttx 44 notile",
    "ttx 45 notile",
    "tex 5 env 0.07 0.07 notile coord 2",
    "tex 6 col 5 usecol att 46",
    "tex 7 col 5 usecol att 46",
    "tex 8 col 5 usecol att 46",
    "tex 9 env 0.07 0.07 notile coord 3",
    "tex 10 move 0 -0.05 coord 2 att 50",
    "tex 11 move 0 -0.125 att 51 \"wave\"",
    "tex 12 move 0 -0.125 att 51 \"wave\"",
    "ttx 20 col 256",
    "ttx 22 col 256 notile",
    "tex 35 notile",
    "ttx 38",
    "tex 43 notile",
    "ttx 50",
    "ttx 51 notile",
    "ttx 54 notile",
    "ttx 55 notile",
    "ttx 56 notile",
    "ttx 57 notile",
    "ttx 58 notile",
    "ttx 59 notile",
    "ttx 60 notile",
    "tex 9 env 0.07 0.07 notile coord 2",
    "tex 10 att 50",
    "ttx 19 col 256",
    "ttx 24 col 256 notile",
    "ttx 25 notile",
    "ttx 26 notile",
    "ttx 27 notile",
    "ttx 28 notile",
    "ttx 29 notile",
    "ttx 17 col 256",
    "ttx 18 col 256",
    "ttx 19 col 256 notile",
    "tex 23 notile",
    "tex 31 notile",
    "ttx 33",
    "ttx 13 col 256 notile",
    "ttx 14 col 256 notile",
    "ttx 15 col 256",
    "ttx 16 col 256 notile",
    "tex 17 env 0.07 0.07 notile coord 2",
    "tex 18 col 5 usecol att 58",
    "tex 19 col 5 usecol att 58",
    "tex 20 col 5 usecol att 58",
    "tex 21 col 5 usecol att 58",
    "tex 22 col 5 usecol att 58",
    "ttx 29",
    "ttx 34",
    "tex 51",
    "ttx 56",
    "tex 62 notile",
    "ttx 63 notile",
    "ttx 64 notile",
    "ttx 65 notile",
    "ttx 66 notile",
    "tex 71",
    "tex 72",
    "tex 73",
    "tex 74",
    "tex 75",
    "tex 76",
    "tex 77",
    "tex 78",
    "tex 79",
    "tex 80",
    "tex 81",
    "ttx 82 notile",
    "ttx 83 notile",
    "ttx 84 notile",
    "ttx 85 notile",
    "ttx 86 notile",
    "ttx 87 notile",
    "ttx 88 notile",
    "ttx 89 notile",
    "tex 90",
    "tex 91",
    "tex 4 env 0.07 0.07 notile coord 2",
    "tex 5 col 5 usecol att 45",
    "tex 6 coord 3",
    "tex 7 coord 2 att 47",
    "tex 8 move -0.125 0 att 48",
    "tex 9 coord 3",
    "tex 10 coord 2 att 50",
    "tex 8 move -0.125 0 att 51",
    "ttx 30 notile",
    "ttx 31 notile",
    "ttx 32 notile",
    "ttx 33 notile",
    "ttx 34 notile",
    "ttx 35 notile",
    "ttx 13",
    "ttx 14 col 256",
    "ttx 16 col 256",
    "ttx 18 col 256 notile",
    "tex 26 notile",
    "tex 34 notile",
    "ttx 41",
    "ttx 46 notile",
    "ttx 7 col 256 notile",
    "ttx 8 col 256 notile",
    "tex 11 col 5 usecol att 51",
    "tex 12 col 5 usecol att 51",
    "tex 13 col 5 usecol att 51",
    "ttx 18",
    "ttx 19",
    "ttx 20",
    "tex 24 notile",
    "tex 45 notile",
    "ttx 47 notile",
    "ttx 50 notile",
    "tex 67",
    "ttx 74",
    "ttx 76",
    "ttx 79",
    "ttx 80",
    "ttx 6 col 256 notile",
    "tex 16 env 0.07 0.07 notile coord 2",
    "tex 17 col 5 usecol att 57",
    "tex 18 col 5 usecol att 57",
    "ttx 57",
    "tex 11 env 0.07 0.07 notile coord 2",
    "tex 12 col 5 usecol att 52",
    "tex 13 col 5 usecol att 52",
    "tex 14 col 5 usecol att 52",
    "tex 15 env 0.1 0.1 notile coord 3",
    "tex 16 coord 2 att 56",
    "tex 17 att 57 \"wave\"",
    "tex 18 env 0.07 0.07 notile coord 3",
    "tex 19 move 0 -0.05 coord 2 att 59",
    "tex 20 move 0 -0.125 att 60 \"wave\"",
    "tex 21 move 0 -0.125 att 60 \"wave\"",
    "ttx 61 notile",
    "ttx 62 notile",
    "tex 8 env 0.07 0.07 notile coord 2",
    "tex 9 col 5 usecol att 49",
    "tex 10 col 5 usecol att 49",
    "tex 11 col 5 usecol att 49",
    "ttx 17 col 256 notile",
    "ttx 20 col 256 notile",
    "ttx 21",
    "ttx 30",
    "ttx 32",
    "tex 40 notile",
    "ttx 60",
    "tex 66 notile",
    "ttx 69 notile",
    "ttx 70 notile",
    "ttx 71 notile",
    "tex 82",
    "tex 84",
    "tex 85",
    "ttx 90 notile",
    "ttx 91 notile",
    "ttx 92 notile",
    "ttx 93 notile",
    "ttx 94 notile",
    "ttx 95 notile",
    "tex 96",
    "tex 97",
    "ttx 25 col 256 notile",
    "tex 37 notile",
    "ttx 44",
    "tex 46 notile",
    "tex 48 notile",
    "tex 49 notile",
    "tex 12 env 0.07 0.07 notile coord 2",
    "tex 13 col 5 usecol att 53",
    "tex 14 col 5 usecol att 53",
    "tex 18 coord 3",
    "tex 19 coord 2 att 59",
    "tex 20 move -0.125 0 att 60",
    "ttx 26 col 256 notile",
    "ttx 27 col 256",
    "ttx 28 col 256",
    "ttx 30 col 256",
    "ttx 37",
    "tex 42 notile",
    "ttx 49",
    "ttx 15 col 256 notile",
    "tex 18 env 0.07 0.07 notile coord 2",
    "tex 19 col 5 usecol att 59",
    "tex 20 col 5 usecol att 59",
    "tex 21 col 5 usecol att 59",
    "tex 22 col 5 usecol att 59",
    "ttx 43",
    "tex 54 notile",
    "tex 56 notile",
    "tex 58 notile",
    "tex 59 notile",
    "tex 80 notile",
    "ttx 81 notile",
    "tex 86",
    "tex 88",
    "tex 89",
    "tex 92",
    "tex 93",
    "tex 94",
    "tex 95",
    "tex 98",
    "ttx 99 notile",
    "ttx 100 notile",
    "ttx 101 notile",
    "ttx 102 notile",
    "ttx 103 notile",
    "ttx 104 notile",
    "ttx 105 notile",
    "ttx 106 notile",
    "tex 107",
    "tex 108",
    "ttx 109 notile",
    "ttx 110 notile",
    "ttx 111 notile",
    "ttx 112 notile",
    "tex 113",
    "tex 114",
    "tex 115",
    "tex 8 env 0.07 0.07 notile coord 3",
    "tex 9 move 0 -0.05 coord 2 att 49",
    "tex 10 move 0 -0.125 att 50 \"wave\"",
    "tex 11 move 0 -0.125 att 50 \"wave\"",
    "tex 6 col 5 usecol att 45",
    "tex 7 env 0.07 0.07 notile coord 3",
    "tex 8 coord 2 att 48",
    "tex 9 att 49 \"wave\"",
    "tex 10 coord 3",
    "tex 11 coord 2 att 51",
    "tex 12 move 0 -0.125 att 52",
    "ttx 14",
    "ttx 15",
    "ttx 20 notile",
    "ttx 21 notile",
    "tex 2 env 0.24 0.24 notile coord 3",
    "tex 3 coord 2 att 43",
    "tex 4 move -0.1 0.1 att 44 \"wave\"",
    "tex 18 notile",
    "ttx 32 col 256",
    "ttx 33 col 256",
    "ttx 34 col 256",
    "tex 35 col 5 usecol att 42",
    "tex 38 col 5 usecol att 42",
    "tex 12 notile",
    "tex 13 notile",
    "tex 14 notile",
    "tex 16 notile",
    "tex 39 notile",
    "ttx 48 notile",
    "ttx 49 notile",
    "ttx 51 col 256",
    "ttx 52 col 256",
    "tex 53 col 5 usecol att 42",
    "ttx 62 col 256",
    "ttx 64",
    "ttx 65",
    "tex 67 col 5 usecol att 42",
    "ttx 81",
    "ttx 82",
    "tex 83",
    "ttx 84",
    "ttx 87",
    "ttx 88",
    "ttx 35 col 256",
    "ttx 36 col 256",
    "ttx 42 col 4 move 0 -1.5",
    "ttx 43 col 4 move 0 -0.3",
    "ttx 43 col 4 move 0 -0.5",
    "ttx 46 col 256",
    "ttx 47",
    "ttx 48",
    "ttx 52",
    "ttx 54",
    "ttx 59",
    "ttx 62",
    "ttx 63",
    "tex 1 env 0.07 0.07 notile coord 3",
    "tex 2 coord 2 att 42",
    "tex 3 att 43 \"wave\"",
    "tex 4 notile",
    "tex 11 notile",
    "tex 19 notile",
    "tex 63 col 5 usecol att 46",
    "ttx 72 notile",
    "tex 73 col 5 usecol att 46",
    "tex 77 col 5 usecol att 46",
    "ttx 79 notile",
    "ttx 80 notile",
    "ttx 81 col 4 notile",
    "ttx 10 notile",
    "ttx 11 notile",
    "ttx 12 notile",
    "ttx 13 notile",
    "ttx 14 notile",
    "ttx 15 notile",
    "ttx 16 notile",
    "ttx 17 notile",
    "ttx 18 notile",
    "ttx 19 notile",
    "tex 22 col 5 usecol att 46",
    "tex 16 col 5 usecol att 46",
    "tex 10 notile",
    "tex 30 col 5 usecol att 46",
    "ttx 44 RotPoint 0.5 0.5 rot 100",
    "ttx 44 RotPoint 0.5 0.5 rot -500",
    "tex 1 notile",
    "tex 2 notile",
    "tex 3 notile",
    "tex 10 col 5 usecol att 50",
    "ttx 37 col 256",
    "tex 15 env 0.07 0.07 notile coord 3",
    "tex 16 move 0 -0.05 coord 2 att 56",
    "tex 17 move 0 -0.125 att 57 \"wave\"",
    "ttx 36 notile",
    "ttx 63 col 256",
    "ttx 66 col 256",
    "tex 67 notile",
    "tex 68 notile",
    "tex 69 notile",
    "tex 70 notile",
    "tex 72 notile",
    "ttx 76 col 256",
    "tex 82 notile",
    "ttx 85 col 256",
    "ttx 86 col 256",
    "ttx 87 col 256",
    "ttx 22 notile",
    "ttx 25 col 256",
    "ttx 26 col 256",
    "ttx 53",
    "tex 10 env 0.07 0.07 notile coord 3",
    "tex 11 move 0 -0.05 coord 2 att 51",
    "tex 12 move 0 -0.125 att 52 \"wave\"",
    "tex 13 move 0 -0.125 att 52 \"wave\"",
    "ttx 23 notile",
    "ttx 24 notile",
    "ttx 29 col 256",
    "ttx 61 col 256",
    "ttx 13 col 256",
    "tex 12 move 0 -0.083 att 52 \"wave\"",
    "tex 12 move 0 -0.042 att 52 \"wave\"",
    "tex 12 att 52 \"wave\"",
    "tex 13 move 0 -0.07 att 52 \"wave\"",
    "tex 13 move 0 -0.04 att 52 \"wave\"",
    "tex 13 move 0 -0.02 att 52 \"wave\"",
    "tex 13 att 52 \"wave\"",
    "tex 14 move 0.02 0 att 52 \"wave\"",
    "tex 15 move 0 -2",
    "tex 15 move 0 -0.5",
    "tex 12 move 0 -0.5 att 52 \"wave\"",
    "ttx 3 col 245 notile",
    "ttx 14 col 245 notile",
    "ttx 15 col 245 notile",
    "ttx 16 col 245 notile",
    "tex 17 notile",
    "ttx 20 col 245 notile",
    "tex 32 notile",
    "ttx 58",
    "tex 64 notile",
    "ttx 73 notile",
    "ttx 74 notile",
    "ttx 75 notile",
    "ttx 76 notile",
    "tex 87",
    "ttx 94",
    "tex 7 env 0.07 0.07 notile coord 2",
    "tex 8 col 5 usecol att 48",
    "ttx 12 col 256",
    "ttx 16",
    "tex 2 env 0.07 0.07 notile coord 3",
    "tex 4 att 44 \"wave\"",
    "ttx 27 col 256 notile",
    "tex 41 notile",
    "tex 47 notile"    
};


#define CHECK_FIELD(md, field, sample)                                                                        \
do                                                                                                            \
{                                                                                                             \
    if (!md.field)                                                                                            \
    {                                                                                                         \
        throw std::runtime_error(std::format("Sample {}, " BOOST_PP_STRINGIZE(field) " is not set", sample)); \
    }                                                                                                         \
} while(0)                                                                                                    \



void CheckMaterialParserForSample(const std::string_view& sample)
{
    MaterialDescriptor md = ParseMaterialDescriptor(sample);

    if (sample.substr(0, 3) == "tex")
    {
        if (md.type != MaterialType::tex)
        {
            throw std::runtime_error(std::format("Sample {}, type: {}, expected {}", sample, static_cast<int>(md.type), static_cast<int>(MaterialType::tex)));
        }
    }
    else if (sample.substr(0, 3) == "ttx")
    {
        if (md.type != MaterialType::ttx)
        {
            throw std::runtime_error(std::format("Sample {}, type: {}, expected {}", sample, static_cast<int>(md.type), static_cast<int>(MaterialType::ttx)));
        }
    }
    else if (sample.substr(0, 3) == "itx")
    {
        if (md.type != MaterialType::itx)
        {
            throw std::runtime_error(std::format("Sample {}, type: {}, expected {}", sample, static_cast<int>(md.type), static_cast<int>(MaterialType::itx)));
        }
    }
    else if (sample.substr(0, 3) == "col")
    {
        if (md.type != MaterialType::col)
        {
            throw std::runtime_error(std::format("Sample {}, type: {}, expected {}", sample, static_cast<int>(md.type), static_cast<int>(MaterialType::col)));
        }
    }

    if (sample.find("noz") != sample.npos)
    {
        CHECK_FIELD(md, noz, sample);
    }
    if (sample.find("transp") != sample.npos)
    {
        CHECK_FIELD(md, transparency, sample);
    }
    if (sample.find("notile") != sample.npos)
    {
        CHECK_FIELD(md, notile, sample);
    }
    if (sample.find("move") != sample.npos)
    {
        CHECK_FIELD(md, move, sample);
    }
    if (sample.find("nof") != sample.npos)
    {
        CHECK_FIELD(md, nof, sample);
    }
    if (sample.find("env") != sample.npos)
    {
        CHECK_FIELD(md, env, sample);
    }
    if (sample.find("coord") != sample.npos)
    {
        CHECK_FIELD(md, coord, sample);
    }
    if (sample.find("att") != sample.npos)
    {
        CHECK_FIELD(md, att, sample);
    }
    if (sample.find("wave") != sample.npos)
    {
        CHECK_FIELD(md, wave, sample);
    }
    if (sample.find("usecol") != sample.npos)
    {
        CHECK_FIELD(md, usecol, sample);
    }
    if (sample.find("RotPoint") != sample.npos)
    {
        CHECK_FIELD(md, rotPoint, sample);
    }
    if (sample.find("rot") != sample.npos)
    {
        CHECK_FIELD(md, rot, sample);
    }
    if (sample.find("specular") != sample.npos)
    {
        CHECK_FIELD(md, specular, sample);
    }
    if (sample.find("power") != sample.npos)
    {
        CHECK_FIELD(md, power, sample);
    }
    if (sample.find("par") != sample.npos)
    {
        CHECK_FIELD(md, par, sample);
    }
    const auto colPos = sample.find("col");
    if (colPos != sample.npos)
    {
        if (colPos > 0)
        {
            CHECK_FIELD(md, col, sample);
        }
    }
}

void CheckMaterialParser()
{
    for (const auto& sample : samples)
    {
        CheckMaterialParserForSample(sample);
    }
}



} // namespace res
} // namespace archive
} // namespace resource
} // namespace d2_hack  

int main()
{
    try
    {
        using namespace d2_hack::resource::archive::res;

        CheckMaterialParser();
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}
