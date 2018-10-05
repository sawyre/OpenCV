#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "iostream"

using namespace std;
using namespace cv;

void myBlur(const Mat &input, Mat &output, const Mat &test, const Size &ksize) {
	output = input.clone();
	uint sum[3] = { 0, 0, 0 };
	uint half_ROI_width = ksize.width / 2;
	uint half_ROI_height = ksize.height / 2;
	for (int i = half_ROI_width; i < (output.cols - half_ROI_width - 1); i++) {
		for (int j = half_ROI_height; j < (output.rows - half_ROI_height - 1); j++) {

			//сумма элементов RGB в квадрате 3x3
			for (int w = i - half_ROI_height; w <= i + half_ROI_height; w++) {
				for (int h = j - half_ROI_width; h <= j + half_ROI_width; h++) {
					sum[0] += output.at<Vec3b>(w, h)[0];
					sum[1] += output.at<Vec3b>(w, h)[1];
					sum[2] += output.at<Vec3b>(w, h)[2];
				}
			}
			output.at<Vec3b>(i, j)[0] = sum[0] / (ksize.width * ksize.height);
			output.at<Vec3b>(i, j)[1] = sum[1] / (ksize.width * ksize.height);
			output.at<Vec3b>(i, j)[2] = sum[2] / (ksize.width * ksize.height);

			/*
			cout << (uint)output.at<Vec3b>(i + half_ROI_width, j + half_ROI_height)[0] << ' ';
			cout << (uint)output.at<Vec3b>(i + half_ROI_width, j + half_ROI_height)[1] << ' ';
			cout << (uint)output.at<Vec3b>(i + half_ROI_width, j + half_ROI_height)[2] << '\n';
			*/

			sum[0] = 0;
			sum[1] = 0;
			sum[2] = 0;
			/*
			cout << (uint)test.at<Vec3b>(i + half_ROI_width, j + half_ROI_height)[0] << ' ';
			cout << (uint)test.at<Vec3b>(i + half_ROI_width, j + half_ROI_height)[1] << ' ';
			cout << (uint)test.at<Vec3b>(i + half_ROI_width, j + half_ROI_height)[2] << '\n' << '\n';
			*/
		}
	}
}

double dif(const Mat &first, const Mat &second) {
	uint sumDif = 0;
	for (int i = 1; i < (first.cols - 1); i++) {
		for (int j = 1; j < (first.rows - 1); j++) {
			sumDif += abs(first.at<Vec3b>(i, j)[0] - second.at<Vec3b>(i, j)[0]);
			sumDif += abs(first.at<Vec3b>(i, j)[1] - second.at<Vec3b>(i, j)[1]);
			sumDif += abs(first.at<Vec3b>(i, j)[2] - second.at<Vec3b>(i, j)[2]);
		}
	}
	return (double)sumDif/(260100*255*3);
}

int main(int argc, char** argv) {
	namedWindow("Original image");
	namedWindow("Standart Blur 3x3");
	namedWindow("Gaussian Blur 3x3");
	namedWindow("Median Blur 3x3");
	namedWindow("My Blur 3x3");
	
	double time_standart = 0;
	double time_gaussian = 0;
	double time_median = 0;
	double time_myBlur = 0;
	int64 e1 = 0;
	int64 e2 = 0;

	Mat noisyImage = imread("noisy.jpg");

	//Standart blur
	Mat blurImage3;
	e1 = getTickCount();
	blur(noisyImage, blurImage3, { 3 , 3 });
	e2 = getTickCount();
	time_standart = (e2 - e1) / getTickFrequency();

	//Gaussian blur
	Mat gaussianImage3;
	e1 = getTickCount();
	GaussianBlur(noisyImage, gaussianImage3, { 3 , 3 }, 0 , 0);
	e2 = getTickCount();
	time_gaussian = (e2 - e1) / getTickFrequency();

	//Median blur
	Mat medianImage3;
	e1 = getTickCount();
	medianBlur(noisyImage, medianImage3, 3);
	e2 = getTickCount();
	time_median = (e2 - e1) / getTickFrequency();

	//My blur
	Mat myBlurImage3;
	e1 = getTickCount();
	myBlur(noisyImage, myBlurImage3, blurImage3, { 3, 3 });
	e2 = getTickCount();
	time_myBlur = (e2 - e1) / getTickFrequency();

	imshow("Original image", noisyImage);
	imshow("Standart Blur 3x3", blurImage3);
	imshow("Gaussian Blur 3x3", gaussianImage3);
	imshow("Median Blur 3x3", medianImage3);
	imshow("My Blur 3x3", myBlurImage3);

	cout << "Difference between two methods in Pixels: " << dif(blurImage3, myBlurImage3) << '\n';
	cout << "Standart blur execution time: " << '\t' << time_standart << "s\n";
	cout << "Gaussian blur execution time: " << '\t' << time_gaussian << "s\n";
	cout << "Median blur execution time: " << '\t' << time_median << "s\n";
	cout << "My blur execution time: " << '\t' << time_myBlur << "s\n";

	waitKey(0);
	return 0;
}