#include "simData.h"
#include "util.h"
#include "string"
#include <iostream>


int main(){
simData data;
std::string caseName="lol";
data.name=caseName;
bool runSim=true;
if (runSim){
    PrepHotRoomCase(caseName,"heaterTest");
    runCmd("bash -c 'source /usr/lib/openfoam/openfoam2312/etc/bashrc && cd ../../ZZ_cases/00_breads/"+caseName+" && ./Allclean && ./Allrun'");
}
runCmd("bash -c 'source /usr/lib/openfoam/openfoam2312/etc/bashrc && cd ../../ZZ_cases/00_breads/"+caseName+" && intHeat>log.intHeat'");
data.loadExp({"../expData/1.txt","../expData/1.txt","../expData/1.txt","../expData/1.txt","../expData/1.txt"},0);
data.probeSim({"(0 0 0)","(0.1 0.1 0.1)","(0.2 0.2 0.2)","(0.3 0.3 0.3)","(0.25 0.25 0.25)"});
data.trimSimData();
std::array<float,5> sums=data.sumSq();
for (int i=0;i<5;i++){
    std::cout<<sums[i]<<"\n";
}

return 1;
}