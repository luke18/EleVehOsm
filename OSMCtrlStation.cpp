#include "stdafx.h"
#include "OSMCtrlStation.h"

IMPLEMENT_SERIAL(COSMCtrlStation,CObject,1); //implementation of serial

COSMCtrlStation::COSMCtrlStation(void)
{
}


COSMCtrlStation::~COSMCtrlStation(void)
{
}

void COSMCtrlStation::AddStation(CString busName,double longitude, double latitude,int volGrade,int busNumber,double pdPower[96],double qdPower[96],double pgPower[96],double qgPower[96])
{
	StationStruct *station = new StationStruct;
	station->busName = busName;
	station->longitude=longitude;
	station->latitude=latitude;
	station->volGrade = volGrade;
	station->busNumber = busNumber;
	for (int i = 0; i<96; i++) {
		station->pdPower[i] = pdPower[i];
		station->qdPower[i] = qdPower[i];
		station->pgPower[i] = pgPower[i];
		station->qgPower[i] = qgPower[i];
	}
	m_stationArray.Add(station);
}





int COSMCtrlStation::GetStationCount()
{
	return m_stationArray.GetSize();
}

void COSMCtrlStation::GetStation(int stationNum, StationStruct *station)
{
	StationStruct *stationStruct = (StationStruct*)m_stationArray.GetAt(stationNum);
	station->busName = stationStruct->busName;
	station->longitude = stationStruct->longitude;
	station->latitude = stationStruct->latitude;
	station->volGrade = stationStruct->volGrade;
	station->busNumber = stationStruct->busNumber;
	for (int i = 0; i<96; i++) {
		station->pdPower[i] = stationStruct->pdPower[i];
		station->qdPower[i] = stationStruct->qdPower[i];
		station->pgPower[i] = stationStruct->pgPower[i];
		station->qgPower[i] = stationStruct->qgPower[i];
	}
}

/*
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
		m_stationArray.RemoveAll(); // delete exsiting objects
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
*/

