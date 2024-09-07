#include <iostream>
#include <fstream>

#include <falkolib/Feature/FALKO.h>
#include <falkolib/Feature/CGH.h>
#include <falkolib/Feature/BSC.h>
#include <falkolib/Feature/FALKOExtractor.h>

#include <falkolib/Feature/BSCExtractor.h>
#include <falkolib/Feature/CGHExtractor.h>

#include <falkolib/Matching/NNMatcher.h>
#include <falkolib/Matching/AHTMatcher.h>

#include <nlohmann/json.hpp>
#include "test_data_sintetic.h"

using json = nlohmann::json;

using namespace std;
using namespace falkolib;

int main(int argc, char** argv) {
	ifstream file("experiment.json");
	json jf = json::parse(file);
	std::cout << "MinExtractionRange: " << jf["MinExtractionRange"] << std::endl;
	std::cout << "MaxExtractionRange: " << jf["MaxExtractionRange"] << std::endl;
	std::cout << "EnableSubbeam: " << jf["EnableSubbeam"] << std::endl;
	std::cout << "NMSRadius: " << jf["NMSRadius"] << std::endl;
	std::cout << "NeighB: " << jf["NeighB"] << std::endl;
	std::cout << "BRatio: " << jf["BRatio"] << std::endl;
	std::cout << "GridSectors: " << jf["GridSectors"] << std::endl;
	std::cout << "ScanAngleMin: " << jf["ScanAngleMin"] << std::endl;
	std::cout << "ScanFOV: " << jf["ScanFOV"] << std::endl;
	std::cout << "ScanNumBeams: " << jf["ScanNumBeams"] << std::endl;
	std::cout << "ScanRanges size: " << jf["ScanRanges"].size() << std::endl;
	std::cout << "------------------------------------------------------------" << std::endl;

	FALKOExtractor fe;
	fe.setMinExtractionRange(jf["MinExtractionRange"]);
	fe.setMaxExtractionRange(jf["MaxExtractionRange"]);
	fe.enableSubbeam(jf["EnableSubbeam"]);
	fe.setNMSRadius(jf["NMSRadius"]);
	fe.setNeighB(jf["NeighB"]);
	fe.setBRatio(jf["BRatio"]);
	fe.setGridSectors(jf["GridSectors"]);

	LaserScan scan(jf["ScanAngleMin"], jf["ScanFOV"], jf["ScanNumBeams"]);
	std::vector<double> ranges(jf["ScanRanges"].begin(), jf["ScanRanges"].end());
	scan.fromRanges(ranges);

	std::vector<FALKO> keypoints;

	fe.extract(scan, keypoints);

	cout << "NumKeypoints: " << keypoints.size() << std::endl;
	cout << "Keypoints:";
	for(int i = 0; i < keypoints.size(); ++i)
    {
		cout << " " << keypoints[i].index;
    }
	cout << std::endl;

	return 0;
}

