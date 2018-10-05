#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "vector"
#include "iostream"

using namespace std;
using namespace cv;

class BinnaryParametrs {
public:
	BinnaryParametrs(Mat *bam, int par[6]): 
	bam1(bam[0].clone()), bam2(bam[1].clone()), bam3(bam[2].clone()), bam4(bam[3].clone()),
	H_lower{par[0]}, H_upper(par[1]), S_lower(par[2]), S_upper(par[3]), V_lower(par[4]), V_upper(par[5]) {
	}
	BinnaryParametrs() {};
	~BinnaryParametrs() {};

	Mat bam1;
	Mat bam2;
	Mat bam3;
	Mat bam4;
	Mat bam_binnary[4];
	vector<vector<Point>> cnts;
	int H_lower;
	int H_upper;
	int S_lower;
	int S_upper;
	int V_lower;
	int V_upper;
};

void show_targets(BinnaryParametrs &tar, Mat *bam) {
	Mat bm[4];
	for (int i = 0; i < 4; i++) {
		bm[i] = bam[i].clone();
	}
	for (int i = 0; i < 4; i++) {
		findContours(tar.bam_binnary[i], tar.cnts, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
		polylines(bm[i], tar.cnts, true, { 255, 0, 0 });
		for (int j = 0; j < tar.cnts.size(); j++) {
			if (tar.cnts[j].size() > 10) {
				Moments mnts = moments(tar.cnts[j]);
				double m00 = mnts.m00;
				double m10 = mnts.m10;
				double m01 = mnts.m01;
				circle(bm[i], { int(m10 / m00), int(m01 / m00) }, 4, { 255, 0, 0 }, -1);
				imshow("Target " + to_string(i), bm[i]);
			}
		}
	}
}

void TrackbarCB(int pos, void* userdata) {
	BinnaryParametrs *tmp = (BinnaryParametrs*)userdata;
	inRange(tmp->bam1, Vec3b(tmp->H_lower, tmp->S_lower, tmp->V_lower), Vec3b(tmp->H_upper, tmp->S_upper, tmp->V_upper), tmp->bam_binnary[0]);
	inRange(tmp->bam2, Vec3b(tmp->H_lower, tmp->S_lower, tmp->V_lower), Vec3b(tmp->H_upper, tmp->S_upper, tmp->V_upper), tmp->bam_binnary[1]);
	inRange(tmp->bam3, Vec3b(tmp->H_lower, tmp->S_lower, tmp->V_lower), Vec3b(tmp->H_upper, tmp->S_upper, tmp->V_upper), tmp->bam_binnary[2]);
	inRange(tmp->bam4, Vec3b(tmp->H_lower, tmp->S_lower, tmp->V_lower), Vec3b(tmp->H_upper, tmp->S_upper, tmp->V_upper), tmp->bam_binnary[3]);
	imshow("Binnary 1", tmp->bam_binnary[0]);
	imshow("Binnary 2", tmp->bam_binnary[1]);
	imshow("Binnary 3", tmp->bam_binnary[2]);
	imshow("Binnary 4", tmp->bam_binnary[3]);
}

int main(int argc, char** argv) {
	namedWindow("Parametrs HSV");
	Mat *bam = new Mat[4];
	bam[0] = imread("121.jpg");
	bam[1] = imread("133.jpg");
	bam[2] = imread("166.jpg");
	bam[3] = imread("188.png");
	
	Mat *bam_hsv = new Mat[4];
	for (int i = 0; i < 4; i++) {
		cvtColor(bam[i], bam_hsv[i], CV_BGR2HSV);
	}

	int a[] = { 0, 179, 0, 255, 0, 255 };
	BinnaryParametrs targets(bam_hsv, a);

	createTrackbar("H_lower", "Parametrs HSV", &targets.H_lower, 179, TrackbarCB, &targets);
	createTrackbar("H_upper", "Parametrs HSV", &targets.H_upper, 179, TrackbarCB, &targets);
	createTrackbar("S_lower", "Parametrs HSV", &targets.S_lower, 255, TrackbarCB, &targets);
	createTrackbar("S_upper", "Parametrs HSV", &targets.S_upper, 255, TrackbarCB, &targets);
	createTrackbar("V_lower", "Parametrs HSV", &targets.V_lower, 255, TrackbarCB, &targets);
	createTrackbar("V_upper", "Parametrs HSV", &targets.V_upper, 255, TrackbarCB, &targets);

	namedWindow("Main window");
	imshow("HSV 1", targets.bam1);
	imshow("HSV 2", targets.bam2);
	imshow("HSV 3", targets.bam3);
	imshow("HSV 4", targets.bam4);

	waitKey(0);

	show_targets(targets, bam);

	waitKey(0);
	return 0;
}