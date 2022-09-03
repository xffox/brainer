#ifndef CSV_CSV_H
#define CSV_CSV_H

#include <algorithm>
#include <string>
#include <vector>
#include <istream>
#include <utility>
#include <cstddef>
#include <sstream>
#include <iostream>
#include <array>

namespace csv
{
    template<class CharT>
    class Csv
    {
    public:
        using String = std::basic_string<CharT>;
        using StringCollection = std::vector<String>;

    public:
        Csv(std::basic_istream<CharT> &stream,
            CharT delim=',', CharT bracket='"', CharT esc='\\')
            :stream(stream), delim(delim), bracket(bracket), esc(esc),
            quoteSpecialChars(makeQuoteSpecialChars(delim, bracket)),
            lineNumber(0), buffer{},
            bufferPos(0), bufferSize(0), expectedColumns(1),
            rowOp(esc != L'\0'?&Csv::rowWithEsc:&Csv::rowQuote)
        {}

        std::pair<StringCollection, bool> row();

    private:
        enum class PrevCategory
        {
            REG,
            BRACE,
            CR
        };
        using CharBuffer = std::array<CharT, 4096>;
        using CharFlagCol = std::array<bool, 128>;

    private:
        std::pair<StringCollection, bool> rowWithEsc();
        std::pair<StringCollection, bool> rowQuote();

        static CharFlagCol makeQuoteSpecialChars(CharT delim, CharT bracket)
        {
            CharFlagCol res{};
            res[delim] = true;
            res[bracket] = true;
            res['\r'] = true;
            res['\n'] = true;
            return res;
        }

    private:
        std::basic_istream<CharT> &stream;
        const CharT delim;
        const CharT bracket;
        const CharT esc;
        const CharFlagCol quoteSpecialChars;
        std::size_t lineNumber;
        CharBuffer buffer;
        std::size_t bufferPos;
        std::size_t bufferSize;
        std::size_t expectedColumns;
        std::pair<typename Csv::StringCollection, bool> (Csv::*const rowOp)();
    };

    template<class CharT>
    std::pair<typename Csv<CharT>::StringCollection, bool> Csv<CharT>::row()
    {
        return (this->*rowOp)();
    }

    template<class CharT>
    std::pair<typename Csv<CharT>::StringCollection, bool>
    Csv<CharT>::rowWithEsc()
    {
        String line;
        std::pair<StringCollection, bool> res{StringCollection(), false};
        ++lineNumber;
        String cur;
        bool escaped = false;
        bool quoted = false;
        bool cr = false;
        bool closed = false;
        if(!stream && bufferPos == bufferSize)
        {
            return res;
        }
        bool eol = false;
        std::size_t i = 1;
        while(!eol)
        {
            if(bufferPos == bufferSize)
            {
                if(stream)
                {
                    stream.read(&buffer.front(), buffer.size());
                    bufferSize = stream.gcount();
                    bufferPos = 0;
                    if(bufferPos == bufferSize)
                        break;
                }
                else
                {
                    break;
                }
            }
            const auto v = buffer[bufferPos];
            if(v == '\n')
            {
                if(!quoted)
                {
                    if(!escaped)
                    {
                        eol = true;
                    }
                    else
                    {
                        // new line escape
                        cur.push_back(v);
                    }
                }
                else
                {
                    // new line escape case included
                    cur.push_back(v);
                }
                escaped = false;
            }
            else if(v == '\r')
            {
                if(cr)
                {
                    break;
                }
                if(!quoted)
                {
                    if(!escaped)
                    {
                        cr = true;
                    }
                    else
                    {
                        // cr escape
                        cur.push_back(v);
                    }
                }
                else
                {
                    // cr escape case included
                    cur.push_back(v);
                }
                escaped = false;
            }
            else if(v == esc)
            {
                if(cr)
                {
                    break;
                }
                if(closed)
                {
                    std::stringstream ss;
                    ss<<"csv parse error for line: "
                        <<"value after bracket is not empty: "
                        <<lineNumber<<':'<<i<<'='<<v;
                    throw std::runtime_error(ss.str());
                }
                if(!escaped)
                {
                    escaped = true;
                }
                else
                {
                    cur.push_back(v);
                    escaped = false;
                }
            }
            else if(v == delim)
            {
                if(cr)
                {
                    break;
                }
                if(!quoted)
                {
                    if(!escaped)
                    {
                        res.first.push_back(std::move(cur));
                        cur.clear();
                        closed = false;
                    }
                    else
                    {
                        cur.push_back(v);
                    }
                }
                else
                {
                    if(!escaped)
                    {
                        cur.push_back(v);
                    }
                    else
                    {
                        // escapes inside quotes
                        cur.push_back(v);
                    }
                }
                escaped = false;
            }
            else if(v == bracket)
            {
                if(cr)
                {
                    break;
                }
                if(!quoted)
                {
                    if(!escaped)
                    {
                        if(cur.empty())
                        {
                            quoted = true;
                        }
                        else
                        {
                            std::stringstream ss;
                            ss<<"csv parse error for line: "<<
                                "value before bracket is not empty: "
                                <<lineNumber<<':'<<i<<'='<<v;
                            throw std::runtime_error(ss.str());
                        }
                    }
                    else
                    {
                        cur.push_back(v);
                    }
                }
                else
                {
                    if(!escaped)
                    {
                        quoted = false;
                        closed = true;
                    }
                    else
                    {
                        cur.push_back(v);
                    }
                }
                escaped = false;
            }
            else
            {
                if(cr)
                {
                    break;
                }
                if(closed)
                {
                    std::stringstream ss;
                    ss<<"csv parse error for line: "
                        <<"value after bracket is not empty: "
                        <<lineNumber<<':'<<i<<'='<<v;
                    throw std::runtime_error(ss.str());
                }
                if(!escaped)
                {
                    cur.push_back(v);
                }
                else
                {
                    // unknown escapes
                    cur.push_back(esc);
                    cur.push_back(v);
                    escaped = false;
                }
            }
            ++i;
            ++bufferPos;
        }
        if(!res.first.empty() || !cur.empty())
        {
            res.first.push_back(std::move(cur));
            cur.clear();
            res.second = true;
        }
        return res;
    }

