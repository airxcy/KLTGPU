#ifndef BUFFERS_H
#define BUFFERS_H
//#include <iostream>
#include <string.h>
#include <stdlib.h>
typedef unsigned char BYTE;
typedef double REAL;
typedef int PntT;
template <typename ELEM_T> class Buff
{
public:
    int tailidx, frame_step_size, buff_len, frame_byte_size, len,lastupdate;
	ELEM_T *tailptr, *cur_frame_ptr;
	ELEM_T *data;
	Buff();
	~Buff()
	{
		free(data);
		//delete data;
	}
	bool init(int fbsize, int l);
	void clear();
	void updateAFrame(ELEM_T* new_frame_ptr);
	ELEM_T *getPtr(int i);
    void clone(Buff<ELEM_T> * target);
};

template <typename ELEM_T> class QueBuff
{
public:
	int headidx,tailidx,frame_step_size,buff_len,frame_byte_size,len;
	ELEM_T *headptr, *tailptr, *cur_frame_ptr;
	ELEM_T *data;
	QueBuff();
	~QueBuff()
	{
		free(data);
		//delete data;
    }
	bool init(int fbsize,int l);
	void clear();
	void increPtr();
	void updateAFrame(ELEM_T* new_frame_ptr);
	ELEM_T *getPtr(int i);
};



class FrameBuff : public QueBuff<BYTE>
{
public:
	int elem_byte_size,width,height;
	FrameBuff();
	bool init(int bsize,int w, int h,int l);
};

typedef struct {
    PntT x;
    PntT y;
	int t;
} TrkPts, *TrkPts_p;
typedef struct {
    REAL x;
    REAL y;
    int t;
}FeatPts,*FeatPts_p;

typedef struct
{
    double left;
    double top;
    double right;
    double bottom;
    int frameidx;
}BB, *BB_p;
class TrackBuff : public Buff<TrkPts>
{
public:
    TrackBuff():Buff(){isCurved=false;}
    bool isCurved;
    void clone(TrackBuff* target);
    void clear();
};
class FeatBuff : public Buff<FeatPts>
{
public:
    FeatBuff():Buff(){isCurved=false;}
    bool isCurved;
    void clone(FeatBuff* target);
    void clear();
};
class BBoxBuff : public Buff<BB>
{
public:
    BBoxBuff():Buff(){}
};
template <typename ELEM_T> class Map3D
{
public:
    ELEM_T* data;
    int e_byte_size,e_step,width,height;
    Map3D();
    Map3D(int h,int w,int step);

    ELEM_T& operator()(int i,int j,int k);

};


#endif
