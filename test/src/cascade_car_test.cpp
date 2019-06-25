#include <opencv2/opencv.hpp>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

using namespace cv;
using namespace std;

bool cmp(Rect r1, Rect r2)
{
	return r1.width*r1.height > r2.width*r2.height;
}

int main(int argc, char** argv) {
	
	String src_path = argv[1];//检测图片路径
	String cascade_car_file = argv[2];//车分类器文件路径
	String cascade_plate_file = argv[3];//车牌分类器文件路径
	double 	scaleFactor = strtod(argv[4], NULL);//图形每次缩小比例
	int minNeighbors = atoi(argv[5]);//成功匹配所需要的周围矩形框的数目
	int flags = atoi(argv[6]);//检测方式，默认0
	int car_minSize_weigh = atoi(argv[7]);
	int car_minSize_height = atoi(argv[8]);
	int plate_minSize_weigh = atoi(argv[9]);
	int plate_minSize_height = atoi(argv[10]);
	int distance = atoi(argv[11]);
	int th = atoi(argv[12]);
	int element_size = atoi(argv[13]);
	cv::Size car_min_size(car_minSize_weigh,car_minSize_height);//匹配车的最小矩形
	cv::Size plate_min_size(plate_minSize_weigh,plate_minSize_height);//匹配车牌最小矩形
	cv::namedWindow("input_image",WINDOW_NORMAL);
	cv::namedWindow("output_image",WINDOW_NORMAL);

	CascadeClassifier car_classifier;
	if(!car_classifier.load(cascade_car_file)) {
		printf("could not load car data...\n");
		return -1;
	}

	CascadeClassifier plate_classifier;
	if(!plate_classifier.load(cascade_plate_file)) {
		printf("could not load plate data...\n");
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
	Rect averRect;
	car_classifier.detectMultiScale(gray, rects, scaleFactor, minNeighbors, flags, car_min_size);
	if(rects.size() == 0){
		printf("未检测到车辆目标\n");
		return -1;	
	}

	std::sort(rects.begin(), rects.end(), cmp);
	
	int rect_num = 0;
	for (size_t t = 0; t < rects.size() && t < 5; t++) {
		if(sqrt(pow(rects[0].x - rects[t].x, 2) + pow(rects[0].y - rects[t].y, 2)) <= distance){
			printf("x = %d, y = %d, width = %d, height = %d\n",rects[t].x, rects[t].y, rects[t].width, rects[t].height);
			rectangle(srcImg, rects[t], Scalar(0, 0, 255), 4, 8, 0);
			averRect.x += rects[t].x;
			averRect.y += rects[t].y;
			rect_num++;
		}
				
	}

	averRect.x = averRect.x/rect_num;
	averRect.y = averRect.y/rect_num;
	averRect.width = rects[0].width;
	averRect.height = rects[0].height;
	

	rectangle(srcImg, averRect, Scalar(255, 0, 0), 4, 8, 0);

	vector<Rect> plate;
	Rect plate_roi;
	Rect aver_plate;
	plate_roi.x = averRect.x;
	plate_roi.y = averRect.y + (averRect.height*3)/5;
	plate_roi.width = averRect.width;
	plate_roi.height = (averRect.height*1)/4;
	rectangle(srcImg, plate_roi, Scalar(0, 0, 0), 4, 8, 0);
	Mat roi = gray(plate_roi);
	plate_classifier.detectMultiScale(roi, plate, scaleFactor, minNeighbors, flags, plate_min_size);
	if(plate.size() == 0){
		printf("未检测到车牌目标\n");
		imshow("output_image", srcImg);
		int c = waitKey(0);
        	if ((char)c == 27) { return 0; } 
		return -1;	
	}

	for (size_t t = 0; t < plate.size(); t++) {
		plate[t].x = plate[t].x + averRect.x;
		plate[t].y = plate[t].y + plate_roi.y;		
	}

	std::sort(plate.begin(), plate.end(), cmp);

	aver_plate.x = plate[0].x;
	aver_plate.y = plate[0].y;
	aver_plate.width = plate[0].width;
	aver_plate.height = plate[0].height;

	rectangle(srcImg, aver_plate, Scalar(0, 255, 0), 4, 8, 0);

	Mat final_plate = srcImg(aver_plate);
	Mat final_plate_clone = final_plate.clone();
	imshow("final_plate", final_plate_clone);
	cvtColor(final_plate, final_plate, COLOR_BGR2GRAY);
	GaussianBlur(final_plate, final_plate, Size(3,3), 5);
	
	Mat grad_x, grad_y;
    	Mat abs_grad_x, abs_grad_y;
	Sobel(final_plate, grad_x, CV_16S, 1, 0, 3, 1, 1, BORDER_DEFAULT);
    	convertScaleAbs(grad_x, abs_grad_x);
	Sobel(final_plate, grad_y, CV_16S, 0, 1, 3, 1, 1, BORDER_DEFAULT);
    	convertScaleAbs(grad_y, abs_grad_y);
		
	addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, final_plate);
   	imshow("T_Sobel", final_plate);
	
	cv::threshold(final_plate, final_plate, th, 255, CV_THRESH_BINARY_INV);
	imshow("binnry", final_plate);

	Mat structure_element = getStructuringElement(MORPH_RECT, Size(element_size,element_size)); 

	erode(final_plate, final_plate, structure_element); //腐蚀
    	namedWindow("erode_img", CV_WINDOW_AUTOSIZE);
    	imshow("erode_img", final_plate); 

	dilate(final_plate, final_plate, structure_element); //膨胀
    	namedWindow("dilate_img", CV_WINDOW_AUTOSIZE);
    	imshow("dilate_img", final_plate);

	imshow("output_image", srcImg);
	int c = waitKey(0);
        if ((char)c == 27) { return 0; } 
	return 0;
}
