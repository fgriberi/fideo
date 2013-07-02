/*
 * File:   RNAForester.h
 * Author: Santiago Videla <santiago.videla at gmail.com>
 *
 * Created on October 1, 2010, 7:00 PM
 *
 * Copyright (C) 2010  Santiago Videla, FuDePAN
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

#ifndef _RNAFORESTER_H
#define _RNAFORESTER_H


#include <sstream>
#include <etilico/etilico.h>
#include "fideo/FideoHelper.h"
#include "fideo/FideoStructureParser.h"
#include "fideo/IStructureCmp.h"

namespace fideo
{

/**
 * Implementation using system call to RNAforester
 */
class RNAForester : public IStructureCmp
{
    static const FilePath IN;
    static const FilePath OUT;
    static const FileLineNo LINE_NO;
    static const std::string RNAforester_PROG;
    virtual Similitude compare(const biopp::SecStructure&, const biopp::SecStructure&) const;
};

REGISTER_FACTORIZABLE_CLASS(IStructureCmp, RNAForester, std::string, "RNAForester");

const FilePath RNAForester::IN = "forester.in";
const FilePath RNAForester::OUT = "forester.out";
const FileLineNo RNAForester::LINE_NO = 1;
const std::string RNAForester::RNAforester_PROG = "RNAforester";

Similitude RNAForester::compare(const biopp::SecStructure& struct1, const biopp::SecStructure& struct2) const
{
    std::stringstream ss;
    ss << RNAforester_PROG << " -r --score -f "
       << IN << " > " << OUT;

    const etilico::Command CMD = ss.str();

    FileLinesCt lines;
    std::string struct1_str;
    std::string struct2_str;
    ViennaParser::toString(struct1, struct1_str);
    ViennaParser::toString(struct2, struct2_str);

    insert_into(lines, struct1_str);
    insert_into(lines, struct2_str);

    helper::write(IN, lines);
    etilico::runCommand(CMD);

    FileLine aux;
    helper::readLine(OUT, LINE_NO, aux);

    Similitude s;
    helper::readValue(aux, s);
    return s;
}

}//end namespace

#endif  /* _RNAFORESTER_H */

