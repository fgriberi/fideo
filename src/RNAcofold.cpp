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
///Vienna package
class RNAcofold : public IHybridize
{
    virtual Fe hybridize(const biopp::NucSequence& longerSeq, const biopp::NucSequence& shorterSeq, bool longerCirc) const;

    ///Class that allows parsing the body of a file
    class ParseBody
    {
        enum Columns
        {
            ColRNAcofoldResult,
            ColOpenParenthesis,
            ColdG,
            NumberOfColumns
        };

    public:
        Fe dG; ///free energy

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
                throw RNABackendException("Invalid output RNAcofold.");
            const string deltaG = result[ColdG].substr(0, result[ColdG].size() - 1);
            if (!from_string(deltaG, dG))
                throw RNABackendException("Failed to convert the string to value type.");
        }
    };
};

REGISTER_FACTORIZABLE_CLASS(IHybridize, RNAcofold, std::string, "RNAcofold");

Fe RNAcofold::hybridize(const biopp::NucSequence& longerSeq, const biopp::NucSequence& shorterSeq, bool longerCirc) const
{
    if (longerCirc)
        throw UnsupportedException();
    const string seq1 = longerSeq.getString();
    const string seq2 = shorterSeq.getString();

    string inputTmpFile;
    helper::createTmpFile(inputTmpFile);
    string outputTmpFile;
    helper::createTmpFile(outputTmpFile);

    ///Constructed as required by RNAcofold
    ofstream toHybridize(inputTmpFile.c_str());
    toHybridize << seq1 << "&" << seq2;
    toHybridize.close();

    stringstream command;
    command << "RNAcofold ";
    command << "< " << inputTmpFile;
    command << " > " << outputTmpFile;

    const Command cmd = command.str(); /// RNAcofold < inputTmpFile > outputTmpFile
    helper::runCommand(cmd);

    ifstream fileOutput(outputTmpFile.c_str());
    if (!fileOutput)
        throw NotFoundFileException();

    string temp;
    getline(fileOutput, temp);
    getline(fileOutput, temp);

    ParseBody body;
    body.parse(temp);
    helper::removeFile(inputTmpFile);
    helper::removeFile(outputTmpFile);
    return body.dG;
}
} // namespace fideo
