// StationDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "OSMCtrlApp.h"
#include "StationDialog.h"
#include "afxdialogex.h"


// StationDialog 对话框

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
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}
