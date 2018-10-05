#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "iostream"

using namespace std;
using namespace cv;

void myBlur(const Mat &input, Mat &output, const Size &ksize) {
	Mat integralSum;
	Mat integralSqsum;
	Mat integralTilted;
	integral(input, integralSum, integralSqsum, integralTilted);
	imshow("Sum", integralSum);
	imshow("Sqsum", integralSqsum);
	imshow("Tilted", integralTilted);

	Rect regOfInterest;
	Mat ROI;
	output = input.clone();

	uint sum[3] = { 0, 0, 0 };
	uint half_ROI_width = ksize.width / 2;
	uint half_ROI_height = ksize.height / 2;
	Rect sub(half_ROI_width, half_ROI_height, (output.cols - ksize.width - 1), (output.rows - ksize.height - 1));
	Mat subImage = output(sub);

	MatIterator_<Vec3b> it, end;
	MatIterator_<Vec3b> it_ROI, end_ROI;
	int i = 0;
	int j = 0;

	for (it = subImage.begin<Vec3b>(), end = subImage.end<Vec3b>(); it != end; ++it) {
		regOfInterest = Rect(i % subImage.cols, j / subImage.rows, ksize.width, ksize.height);
		i++;
		j++;
		ROI = input(regOfInterest);

		//сумма элементов RGB в квадрате 3x3
		for (it_ROI = ROI.begin<Vec3b>(), end_ROI = ROI.end<Vec3b>(); it_ROI != end_ROI; ++it_ROI) {
			sum[0] += (*it_ROI)[0];
			sum[1] += (*it_ROI)[1];
			sum[2] += (*it_ROI)[2];
		}
		(*it)[0] = sum[0] / (ksize.width * ksize.height);
		(*it)[1] = sum[1] / (ksize.width * ksize.height);
		(*it)[2] = sum[2] / (ksize.width * ksize.height);

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

	//My blur
	Mat myBlurImage3;
	e1 = getTickCount();
	myBlur(noisyImage, myBlurImage3, { 5, 5 });
	e2 = getTickCount();
	time_myBlur = (e2 - e1) / getTickFrequency();

	//Standart blur
	Mat blurImage3;
	e1 = getTickCount();
	blur(noisyImage, blurImage3, { 5 , 5 });
	e2 = getTickCount();
	time_standart = (e2 - e1) / getTickFrequency();

	//Gaussian blur
	Mat gaussianImage3;
	e1 = getTickCount();
	GaussianBlur(noisyImage, gaussianImage3, { 7 , 7 }, 0 , 0);
	e2 = getTickCount();
	time_gaussian = (e2 - e1) / getTickFrequency();

	//Median blur
	Mat medianImage3;
	e1 = getTickCount();
	medianBlur(noisyImage, medianImage3, 7);
	e2 = getTickCount();
	time_median = (e2 - e1) / getTickFrequency();

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