#pragma once


// StationDialog 对话框

class StationDialog : public CDialogEx
{
	DECLARE_DYNAMIC(StationDialog)

public:
	StationDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~StationDialog();

// 对话框数据
	enum { IDD = IDD_DIALOG2 };
	double m_Edit1;
	double m_Edit2;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	//void afx_msg OnOK();  // overide of OnOK

	DECLARE_MESSAGE_MAP()
};
