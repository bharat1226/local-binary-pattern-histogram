#include <opencv2/core/core.hpp> 
#include <opencv2/highgui/highgui.hpp> 
#include <boost/filesystem.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>


using namespace boost::filesystem;
using namespace cv;
using namespace std;

struct ImageHist {
	string filename;
	double hist[10];
};

void imageLBPfunction(Mat& boundaryImage, Mat& imageLBP);
void labelAndOnesCalculation(int& numOfOnes, int& temp1, int& counter, int arr[8]);
void calculateHistogramsTxt(vector<string>& fileNames, vector<ImageHist>& vectorOfStructs);
void calculateDistancesTxt(vector<ImageHist>& vectorOfStructs);

void imageLBPfunction(Mat& boundaryImage, Mat& imageLBP) {
	int arr[8];
	for (int i = 1; i < boundaryImage.rows - 1; i++) {
		for (int j = 1; j < boundaryImage.cols - 1; j++) {

			int boundaryImagePixel, comparablePixel;
			boundaryImagePixel = boundaryImage.at<uchar>(i, j);

			// I can't pass by reference arrays. So I did it manually.------------------------

			comparablePixel = boundaryImage.at<uchar>(i - 1, j);
			if (comparablePixel > boundaryImagePixel) {
				arr[0] = 1;
			}
			else {
				arr[0] = 0;
			}

			comparablePixel = boundaryImage.at<uchar>(i - 1, j + 1);
			if (comparablePixel > boundaryImagePixel) {
				arr[1] = 1;
			}
			else {
				arr[1] = 0;
			}

			comparablePixel = boundaryImage.at<uchar>(i, j + 1);
			if (comparablePixel > boundaryImagePixel) {
				arr[2] = 1;
			}
			else {
				arr[2] = 0;
			}

			comparablePixel = boundaryImage.at<uchar>(i + 1, j + 1);
			if (comparablePixel > boundaryImagePixel) {
				arr[3] = 1;
			}
			else {
				arr[3] = 0;
			}

			comparablePixel = boundaryImage.at<uchar>(i + 1, j);
			if (comparablePixel > boundaryImagePixel) {
				arr[4] = 1;
			}
			else {
				arr[4] = 0;
			}

			comparablePixel = boundaryImage.at<uchar>(i + 1, j - 1);
			if (comparablePixel > boundaryImagePixel) {
				arr[5] = 1;
			}
			else {
				arr[5] = 0;
			}

			comparablePixel = boundaryImage.at<uchar>(i, j - 1);
			if (comparablePixel > boundaryImagePixel) {
				arr[6] = 1;
			}
			else {
				arr[6] = 0;
			}

			comparablePixel = boundaryImage.at<uchar>(i - 1, j - 1);
			if (comparablePixel > boundaryImagePixel) {
				arr[7] = 1;
			}
			else {
				arr[7] = 0;
			}

			int temp1 = arr[0];
			int numOfOnes = 0;
			if (temp1 == 1) {
				numOfOnes = 1;
			}

			int counter = 0;
			labelAndOnesCalculation(numOfOnes, temp1, counter, arr);

			//Determine label
			int label;
			if (counter <= 2) {
				label = numOfOnes;
			}
			else {
				label = 9;
			}
			imageLBP.at<uchar>(i - 1, j - 1) = label;
		}
	}
}

void labelAndOnesCalculation(int& numOfOnes, int& temp1, int& counter, int arr[8]) {
	for (int i = 0; i < 7; i++) {
		if (temp1 != arr[i + 1]) {
			counter++;
			temp1 = arr[i + 1];
		}
		else {
			temp1 = arr[i];
		}
		// transition from 7 to 0
		if (i == 6) {
			if (arr[i + 1] != arr[i - 6]) {
				counter++;
			}
		}
		// number of ones
		if (arr[i + 1] == 1) {
			numOfOnes++;
		}
	}
}

void calculateHistogramsTxt(vector<string>& fileNames, vector<ImageHist>& vectorOfStructs) {
	for (int i = 0; i < fileNames.size(); i++) {
		string fileName = "./inputDir/";
		fileName.append(fileNames.at(i));
		Mat inputImage = imread(fileName, IMREAD_GRAYSCALE);
		Mat boundaryImage = Mat::zeros(Size(inputImage.cols + 2, inputImage.rows + 2), CV_8UC1);
		Mat imageLBP = Mat::zeros(Size(inputImage.cols, inputImage.rows), CV_8UC1);
		for (int i = 1; i < boundaryImage.rows - 1; i++) {
			for (int j = 1; j < boundaryImage.cols - 1; j++) {
				int temp = inputImage.at<uchar>(i - 1, j - 1);
				boundaryImage.at<uchar>(i, j) = temp;
			}
		}

		imageLBPfunction(boundaryImage, imageLBP);

		double histogram[10] = { 0,0,0,0,0,0,0,0,0,0 };
		int label;

		for (int i = 0; i < imageLBP.rows; i++) {
			for (int j = 0; j < imageLBP.cols; j++) {
				label = imageLBP.at<uchar>(i, j);
				for (int k = 0; k < 10; k++) {
					if (label == k) {
						histogram[k] += 1;
					}
				}
			}
		}

		double total = imageLBP.total();

		for (int i = 0; i < 10; i++) {
			histogram[i] = histogram[i] / total;
		}

		string histogramTextFile = "./outputDir/HISTOGRAMS.txt";

		std::ofstream outputTextFile;
		outputTextFile.precision(6);
		outputTextFile.open(histogramTextFile, ios::app);
		outputTextFile << fileNames.at(i) << " ";

		for (int i = 0; i < 10; i++) {
			outputTextFile << histogram[i] << " ";
		}
		outputTextFile << endl;
		outputTextFile.close();

		ImageHist lbp;
		lbp.filename = fileNames.at(i);
		for (int i = 0; i < 10; i++) {
			lbp.hist[i] = histogram[i];
		}
		vectorOfStructs.push_back(lbp);
	}
}

void calculateDistancesTxt(vector<ImageHist>& vectorOfStructs) {
	double targetHist[10];
	for (int i = 0; i < vectorOfStructs.size(); i++) {
		if (vectorOfStructs[i].filename == "droid_1.bmp") {
			for (int j = 0; j < 10; j++) {
				targetHist[j] = vectorOfStructs[i].hist[j];
			}
		}
	}

	std::ofstream outputDistanceFile;
	outputDistanceFile.precision(6);
	string outputDistance = "./outputDir/DISTANCES.txt";

	for (int i = 0; i < vectorOfStructs.size(); i++) {
		double distance = 0;
		for (int j = 0; j < 10; j++) {
			distance += fabs(vectorOfStructs[i].hist[j] - targetHist[j]);
		}
		outputDistanceFile.open(outputDistance, ios::app);
		outputDistanceFile << vectorOfStructs[i].filename << " " << distance << endl;
		outputDistanceFile.close();
	}
}