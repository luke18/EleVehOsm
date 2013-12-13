#include "stdafx.h"
#include "OSMCtrlStation.h"

IMPLEMENT_SERIAL(COSMCtrlStation,CObject,1); //implementation of serial

COSMCtrlStation::COSMCtrlStation(void)
{
}


COSMCtrlStation::~COSMCtrlStation(void)
{
}

void COSMCtrlStation::AddStation(double longitude,double latitude)
{
	StationStruct *station = new StationStruct;
	station->longitude=longitude;
	station->latitude=latitude;
	m_stationArray.Add(station);
}

int COSMCtrlStation::GetStationCount()
{
	return m_stationArray.GetSize();
}

void COSMCtrlStation::GetStation(int stationNum, StationStruct *station)
{
	StationStruct *stationStruct = (StationStruct*)m_stationArray.GetAt(stationNum);
	station->longitude = stationStruct->longitude;
	station->latitude = stationStruct->latitude;
}

void COSMCtrlStation::Serialize(CArchive&ar)
{
	CObject::Serialize(ar);
	if(ar.IsStoring())  // store data of station
	{
		StationStruct station;
		int size = GetStationCount();
		ar << size;
		for(int i = 0;i < size;i++)
		{
			GetStation(i,&station);
			ar << station.longitude;
			ar << station.latitude;
		}
	}
	else  // read data of station
	{
		int size;
		ar >> size;
		for(int i = 0;i < size;i++)
		{
			double longitude;
			double latitude;
			ar >> longitude;
			ar >> latitude;
			AddStation(longitude, latitude);
		}
	}
}


