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
	ON_EN_CHANGE(IDC_EDIT1, &StationDialog::OnEnChangeEdit1)
END_MESSAGE_MAP()


void StationDialog::OnEnChangeEdit1()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}
