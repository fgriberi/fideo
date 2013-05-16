/*
 * @file   FideoHelperInline.h
 * @brief  Inline definitions.
 *
 * @author Santiago Videla
 * @email  santiago.videla AT gmail.com
 *
 * @author Franco Riberi
 * @email  fgriberi AT gmail.com
 *
 * Contents:  Header file for fideo providing necessary methods
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

#ifndef FIDEO_HELPER_INLINE_H
#error Internal header file, DO NOT include this.
#endif

/** @brief Convert from string to type T
 *
 * @param from: string to convert
 * @param to: converted of 'from' parameter.
 * @return void
 */
template<class T>
inline void convertFromString(const std::string& from, T& to)
{
    if (!mili::from_string(from, to))
    {
        throw FromStringException();
    }
}

/** @brief Read a value
 *
 * Read a value from a file line using offset and length
 * @param line: file line
 * @param offset: offset in the line
 * @param n: length to read
 * @param t: where to write the value.
 * @return void
 */
template<class T>
inline void readValue(const FileLine& line, const FileLine::size_type offset, const size_t n, T& t)
{
    const bool success = mili::from_string(line.substr(offset, n), t);
    if (!success)
    {
        throw NotReadValueUsingOffsetException();
    }
}

/** @brief Read a value from a line file.
 * 
 * @param line: file line
 * @param t: where to write the value
 * @return void
 */
template<class T>
inline void readValue(const FileLine& line, T& t)
{
    const bool success = mili::from_string(line, t);
    if (!success)
    {
        throw NotReadValueException();
    }
}
