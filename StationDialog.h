#pragma once


// StationDialog �Ի���

class StationDialog : public CDialogEx
{
	DECLARE_DYNAMIC(StationDialog)

public:
	StationDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~StationDialog();

// �Ի�������
	enum { IDD = IDD_DIALOG2 };
	double m_Edit1;
	double m_Edit2;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	//void afx_msg OnOK();  // overide of OnOK

	DECLARE_MESSAGE_MAP()
};
