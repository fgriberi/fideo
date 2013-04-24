/*
 * @file   RNAcofold.cpp
 * @brief  RNAcofold is the implementation of IHybridize interface. It's a specific backend to hybridize.
 *
 * @author Franco Riberi
 * @email  fgriberi AT gmail.com
 *
 * Contents:  Source file for fideo providing backend RNAcofold implementation.
 *
 * System:    fideo: Folding Interface Dynamic Exchange Operations
 * Language:  C++
 *
 * @date October 28, 2012, 10:32 AM
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
//Vienna package
class RNAcofold : public IHybridize
{
    virtual Fe hybridize(const biopp::NucSequence& longerSeq, bool longerCirc, const biopp::NucSequence& shorterSeq) const;

	///Class that allows parsing the body of a file
    class BodyParser
    {     
    public:      

		/** @brief Parse the line and get the value dG
         *
         * @param line: line to parser
         * @return void
         */
        void parse(string& line)
        {
            stringstream ss(line);
            vector<string> result;
            ss >> mili::Separator(result, ' ');
            if (result.size() != NumberOfColumns)
            {
                throw RNABackendException("Invalid output RNAcofold.");
            }
            const string deltaG = result[ColdG].substr(0, result[ColdG].size() - 1);
            helper::convert_from_string(deltaG, dG);    
        }

        Fe dG; ///free energy

    private:
        enum Columns
        {
            ColRNAcofoldResult,
            ColOpenParenthesis,
            ColdG,
            NumberOfColumns
        };
    };
};

REGISTER_FACTORIZABLE_CLASS(IHybridize, RNAcofold, std::string, "RNAcofold");

Fe RNAcofold::hybridize(const biopp::NucSequence& longerSeq, bool longerCirc, const biopp::NucSequence& shorterSeq) const
{
    if (longerCirc)
    {
        throw RNABackendException("Unsupported Sequence.");
    }
    const string seq1 = longerSeq.getString();
    const string seq2 = shorterSeq.getString();

    string inputTmpFile;
    helper::createTmpFile(inputTmpFile);
    string outputTmpFile;
    helper::createTmpFile(outputTmpFile);

    ofstream toHybridize(inputTmpFile.c_str());
    toHybridize << seq1 << "&" << seq2;
    toHybridize.close();

    stringstream exec;
    exec << "RNAcofold ";
    exec << "< " << inputTmpFile;
    exec << " > " << outputTmpFile;

    const command cmd = exec.str(); /// RNAcofold < inputTmpFile > outputTmpFile
    helper::runCommand(cmd);

    ifstream fileOutput(outputTmpFile.c_str());
    if (!fileOutput)
    {
        throw NotFoundFileException();
    }

    string temp;
    getline(fileOutput, temp);
    getline(fileOutput, temp);

    BodyParser body;
    body.parse(temp);
    helper::removeFile(inputTmpFile);
    helper::removeFile(outputTmpFile);
    return body.dG;
}
} // namespace fideo
