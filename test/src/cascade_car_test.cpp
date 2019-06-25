#include <opencv2/opencv.hpp>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

using namespace cv;
using namespace std;


int main(int argc, char** argv) {
	
	String src_path = argv[1];//检测图片路径
	String cascade_file = argv[2];//分类器文件路径
	double 	scaleFactor = strtod(argv[3], NULL);//图形每次缩小比例
	int minNeighbors = atoi(argv[4]);//成功匹配所需要的周围矩形框的数目
	int flags = atoi(argv[5]);//检测方式，默认0
	int minSize_weigh = atoi(argv[6]);
	int minSize_height = atoi(argv[7]);
	int maxSize_weigh = atoi(argv[8]);
	int maxSize_height = atoi(argv[9]);
	cv::Size min_size(minSize_weigh,minSize_height);//匹配物体的最小矩形
	cv::Size max_size(maxSize_weigh,maxSize_height);//匹配物体最大矩形
	cv::namedWindow("input_image",WINDOW_NORMAL);
	cv::namedWindow("output_image",WINDOW_NORMAL);

	CascadeClassifier classifier;
	if(!classifier.load(cascade_file)) {
		printf("could not load data...\n");
		return -1;
	}

	Mat srcImg = imread(src_path);
	if (srcImg.empty()) {
		printf("could not load image...\n");
		return -1;
	}
	imshow("input_image", srcImg);
	Mat gray;
	cvtColor(srcImg, gray, COLOR_BGR2GRAY);
	equalizeHist(gray, gray);

	vector<Rect> rects;
	classifier.detectMultiScale(gray, rects, scaleFactor, minNeighbors, flags, min_size, max_size);
	for (size_t t = 0; t < rects.size(); t++) {
		rectangle(srcImg, rects[static_cast<int>(t)], Scalar(0, 0, 255), 2, 8, 0);
	}

	imshow("output_image", srcImg);
	int c = waitKey(0);
        if ((char)c == 27) { return 0; } 
	return 0;
}
