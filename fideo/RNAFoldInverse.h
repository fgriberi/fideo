/*
 * File:   RNAFoldInverse.h
 * Author: Santiago Videla <santiago.videla at gmail.com>
 *
 * Created on October 25, 2010, 12:25 PM
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

#ifndef _RNAFoldInverse_H
#define _RNAFoldInverse_H

#include "fideo/FideoHelper.h"
#include "fideo/RNAStartInverse.h"

namespace fideo
{
class RNAinverse : public RNAStartInverse
{
    static const FilePath IN;
    static const FilePath OUT;
    static const FileLineNo LINE_NO;
 
    size_t read_hamming_distance(FileLine&, size_t, Distance&) const;
    size_t read_structure_distance(FileLine&, size_t, Similitude&) const;

    virtual void execute(std::string&, Distance&, Similitude&);
    virtual void query_start(IStartProvider*);
public:
    RNAinverse(const biopp::SecStructure&, Similitude, Distance, CombinationAttempts);
};

}// end namespace fideo

#endif