    template<class CharT>
    std::pair<typename Csv<CharT>::StringCollection, bool>
    Csv<CharT>::rowQuote()
    {
        struct RowQuoteState
        {
            bool quoted = false;
            bool eol = false;
            std::size_t i = 1;
            PrevCategory prev = PrevCategory::REG;
        };
        std::pair<StringCollection, bool> res{
            StringCollection(expectedColumns), false};
        ++lineNumber;
        RowQuoteState state;
        std::size_t column = 0;
        while(!state.eol)
        {
            auto &curValue = res.first[column];
            if(bufferPos == bufferSize)
            {
                if(stream)
                {
                    stream.read(&buffer.front(), buffer.size());
                    bufferSize = stream.gcount();
                    bufferPos = 0;
                    if(bufferPos == bufferSize)
                        break;
                }
                else
                {
                    break;
                }
            }
            res.second = true;
            if(state.prev == PrevCategory::REG)
            {
                auto iter = std::find_if(
                    std::begin(buffer)+bufferPos, std::begin(buffer)+bufferSize,
                    [this](CharT v){
                        return quoteSpecialChars[v%quoteSpecialChars.size()];
                    });
                curValue.append(std::begin(buffer)+bufferPos, iter);
                bufferPos = iter - std::begin(buffer);
            }
            if(bufferPos == bufferSize)
                continue;
            const auto v = buffer[bufferPos];
            if(v == delim)
            {
                if(!state.quoted)
                {
                    if(state.prev == PrevCategory::BRACE)
                    {
                        if(curValue.empty())
                        {
                            curValue.push_back(v);
                            state.quoted = true;
                        }
                        else
                        {
                            std::stringstream ss;
                            ss<<"csv parse error for line: "<<
                                "value before bracket is not empty: "
                                <<lineNumber<<':'<<state.i<<'='<<v;
                            throw std::runtime_error(ss.str());
                        }
                    }
                    else if(state.prev == PrevCategory::CR)
                    {
                        state.eol = true;
                        break;
                    }
                    else
                    {
                        ++column;
                        if(column >= res.first.size())
                        {
                            res.first.emplace_back();
                        }
                    }
                }
                else
                {
                    if(state.prev == PrevCategory::BRACE)
                    {
                        state.quoted = false;
                        ++column;
                        if(column >= res.first.size())
                        {
                            res.first.emplace_back();
                        }
                    }
                    else
                    {
                        curValue.push_back(v);
                    }
                }
                state.prev = PrevCategory::REG;
            }
            else if(v == '\n')
            {
                if(!state.quoted)
                {
                    if(state.prev == PrevCategory::BRACE)
                    {
                        if(res.first.empty())
                        {
                            curValue.push_back(v);
                            state.quoted = true;
                        }
                        else
                        {
                            std::stringstream ss;
                            ss<<"csv parse error for line: "<<
                                "value before bracket is not empty: "
                                <<lineNumber<<':'<<state.i<<'='<<v;
                            throw std::runtime_error(ss.str());
                        }
                    }
                    else
                    {
                        state.eol = true;
                    }
                }
                else
                {
                    if(state.prev == PrevCategory::BRACE)
                    {
                        state.quoted = false;
                        state.eol = true;
                    }
                    else
                    {
                        curValue.push_back(v);
                    }
                }
                state.prev = PrevCategory::REG;
            }
            else if(v == '\r')
            {
                if(!state.quoted)
                {
                    if(state.prev == PrevCategory::BRACE)
                    {
                        if(curValue.empty())
                        {
                            curValue.push_back(v);
                            state.quoted = true;
                        }
                        else
                        {
                            std::stringstream ss;
                            ss<<"csv parse error for line: "<<
                                "value before bracket is not empty: "
                                <<lineNumber<<':'<<state.i<<'='<<v;
                            throw std::runtime_error(ss.str());
                        }
                    }
                    else if(state.prev == PrevCategory::CR)
                    {
                        state.eol = true;
                        break;
                    }
                }
                else
                {
                    if(state.prev == PrevCategory::BRACE)
                    {
                        state.quoted = false;
                    }
                    else
                    {
                        curValue.push_back(v);
                    }
                }
                state.prev = PrevCategory::CR;
            }
            else if(v == bracket)
            {
                auto nextPrev = PrevCategory::BRACE;
                if(!state.quoted)
                {
                    if(state.prev == PrevCategory::BRACE)
                    {
                        if(curValue.empty())
                        {
                            state.quoted = true;
                            nextPrev = PrevCategory::BRACE;
                        }
                        else
                        {
                            std::stringstream ss;
                            ss<<"csv parse error for line: "<<
                                "value before bracket is not empty: "
                                <<lineNumber<<':'<<state.i<<'='<<v;
                            throw std::runtime_error(ss.str());
                        }
                    }
                    else if(state.prev == PrevCategory::CR)
                    {
                        state.eol = true;
                        nextPrev = PrevCategory::REG;
                        break;
                    }
                    else
                    {
                        nextPrev = PrevCategory::BRACE;
                    }
                }
                else
                {
                    if(state.prev == PrevCategory::BRACE)
                    {
                        curValue.push_back(v);
                        nextPrev = PrevCategory::REG;
                    }
                    else
                    {
                        nextPrev = PrevCategory::BRACE;
                    }
                }
                state.prev = nextPrev;
            }
            else
            {
                if(!state.quoted)
                {
                    if(state.prev == PrevCategory::BRACE)
                    {
                        if(curValue.empty())
                        {
                            curValue.push_back(v);
                            state.quoted = true;
                        }
                        else
                        {
                            std::stringstream ss;
                            ss<<"csv parse error for line: "<<
                                "value before bracket is not empty: "
                                <<lineNumber<<':'<<state.i<<'='<<v;
                            throw std::runtime_error(ss.str());
                        }
                    }
                    else if(state.prev == PrevCategory::CR)
                    {
                        state.eol = true;
                        break;
                    }
                    else
                    {
                        curValue.push_back(v);
                    }
                }
                else
                {
                    if(state.prev == PrevCategory::BRACE)
                    {
                        throw std::runtime_error("");
                    }
                    else
                    {
                        curValue.push_back(v);
                    }
                }
                state.prev = PrevCategory::REG;
            }
            ++state.i;
            ++bufferPos;
        }
        const auto curColumns = column+1;
        if(curColumns < res.first.size())
        {
            res.first.resize(curColumns);
        }
        expectedColumns = curColumns;
        return res;
    }
}

#endif
