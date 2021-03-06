#pragma once
#include "FormCommandView.h"
#include "OSMCtrlAppView.h"
#include "FunctionView.h"
#include "InfoView.h"


class CMainFrame : public CFrameWnd
{
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

public:
	friend class CFormCommandView;
	CFormCommandView *pLeftView;
	COSMCtrlAppView *pRightView;
	CInfoView *pInfoView;
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// Implementation
public:
	CSplitterWnd m_splitter;
	CSplitterWnd m_splitter2;
	CSplitterWnd m_splitter3;
	virtual ~CMainFrame();
protected:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;

// Generated message map functions
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnUpdatePosition(CCmdUI* pCmdUI);
	afx_msg void OnUpdateLength(CCmdUI* pCmdUI);
	DECLARE_MESSAGE_MAP()
};


