#ifndef TRACKER_H
#define TRACKER_H
#include "trackers/utils.h"
#include "trackers/buffers.h"
class Tracker
{
public:
	int elem_size, frame_width, frame_height,gt_N;
	bool gt_inited;
    //int* targetPts,BBox;
	float* Features;

	virtual int init(int bsize,int w,int h)=0;
	int initparams();
	int initTargetsLoc(char* fname);
	int initTargetsLoc(int* points,int N);
	int initTargetsBB(char* fname);
	int initTargetsBB(int* points,int N);
    virtual int updateAframe(unsigned char* framedata,int fidx)=0;
	int getCurLoc(int* points,int N);
	int getCurBB(int* points,int N);
	int curToFile(char* fname);
	int getCurFeats(float* feats,int dim,int N);
};
#endif
