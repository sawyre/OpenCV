#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "vector"

using namespace std;
using namespace cv;

int main(int argc, char** argv) {
	Mat bam1 = imread("ig_0.jpg");
	//Mat bam1_1channel;
	//cvtColor(bam1, bam1_1channel, CV_BGR2HSV);
	Mat bam1_binary;
	Mat bam1_Canny;
	threshold(bam1, bam1_binary, 230, 255, THRESH_BINARY_INV);
	Canny(bam1_binary, bam1_Canny, 100, 200);
	imshow("bam1", bam1);
	imshow("bam1_binary", bam1_binary);
	imshow("bam1_Canny", bam1_Canny);
	
	vector<vector<Point>> bam1_cnts;

	findContours(bam1_binary, bam1_cnts, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);

	waitKey(0);
	return 0;
}