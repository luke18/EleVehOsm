#pragma once

#include "cnominatim.h" //If you get a compilation error about this missing header file, then you need to download my CNominatim class from http://www.naughter.com/nominatim.html


class CSearchResultsDlg : public CDialog
{
	DECLARE_DYNAMIC(CSearchResultsDlg)

public:
//Constructors / Destructors
	CSearchResultsDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSearchResultsDlg();

//Member variables
	enum { IDD = IDD_SEARCH_RESULTS };
  CListBox m_ctrlResults;
  CArray<CNominatimSearchPlace, CNominatimSearchPlace& >* m_pResults;
  int m_nResult;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

  afx_msg void OnLbnDblclkSearchResults();
};
