// StationDialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "OSMCtrlApp.h"
#include "StationDialog.h"
#include "afxdialogex.h"


// StationDialog �Ի���

IMPLEMENT_DYNAMIC(StationDialog, CDialogEx)

StationDialog::StationDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(StationDialog::IDD, pParent)
{

}

StationDialog::~StationDialog()
{
}

void StationDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX,IDC_EDIT1,m_Edit1);
	DDX_Text(pDX,IDC_EDIT2,m_Edit2);
}


BEGIN_MESSAGE_MAP(StationDialog, CDialogEx)
END_MESSAGE_MAP()


// StationDialog ��Ϣ�������

