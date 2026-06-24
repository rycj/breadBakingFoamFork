# MUST CHANGE FILE NAME
import numpy as np
import re
import os
from OF_caseClass import OpenFOAMCase
import matplotlib.pyplot as plt
from pathlib import Path


def loadData(filePath: str, tempInC: bool = False) -> np.ndarray:
    """Reads data from filePath and returns it as a numpy array. The file is assumed to have one float per line."""
    a = np.loadtxt(filePath).transpose()
    if tempInC:
        for i in range(len(a)):
            a[i] += 273.15
    return a


def loadExperiments() -> np.ndarray:
    """Loads all experimental data from the ../expData directory and returns a 2D numpy array. Assumes that each file contains one column of data. Sort the files in the directory by name, so that they are in the same order as the corresponding simulation probe locations."""
    expData = np.array(
        [
            loadData("../expData/" + series, True)
            for series in sorted(os.listdir("../expData"))
        ]
    )
    return expData


def probeSim(field: str, locs: list[str], case: OpenFOAMCase) -> None:
    """Probes the simulation at the specified locations with OpenFOAM probe utility. The locations should be in the format "(x y z)"."""
    locsString = "("
    for i in range(len(locs)):
        locsString += locs[i]
    locsString += ")"
    case.replace(
        [
            [
                "system/probeField",
                ["locationPlaceholder", "fieldPlaceholder"],
                [locsString, field],
            ]
        ]
    )
    case.runCommands(["postProcess -func probeField -dict system/probeField"])
    case.replace(
        [
            [
                "system/probeField",
                [locsString, field],
                ["locationPlaceholder", "fieldPlaceholder"],
            ]
        ]
    )


def loadSim(case: OpenFOAMCase, field: str) -> np.ndarray:
    """Loads simulation data from case"""
    data = np.loadtxt(
        case.dir + "/postProcessing/probeField/0/" + field,
        skiprows=3,  # IMPORTANT!!! this should be probe count +1, temporarily set directly
    ).transpose()
    return data


def linInterpol(a: float, b: float, ratio: float) -> float:
    """Linear interpolation between a and b with ratio. ratio should be between 0 and 1."""
    return a - (a - b) * ratio


def trimData(expData: np.ndarray, simData: np.ndarray) -> np.ndarray:
    """Trims the longer of expData and simData to match the length of the shorter one using linear interpolation. Assumes start and end times to be the same for both."""
    if len(simData[0] != len(expData[0])):
        expLong: bool = len(simData[0]) < len(expData[0])
        if expLong:
            data = [simData, expData]
        else:
            data = [expData, simData]
        dataShortened = np.zeros([len(simData), min(len(simData[0]), len(expData[0]))])

        for probe in range(len(data[1])):
            nLong = len(data[1][probe])
            nShort = len(data[0][probe])
            dataTemp = np.zeros([len(data[0][probe])])
            dataTemp[0] = data[1][probe][0]
            for datI in range(1, len(dataTemp) - 1):
                dataTemp[datI] = linInterpol(
                    data[1][probe][int(np.floor(datI * (nLong / nShort)))],
                    data[1][probe][int(np.floor(datI * (nLong / nShort)) + 1)],
                    (datI * (nLong / nShort)) - np.floor(datI * (nLong / nShort)),
                )
            dataTemp[-1] = data[1][probe][-1]
            dataShortened[probe] = dataTemp
        data[1] = dataShortened
        if expLong:
            return [data[1], data[0]]
        else:
            return data
    else:
        return [expData, simData]


def runSingleSim(baseCaseDir: str, targetDir: str, runSim: bool):
    case = OpenFOAMCase()
    case.loadOFCaseFromBaseCase("../../tutorials/" + baseCaseDir)
    case.changeOFCaseDir("../../ZZ_cases" + targetDir)
    if runSim:
        case.copyBaseCase()
        case.runCommands(["./Allclean", "./Allrun"])
    expData: np.ndarray = loadExperiments()
    probeSim(
        "T",
        [
            "(0.576 0 0.320)",
            "(0.057 0.071 0.595)",
            "(0.536 0.097 0.580)",
            "(0.056 0.074 0.058)",
            "(0.544 0.082 0.062)",
            "(0.024 0.150 0.572)",
        ],
        case,
    )
    simData: np.ndarray = loadSim(case, "T")
    simData = simData[1:]
    expData, simData = trimData(expData, simData)
    plotResults(expData, simData)


def plotResults(expData: np.ndarray, simData: np.ndarray):
    xs = range(len(expData[0]))
    for i in enumerate(simData):
        plt.plot(xs, expData[i[0]], xs, i[1])
        plt.show()


runSingleSim("ovenTest", "/00_breads/lol", True)
