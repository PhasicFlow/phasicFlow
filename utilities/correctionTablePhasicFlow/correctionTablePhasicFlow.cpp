/*------------------------------- phasicFlow ---------------------------------
      O        C enter of
     O O       E ngineering and
    O   O      M ultiscale modeling of
   OOOOOOO     F luid flow
------------------------------------------------------------------------------
  Copyright (C): www.cemf.ir
  email: hamid.r.norouzi AT gmail.com
------------------------------------------------------------------------------
Licence:
  This file is part of phasicFlow code. It is a free software for simulating
  granular and multiphase flows. You can redistribute it and/or modify it under
  the terms of GNU General Public License v3 or any other later versions.

  phasicFlow is distributed to help others in their research in the field of
  granular and multiphase flows, but WITHOUT ANY WARRANTY; without even the
  implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

-----------------------------------------------------------------------------*/

#include <fstream>
#include <iomanip>
#include <vector>

#include "KokkosTypes.hpp"
#include "systemControl.hpp"
#include "commandLine.hpp"
#include "vocabs.hpp"
#include "dictionary.hpp"
#include "grainCorrectionTable.hpp"

using pFlow::output;
using pFlow::endl;
using pFlow::commandLine;
using pFlow::dictionary;
using pFlow::fileSystem;
using pFlow::Logical;
using pFlow::real;
using pFlow::realVector;
using pFlow::word;
using pFlow::wordList;

/// Writes the banner and the header entries of a phasicFlow dictionary
/// file (the same lines as IOfileHeader::writeHeader).
static void writeFileHeader(std::ofstream& os)
{
    os  << "/* -------------------------------*- C++ -*------------------"
        << "---------------- *\\\n"
        << "|  phasicFlow File                                           "
        << "                 |\n"
        << "|  copyright: www.cemf.ir                                    "
        << "                 |\n"
        << "\\* ----------------------------------------------------------"
        << "--------------- */\n"
        << "objectName  correctionTables;\n"
        << "objectType  dictionary;\n"
        << "fileFormat  ASCII;\n"
        << "/*-----------------------------------------------------------"
        << "----------------*/\n";
}

/// Writes the entry name with the values of v, five per line.
static void writeVector(
    std::ofstream&      os,
    const char*         name,
    const realVector&   v)
{
    os << "    " << name << "\n    (";

    for (size_t i = 0; i < v.size(); ++i)
    {
        if (i % 5 == 0) os << "\n       ";
        os << ' ' << v[i];
    }

    os << "\n    );\n";
}

int main( int argc, char* argv[] )
{

    commandLine cmds(
        "correctionTablePhasicFlow",
        "Builds the correction tables of the grain model for the reactions "
        "of caseSetup/thermoPhysicalInteraction that have correctionTable "
        "Yes, and writes them to caseSetup/correctionTables");

    if(!cmds.parse(argc, argv)) return 0;

// this should be palced in each main
#include "initialize.hpp"

    const fileSystem caseSetupDir(pFlow::caseSetupFolder__);

    dictionary thermoDict(
        "thermoPhysicalInteraction",
        caseSetupDir + "thermoPhysicalInteraction");

    const dictionary& rxnDict =
        thermoDict.subDict("multiReactiveProperties").subDict("reactions");

    wordList rxnNames = rxnDict.dictionaryKeywords();

    const fileSystem tableFile = caseSetupDir + "correctionTables";

    std::ofstream os(tableFile.wordPath());

    if (!os)
    {
        fatalErrorInFunction
            << "Cannot open file " << tableFile << " for writing." << endl;
        return 1;
    }

    writeFileHeader(os);

    os  << "// Written by correctionTablePhasicFlow. One table per reaction:\n"
        << "// ln c(sigma2, g, X) with the index (i*size(g) + j)*size(X)"
        << " + k.\n";

    os << std::setprecision(10);

    // Tables built so far, one per reaction order
    std::vector<real>       orders;
    std::vector<realVector> sigma2s, gs, Xs, lncs;

    pFlow::int32 numTables = 0;

    for (const auto& name : rxnNames)
    {
        const dictionary& rxn = rxnDict.subDict(name);

        const word model = rxn.getValOrSet<word>("particleModel", "none");
        const Logical useTable =
            rxn.getValOrSet<Logical>("correctionTable", Logical("No"));

        if (model != "grain" || !useTable) continue;

        const real order = rxn.getVal<real>("gasOrder");

        size_t t = 0;

        while (t < orders.size() && orders[t] != order) ++t;

        if (t == orders.size())
        {
            REPORT(0) << "\nBuilding the correction table for reaction order "
                << Yellow_Text(order) << " . . ." << END_REPORT;

            orders.push_back(order);
            sigma2s.emplace_back();
            gs.emplace_back();
            Xs.emplace_back();
            lncs.emplace_back();

            pFlow::buildGrainCorrectionTable(
                order, sigma2s[t], gs[t], Xs[t], lncs[t]);
        }

        os  << '\n' << name << "\n{\n"
            << "    reactionOrder " << order << ";\n";

        writeVector(os, "sigma2", sigma2s[t]);
        writeVector(os, "g", gs[t]);
        writeVector(os, "X", Xs[t]);
        writeVector(os, "lnc", lncs[t]);

        os << "}\n";

        REPORT(1) << "Table of reaction " << Green_Text(name)
            << " (reaction order " << order << ") is written."
            << END_REPORT;

        ++numTables;
    }

    os.close();

    REPORT(0) << "\n" << numTables << " correction table(s) written to "
        << Green_Text(tableFile) << END_REPORT;

// this should be palced in each main
#include "finalize.hpp"

    return 0;
}
