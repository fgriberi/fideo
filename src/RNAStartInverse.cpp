/*
 * File:   RNAStartInverse.cpp
 * Author: Santiago Videla <santiago.videla at gmail.com>
 *
 * Created on November 10, 2010, 4:26 PM
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

#include <mili/mili.h>
#include "fideo/RNAStartInverse.h"

namespace fideo
{

RNAStartInverse::RNAStartInverse(const biopp::SecStructure& structure, Similitude sd, Distance hd, const CombinationAttempts& ca)
    : combination_attempts(ca),
      combinator(new SeqIndexesCombinator(structure.size(), hd)),
      structure(structure),
      max_structure_distance(sd),
      max_sequence_distance(hd)
{}

RNAStartInverse::~RNAStartInverse()
{
    delete combinator;
}

void RNAStartInverse::fold_inverse(biopp::NucSequence& sequence)
{
    std::string seq;
    Distance hd;
    Similitude sd;

    CombinationAttempts i = combination_attempts;
    bool c;
    do
    {
        --i;
        do
        {
            execute(seq, hd, sd);
        }
        while (sd > max_structure_distance);

        //If the sequence found was already returned and we reach the number
        //of attempts for the current combination of free positions in 'start',
        //we move to the next combination, change the 'start' and re-start the
        //number of attempts.
        c = mili::contains(found, seq);
        if (c && i == 0)
        {
            combinator->next(positions);
            change_start();
            i = combination_attempts;
        }
    }
    while (c);

    //Adds the sequence found to the set.
    mili::insert_into(found, seq);
    sequence = biopp::NucSequence(seq);
}

void RNAStartInverse::set_start(const biopp::NucSequence& sequence)
{
    if (sequence.length() < max_sequence_distance)
        throw RNABackendException("Start sequence must have at least 'max_sequence_distance' length");

    //clear any previous start and found set.
    start.clear();
    found.clear();
    //Sets the start in lowercase. We need this to avoid that RNAinverse
    //make changes everywhere.
    for (size_t i = 0; i < sequence.length(); ++i)
    {
        start += tolower(sequence[i].as_char());
    }
    //Adds the start to the set of found sequences.
    mili::insert_into(found, start);

    //rembember the original start.
    rstart = start;

    //Begin the combinator and set the first free positions.
    combinator->begin();
    combinator->next(positions);
    change_start();
}

void RNAStartInverse::change_start()
{
    //Gets the original start
    start = rstart;
    mili::CAutonomousIterator<SeqIndexesCombination> it(positions);
    while (!it.end())
    {
        //This let to search for possible changes in each position.
        start[*it] = WILDCARD;
        ++it;
    }
}


} //end namespace fideo
