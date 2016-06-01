#include <cppunit/TestCase.h>
#include <cppunit/extensions/HelperMacros.h>

#include "fact/Expression.h"

namespace fact
{
    namespace test
    {
        class ExpressionTest: public CppUnit::TestCase
        {
            CPPUNIT_TEST_SUITE(ExpressionTest);
            CPPUNIT_TEST(testArgumentlessInit);
            CPPUNIT_TEST(testArgumentfullInit);
            CPPUNIT_TEST(testOneLevelArgumentlessDot);
            CPPUNIT_TEST(testOneLevelArgumentfullDot);
            CPPUNIT_TEST(testOneLevelArgumentCombineDot);
            CPPUNIT_TEST(testMultilevelDot);
            CPPUNIT_TEST(testMultiLevelDotLast);
            CPPUNIT_TEST_SUITE_END();
        public:
            void testArgumentlessInit()
            {
                const LogicType wordType{"word", LogicType::ParameterCol()};
                const LogicFunc::ArgumentCol arguments;
                const Expression func(LogicFunc("func", wordType, arguments));
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
                const Expression func(LogicFunc("func", wordType, arguments));
                CPPUNIT_ASSERT(wordType == func.result());
                CPPUNIT_ASSERT(arguments == func.arguments());
            }

            void testOneLevelArgumentlessDot()
            {
                const LogicType wordType{"word", LogicType::ParameterCol()};
                const LogicType otherType{"other", LogicType::ParameterCol()};
                const LogicType complexType{"other",
                    LogicType::ParameterCol{otherType}};
                const Expression topFunc(LogicFunc("topFunc", wordType,
                    LogicFunc::ArgumentCol{otherType}));
                const Expression bottomFunc(LogicFunc("bottomFunc", otherType,
                    LogicFunc::ArgumentCol()));
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
                const Expression topFunc(LogicFunc("topFunc", wordType,
                    LogicFunc::ArgumentCol{otherType}));
                const Expression bottomFunc(LogicFunc("bottomFunc", otherType,
                    LogicFunc::ArgumentCol{wordType, otherType}));
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
                const Expression topFunc(LogicFunc("topFunc", wordType,
                    LogicFunc::ArgumentCol{complexType, otherType}));
                const Expression bottomFunc(LogicFunc("bottomFunc", otherType,
                    LogicFunc::ArgumentCol{wordType, otherType}));
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
                const Expression topFunc(LogicFunc("topFunc", wordType,
                    LogicFunc::ArgumentCol{wordType, otherType, complexType}));
                const Expression secondFunc(LogicFunc("secondFunc", otherType,
                    LogicFunc::ArgumentCol{complexType, wordType}));
                const Expression bottomFunc(LogicFunc("bottomFunc",
                        complexType, LogicFunc::ArgumentCol{halfType}));
                const auto res =
                    topFunc.dot(1, secondFunc).dot(1, bottomFunc);
                CPPUNIT_ASSERT(wordType == res.result());
                LogicFunc::ArgumentCol expArguments{
                    wordType, halfType, wordType, complexType};
                CPPUNIT_ASSERT(expArguments == res.arguments());
            }

            void testMultiLevelDotLast()
            {
                const LogicType wordType{"word", LogicType::ParameterCol()};
                const LogicType otherType{"other", LogicType::ParameterCol()};
                const LogicType complexType{"other",
                    LogicType::ParameterCol{otherType}};
                const LogicType halfType{"half", LogicType::ParameterCol()};
                const Expression topFunc(LogicFunc("topFunc", wordType,
                    LogicFunc::ArgumentCol{wordType, otherType, complexType, otherType}));
                const Expression secondFunc(LogicFunc("secondFunc", wordType,
                    LogicFunc::ArgumentCol{complexType}));
                const Expression thirdFunc(LogicFunc("thirdFunc",
                        complexType, LogicFunc::ArgumentCol{halfType}));
                const Expression lastFunc(LogicFunc("lastFunc",
                        otherType, LogicFunc::ArgumentCol{wordType}));
                const auto res =
                    topFunc.dot(0, secondFunc).dot(0, thirdFunc).dot(3, lastFunc);
                CPPUNIT_ASSERT(wordType == res.result());
                LogicFunc::ArgumentCol expArguments{
                    halfType, otherType, complexType, wordType};
                CPPUNIT_ASSERT(expArguments == res.arguments());
            }
        };
        CPPUNIT_TEST_SUITE_REGISTRATION(ExpressionTest);
    }
}
