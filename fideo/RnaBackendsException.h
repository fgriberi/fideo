/*
 * @file     rna_backends_exceptions.h
 * @brief    This header defines all fideo exceptions
 *
 * @author   Santiago Videla
 * @email    santiago.videla AT gmail.com
 *
 * @author   Franco Riberi
 * @email    fgriberi AT gmail.com
 *
 * Contents: Header file for fideo providing system exceptions definitions.
 *
 * System:   fideo: Folding Interface Dynamic Exchange Operations
 * Language: C++
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

namespace fideo
{

struct FideoExceptionHierarchy {};
typedef mili::GenericException<FideoExceptionHierarchy> FideoException;

//State exception
class StateException: public FideoException
{
public:
    StateException(const std::string& description): FideoException(description) {}
};

DEFINE_SPECIFIC_EXCEPTION_TEXT(RNABackendException, FideoExceptionHierarchy, "RNABackend");
DEFINE_SPECIFIC_EXCEPTION_TEXT(FromStringException, FideoExceptionHierarchy, "Wrong column type");
DEFINE_SPECIFIC_EXCEPTION_TEXT(NotReadValueUsingOffsetException, FideoExceptionHierarchy, "Could not read the value from given line, offset and length");
DEFINE_SPECIFIC_EXCEPTION_TEXT(NotReadValueException, FideoExceptionHierarchy, "Could not read the value from given line");
DEFINE_SPECIFIC_EXCEPTION_TEXT(InvalidStructureException, FideoExceptionHierarchy, "Invalid structure");
DEFINE_SPECIFIC_EXCEPTION_TEXT(InvalidIntaPath, FideoExceptionHierarchy, "Invalid path of IntaRNA executable");
DEFINE_SPECIFIC_EXCEPTION_TEXT(SeparatorException, FideoExceptionHierarchy, "Error in string parser");
DEFINE_SPECIFIC_EXCEPTION_TEXT(UnsupportedException, FideoExceptionHierarchy, "Unsupported Sequence");
DEFINE_SPECIFIC_EXCEPTION_TEXT(NotFoundFileException, FideoExceptionHierarchy, "Output file not found");
DEFINE_SPECIFIC_EXCEPTION_TEXT(InvalidOutputRNACofold, FideoExceptionHierarchy, "Invalid output RNAcofold.");
DEFINE_SPECIFIC_EXCEPTION_TEXT(InvalidOutputRNADuplex, FideoExceptionHierarchy, "Invalid output RNAduplex.");
DEFINE_SPECIFIC_EXCEPTION_TEXT(InvalidOutputRNAUp, FideoExceptionHierarchy, "Invalid output RNAup.");
DEFINE_SPECIFIC_EXCEPTION_TEXT(UnlinkException, FideoExceptionHierarchy, "Error unlink");
DEFINE_SPECIFIC_EXCEPTION_TEXT(InvalidInputName, FideoExceptionHierarchy, "Invalid input name");
DEFINE_SPECIFIC_EXCEPTION_TEXT(InvalidName, FideoExceptionHierarchy, "Name of file to rename is invalid.");
DEFINE_SPECIFIC_EXCEPTION_TEXT(InvalidMotif, FideoExceptionHierarchy, "Invalid motif");
DEFINE_SPECIFIC_EXCEPTION_TEXT(FileNotExist, FideoExceptionHierarchy, "The .det file not exist");
DEFINE_SPECIFIC_EXCEPTION_TEXT(IndexOutOfRange, FideoExceptionHierarchy, "Index out of range");
DEFINE_SPECIFIC_EXCEPTION_TEXT(InvalidDerived, FideoExceptionHierarchy, "Invalid derived class");
DEFINE_SPECIFIC_EXCEPTION_TEXT(InvalidBodyLine, FideoExceptionHierarchy, "Invalid BodyLine");
DEFINE_SPECIFIC_EXCEPTION_TEXT(InvalidaHeader, FideoExceptionHierarchy, "Invalid Header");
DEFINE_SPECIFIC_EXCEPTION_TEXT(FailOperation, FideoExceptionHierarchy, "Failured operation >>");

}// namespace fideo
#endif  /* _RNA_BACKENDS_EXCEPTIONS_H */
