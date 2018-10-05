#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "vector"
#include "iostream"

using namespace std;
using namespace cv;

class Allababah {
public:
	//Constructor
	Allababah(const Mat &input1, const Mat &input2, const Mat&input3, int thr, int thr1, int thr2) :
		bam1(input1.clone()), bam2(input2.clone()), bam3(input3.clone()),
		thres(thr), thres1(thr1), thres2(thr2) {
	}
	//Copy constructor
	Allababah(const Allababah &rsh) :
		bam1(rsh.bam1.clone()), bam2(rsh.bam2.clone()), bam3(rsh.bam3.clone()),
		thres(rsh.thres), thres1(rsh.thres1), thres2(rsh.thres2) {
	}
	//Default constructor
	Allababah() {
	}

	~Allababah() {};
	Mat bam1;
	Mat bam2;
	Mat bam3;
	int thres;
	int thres1;
	int thres2;
};

void show_target(const Mat &img, Moments &mnts, int thres, int thres1, int thres2, string str) {
	Mat img_binary;
	threshold(img, img_binary, thres, 255, THRESH_BINARY_INV);
	Mat canny;
	Canny(img_binary, canny, thres1, thres2);

	vector<vector<Point>> bam1_cnts;
	findContours(canny, bam1_cnts, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);

	// you could also reuse img1 here
	Mat mask(canny.rows, canny.cols, CV_8UC1, Scalar(255));
	// CV_FILLED fills the connected components found
	drawContours(mask, bam1_cnts, -1, Scalar(0), CV_FILLED);
	imshow("mask" + str, mask);
	// let's create a new image now
	Mat crop(img.rows, img.cols, CV_8UC3);
	// set background to red
	crop.setTo(Scalar(0, 0, 255));
	// and copy the magic apple
	img.copyTo(crop, mask);

	Mat target = img.clone();

	//Image moments
	for (int i = 0; i < bam1_cnts.size(); i++) {
		if (bam1_cnts[i].size() > 10) {
			mnts = moments(bam1_cnts[i]);
			double m00 = mnts.m00;
			double m01 = mnts.m01;
			double m10 = mnts.m10;
			circle(target, { int(m10 / m00), int(m01 / m00) }, 4, Scalar(0, 255, 0), -1);
			imshow("target " + str, target);
		}
	}
	imshow("target " + str, target);
}

void show_all_target(int pos, void* userdata) {
	Allababah *bah = (Allababah*) userdata;
	cout << bah->thres;
	Moments mnts;
	show_target(bah->bam1, mnts, bah->thres, bah->thres1, bah->thres2,"one");
	show_target(bah->bam2, mnts, bah->thres, bah->thres1, bah->thres2, "two");
	show_target(bah->bam3, mnts, bah->thres, bah->thres1, bah->thres2, "three");
}

int main(int argc, char** argv) {
	Mat bam1 = imread("ig_0.jpg");
	Mat bam2 = imread("ig_1.jpg");
	Mat bam3 = imread("ig_2.jpg");
	//Mat bam1_1channel;
	//cvtColor(bam1, bam1_1channel, CV_BGR2HSV);

	namedWindow("Main window");
	Allababah *babah = new Allababah(bam1, bam2, bam3, 225, 100, 200);
	createTrackbar("threshold", "Main window", &(babah->thres), 255, show_all_target, babah);
	createTrackbar("threshold1", "Main window", &(babah->thres1), 255, show_all_target, babah);
	createTrackbar("threshold2", "Main window", &(babah->thres2), 255, show_all_target, babah);

	waitKey(0);
	return 0;
}