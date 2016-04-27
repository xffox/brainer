#ifndef CSV_CSV_H
#define CSV_CSV_H

#include <string>
#include <vector>
#include <istream>
#include <utility>
#include <cstddef>
#include <sstream>
#include <iostream>

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
            :stream(stream), delim(delim), bracket(bracket), esc(esc), lineNumber(0)
        {}

        std::pair<StringCollection, bool> row();

    private:
        std::basic_istream<CharT> &stream;
        CharT delim;
        CharT bracket;
        CharT esc;
        std::size_t lineNumber;
    };

    template<class CharT>
    std::pair<typename Csv<CharT>::StringCollection, bool> Csv<CharT>::row()
    {
        String line;
        std::pair<StringCollection, bool> res;
        getline(stream, line);
        if(!stream)
            return res;
        ++lineNumber;
        String cur;
        bool escaped = false;
        bool quoted = false;
        for(std::size_t i = 0; i < line.size(); ++i)
        {
            const auto v = line[i];
            if(!escaped)
            {
                if(!quoted)
                {
                    if(v != delim)
                    {
                        if(i > 0 && line[i-1] == bracket &&
                            (cur.empty() || cur.back() != bracket))
                        {
                            std::stringstream ss;
                            ss<<"csv parse error for line: "
                                <<"value after bracket is not empty: "
                                <<lineNumber<<':'<<i;
                            throw std::runtime_error(ss.str());
                        }
                        if(v == esc)
                        {
                            escaped = true;
                        }
                        else if(v == bracket)
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
                                    <<lineNumber<<':'<<i;
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
                        res.first.push_back(cur);
                        cur.clear();
                    }
                }
                else
                {
                    if(v == esc)
                    {
                        escaped = true;
                    }
                    else if(v == delim)
                    {
                        cur.push_back(v);
                    }
                    else if(v == bracket)
                    {
                        quoted = false;
                    }
                    else
                    {
                        cur.push_back(v);
                    }
                }
            }
            else
            {
                if(!quoted)
                {
                    if(v == delim || v == esc || v == bracket)
                    {
                        cur.push_back(v);
                    }
                    else
                    {
                        std::stringstream ss;
                        ss<<"csv parse error for line: "
                            <<"unknown escape char: "<<lineNumber
                            <<':'<<i;
                        throw std::runtime_error(ss.str());
                    }
                }
                else
                {
                    if(v == esc || v == bracket)
                    {
                        cur.push_back(v);
                    }
                    else
                    {
                        std::stringstream ss;
                        ss<<"csv parse error for line: "
                            <<"unknown escape char: "<<lineNumber
                            <<':'<<i;
                        throw std::runtime_error(ss.str());
                    }
                }
                escaped = false;
            }
        }
        res.first.push_back(cur);
        res.second = true;
        return res;
    }
}

#endif
