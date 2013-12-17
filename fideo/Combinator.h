/*
 * File:   Combinator.h
 * Author: Santiago Videla <santiago.videla at gmail.com>
 *
 * Created on October 21, 2010, 9:37 AM
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

#ifndef _COMBINATOR_H
#define _COMBINATOR_H

#include <vector>
#include <list>
#include <mili/mili.h>
#include <biopp/biopp.h>
#include "fideo/RnaBackendsTypes.h"
#include "fideo/RnaBackendsException.h"

namespace fideo
{

/**
 * Iterator over the combinations for a given container.
 */
template <class C>
class Combinator
{
public:
    typedef typename C::value_type CType;
    typedef std::vector<CType> Combination;
    /**
     * Sets the iterator at the begining.
     */
    void begin();
    /**
     * Gets the next combination
     * @param comb Combination to write to.
     * @return If there are more combinations.
     */
    bool next(Combination& comb);
    /**
     * Update the size of combinations and restart the iterator
     * @param n the new size
     */
    void update(size_t n);
    /**
     * Constructor
     * @param e the container of elements to generate combinations
     * @param n the size of the combinations.
     */
    Combinator(const C& e, size_t n);
    /**
     * Constructor
     * @param range the container of elements will be generated from 0 to range-1
     * @param n the size of the combinations.
     */
    Combinator(size_t range, size_t n);

private:
    typedef typename C::const_iterator ConstIterator;
    C elements;
    Combination combination;

    size_t k;

    ConstIterator end;
    ConstIterator start;
    ConstIterator kit;

    /**
     * Sets the elements being combined.
     */
    void move();
};

typedef Combinator<std::list<biopp::SeqIndex> > SeqIndexesCombinator;
typedef Combinator<std::list<biopp::SeqIndex> >::Combination SeqIndexesCombination;

#define _COMBINATOR_INLINE_H
#include "CombinatorInline.h"
#undef _COMBINATOR_INLINE_H

}//namespace fideo

#endif  /* _COMBINATOR_H */
