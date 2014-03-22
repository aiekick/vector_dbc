#define BOOST_TEST_MODULE Database
#if !defined(WIN32)
#define BOOST_TEST_DYN_LINK
#endif
#include <boost/test/unit_test.hpp>

#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <boost/filesystem.hpp>

#include "Vector/DBC/Database.h"

void progressCallback(float numerator, float denominator)
{
    std::cout << "Progress: filepos=" << numerator << " percent="<< std::fixed << 100 * (numerator / denominator) << '%' << std::endl;
}

#define SUCCEEDED(code) ((int)(code) >= 0)
#define WARNED(code) ((int)(code) & 0x40000000)
#define FAILED(code) ((int)(code) < 0)

void statusCallback(Vector::DBC::Status status)
{
    if (FAILED(status)) {
        std::cerr << "Error: 0x" << std::hex << (int) status << std::endl;
    } else
    if (WARNED(status)) {
        std::cout << "Warning: 0x" << std::hex << (int) status << std::endl;
    } else
    if (SUCCEEDED(status)) {
        std::cout << "Success: 0x" << std::hex << (int) status << std::endl;
    }
}

BOOST_AUTO_TEST_CASE(Database)
{
    Vector::DBC::Database database;
    database.setProgressCallback(&progressCallback);
    database.setStatusCallback(&statusCallback);

    /* load database */
    boost::filesystem::path infile(CMAKE_CURRENT_SOURCE_DIR "/data/Database.dbc");
    std::string infilename = infile.string();
    BOOST_REQUIRE(database.load(infilename) == Vector::DBC::Status::Ok);

    /* create output directory */
    boost::filesystem::path outdir(CMAKE_CURRENT_BINARY_DIR "/data/");
    if (!exists(outdir)) {
        BOOST_REQUIRE(create_directory(outdir));
    }

    /* save database */
    boost::filesystem::path outfile(CMAKE_CURRENT_BINARY_DIR "/data/Database.dbc");
    std::string outfilename = outfile.string();
    BOOST_REQUIRE(database.save(outfilename) == Vector::DBC::Status::Ok);

    /* loaded and saved file should be equivalent */
    std::ifstream ifs1(infile.c_str());
    std::ifstream ifs2(outfile.c_str());
    std::istream_iterator<char> b1(ifs1), e1;
    std::istream_iterator<char> b2(ifs2), e2;
    BOOST_CHECK_EQUAL_COLLECTIONS(b1, e1, b2, e2);
}
