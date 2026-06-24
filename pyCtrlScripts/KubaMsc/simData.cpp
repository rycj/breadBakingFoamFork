#include "simData.h"
#include "string"
#include "util.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>


void simData::loadExp(std::vector<std::string> inFiles,int fromLine){
    std::string loadedData;
    for (int i=0; i<inFiles.size(); i++){
        expData[i].clear();
        std::ifstream MyReadFile(inFiles[i]);
        while (getline (MyReadFile, loadedData)) {
            expData[i].push_back(std::stof(loadedData));
        }
    }
}
void simData::probeSim(std::vector<std::string>locs){
    std::string locsString="(";
    for (int i=0;i<locs.size();i++){
        locsString+=locs[i];
        }
        locsString+=")";
        replace({"../../ZZ_cases/00_breads/"+name+"/system/probeField","locationPlaceholder",locsString});
        runCmd("bash -c 'source /usr/lib/openfoam/openfoam2312/etc/bashrc && postProcess -case ../../ZZ_cases/00_breads/"+name+" -func probeField -dict system/probeField' >postProc.txt");
        std::vector<std::string> loadedData;
        std::string buf;
        std::ifstream MyReadFile("../../ZZ_cases/00_breads/"+name+"/postProcessing/probeField/0/T");
        while (getline (MyReadFile, buf)) {
            loadedData.push_back(buf);
        }
        std::vector<std::string> line;
        for (int i=6;i<loadedData.size();i++){
            std::istringstream iss(loadedData[i]);
            float time;
            iss >> time;  // first column
            float value;
            int j = 0;
            while (iss >> value) {
                simData[j].push_back(value);
                j++;
            }
        }
}

void simData::trimSimData(){
    for (int i=0; i<5;i++){
        std::cout<<simData[i].size()<<"VS"<<expData[i].size()<<"\n";
        if (simData[i].size()<expData[i].size()){
            std::cout<<"give me more sim data! problem in set "<<i<<"\n";
        }
        else{
            if(abs(std::floor(simData[i].size()/expData[i].size())-(simData[i].size()/expData[i].size()))<0.001)
            {
                std::vector<float> simDataShort;

                for (int j=0;j<simData[i].size();j=j+std::floor(simData[i].size()/expData[i].size()))
                {
                simDataShort.push_back(simData[i][j]);
                }
                simData[i]=simDataShort;
            }
            else{
                std::cout<<"wrong lengths of data! "<<i;
            }

        }

    }
}


std::array<float,6> simData::sumSq(){
    std::array<float,6> sums;
    for (int i=0;i<simData.size();i++){
        float sum=0;
        for (int j=0;j<simData[i].size();j++){
            sum+=pow(simData[i][j]-expData[i][j],2);
        }
        sums[i]=sum;
    }
    return(sums);
}

