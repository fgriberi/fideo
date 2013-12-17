/*
 * @file   FideoStructureParser.h
 * @brief  This header defines necessary methods in fideo
 *
 * @author Santiago Videla
 * @email  santiago.videla AT gmail.com
 *
 * @author Franco Riberi
 * @email  fgriberi AT gmail.com
 *
 * Contents:  Header file for fideo providing parsers methods
 *
 * System:    fideo: Folding Interface Dynamic Exchange Operations
 * Language:  C++
 *
 * @date September 26, 2010, 5:25 PM
 *
 * Copyright (C) 2010  Santiago Videla and Franco Riberi, FuDePAN
 *
 * This file is part of fideo
 *
 * fideo is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * fideo is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with fideo.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef FIDEO_STRUCTURE_PARSER_H
#define FIDEO_STRUCTURE_PARSER_H

#include <stack>
#include <string>
#include <biopp/biopp.h>
#include "fideo/FideoHelper.h"
#include "fideo/RnaBackendsException.h"

namespace fideo
{

struct ViennaParser
{

    static const FileLineNo LINE_NO = 1;
    static const char OPEN_PAIR = '(';
    static const char CLOSE_PAIR = ')';
    static const char UNPAIR = '.';

    /** @brief obtain structure
    *
    * @param str: to parse
    * @param secStrucute: to fill with structure
    * @return void
    */
    static void parseStructure(const std::string& str, biopp::SecStructure& secStructure)
    {
        secStructure.set_sequence_size(str.length());
        std::stack<biopp::SeqIndex> stackIndex;
        for (size_t i = 0; i < str.length(); ++i)
        {
            biopp::SeqIndex open;
            switch (str[i])
            {
                case UNPAIR:
                    secStructure.unpair(i);
                    break;
                case OPEN_PAIR:
                    stackIndex.push(i);
                    break;
                case CLOSE_PAIR:
                    if (!stackIndex.empty())
                    {
                        open = stackIndex.top();
                        secStructure.pair(open, i);
                        stackIndex.pop();
                    }
                    else
                    {
                        throw(InvalidStructureException("Unexpected closing pair"));
                    }
                    break;
                default:
                    throw(InvalidStructureException("Unexpected symbol: " + secStructure.paired_with(i)));
                    break;
            }
        }
        if (!stackIndex.empty())
        {
            throw(InvalidStructureException("Pairs pending to close"));
        }
    }


    static void toString(const biopp::SecStructure& secStructure, std::string& str)
    {
        str.clear();
        for (biopp::SeqIndex i = 0; i < secStructure.size(); ++i)
        {
            if (secStructure.is_paired(i))
            {
                if (i < secStructure.paired_with(i))
                    str += OPEN_PAIR;
                else
                    str += CLOSE_PAIR;
            }
            else
                str += UNPAIR;
        }
    }

};


} //end namespace fideo

#endif
