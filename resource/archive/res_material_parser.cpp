#include "res_material_parser.h"

#include <format>


#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>

#include <OgreException.h>


namespace d2_hack
{
namespace resource
{
namespace archive
{
namespace res
{

namespace qi = boost::spirit::qi;
namespace ascii = boost::spirit::ascii;

MaterialDescriptor ParseMaterialDescriptor(const std::string_view& content)
{
    MaterialDescriptor res;

    auto first = content.begin();
    auto last = content.end();

    typedef std::string_view::const_iterator iterator_type;

    auto setType = [&res](auto param) {res.type = param; };
    auto setIndex = [&res](auto param) {res.index = param; };
    auto setNoz = [&res]() {res.noz = true; };
    auto setTransp = [&res](auto param) {res.transparency = param; };
    auto setNotile = [&res]() {res.notile = true; };
    auto setMove = [&res](auto param) {res.move = param; };
    auto setNof = [&res]() {res.nof = true; };
    auto setEnv = [&res](auto param) {res.env = param; };
    auto setCoord = [&res](auto param) {res.coord = param; };
    auto setAtt = [&res](auto param) {res.att = param; };
    auto setWave = [&res]() {res.wave = true; };
    auto setUsecol = [&res]() {res.usecol = true; };
    auto setRotPoint = [&res](auto param) {res.rotPoint = param; };
    auto setRot = [&res](auto param) {res.rot = param; };
    auto setSpecular = [&res](auto param) {res.specular = param; };
    auto setPower = [&res](auto param) {res.power = param; };
    auto setPar = [&res](auto param) {res.par = param; };
    auto setCol = [&res](auto param) {res.col = param; };

    using qi::int_;
    using qi::float_;
    using qi::lit;
    using qi::_val;
    using qi::char_;

    qi::rule<iterator_type, Ogre::Vector2(), ascii::space_type> ogreVector2Grammar;
    ogreVector2Grammar %= float_[qi::_val[0] = qi::_1] >> float_[qi::_val[1] = qi::_1];

    qi::rule<iterator_type, ascii::space_type> data;
    data %=
        (
            lit("noz")[setNoz]
            || lit("transp") >> float_[setTransp]
            || lit("notile")[setNotile]
            || lit("move") >> ogreVector2Grammar[setMove]
            || lit("nof")[setNof]
            || lit("env") >> ogreVector2Grammar[setEnv]
            || lit("coord") >> int_[setCoord]
            || lit("att") >> int_[setAtt]
            || lit("\"wave\"")[setWave]
            || lit("usecol")[setUsecol]
            || lit("RotPoint") >> ogreVector2Grammar[setRotPoint]
            || lit("rot") >> float_[setRot]
            || lit("specular") >> float_[setSpecular]
            || lit("power") >> int_[setPower]
            || lit("par") >> int_[setPar]
            || lit("col") >> int_[setCol]
            ) >> *data
        ;

    qi::symbols<char, MaterialType> materialTypeSymbols;
    materialTypeSymbols.add
    ("col", MaterialType::col)
        ("tex", MaterialType::tex)
        ("ttx", MaterialType::ttx)
        ("itx", MaterialType::itx);

    qi::rule<iterator_type, ascii::space_type> start;

    start %=
        +qi::char_("a-zA-Z_0-9") >> materialTypeSymbols[setType] >> int_[setIndex] >> *data;

    
    bool isOk = phrase_parse(first, last, start, ascii::space);

    if (!isOk || (first != last))
    {
        OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS, std::format("Invalid content: {}, failed pos: {}", content, std::distance(first, content.begin())));
    }


    return res;
}

} // namespace res
} // namespace archive
} // namespace resource
} // namespace d2_hack  
