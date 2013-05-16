/*
 * @file   rna_backends_exceptions.h
 * @brief  This header defines all fideo exceptions
 *
 * @author Santiago Videla
 * @email  santiago.videla AT gmail.com
 *
 * @author Franco Riberi
 * @email  fgriberi AT gmail.com
 *
 * Contents:  Header file for fideo providing system exceptions definitions.
 *
 * System:    fideo: Folding Interface Dynamic Exchange Operations
 * Language:  C++
 *
 * @date October 27, 2010, 6:42 PM
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

#ifndef _RNA_BACKENDS_EXCEPTIONS_H
#define _RNA_BACKENDS_EXCEPTIONS_H

#include <mili/mili.h>
#ifndef NO_THROW_SPEC
#       define THROW(x) throw x
#else
#       define THROW(x)
#endif

namespace fideo
{
class RNABackendExceptionRoot {};

DEFINE_SPECIFIC_EXCEPTION_TEXT(RNABackendException, RNABackendExceptionRoot, "RNABackend");

DEFINE_SPECIFIC_EXCEPTION_TEXT(InvalidStructureException, RNABackendExceptionRoot, "Invalid structure");

DEFINE_SPECIFIC_EXCEPTION_TEXT(CombinatorException, RNABackendExceptionRoot, "Out of range");

DEFINE_SPECIFIC_EXCEPTION_TEXT(SeparatorException, RNABackendExceptionRoot, "Error in string parser");

DEFINE_SPECIFIC_EXCEPTION_TEXT(FileReadException, RNABackendExceptionRoot, "The config file couldn't be read");

DEFINE_SPECIFIC_EXCEPTION_TEXT(FromStringException, RNABackendExceptionRoot, "Wrong column type");

///FideoHelper exceptions
DEFINE_SPECIFIC_EXCEPTION_TEXT(NotReadValueUsingOffsetException, RNABackendExceptionRoot, "Could not read the value from given line, offset and length");

DEFINE_SPECIFIC_EXCEPTION_TEXT(NotReadValueException, RNABackendExceptionRoot, "Could not read the value from given line");

DEFINE_SPECIFIC_EXCEPTION_TEXT(SystemCallException, RNABackendExceptionRoot, "System call failed");

DEFINE_SPECIFIC_EXCEPTION_TEXT(NonTerminationException, RNABackendExceptionRoot, "Non termination for some reason");

DEFINE_SPECIFIC_EXCEPTION_TEXT(TmpFileCreateException, RNABackendExceptionRoot, "Creation of temp file failed with error");

//IntaRNA
DEFINE_SPECIFIC_EXCEPTION_TEXT(UnsupportedException, RNABackendExceptionRoot, "Unsupported Sequence");

DEFINE_SPECIFIC_EXCEPTION_TEXT(InvalidPathException, RNABackendExceptionRoot, "Invalid path of IntaRNA executable");

DEFINE_SPECIFIC_EXCEPTION_TEXT(NotFoundFileException, RNABackendExceptionRoot, "Output file not found");

DEFINE_SPECIFIC_EXCEPTION_TEXT(ExceptionUnlink, RNABackendExceptionRoot, "Error unlink");

//UNAFold
DEFINE_SPECIFIC_EXCEPTION_TEXT(InvalidMotif, RNABackendExceptionRoot, "Invalid motif");

DEFINE_SPECIFIC_EXCEPTION_TEXT(FileNotExist, RNABackendExceptionRoot, "The .det file not exist");

DEFINE_SPECIFIC_EXCEPTION_TEXT(IndexOutOfRange, RNABackendExceptionRoot, "Index out of range");

}// namespace fideo
#endif  /* _RNA_BACKENDS_EXCEPTIONS_H */


