/*
 * @file      Rule.h
 * @brief     Provides an interface that represents the rules to be used by parse.
 *
 * @author    Franco Riberi
 * @email     fgriberi AT gmail.com
 *
 * Contents:  Header file for fideo providing Rule interface.
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

#ifndef RULE_H
#error Internal header file, Only include in DetFileParser.h file.
#endif

/** @brief Provides an interface used to parse.
*
*/
class UNAFold::DetFileParser::Rule
{
public:

    /** @brief permite calcular los atributos de un motif
    *
    * @param block:
    * @param motif:
    * @return void
    */
    virtual void calcAttrib(const Block& block, IMotifObserver::Motif& motif) const = 0;

    /** @brief Destructor of class
    *
    */
    virtual ~Rule() {}

protected:

    /** @brief Get the substring that is in a certain position
    *
    * @param lineInput: line input
    * @param pos: specific position
    * @param res: substring output
    * @return init position of nucleotid
    */
    static void getSubstrInPos(const std::string& lineInput, size_t pos, std::string& res);

    /** @brief Get init position 
    *
    * @param line: line inut
    * @param motifName: specific motif name
    * @return end position of nucleotid
    */
    static size_t getInitPosOfNucleotid(const std::string line, const std::string motifName);

    /** @brief Get
    *
    * @param line: line input
    * @param motifName: specific motif name
    * @return 
    */
    static size_t getEndPosOfNucleotid(const std::string line, const std::string motifName);

    /** @brief Get second element of list
    *
    * @param list: list to find
    * @param element: to fill with second of list
    * @return void
    */
    static void getSecondElement(const Body& list, std::string& element);
};

class UNAFold::DetFileParser::ExternalRule : public Rule
{
private:
    virtual void calcAttrib(const Block& block, IMotifObserver::Motif& motif) const;
    virtual ~ExternalRule() {}
};

class UNAFold::DetFileParser::InteriorRule : public Rule
{
private:
    virtual void calcAttrib(const Block& block, IMotifObserver::Motif& motif) const;
    virtual ~InteriorRule() {}
};

class UNAFold::DetFileParser::HairpinRule : public Rule
{
private:
    virtual void calcAttrib(const Block& block, IMotifObserver::Motif& motif) const;
    virtual ~HairpinRule() {}
};

class UNAFold::DetFileParser::MultiRule : public Rule
{
private:
    virtual void calcAttrib(const Block& block, IMotifObserver::Motif& motif) const;
    virtual ~MultiRule() {}
};

class UNAFold::DetFileParser::BulgeRule : public Rule
{
private:
    virtual void calcAttrib(const Block& block, IMotifObserver::Motif& motif) const;
    virtual ~BulgeRule() {}
};