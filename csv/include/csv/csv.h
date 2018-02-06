#ifndef CSV_CSV_H
#define CSV_CSV_H

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
        Csv(std::basic_istream<CharT> &stream, CharT delim=',', CharT bracket='"', CharT esc='\\')
            :stream(stream), delim(delim), bracket(bracket), esc(esc), lineNumber(0),
            buffer{}, bufferPos(0), bufferSize(0)
        {}

        std::pair<StringCollection, bool> row();

    private:
        using CharBuffer = std::array<CharT, 1024>;

    private:
        std::pair<StringCollection, bool> rowWithEsc();
        std::pair<StringCollection, bool> rowQuote();

    private:
        std::basic_istream<CharT> &stream;
        CharT delim;
        CharT bracket;
        CharT esc;
        std::size_t lineNumber;
        CharBuffer buffer;
        std::size_t bufferPos;
        std::size_t bufferSize;
    };

    template<class CharT>
    std::pair<typename Csv<CharT>::StringCollection, bool> Csv<CharT>::row()
    {
        if(esc != L'\0')
            return Csv<CharT>::rowWithEsc();
        else
            return Csv<CharT>::rowQuote();
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
            return res;
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
                    if(bufferSize == 0)
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
                        res.first.push_back(cur);
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
                    cur.push_back(v);
                }
            }
            ++i;
            ++bufferPos;
        }
        if(!res.first.empty() || !cur.empty())
        {
            res.first.push_back(cur);
            res.second = true;
        }
        return res;
    }

    template<class CharT>
    std::pair<typename Csv<CharT>::StringCollection, bool>
    Csv<CharT>::rowQuote()
    {
        std::pair<StringCollection, bool> res{StringCollection(), false};
        ++lineNumber;
        String cur;
        bool quoted = false;
        enum class PrevCategory
        {
            REG,
            BRACE,
            CR
        };
        if(!stream && bufferPos == bufferSize)
            return res;
        bool eol = false;
        std::size_t i = 1;
        PrevCategory prev = PrevCategory::REG;
        while(!eol)
        {
            if(bufferPos == bufferSize)
            {
                if(stream)
                {
                    stream.read(&buffer.front(), buffer.size());
                    bufferSize = stream.gcount();
                    bufferPos = 0;
                    if(bufferSize == 0)
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
                    if(prev == PrevCategory::BRACE)
                    {
                        if(cur.empty())
                        {
                            cur.push_back(v);
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
                        eol = true;
                    }
                }
                else
                {
                    if(prev == PrevCategory::BRACE)
                    {
                        quoted = false;
                        eol = true;
                    }
                    else
                    {
                        cur.push_back(v);
                    }
                }
                prev = PrevCategory::REG;
            }
            else if(v == '\r')
            {
                if(!quoted)
                {
                    if(prev == PrevCategory::BRACE)
                    {
                        if(cur.empty())
                        {
                            cur.push_back(v);
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
                    else if(prev == PrevCategory::CR)
                    {
                        eol = true;
                        break;
                    }
                }
                else
                {
                    if(prev == PrevCategory::BRACE)
                    {
                        quoted = false;
                    }
                    else
                    {
                        cur.push_back(v);
                    }
                }
                prev = PrevCategory::CR;
            }
            else if(v == delim)
            {
                if(!quoted)
                {
                    if(prev == PrevCategory::BRACE)
                    {
                        if(cur.empty())
                        {
                            cur.push_back(v);
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
                    else if(prev == PrevCategory::CR)
                    {
                        eol = true;
                        break;
                    }
                    else
                    {
                        // TODO: move
                        res.first.push_back(cur);
                        cur.clear();
                    }
                }
                else
                {
                    if(prev == PrevCategory::BRACE)
                    {
                        quoted = false;
                        // TODO: move
                        res.first.push_back(cur);
                        cur.clear();
                    }
                    else
                    {
                        cur.push_back(v);
                    }
                }
                prev = PrevCategory::REG;
            }
            else if(v == bracket)
            {
                auto nextPrev = PrevCategory::BRACE;
                if(!quoted)
                {
                    if(prev == PrevCategory::BRACE)
                    {
                        if(cur.empty())
                        {
                            quoted = true;
                            nextPrev = PrevCategory::BRACE;
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
                    else if(prev == PrevCategory::CR)
                    {
                        eol = true;
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
                    if(prev == PrevCategory::BRACE)
                    {
                        cur.push_back(v);
                        nextPrev = PrevCategory::REG;
                    }
                    else
                    {
                        nextPrev = PrevCategory::BRACE;
                    }
                }
                prev = nextPrev;
            }
            else
            {
                if(!quoted)
                {
                    if(prev == PrevCategory::BRACE)
                    {
                        if(cur.empty())
                        {
                            cur.push_back(v);
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
                    else if(prev == PrevCategory::CR)
                    {
                        eol = true;
                        break;
                    }
                    else
                    {
                        cur.push_back(v);
                    }
                }
                else
                {
                    if(prev == PrevCategory::BRACE)
                    {
                        throw std::runtime_error("");
                    }
                    else
                    {
                        cur.push_back(v);
                    }
                }
                prev = PrevCategory::REG;
            }
            ++i;
            ++bufferPos;
        }
        if(!res.first.empty() || !cur.empty())
        {
            res.first.push_back(cur);
            res.second = true;
        }
        return res;
    }
}

#endif
