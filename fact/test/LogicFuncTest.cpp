#include <cppunit/TestCase.h>
#include <cppunit/extensions/HelperMacros.h>

#include "fact/LogicFunc.h"

namespace fact
{
    namespace test
    {
        class LogicFuncTest: public CppUnit::TestCase
        {
            CPPUNIT_TEST_SUITE(LogicFuncTest);
            CPPUNIT_TEST(testArgumentlessInit);
            CPPUNIT_TEST(testArgumentfullInit);
            CPPUNIT_TEST(testOneLevelArgumentlessDot);
            CPPUNIT_TEST(testOneLevelArgumentfullDot);
            CPPUNIT_TEST(testOneLevelArgumentCombineDot);
            CPPUNIT_TEST(testMultilevelDot);
            CPPUNIT_TEST_SUITE_END();
        public:
            void testArgumentlessInit()
            {
                const LogicType wordType{"word", LogicType::ParameterCol()};
                const LogicFunc::ArgumentCol arguments;
                const LogicFunc func("func", wordType, arguments);
                CPPUNIT_ASSERT(wordType == func.result());
                CPPUNIT_ASSERT(arguments == func.arguments());
            }

            void testArgumentfullInit()
            {
                const LogicType wordType{"word", LogicType::ParameterCol()};
                const LogicType otherType{"other", LogicType::ParameterCol()};
                const LogicType complexType{"other",
                    LogicType::ParameterCol{otherType}};
                const LogicFunc::ArgumentCol arguments{complexType, otherType};
                const LogicFunc func("func", wordType, arguments);
                CPPUNIT_ASSERT(wordType == func.result());
                CPPUNIT_ASSERT(arguments == func.arguments());
            }

            void testOneLevelArgumentlessDot()
            {
                const LogicType wordType{"word", LogicType::ParameterCol()};
                const LogicType otherType{"other", LogicType::ParameterCol()};
                const LogicType complexType{"other",
                    LogicType::ParameterCol{otherType}};
                const LogicFunc topFunc("topFunc", wordType,
                    LogicFunc::ArgumentCol{otherType});
                const LogicFunc bottomFunc("bottomFunc", otherType,
                    LogicFunc::ArgumentCol());
                const auto res = topFunc.dot(0, bottomFunc);
                CPPUNIT_ASSERT(wordType == res.result());
                CPPUNIT_ASSERT(bottomFunc.arguments() == res.arguments());
            }

            void testOneLevelArgumentfullDot()
            {
                const LogicType wordType{"word", LogicType::ParameterCol()};
                const LogicType otherType{"other", LogicType::ParameterCol()};
                const LogicType complexType{"other",
                    LogicType::ParameterCol{otherType}};
                const LogicFunc topFunc("topFunc", wordType,
                    LogicFunc::ArgumentCol{otherType});
                const LogicFunc bottomFunc("bottomFunc", otherType,
                    LogicFunc::ArgumentCol{wordType, otherType});
                const auto res = topFunc.dot(0, bottomFunc);
                CPPUNIT_ASSERT(wordType == res.result());
                CPPUNIT_ASSERT(bottomFunc.arguments() == res.arguments());
            }

            void testOneLevelArgumentCombineDot()
            {
                const LogicType wordType{"word", LogicType::ParameterCol()};
                const LogicType otherType{"other", LogicType::ParameterCol()};
                const LogicType complexType{"other",
                    LogicType::ParameterCol{otherType}};
                const LogicFunc topFunc("topFunc", wordType,
                    LogicFunc::ArgumentCol{complexType, otherType});
                const LogicFunc bottomFunc("bottomFunc", otherType,
                    LogicFunc::ArgumentCol{wordType, otherType});
                const auto res = topFunc.dot(1, bottomFunc);
                CPPUNIT_ASSERT(wordType == res.result());
                LogicFunc::ArgumentCol expArguments{complexType};
                expArguments.insert(expArguments.end(),
                    bottomFunc.arguments().begin(),
                    bottomFunc.arguments().end());
                CPPUNIT_ASSERT(expArguments == res.arguments());
            }

            void testMultilevelDot()
            {
                const LogicType wordType{"word", LogicType::ParameterCol()};
                const LogicType otherType{"other", LogicType::ParameterCol()};
                const LogicType complexType{"other",
                    LogicType::ParameterCol{otherType}};
                const LogicType halfType{"half", LogicType::ParameterCol()};
                const LogicFunc topFunc("topFunc", wordType,
                    LogicFunc::ArgumentCol{wordType, otherType, complexType});
                const LogicFunc secondFunc("secondFunc", otherType,
                    LogicFunc::ArgumentCol{complexType, wordType});
                const LogicFunc bottomFunc("bottomFunc", complexType,
                    LogicFunc::ArgumentCol{halfType});
                const auto res =
                    topFunc.dot(1, secondFunc).dot(1, bottomFunc);
                CPPUNIT_ASSERT(wordType == res.result());
                LogicFunc::ArgumentCol expArguments{
                    wordType, halfType, wordType, complexType};
                CPPUNIT_ASSERT(expArguments == res.arguments());
            }
        };
        CPPUNIT_TEST_SUITE_REGISTRATION(LogicFuncTest);
    }
}
