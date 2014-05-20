#pragma once
#include "OSMCtrlStation.h"
#include <vector>
#include <string>

class COSMCtrlAppDoc : public CDocument
{
protected: // create from serialization only
	COSMCtrlAppDoc();
	DECLARE_DYNCREATE(COSMCtrlAppDoc)

// Attributes
public:
	//COSMCtrlStation m_Stations;
	std::vector<StationStruct> m_Stations;
	std::vector<GenStruct> m_Gens;
	std::vector<BranchStruct> m_Branchs;
	std::vector<ElecStruct> m_Elecs;

	
	//StationStruct Vehicle;
	//int branchArray[52][2];

// Operations
public:

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// Implementation
public:
	virtual ~COSMCtrlAppDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};


