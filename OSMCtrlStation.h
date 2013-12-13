#pragma once

struct StationStruct
{
	double longitude;
	double latitude;
};

class COSMCtrlStation :
	public CObject
{
	DECLARE_SERIAL(COSMCtrlStation);
protected:
	CPtrArray m_stationArray;
public:
	COSMCtrlStation(void);
	virtual ~COSMCtrlStation(void);
	//Add Pointer and other data of Station to array
	void AddStation(double longitude, double latitude);
	int GetStationCount(); //get length of array
	//get data of station
	void GetStation(int stationNum, StationStruct *station);
	void Serialize(CArchive&ar);  //serialize 
};

