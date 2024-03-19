#include <iostream>

#include <falkolib/Feature/FALKO.h>
#include <falkolib/Feature/CGH.h>
#include <falkolib/Feature/BSC.h>
#include <falkolib/Feature/FALKOExtractor.h>

#include <falkolib/Feature/BSCExtractor.h>
#include <falkolib/Feature/CGHExtractor.h>

#include <falkolib/Matching/NNMatcher.h>
#include <falkolib/Matching/AHTMatcher.h>

#include "test_data_sintetic.h"

using namespace std;
using namespace falkolib;

int main(int argc, char** argv) {
	FALKOExtractor fe;
	fe.setMinExtractionRange(0.5);
	fe.setMaxExtractionRange(7);
	fe.enableSubbeam(true);
	fe.setNMSRadius(0.1);
	fe.setNeighB(0.10);
	fe.setBRatio(8.0);
	fe.setGridSectors(32);

	LaserScan scan(- M_PI / 2., 1.0 * M_PI, 1000);
	scan.fromRanges(test_data_wall);

	std::vector<FALKO> keypoints;

	fe.extract(scan, keypoints);

	cout << "num keypoints extracted: " << keypoints.size() << endl;

	cout << "keypoints:" << endl;
	for(int i = 0; i < keypoints.size(); ++i)
    {
		cout << keypoints[i].index << " ";
    }
	cout << endl;
	cout << "keypoints end" << endl;

	return 0;
}

