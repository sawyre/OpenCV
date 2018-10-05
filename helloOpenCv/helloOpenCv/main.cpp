#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/core.hpp"
#include <iostream>
#include <stdlib.h>

using namespace std;
using namespace cv;

void draw_circle(int e, int x, int y, int d, void *ptr) {
	Mat *image = (Mat*)ptr;
	Point p;
	p.x = x;
	p.y = y;
	cout << "draw_circle complete" << '\n';
	cout << x << ' ' << y << '\n';
	if (e == EVENT_LBUTTONDBLCLK) {
		cout << "Double  click" << '\n';
		circle(*image, p, 50, { 255, 0, 0 }, -1);
		imshow("Game of year", *image);
	}
}

void go(const Mat &img1, const Mat &img2, Mat &res, const Rect &ROI) {
	/** 
		Функция копирует img1 в зону img2, указанную в ROI
		и сохраняет все в res.
	*/
	img2.copyTo(res);
	// устанавливаем ROI(зона интереса)
	Mat image_roi = res(ROI);
	img1.copyTo(image_roi);
	imshow("Game of year", res);
}

void turn(const Mat &img1, const Mat &img2, Mat &res, const Rect &ROI, int rotateCode, int &turn) {
	/**
	Функция поворачивает img1 и копирует его в зону img2, указанную в ROI
	и сохраняет все в res.
	*/
	if (rotateCode <= 2 && rotateCode >= 0) {
		img2.copyTo(res);
		// устанавливаем ROI(зона интереса)
		Mat image_roi = res(ROI);
		rotate(img1, img1, rotateCode);
		img1.copyTo(image_roi);
		imshow("Game of year", res);
		switch (rotateCode) {
		case 0: {
			if (turn <= 2) {
				turn++;
			} else {
				turn = 0;
			}
			break;
		}
		case 1: {
			if (turn <= 1) {
				turn += 2;
			} else if (turn == 2) {
				turn = 0;
			} else if (turn == 3) {
				turn = 1;
			}
			break;
		}
		case 2: {
			if (turn > 0) {
				turn -= 1;
			} else {
				turn = 3;
			}
			break;
		}
		}
	}
}

void shoot(const Mat &img1, Mat &img2, Mat &res, const Rect &ROI,const int &turn) {
	/**
	Функция рисует линию выстрела от носа коробя до края картинки.
	*/
	switch (turn) {
	case 0: {
		line(img2, { (ROI.x + ROI.width / 2), ROI.y }, { (ROI.x + ROI.width / 2), 0 }, { 0, 255, 0 }, 3);
		break;
	}
	case 1: {
		line(img2, { (ROI.x + ROI.width), (ROI.y + ROI.height / 2) }, { 512, (ROI.y + ROI.height / 2) }, { 0, 255, 0 }, 3);
		break;
	}
	case 2: {
		line(img2, { (ROI.x + ROI.width / 2), (ROI.y + ROI.height) }, { (ROI.x + ROI.width / 2), 512 }, { 0, 255, 0 }, 3);
		break;
	}
	case 3: {
		line(img2, { ROI.x, (ROI.y + ROI.height / 2) }, { 0, (ROI.y + ROI.height / 2) }, { 0, 255, 0 }, 3);
		break;
	}
	}
	img2.copyTo(res);
	// устанавливаем ROI(зона интереса)
	Mat image_roi = res(ROI);
	img1.copyTo(image_roi);
	imshow("Game of year", res);
}

int main(int argc, char **argv) {
	namedWindow("Game of year", WINDOW_NORMAL);
	//background переносится на отображаемое изображение
	Mat img1;	
	Mat img = imread("lena.jpg");
	img.copyTo(img1);
	//пока юзлес, но мб пригодится на обработку событий мыши
	Point p;
	setMouseCallback("Game of year", draw_circle, &img);
	//считывание самолетика
	Mat plane = imread("airplane.jpg");
	int x = 100;
	int y = 100;
	//уменьшаем самолетик до 100x100
	resize(plane, plane, { 100, 100 });
	namedWindow("miniplane", WINDOW_NORMAL);
	imshow("miniplane", plane);
	// устанавливаем ROI
	Rect region_of_interest = Rect(x, y, plane.cols, plane.rows);
	Mat image_roi = img1(region_of_interest);

	plane.copyTo(image_roi);
	//add(tank, image_roi, image_roi);
	//показываем изображение
	imshow("Game of year", img1);
	//отражает поворот самолетика
	int turn_plane = 0;
	char a = 0;
	bool loop = true;
	while (loop) {
		a = waitKey(0);
		circle(img, { region_of_interest.x + region_of_interest.width / 2, region_of_interest.y + region_of_interest.height / 2 }, 4, { 0, 0, 255 }, -1);
		switch (a) {
		case 'a': {
			turn(plane, img, img1, region_of_interest, 2, turn_plane);
			cout << "Turn left" << '\n';
			break;
		}
		case 'd': {
			turn(plane, img, img1, region_of_interest, 0, turn_plane);
			cout << "Turn right" << '\n';
			break;
		}
		case 's': {
			switch (turn_plane) {
			case 0: {
				if ((region_of_interest.y + region_of_interest.height) < img.rows) {
					region_of_interest.y++;
				}
				break;
			}
			case 1: {
				if (region_of_interest.x > 0) {
					region_of_interest.x--;
				}
				break;
			}
			case 2: {
				if (region_of_interest.y > 0) {
					region_of_interest.y--;
				}
				break;
			}
			case 3: {
				if ((region_of_interest.x + region_of_interest.width) < img.cols) {
					region_of_interest.x++;
				}
				break;
			}
			}

			go(plane, img, img1, region_of_interest);
			cout << "Go back" << '\n';
			break;
		}
		case 'w': {
			switch (turn_plane) {
			case 0: {
				if (region_of_interest.y > 0) {
					region_of_interest.y--;
				}
				break;
			}
			case 1: {
				if ((region_of_interest.x + region_of_interest.width) < img.cols) {
					region_of_interest.x++;
				}
				break;
			}
			case 2: {
				if ((region_of_interest.y + region_of_interest.height) < img.rows) {
					region_of_interest.y++;
				}
				break;
			}
			case 3: {
				if (region_of_interest.x > 0) {
					region_of_interest.x--;
				}
				break;
			}
			}

			go(plane, img, img1, region_of_interest);
			cout << "Go forward" << '\n';
			break;
		}
		case 'q': {
			shoot(plane, img, img1, region_of_interest, turn_plane);
			cout << "Shoot" << '\n';
			break;

		}
		case 'e': {
			imwrite("saved_file.jpg", img1);
			cout << "Save image" << '\n';
			break;
		}
		default: {
			loop = false;
			cout << "Exit" << '\n';
			break;
		}
		}
	}

	waitKey(0);
	destroyAllWindows();
	return 0;
}
