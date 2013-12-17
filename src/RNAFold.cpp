/*
 * @file   RNAFold.cpp
 * @brief  RNAFold is the implementation of IFold interface. It's a specific backend to folding.
 *
 * @author Santiago Videla
 * @email  santiago.videla AT gmail.com
 *
 * @author Franco Riberi
 * @email  fgriberi AT gmail.com
 *
 * Contents:  Source file for fideo providing backend RNAFold implementation.
 *
 * System:    fideo: Folding Interface Dynamic Exchange Operations
 * Language:  C++
 *
 * @date November 10, 2010, 2012 4:26 PM
 *
 * Copyright (C) 2010  Santiago Videla and Franco Riberi, Franco Riberi, FuDePAN
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

#include <stack>
#include <etilico/etilico.h>
#include "fideo/FideoStructureParser.h"
#include "fideo/IFold.h"

namespace fideo
{
//Vienna package
class RNAFold : public IFold
{
private:

    virtual Fe fold(const biopp::NucSequence& seqRNAm, const bool isCircRNAm, biopp::SecStructure& structureRNAm);
    virtual Fe fold(const biopp::NucSequence& seqRNAm, const bool isCircRNAm, biopp::SecStructure& structureRNAm, IMotifObserver* motifObserver);

    /** @brief Read free energy of line
    *
    * @param file: to read
    * @param energy: to fill with free energy
    * @return index of first ")" in file
    */
    size_t readFreeEnergy(FileLine& file, size_t offset, Fe& energy) const;

};

REGISTER_FACTORIZABLE_CLASS(IFold, RNAFold, std::string, "RNAFold");

size_t RNAFold::readFreeEnergy(FileLine& line, size_t offset, Fe& energy) const
{
    try
    {
        const size_t from = mili::ensure_found(line.find_first_of("(", offset)) + 1;
        const size_t to = mili::ensure_found(line.find_first_of(")", from)) - 1;
        helper::readValue(line, from, to - from, energy);
        return to;
    }
    catch (const mili::StringNotFound& e)
    {
        throw RNABackendException("Could not read free energy");
    }
}

Fe RNAFold::fold(const biopp::NucSequence& seqRNAm, const bool isCircRNAm, biopp::SecStructure& structureRNAm, IMotifObserver* motifObserver)
{
    return 0; //temporal
}

Fe RNAFold::fold(const biopp::NucSequence& seqRNAm, const bool isCircRNAm, biopp::SecStructure& structureRNAm)
{
    structureRNAm.clear();
    structureRNAm.set_circular(isCircRNAm);
    FileLine sseq = seqRNAm.getString();

    std::string fileInput;
    etilico::createTemporaryFile(fileInput);
    std::string fileOutput;
    etilico::createTemporaryFile(fileOutput);

    helper::write(fileInput, sseq);
    std::stringstream ss;
    ss << "RNAfold" << " --noPS ";
    if (isCircRNAm)
    {
        ss << "--circ ";
    }
    ss << "< " << fileInput << " > " << fileOutput;

    const etilico::Command cmd = ss.str(); /// RNAfold -noPS ("" | -circ) < fileInput > fileOutput
    etilico::runCommand(cmd);

    /* output file look like this:
     * CGCAGGGAUCGCAGGUACCCCGCAGGCGCAGAUACCCUA
     * ...(((((((....(..((.....))..).))).)))). (-10.80)
     */
    FileLine aux;
    helper::readLine(fileOutput, ViennaParser::LINE_NO, aux);

    std::string str;
    helper::readValue(aux, 0, seqRNAm.length(), str);
    ViennaParser::parseStructure(str, structureRNAm);

    Fe energy;
    readFreeEnergy(aux, seqRNAm.length(), energy);

    mili::assert_throw<ExceptionUnlink>(unlink(fileInput.c_str()) == 0);
    mili::assert_throw<ExceptionUnlink>(unlink(fileOutput.c_str()) == 0);

    return energy;
}
} //namespace fideo

