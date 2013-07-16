/*
 * File:   IStartProvider.h
 * Author: Santiago Videla <santiago.videla at gmail.com>
 *
 * Created on October 25, 2010, 5:40 PM
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

#ifndef _ISTARTPROVIDER_H
#define _ISTARTPROVIDER_H

namespace fideo
{

class IFoldInverse;

/**
 * Interface for start sequences providers.
 */
struct IStartProvider
{
    /**
     * Gets a partial sequence from a bigger one.
     * @param backend sets the start sequence in this backend
     */
    virtual void get_partial_start(IFoldInverse* const) = 0;
    /**
     * Gets a complete sequence.
     * @param backend sets the start sequence in this backend
     */
    virtual void get_complete_start(IFoldInverse* const) = 0;

    virtual ~IStartProvider() {}
};

} //namespace fideo
#endif  /* _ISTARTPROVIDER_H */
