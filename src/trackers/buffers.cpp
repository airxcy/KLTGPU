#include "trackers/buffers.h"



template <typename ELEM_T>
Buff<ELEM_T>::Buff()
{
    tailidx = 0,lastupdate=0;
	tailptr = NULL, cur_frame_ptr = NULL, data = NULL;
}

template <typename ELEM_T>
bool Buff<ELEM_T>::init(int fbsize, int l)
{
	tailidx = 0;
	frame_step_size = fbsize;
	frame_byte_size = frame_step_size*sizeof(ELEM_T);
	buff_len = l;
	//data = new ELEM_T[frame_step_size*buff_len];
	data = (ELEM_T *)malloc(frame_byte_size*buff_len);
	len = 0;
	tailptr = data + tailidx*frame_step_size;
	cur_frame_ptr = tailptr;
	return true;
}

template <typename ELEM_T>
void Buff<ELEM_T>::clear()
{
	tailidx = 0;
	tailptr = data;// +tailidx*frame_step_size;
	cur_frame_ptr = tailptr;
	len = 0;
    lastupdate=-1;
}

template <typename ELEM_T>
void Buff<ELEM_T>::updateAFrame(ELEM_T* new_frame_ptr)
{
	if (tailidx >= buff_len)
	{
		ELEM_T* tmpptr = (ELEM_T *)malloc(frame_byte_size*buff_len * 2);
		memcpy(tmpptr, data, frame_byte_size*buff_len);
		free(data);
		data = tmpptr;
		//data = (ELEM_T *)realloc((void *)data, frame_byte_size*buff_len * 2);
		tailptr = data + tailidx*frame_step_size;
		cur_frame_ptr = tailptr;
		buff_len *= 2;
	}
	memcpy(tailptr, new_frame_ptr, frame_byte_size);
	cur_frame_ptr = tailptr;
	tailidx++;
	tailptr += frame_step_size;
	len++;
    lastupdate=1;
}
template <typename ELEM_T>
ELEM_T *Buff<ELEM_T>::getPtr(int i)
{
	ELEM_T* ptr = data + i*frame_step_size;
	return ptr;
}

template <typename ELEM_T>
void Buff<ELEM_T>::clone(Buff<ELEM_T>* target)
{
    if(target->buff_len<buff_len)
    {
        free(target->data);
        target->data=(ELEM_T *)malloc(frame_byte_size*buff_len);
    }
    memcpy(target->data,data,frame_byte_size*buff_len);
    target->tailidx=tailidx;
    target->buff_len=buff_len;
    target->len=len;
    target->lastupdate=0;

    target->tailptr=target->data+(tailptr-data);
    target->cur_frame_ptr=target->data+(cur_frame_ptr-data);
}

template class Buff<BYTE>;
template class Buff<int>;
template class Buff<float>;
template class Buff<double>;

template <typename ELEM_T>
QueBuff<ELEM_T>::QueBuff()
{
	headidx=0;tailidx=0;
	headptr=NULL,tailptr=NULL,cur_frame_ptr=NULL,data=NULL;
}

template <typename ELEM_T>
bool QueBuff<ELEM_T>::init(int fbsize,int l)
{
	headidx=0;tailidx=0;
	frame_step_size=fbsize;
	frame_byte_size=frame_step_size*sizeof(ELEM_T);
	buff_len=l;
	//data=new ELEM_T[frame_step_size*buff_len];
	data = (ELEM_T *)malloc(frame_byte_size*buff_len);
	len=0;
	headptr=data+headidx*frame_step_size;
	tailptr=data+tailidx*frame_step_size;
	cur_frame_ptr=tailptr;
	return true;
}
template <typename ELEM_T>
void QueBuff<ELEM_T>::clear()
{
	headidx=0;tailidx=0;
	headptr=data+headidx*frame_step_size;
	tailptr=data+tailidx*frame_step_size;
	cur_frame_ptr=tailptr;
	len=0;
}

template <typename ELEM_T>
void QueBuff<ELEM_T>::increPtr()
{
	tailidx=(tailidx+1)%buff_len;
	if(tailidx==headidx)
	{
		headidx=(headidx+1)%buff_len;
	}
	else
	{
		len++;
	}
	cur_frame_ptr=tailptr;
	headptr=data+headidx*frame_step_size;
	tailptr=data+tailidx*frame_step_size;
}
template <typename ELEM_T>
void QueBuff<ELEM_T>::updateAFrame(ELEM_T* new_frame_ptr)
{
	memcpy(tailptr,new_frame_ptr,frame_byte_size);
	increPtr();
}
template <typename ELEM_T>
ELEM_T *QueBuff<ELEM_T>::getPtr(int i)
{
	ELEM_T* ptr= data+(headidx+i)%buff_len*frame_step_size;
	return ptr;
}
template class QueBuff<BYTE>;
template class QueBuff<int>;
template class QueBuff<float>;
template class QueBuff<double>;


FrameBuff::FrameBuff()
{
	width=0,height=0,elem_byte_size=0;
}
bool FrameBuff::init(int bsize,int w, int h,int l)
{
	width=w,height=h,elem_byte_size=bsize;
	return QueBuff::init(elem_byte_size*width*height,l);
}

template class Buff < TrkPts >;
template class Buff < FeatPts >;
void TrackBuff::clone(TrackBuff* target)
{
    Buff::clone(target);
    target->isCurved=isCurved;
}
void TrackBuff::clear()
{
    isCurved=false;
    Buff::clear();
}

void FeatBuff::clone(FeatBuff* target)
{
    Buff::clone(target);
    target->isCurved=isCurved;
}
void FeatBuff::clear()
{
    isCurved=false;
    Buff::clear();
}
template class Buff <BB>;

template <typename ELEM_T>
Map3D<ELEM_T>::Map3D()
{
}
template <typename ELEM_T>
Map3D<ELEM_T>::Map3D(int h,int w,int step)
{
    e_step=step;
    width=w;
    height=h;
    e_byte_size=e_step*sizeof(ELEM_T);
    data=(ELEM_T *)malloc(h*w*e_byte_size);
    memset(data,0,h*w*e_byte_size);
}

template <typename ELEM_T>
ELEM_T& Map3D<ELEM_T>::operator()(int i,int j,int k)
{
    return data[k+e_step*(j+width*i)];
}
template class Map3D<int>;
template class Map3D<double>;
template class Map3D<float>;

