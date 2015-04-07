#ifndef KLTTRACKER_H
#define KLTTRACKER_H
#include "trackers/tracker.h"
//#include "trackers/klt_c/klt.h"


#include <vector>
class KLTtracker : public Tracker
{
public:
    int *isTracking;
	int frameidx;
    int nFeatures,nSearch; /*** get frature number ***/
	unsigned char* preframedata,* bgdata,*curframedata;
    Buff<int> goodNewPts;
    std::vector<TrackBuff> trackBuff;
    TrkPts pttmp;
    unsigned char* kltdense;


	int init(int bsize,int w,int h);
	int selfinit(unsigned char* framedata);
	int initBG(int mode,unsigned char* bgptr=NULL);
    int updateAframe(unsigned char* framedata,int fidx);
	void updateFGMask(unsigned char* bgptr);

	int checkFG(int x,int y);
    bool checkTrackMoving(TrackBuff &strk);
    bool checkCurve(TrackBuff &strk);

	int endTraking();
};
#endif
