#include <iostream>
#include <opencv2\opencv.hpp>
#include <opencv2\gpu\gpu.hpp>
using namespace cv;

int main()
{
	VideoCapture cap;
	Mat frame,gray,preframe,pregray;
	int nFeature = 1000;
	gpu::GoodFeaturesToTrackDetector_GPU detector(nFeature);
	gpu::PyrLKOpticalFlow tracker;
	tracker.getMinEigenVals = true;
	cap.open("C:/Users/xcy/Documents/CVProject/data/labelCross/STREETCROSS_00026.mp4");
	if (cap.isOpened())
	{
		cap >> frame;
		cvtColor(frame, gray, CV_BGR2GRAY);
		pregray = gray.clone();
		Mat corners,prePts,nextPts,status,eigenvec;
		gpu::GpuMat gpugray(gray.size[0], gray.size[1], CV_8UC1), gpuPreGray(gray.size[0], gray.size[1], CV_8UC1), gpucorners, gpuPrePts, gpuNextPts,gpuStatus,gpuEigenvec;
		gpugray.upload(gray);
		gpuPreGray.upload(gray);
		detector(gpugray, gpucorners);
		gpucorners.download(corners);
		gpucorners.copyTo(gpuPrePts);
		while (cap.isOpened())
		{
			
			cap >> frame;
			cvtColor(frame, gray, CV_BGR2GRAY);
			gpugray.copyTo(gpuPreGray);
			gpugray.upload(gray);
			tracker.sparse(gpuPreGray, gpugray, gpuPrePts, gpuNextPts, gpuStatus, &gpuEigenvec);
			//std::cout << "here" << std::endl;
			gpuStatus.download(status);
			gpuNextPts.download(prePts);
			detector(gpugray, gpucorners);
			gpucorners.download(corners);
			int j = 0;
			for (int i = 0; i < status.size[1]; i++)
			{
				int istracking = status.at<int>(i);
				if (!istracking)
				{
					prePts.at<Vec2f>(i) = corners.at<Vec2f>(j++);
				}
			}
			gpuPrePts.upload(prePts);
			//std::cout << corners.size[1] << std::endl;
			for (int i = 0; i < prePts.size[1]; i++)
			{
				Vec2f p = prePts.at<Vec2f>(i);
				circle(gray,Point(p), 2, Scalar(255));
			}
				imshow("frame", gray);

			waitKey(1);
		}
	}

}