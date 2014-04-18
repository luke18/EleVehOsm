#pragma once


struct StationStruct
{
	CString busName;
	double longitude;
	double latitude;
	double bus_i, type, pdMax, qdMax,gs,bs,area,baseKv,zone,vmax,vmin,capacity;
	double volGrade;
	double father, kind;
	//int busNumber;
	double pdPower[96], qdPower[96];
	double voltageM[96], voltageA[96];
	double loadP[96], loadQ[96];
};

struct GenStruct
{
	double bus_i,pgmax,qgmax,qmax,qmin,vg,mbase,status,pmax;
	double pgPower[96], qgPower[96]; 
};

struct BranchStruct
{
	double fbus,tbus,br,bx,bb,ratea,rateb,ratec,ratio,angle,status,angmin,angmax,voltagegrade;
	double fromP[96], toP[96], fromQ[96], toQ[96];
	int startBus, endBus; //on the simple map
};

/*
int FindBusNumByI(double bus_i, std::vector<StationStruct> m_Stations)
{
	double fatherNum;
	int busNum;
	for (int i =0; i<m_Stations.size();i++)
	{
		if(m_Stations[i].bus_i == bus_i)
		{
			fatherNum = m_Stations[i].father;
			for (int j =0; j<m_Stations.size();j++)
			{
				if(m_Stations[j].bus_i == fatherNum)
				{
					busNum =j;
					break;
				}
			}
			break;
		}
	}
	return busNum;
}

*/
class COSMCtrlStation :
	public CObject
{
	DECLARE_SERIAL(COSMCtrlStation);
public:
	CPtrArray m_stationArray;
public:
	COSMCtrlStation(void);
	virtual ~COSMCtrlStation(void);
	//Add Pointer and other data of Station to array
	void AddStation(CString busName,double longitude, double latitude,int volGrade,int busNumber,double pdPower[96],double qdPower[96],double pgPower[96],double qgPower[96]);
	//void UpdateBus(double voltageM, double voltageA, int num);
	int GetStationCount(); //get length of array
	//get data of station
	void GetStation(int stationNum, StationStruct *station);
	//void Serialize(CArchive&ar);  //serialize 
};

