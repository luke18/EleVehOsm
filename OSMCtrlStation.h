#pragma once

struct StationStruct
{
	CString busName;
	double longitude;
	double latitude;
	int volGrade;
	int busNumber;
	double pdPower[96], qdPower[96];
	double pgPower[96], qgPower[96];
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
	void AddStation(CString busName,double longitude, double latitude,int volGrade,int busNumber,double pdPower[96],double qdPower[96],double pgPower[96],double qgPower[96]);
	int GetStationCount(); //get length of array
	//get data of station
	void GetStation(int stationNum, StationStruct *station);
	//void Serialize(CArchive&ar);  //serialize 
};

