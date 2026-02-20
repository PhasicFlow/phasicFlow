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

#include "systemControl.hpp"
#include "timeFolder.hpp"
#include "commandLine.hpp"
#include "ranges.hpp"
#include "pointStructure.hpp"
#include "pointFields.hpp"
#include "vtkFile.hpp"
#include <algorithm>

namespace
{

bool writeForceChainLegacyVTK(
    pFlow::systemControl& control,
    const pFlow::fileSystem& destPath,
    const pFlow::word& baseName,
    pFlow::word& filename)
{
    using namespace pFlow;

    const fileSystem forceChainDir = control.time().path() / word("forceChain");

    IOfileHeader pairsHeader(
        objectFile(
            "pairs",
            forceChainDir,
            objectFile::READ_ALWAYS,
            objectFile::WRITE_NEVER));

    if (!pairsHeader.headerOk(true))
    {
        REPORT(2) << "No force-chain data in " << Yellow_Text(control.time().path())
                  << ". Skipping this folder..." << END_REPORT;
        return true;
    }

    pointStructure pStruct(control, 0.0005);
    const auto posHost = pStruct.pointPositionHost();

    realx3PointField_H pairs(
        objectFile("pairs", forceChainDir, objectFile::READ_ALWAYS, objectFile::WRITE_NEVER),
        pStruct,
        zero3);

    uint32 validPairs = 0;
    bool pairCountAvailable = false;
    {
        IOfileHeader pairCountHeader(
            objectFile(
                "pairCount",
                forceChainDir,
                objectFile::READ_ALWAYS,
                objectFile::WRITE_NEVER));

        if (pairCountHeader.headerOk(true))
        {
            uint32PointField_H pairCount(
                objectFile("pairCount", forceChainDir, objectFile::READ_ALWAYS, objectFile::WRITE_NEVER),
                pStruct,
                0u);
            const auto counterHost = pairCount.hostView();
            validPairs = counterHost.size() > 0 ? counterHost[0] : 0u;
            pairCountAvailable = true;
        }
    }

    const auto pairsHost = pairs.hostViewAll();
    if(pairCountAvailable)
    {
        validPairs = std::min(validPairs, static_cast<uint32>(pairsHost.size()));
    }
    else
    {
        validPairs = static_cast<uint32>(pairsHost.size());
    }

    vtkFile vtk(destPath, baseName, control.time().currentTime(), false);
    if (!vtk)
    {
        return false;
    }

    filename = vtk.fileName().wordPath();

    auto& os = vtk();
    os << "DATASET POLYDATA\n";
    os << "POINTS " << pStruct.numActive() << " float\n";

    for (uint32 i = 0; i < pStruct.numActive(); ++i)
    {
        os << static_cast<float>(posHost[i].x()) << ' '
           << static_cast<float>(posHost[i].y()) << ' '
           << static_cast<float>(posHost[i].z()) << '\n';
    }

    uint32 writtenPairs = 0;
    for (uint32 p = 0; p < validPairs; ++p)
    {
        const auto& pair = pairsHost[p];
        const auto i = static_cast<uint32>(pair.x());
        const auto j = static_cast<uint32>(pair.y());
        if (i < pStruct.numActive() && j < pStruct.numActive())
        {
            ++writtenPairs;
        }
    }

    os << "LINES " << writtenPairs << ' ' << (3 * writtenPairs) << '\n';
    for (uint32 p = 0; p < validPairs; ++p)
    {
        const auto& pair = pairsHost[p];
        const auto i = static_cast<uint32>(pair.x());
        const auto j = static_cast<uint32>(pair.y());

        if (i >= pStruct.numActive() || j >= pStruct.numActive())
        {
            continue;
        }

        os << "2 " << i << ' ' << j << '\n';
    }

    os << "CELL_DATA " << writtenPairs << "\n";
    os << "SCALARS forceMagnitude float 1\n";
    os << "LOOKUP_TABLE default\n";
    for (uint32 p = 0; p < validPairs; ++p)
    {
        const auto& pair = pairsHost[p];
        const auto i = static_cast<uint32>(pair.x());
        const auto j = static_cast<uint32>(pair.y());
        if (i >= pStruct.numActive() || j >= pStruct.numActive())
        {
            continue;
        }
        os << static_cast<float>(pair.z()) << '\n';
    }

    return true;
}

} // namespace

int main(int argc, char** argv)
{
    pFlow::word outFolder = (pFlow::CWD() / pFlow::word("VTK/forceChain")).wordPath();

    pFlow::commandLine cmds(
        "forceChainToVTK",
        "Converts stored force-chain data in time folders to VTK polyline files.");

    pFlow::wordVector times;

    cmds.addOption("-o,--out-folder", outFolder, "path to output folder of force-chain VTK", "path");
    cmds.addOption(
        "-t,--time",
        times.vectorField(),
        "a space separated list of time folders, or a strided range begin:stride:end, or an interval begin:end",
        " ");

    if (!cmds.parse(argc, argv)) return 0;

    #include "initialize_Control.hpp"

    pFlow::timeFolder folders(Control);
    const auto destFolder = pFlow::fileSystem(outFolder);

    pFlow::realCombinedRange validRange;
    if (cmds.count("--time"))
    {
        if (!validRange.addRanges(times))
        {
            fatalExit;
        }
    }
    else
    {
        validRange.addIntervalRange(folders.startTime(), folders.endTime());
    }

    pFlow::word outputName;

    do
    {
        Control.time().setTime(folders.currentTime());
        if (!validRange.isMember(folders.currentTime())) continue;

        pFlow::output << "time: " << Cyan_Text(folders.currentTime()) << " s" << pFlow::endl;

        if (!writeForceChainLegacyVTK(Control, destFolder, "forceChain", outputName))
        {
            fatalExit;
        }

        pFlow::output << pFlow::endl;

    } while (folders++);

    pFlow::output << "\nFinished successfully.\n";

    #include "finalize.hpp"
    return 0;
}
