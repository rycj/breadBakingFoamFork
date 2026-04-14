#include "string"
#include "array"
#include "vector"
#include <iostream>
#include <fstream>
#include <sys/stat.h>

std::vector<std::string> tokenize(std::string s, std::string del)
{
    int start, end = -1*del.size();
    std::vector<std::string> tokenArr;
    do {
        start = end + del.size();
        end = s.find(del, start);
        tokenArr.push_back(s.substr(start, end - start));
    } while (end != -1);
    return tokenArr;
}
inline void runCmd(std::string command){
    std::system(command.c_str());
}

void PrepHotRoomCase(std::string newName,std::string oldName){
    struct stat buffer;
    std::string name="../../ZZ_cases/00_breads";

    if (!stat (name.c_str(), &buffer) == 0){
        std::cout<<"I don't exist";
        runCmd("mkdir ../../ZZ_cases");
        runCmd("mkdir ../../ZZ_cases/00_breads");
    }
    runCmd("rm ../../ZZ_cases/00_breads/"+newName+" -r");
    runCmd("cp ../../tutorials/"+oldName+" ../../ZZ_cases/00_breads -r");
    runCmd("mv ../../ZZ_cases/00_breads/"+oldName+" ../../ZZ_cases/00_breads/"+newName);
    }

void replace(std::array<std::string,3> repl){


    std::ifstream inFile(repl[0]);
    std::vector<std::string> linesInFl;
    std::string line;

    while (std::getline(inFile, line)) {
        linesInFl.push_back(line);
    }
    inFile.close();

    // Process lines
    for (size_t lnI = 0; lnI < linesInFl.size(); lnI++) {
        if (linesInFl[lnI].find(repl[1]) != std::string::npos) {

            size_t pos = 0;
            while ((pos = linesInFl[lnI].find(repl[1], pos)) != std::string::npos) {
                linesInFl[lnI].replace(pos, repl[1].length(), repl[2]);
                pos += repl[2].length();
            }
        }
    }

    // Write file back
    std::ofstream outFile(repl[0]);

    for (const auto& outLine : linesInFl) {
        outFile << outLine << '\n';
    }

}
