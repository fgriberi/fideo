/*
 * @file      DetFileParser.h
 * @brief     DetFileParser provides the interface to parse .det file
 *
 * @author    Franco Riberi
 * @email     fgriberi AT gmail.com
 *
 * Contents:  Header file for fideo providing DetFileParser interface.
 *
 * System:    fideo: Folding Interface Dynamic Exchange Operations
 * Language:  C++
 *
 * @date May 12, 2013
 *
 * Copyright (C) 2013 Franco Riberi, FuDePAN
 *
 * This file is part of fideo.
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
 * along with vac-o.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <fstream>
#include "fideo/IMotifObserver.h"
#include "fideo/FideoHelper.h"

#ifndef DET_FILE_PARSER_H
#error Internal header file, Only include in UNAFold file.
#endif

/** @brief Represents a block lines
*
*/
typedef std::list<std::string> Body;

/** @brief Represents the block name
*
*/
typedef std::string BlockName;

/** @brief Represents the file name
*
*/
typedef std::string FileName;

/** @brief Represents a block of the file to parse
*
*/
struct Block
{
    BlockName motifName;
    Body lines;
};

/** @brief Interface to parse .det file.
*
*/
class UNAFold::DetFileParser
{
public:

    /** @brief Parse a det file
    *
    * @param file: file name to parse
    * @param observer: specific implementacion
    * @return void
    */
    void parseDet(const FileName& file, IMotifObserver* observer);

    ~DetFileParser();
    
private:
    class Rule;
    class ExternalRule;
    class InteriorRule;
    class HairpinRule;
    class MultiRule;
    class BulgeRule;   

    /** @brief Allows placing the file in the correct line
    *
    * @param file: file input
    * @return void
    */
    void goToBegin(File& file) const;

    /** @brief Prepares a block to parse
    *
    * @param file: file to read
    * @param block: to fill with data of file
    * @return void
    */
    void buildBlock(std::ifstream& file, Block& block) const;

    /** @brief Remove whitespace repeated
    *
    * @param src: string to analyze
    * @param dest: strig without whitespace repeated
    * @return void
    */
    void removeConsecutiveWhiteSpaces(const std::string& src, std::string& dest) const;

    /** @brief Stored in block the motif name and the rest of the data
    *
    * @param line: line to parse
    * @param block: block to fill
    * @return void
    */
    void parseMotifLine(const std::string& line, Block& block) const;

    /** @brief Parse a stack line.
    *
    * @param line: line to parse
    * @param nameToFill: to fill with stack
    * @return void
    */
    void parseStackLine(const std::string& line, std::string& nameToFill) const;

    /** @brief Parse the current block and fill the motif
    *
    * @param block: block to parse
    * @param motif: specific implementation of IMotifObserver
    * @return void
    */
    void parseBlock(const Block& block, IMotifObserver::Motif& motif);

    /** @brief Load the available rules
    *
    * @return void
    */
    void fillRules();

    void freeRules();

    /** @brief To store the available rules
    *
    */
    typedef std::map<std::string, Rule*> Rules;

    Rules availableRules;
};

#define RULE_H
#include "fideo/Rule.h"
#undef RULE_H