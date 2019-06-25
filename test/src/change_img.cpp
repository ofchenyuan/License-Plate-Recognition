#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <string>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
 
int main(int argc,char** argv)
{	
	std::stringstream ss;
	cv::String path=argv[1];
	cv::String dest=argv[2];
	int width = atoi(argv[3]);
	int height= atoi(argv[4]);
	int start;
	cv::String savedfilename;
	std::vector<cv::String> filenames;
	cv::Mat dstImgGray,dstImgResize;
	
	if(argv[5]==NULL)
	{
		start = 0;
	}else
	{
		start = atoi(argv[5]);
	}	

	cv::Size dst_size(width,height);
	cv::glob(path, filenames);

	for (int i = 0; i < filenames.size()-1; i++,start++) 
	{
		dstImgGray = cv::imread(filenames[i],0);
	//	cv::cvtColor(srcImg, dstImgGray, CV_BGR2GRAY);
		resize(dstImgGray,dstImgResize,dst_size);
		ss << start;	
		savedfilename = dest + cv::String(ss.str()) + cv::String(".jpg");
		ss.str("");    
		cv::imwrite(savedfilename, dstImgResize);
		std::cout << "保存" << savedfilename << "成功!" << std::endl;
		cv::waitKey(1);
	}
	
	return 0;
}
