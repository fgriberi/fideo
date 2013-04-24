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

#include "fideo/IHybridize.h"

using namespace biopp;
using namespace mili;
using namespace std;

namespace fideo
{
class RNAHybrid : public IHybridize
{
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
        void parse(std::ifstream& file)
        {
            string temp;
            for (size_t i = 0; i < OBSOLETE_LINES; ++i)
            {
                getline(file, temp);
            }
            stringstream ss(temp);
            vector<string> result;
            ss >> Separator(result, ' ');
            if (result.size() != SIZE_LINE)
            {
                dG = OBSOLETE_dG; //no significant hybridization found
            }
            else
            {
                const string deltaG = result[DELTA_G];
                helper::convert_from_string(deltaG, dG);                
            }
        }

        Fe dG; ///free energy
        static const unsigned int OBSOLETE_dG = 1000; //no significant hybridization found
        static const unsigned int SIZE_LINE = 3;
        static const unsigned int DELTA_G = 1;
    };
};

REGISTER_FACTORIZABLE_CLASS(IHybridize, RNAHybrid, std::string, "RNAHybrid");

Fe RNAHybrid::hybridize(const biopp::NucSequence& longerSeq, bool longerCirc, const biopp::NucSequence& shorterSeq) const
{
    if (longerCirc)
    {
         throw UnsupportedException();
    }

	///Add obsolete description in sequence. RNAHybrid requires FASTA formatted file
    fileLine targetSequence = ">HeadToTargetSequence \n" + longerSeq.getString();
    fileLine querySequence = ">HeadToQuerySequence \n" + shorterSeq.getString();

    string fileTmpTarget;
    helper::createTmpFile(fileTmpTarget);
    string fileTmpQuery;
    helper::createTmpFile(fileTmpQuery);
    string fileTmpOutput;
    helper::createTmpFile(fileTmpOutput);

    helper::write(fileTmpTarget, targetSequence);
    helper::write(fileTmpQuery, querySequence);

    stringstream exec;
    exec << "RNAhybrid -s 3utr_human ";
    exec << "-t " << fileTmpTarget;
    exec << " -q " << fileTmpQuery;
    exec << " > " << fileTmpOutput;

    const command cmd = exec.str();  /// RNAhybrid -s 3utr_human -t fileRNAm -q filemiRNA > fileTmpOutput
    helper::runCommand(cmd);

    ifstream fileOutput(fileTmpOutput.c_str());
    if (!fileOutput)
    {
		throw NotFoundFileException();
    }
    BodyParser body;
    body.parse(fileOutput);

    helper::removeFile(fileTmpTarget);
    helper::removeFile(fileTmpQuery);
    helper::removeFile(fileTmpOutput);
    return body.dG;
}
} // namespace fideo
