#include "filter.h"

int main(int argc, char **argv) {

	path imagesFolderPath(argv[1]);
	path outputFolderPath(argv[2]);

	vector<string> fileNames;

	directory_iterator end_itr;
	directory_iterator itr(imagesFolderPath);
	for (directory_iterator itr(imagesFolderPath); itr != end_itr; ++itr) {
		string bmpCheck = itr->path().extension().string();
		if (bmpCheck == ".bmp" || bmpCheck == ".BMP") {
			string fileName = itr->path().filename().string();
			fileNames.push_back(fileName);
		}
	}

	sort(fileNames.begin(), fileNames.end());

	vector<ImageHist> vectorOfStructs;

	calculateHistogramsTxt(fileNames, vectorOfStructs);
	calculateDistancesTxt(vectorOfStructs);

	return 0;
}


