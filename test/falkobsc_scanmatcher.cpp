#include <iostream>
#include <iomanip>
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

using namespace std;
using namespace falkolib;
using json = nlohmann::json;

namespace falkolib
{
	void to_json(json& j, const FALKO& keypoint)
	{
		j = {{"index", keypoint.index}, {"radius", keypoint.radius}, {"orientation", keypoint.orientation}};
	}
}

#define NEW_INPUT(VAR_TYPE, VAR_NAME)\
	VAR_TYPE VAR_NAME = json_in[#VAR_NAME];\
	std::cout << #VAR_NAME << ": " << VAR_NAME << std::endl;\
	json_out[#VAR_NAME] = VAR_NAME

int main(int argc, char** argv)
{
    json json_out;
    
	ifstream file_in("in.json");
	json json_in = json::parse(file_in);

	NEW_INPUT(double, MinExtractionRange);
	NEW_INPUT(double, MaxExtractionRange);
	NEW_INPUT(bool, EnableSubbeam);
	NEW_INPUT(double, NMSRadius);
	NEW_INPUT(double, NeighA);
	NEW_INPUT(double, NeighB);
	NEW_INPUT(double, BRatio);
	NEW_INPUT(int, GridSectors);

	NEW_INPUT(double, ScanAngleMin);
	NEW_INPUT(double, ScanFOV);
	NEW_INPUT(int, ScanNumBeams);

	NEW_INPUT(int, CircularSectorNumber);
	NEW_INPUT(int, RadialRingNumber);

	NEW_INPUT(double, DistanceThreshold);
	NEW_INPUT(double, DescriptorThreshold);

	std::vector<double> ScanRanges1(json_in["ScanRanges1"].begin(), json_in["ScanRanges1"].end());
	std::cout << "ScanRanges1 size: " << ScanRanges1.size() << std::endl;
	json_out["ScanRanges1"] = ScanRanges1;

	LaserScan scan1(ScanAngleMin, ScanFOV, ScanNumBeams);
	scan1.fromRanges(ScanRanges1);

	std::vector<double> ScanRanges2(json_in["ScanRanges2"].begin(), json_in["ScanRanges2"].end());
	std::cout << "ScanRanges2 size: " << ScanRanges2.size() << std::endl;
	json_out["ScanRanges2"] = ScanRanges2;

	LaserScan scan2(ScanAngleMin, ScanFOV, ScanNumBeams);
	scan2.fromRanges(ScanRanges2);

	FALKOExtractor fe;
	fe.setMinExtractionRange(MinExtractionRange);
	fe.setMaxExtractionRange(MaxExtractionRange);
	fe.enableSubbeam(EnableSubbeam);
	fe.setNMSRadius(NMSRadius);
	fe.setNeighA(NeighA);
	fe.setNeighB(NeighB);
	fe.setBRatio(BRatio);
	fe.setGridSectors(GridSectors);

	std::vector<FALKO> keypoints1;
	fe.extract(scan1, keypoints1);
	json_out["keypoints1"] = keypoints1;
	std::cout << "keypoints1: " << keypoints1.size() << std::endl;
	for(int i = 0; i < keypoints1.size(); ++i)
    {
		std::cout << keypoints1[i].index << ", " << keypoints1[i].radius << ", " << keypoints1[i].orientation << std::endl;
    }

	std::vector<FALKO> keypoints2;
	fe.extract(scan2, keypoints2);
	json_out["keypoints2"] = keypoints2;
	std::cout << "keypoints2: " << keypoints2.size() << std::endl;
	for(int i = 0; i < keypoints2.size(); ++i)
    {
		std::cout << keypoints2[i].index << ", " << keypoints2[i].radius << ", " << keypoints2[i].orientation << std::endl;
    }

	std::vector<falkolib::BSC> bscDesc1;
    falkolib::BSCExtractor<falkolib::FALKO> bsc1(CircularSectorNumber, RadialRingNumber);
    bsc1.compute(scan1, keypoints1, bscDesc1);

	std::vector<falkolib::BSC> bscDesc2;
    falkolib::BSCExtractor<falkolib::FALKO> bsc2(CircularSectorNumber, RadialRingNumber);
    bsc1.compute(scan2, keypoints2, bscDesc2);

	falkolib::NNMatcher<falkolib::FALKO, falkolib::BSC> matcherFALKOBSC;
	matcherFALKOBSC.setDistanceThreshold(DistanceThreshold);
	matcherFALKOBSC.setDescriptorThreshold(DescriptorThreshold);

	std::vector<std::pair<int, int>> asso;
	int matches = matcherFALKOBSC.match(keypoints1, bscDesc1, keypoints2, bscDesc2, asso);

	json_out["asso"] = asso;
	std::cout << "asso: " << asso.size() << std::endl;
	for(int i = 0; i < keypoints2.size(); ++i)
    {
		std::cout << asso[i].first << ", " << asso[i].second << std::endl;
    }

    std::ofstream file_out("out.json");
    file_out << json_out;

	return 0;
}

