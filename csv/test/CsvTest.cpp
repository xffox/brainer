#include <sstream>
#include <cstddef>
#include <vector>
#include <cppunit/TestCase.h>
#include <cppunit/extensions/HelperMacros.h>

#include "csv/csv.h"

namespace csv
{
    namespace test
    {
        class CsvTest: public CppUnit::TestCase
        {
            CPPUNIT_TEST_SUITE(CsvTest);
            CPPUNIT_TEST(testEmpty);
            CPPUNIT_TEST(testSingleLineOneValue);
            CPPUNIT_TEST(testSingleLineMultipleValues);
            CPPUNIT_TEST(testMultipleLinesSingleValue);
            CPPUNIT_TEST(testMultipleLinesMultipleValues);
            CPPUNIT_TEST(testDelimEscape);
            CPPUNIT_TEST(testSingleQuote);
            CPPUNIT_TEST(testMultipleQuote);
            CPPUNIT_TEST(testQuotedEol);
            CPPUNIT_TEST(testSimpleWithoutEsc);
            CPPUNIT_TEST(testQuotedWithoutEsc);
            CPPUNIT_TEST(testQuotedBracesWithoutEsc);
            CPPUNIT_TEST_SUITE_END();
        public:
            void testEmpty()
            {
                std::stringstream ss("");
                Csv<char> csv(ss);
                const auto res = csv.row();
                CPPUNIT_ASSERT(!res.second);
            }

            void testSingleLineOneValue()
            {
                const std::string val("value");
                std::stringstream ss(val);
                Csv<char> csv(ss);
                auto res = csv.row();
                CPPUNIT_ASSERT(res.second);
                CPPUNIT_ASSERT_EQUAL(static_cast<std::size_t>(1),
                    res.first.size());
                CPPUNIT_ASSERT_EQUAL(val, res.first[0]);
                res = csv.row();
                CPPUNIT_ASSERT(!res.second);
            }

            void testSingleLineMultipleValues()
            {
                std::stringstream ss("first,second,third");
                Csv<char> csv(ss);
                auto res = csv.row();
                CPPUNIT_ASSERT(res.second);
                CPPUNIT_ASSERT_EQUAL(static_cast<std::size_t>(3),
                    res.first.size());
                CPPUNIT_ASSERT_EQUAL(std::string("first"), res.first[0]);
                CPPUNIT_ASSERT_EQUAL(std::string("second"), res.first[1]);
                CPPUNIT_ASSERT_EQUAL(std::string("third"), res.first[2]);
                res = csv.row();
                CPPUNIT_ASSERT(!res.second);
            }

            void testMultipleLinesSingleValue()
            {
                std::stringstream ss("first\nsecond\nthird");
                const std::vector<std::string> VALS{"first", "second", "third"};
                Csv<char> csv(ss);
                for(const auto &s : VALS)
                {
                    auto res = csv.row();
                    CPPUNIT_ASSERT(res.second);
                    CPPUNIT_ASSERT_EQUAL(static_cast<std::size_t>(1),
                        res.first.size());
                    CPPUNIT_ASSERT_EQUAL(s, res.first[0]);
                }
                const auto res = csv.row();
                CPPUNIT_ASSERT(!res.second);
            }

            void testMultipleLinesMultipleValues()
            {
                std::stringstream ss("first,morefirst\nsecond,moresecond\nthird,morethird");
                const std::vector<std::vector<std::string>> VALS{
                    {"first", "morefirst"}, {"second", "moresecond"}, {"third", "morethird"}};
                Csv<char> csv(ss);
                for(const auto &s : VALS)
                {
                    auto res = csv.row();
                    CPPUNIT_ASSERT(res.second);
                    CPPUNIT_ASSERT(s == res.first);
                }
                const auto res = csv.row();
                CPPUNIT_ASSERT(!res.second);
            }

            void testDelimEscape()
            {
                const std::string val("value\\,morevalue");
                std::stringstream ss(val);
                Csv<char> csv(ss, ',', '"', '\\');
                auto res = csv.row();
                CPPUNIT_ASSERT(res.second);
                CPPUNIT_ASSERT_EQUAL(static_cast<std::size_t>(1),
                    res.first.size());
                CPPUNIT_ASSERT_EQUAL(std::string("value,morevalue"), res.first[0]);
                res = csv.row();
                CPPUNIT_ASSERT(!res.second);
            }

            void testSingleQuote()
            {
                const std::string val("row1,row2,\"value,morevalue\"");
                std::stringstream ss(val);
                Csv<char> csv(ss);
                auto res = csv.row();
                CPPUNIT_ASSERT(res.second);
                CPPUNIT_ASSERT_EQUAL(static_cast<std::size_t>(3),
                    res.first.size());
                CPPUNIT_ASSERT_EQUAL(std::string("value,morevalue"), res.first[2]);
                res = csv.row();
                CPPUNIT_ASSERT(!res.second);
            }

            void testMultipleQuote()
            {
                const std::string val("\"first,morefirst\",second,\"third,morethird\"");
                std::stringstream ss(val);
                Csv<char> csv(ss);
                auto res = csv.row();
                CPPUNIT_ASSERT(res.second);
                std::vector<std::string> EXP{"first,morefirst", "second", "third,morethird"};
                CPPUNIT_ASSERT(EXP == res.first);
                res = csv.row();
                CPPUNIT_ASSERT(!res.second);
            }

            void testQuotedEol()
            {
                const std::string val(
                    "first,\"second\nthird\"\n"
                    "\"fourth\nfifth\",six");
                const std::vector<std::string> EXP1{
                    "first", "second\nthird"};
                const std::vector<std::string> EXP2{
                    "fourth\nfifth", "six"};
                std::stringstream ss(val);
                Csv<char> csv(ss);
                auto res = csv.row();
                CPPUNIT_ASSERT(res.second);
                CPPUNIT_ASSERT(EXP1 == res.first);
                res = csv.row();
                CPPUNIT_ASSERT(res.second);
                CPPUNIT_ASSERT(EXP2 == res.first);
                res = csv.row();
                CPPUNIT_ASSERT(!res.second);
            }

            void testSimpleWithoutEsc()
            {
                const std::string val(
                    "first,second");
                const std::vector<std::string> EXP{
                    "first", "second"};
                std::stringstream ss(val);
                Csv<char> csv(ss, ',', '"', '\0');
                auto res = csv.row();
                CPPUNIT_ASSERT(res.second);
                CPPUNIT_ASSERT(EXP == res.first);
                res = csv.row();
                CPPUNIT_ASSERT(!res.second);
            }

            void testQuotedWithoutEsc()
            {
                const std::string val(
                    "first,\"second third\"");
                const std::vector<std::string> EXP{
                    "first", "second third"};
                std::stringstream ss(val);
                Csv<char> csv(ss, ',', '"', '\0');
                auto res = csv.row();
                CPPUNIT_ASSERT(res.second);
                CPPUNIT_ASSERT(EXP == res.first);
                res = csv.row();
                CPPUNIT_ASSERT(!res.second);
            }

            void testQuotedBracesWithoutEsc()
            {
                const std::string val(
                    "first,\"second\"\"third\"\"\"");
                const std::vector<std::string> EXP{
                    "first", "second\"third\""};
                std::stringstream ss(val);
                Csv<char> csv(ss, ',', '"', '\0');
                auto res = csv.row();
                CPPUNIT_ASSERT(res.second);
                CPPUNIT_ASSERT(EXP == res.first);
                res = csv.row();
                CPPUNIT_ASSERT(!res.second);
            }
        };
        CPPUNIT_TEST_SUITE_REGISTRATION(CsvTest);
    }
}
