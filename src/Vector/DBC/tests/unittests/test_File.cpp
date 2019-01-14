#define BOOST_TEST_MODULE File
#if !defined(WIN32)
#define BOOST_TEST_DYN_LINK
#endif
#include <boost/test/unit_test.hpp>

#include <fstream>
#include <string>
#include <boost/filesystem.hpp>

#include <Vector/DBC.h>

BOOST_AUTO_TEST_CASE(File)
{
    Vector::DBC::Network network;

    /* load database file */
    boost::filesystem::path infile(CMAKE_CURRENT_SOURCE_DIR "/data/Database.dbc");
    std::ifstream ifs(infile.string());
    BOOST_REQUIRE(ifs.is_open());
    ifs >> network;
    BOOST_REQUIRE(network.successfullyParsed);
    ifs.close();

    /* create output directory */
    boost::filesystem::path outdir(CMAKE_CURRENT_BINARY_DIR "/data/");
    if (!exists(outdir)) {
        BOOST_REQUIRE(create_directory(outdir));
    }

    /* save database file */
    boost::filesystem::path outfile(CMAKE_CURRENT_BINARY_DIR "/data/Database.dbc");
    std::ofstream ofs(outfile.string());
    BOOST_REQUIRE(ofs.is_open());
    ofs << network;
    ofs.close();

    /* loaded and saved file should be equivalent */
    std::ifstream ifs1(infile.c_str());
    std::ifstream ifs2(outfile.c_str());
    std::istream_iterator<char> b1(ifs1), e1;
    std::istream_iterator<char> b2(ifs2), e2;
    BOOST_CHECK_EQUAL_COLLECTIONS(b1, e1, b2, e2);
}
