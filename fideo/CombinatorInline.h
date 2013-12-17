/*
 * @file     CombinatorInline.h
 * @brief    Inline definitions.
 *
 * @author   Franco Riberi
 * @email    fgriberi AT gmail.com
 *
 * Contents: Header file for fideo providing necessary methods
 *
 * System:   fideo: Folding Interface Dynamic Exchange Operations
 * Language: C++
 *
 * @date Dicember 17, 2012, 12:25 AM
 *
 * Copyright (C) 2010  Franco Riberi, FuDePAN
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

#ifndef _COMBINATOR_INLINE_H
#error Internal header file, DO NOT include this.
#endif

template<class C>
inline Combinator<C>::Combinator(const C& e, size_t n)
    : elements(e),
      combination(n),
      k(n),
      end(),
      start(),
      kit()
{
    if (e.size() < n)
        throw CombinatorException();

    begin();
}

template<class C>
inline Combinator<C>::Combinator(size_t range, size_t n)
    : elements(range),
      combination(n),
      k(n),
      end(),
      start(),
      kit()
{
    if (range < n)
        throw CombinatorException();

    C range_ct;
    for (size_t i = 0; i < range; ++i)
    {
        insert_into(range_ct, i);
    }
    elements = range_ct;
    begin();
}

template<class C>
inline void Combinator<C>::update(size_t n)
{
    k = n;
    combination.clear();
    combination.resize(n);
    begin();
}

template<class C>
inline void Combinator<C>::begin()
{
    start = elements.begin();
    end = elements.end();
    move();
}

template<class C>
inline void Combinator<C>::move()
{
    if (k == 0)
        kit = end;
    else
    {
        ConstIterator it = start;
        for (size_t i = 0; i < k; ++i)
        {
            combination[i] = *it;
            if (i == k - 1)
                kit = it;
            ++it;
        }
    }
}

template<class C>
inline bool Combinator<C>::next(Combination& comb)
{
    bool more(true);

    if (kit != end)
    {
        comb = combination;
        ++kit;
        if (kit == end && k > 1)
        {
            ++start;
            move();
        }
        else
            combination[k - 1] = *kit;
    }
    else
    {
        more = false;
        //Make it cyclic
        begin();
        comb = combination;
    }

    return more;
}
