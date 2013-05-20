/*
 * @file   RANHybrid.cpp
 * @brief  RANHybrid is the implementation of IHybridize interface. It's a specific backend to hybridize.
 *
 * @author Franco Riberi
 * @email  fgriberi AT gmail.com
 *
 * Contents:  Source file for fideo providing backend RANHybrid implementation.
 *
 * System:    fideo: Folding Interface Dynamic Exchange Operations
 * Language:  C++
 *
 * @date November 02, 2012, 19:35 PM
 *
 * Copyright (C) 2012 Franco Riberi, FuDePAN
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with fideo. If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <etilico/etilico.h> 
#include "fideo/IHybridize.h"

namespace fideo
{
class RNAHybrid : public IHybridize
{
private:
    virtual Fe hybridize(const biopp::NucSequence& longerSeq, bool longerCirc, const biopp::NucSequence& shorterSeq) const;
    static const unsigned int OBSOLETE_LINES = 6;

	///Class that allows parsing the body of a file
    class BodyParser
    {
    public: 
 		/** @brief Parse the file and get the value dG
         *
         * @param file: file to parser
         * @return void
         */      
        void parse(std::ifstream& file);
        
        Fe dG; ///free energy
        static const unsigned int OBSOLETE_dG = 1000; //no significant hybridization found
        static const unsigned int SIZE_LINE = 3;
        static const unsigned int DELTA_G = 1;
    };
};

void RNAHybrid::BodyParser::parse(std::ifstream& file)
{
    std::string temp;
    for (size_t i = 0; i < OBSOLETE_LINES; ++i)
    {
        getline(file, temp);
    }
    std::stringstream ss(temp);
    std::vector<std::string> result;
    ss >> mili::Separator(result, ' ');
    if (result.size() != SIZE_LINE)
    {
        dG = OBSOLETE_dG; //no significant hybridization found
    }
    else
    {
        const std::string deltaG = result[DELTA_G];
        helper::convertFromString(deltaG, dG);                
    }
}

REGISTER_FACTORIZABLE_CLASS(IHybridize, RNAHybrid, std::string, "RNAHybrid");

Fe RNAHybrid::hybridize(const biopp::NucSequence& longerSeq, bool longerCirc, const biopp::NucSequence& shorterSeq) const
{
    if (longerCirc)
    {
         throw UnsupportedException();
    }

	///Add obsolete description in sequence. RNAHybrid requires FASTA formatted file
    FileLine targetSequence = ">HeadToTargetSequence \n" + longerSeq.getString();
    FileLine querySequence = ">HeadToQuerySequence \n" + shorterSeq.getString();

    std::string fileTmpTarget;
    etilico::createTemporaryFilename(fileTmpTarget);
    std::string fileTmpQuery;
    etilico::createTemporaryFilename(fileTmpQuery);
    std::string fileTmpOutput;
    etilico::createTemporaryFilename(fileTmpOutput);

    helper::write(fileTmpTarget, targetSequence);
    helper::write(fileTmpQuery, querySequence);

    std::stringstream exec;
    exec << "RNAhybrid -s 3utr_human ";
    exec << "-t " << fileTmpTarget;
    exec << " -q " << fileTmpQuery;
    exec << " > " << fileTmpOutput;

    const etilico::Command cmd = exec.str();  /// RNAhybrid -s 3utr_human -t fileRNAm -q filemiRNA > fileTmpOutput
    etilico::runCommand(cmd);

    std::ifstream fileOutput(fileTmpOutput.c_str());
    if (!fileOutput)
    {
		throw NotFoundFileException();
    }
    BodyParser body;
    body.parse(fileOutput);

    mili::assert_throw<ExceptionUnlink>(unlink(fileTmpTarget.c_str()) == 0);
    mili::assert_throw<ExceptionUnlink>(unlink(fileTmpQuery.c_str()) == 0);
    mili::assert_throw<ExceptionUnlink>(unlink(fileTmpOutput.c_str()) == 0);
    
    return body.dG;
}
} // namespace fideo