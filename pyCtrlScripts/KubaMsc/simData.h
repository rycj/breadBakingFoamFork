#include "string"
#include "array"
#include "vector"
class simData{
    public:
        void loadExp(std::vector<std::string> inFiles,int fromLine);
        void probeSim(std::vector<std::string>locs);
        void trimSimData();
        std::array<float,6> sumSq();

        std::string name;
        std::array<std::vector<float>,6> expData;
        std::array<std::vector<float>,6> simData;
};