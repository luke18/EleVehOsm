/*
Module : OSMCtrl.H
Purpose: Defines the interface for an MFC GUI control which implements display of OpenStreetMaps tiles
Created: PJN / 28-11-2009

Copyright (c) 2009 - 2011 by PJ Naughter (Web: www.naughter.com, Email: pjna@naughter.com)

All rights reserved.

Copyright / Usage Details:

You are allowed to include the source code in any product (commercial, shareware, freeware or otherwise) 
when your product is released in binary form. You are allowed to modify the source code in any way you want 
except you cannot modify the copyright details at the top of each module. If you want to distribute source 
code with your application, then you are only allowed to distribute versions released by the author. This is 
to maintain a single distribution point for the source code. 

*/


///////////////////////// Macros / Includes ///////////////////////////////////

#pragma once

#ifndef __OSMCTRL_H__
#define __OSMCTRL_H__

#include "OSMCtrlIcon.h"
#include "OSMCtrlMarker.h"
#include "OSMCtrlPolyline.h"
#include "OSMCtrlPolygon.h"
#include "OSMCtrlCircle.h"
#include "OSMCtrlMapOperations.h"
#include "OSMCtrlTileProviders.h"
#include "OSMCtrlHandler.h"
#include "OSMCtrlCachedTile.h"

#ifdef COSMCTRL_NOD2D
#ifndef _GDIPLUS_H
#pragma message("To avoid this message, please put gdiplus.h in your pre compiled header (normally stdafx.h)")
#include <gdiplus.h>
#endif
#else
#include <afxrendertarget.h> //If you get a compilation error about this missing header file, then you need to compile with VC 2010 + SP1
#endif

#ifndef COSMCTRL_NOANIMATION
  #include <UIAnimation.h>  //If you get a compilation error about this missing header file, then you need to download and install the Windows 7 SDK
#endif

#include "SortedArray.h"  //If you get a compilation error about this missing header file, then you need to download my CSortedArray class from http://www.naughter.com/sortedarray.html


////////////////////////////////// Classes ////////////////////////////////////


//The main control class
class COSMCtrl : public CStatic
{
public:
//Enums
  enum DownloadOrder
  {
    ClosestToCenterFirst,
    YOuterXInnerLeftToRight
  };
  enum ControlAnchorPosition
  {
    Undefined = -1,
    TopLeft,
    TopRight,
    BottomLeft,
    BottomRight
  };
  enum Misc
  {
    OSMTileWidth = 256,
    OSMTileHeight = 256,
    OSMHalfTileWidth = 128,
    OSMHalfTileHeight = 128,
    OSMMaxZoom = 18,
    OSMMinZoom = 0,
    SLIDER_ID = 1,
    COPYRIGHT_ID = 2
  };
  enum MapItem
  {
    None,
    Map,
    ScrollNorth,
    ScrollSouth,
    ScrollEast,
    ScrollWest,
    ZoomIn,
    ZoomOut,
    ZoomBar,
    Marker,
    Polyline,
    Polygon,
    PolylineNode,
    PolygonNode,
    Circle,
    CircleCircumference,
  };
  enum MapMode
  {
    Normal,
    Selection,
    PolylineCreation,
    PolygonCreation,
    MarkerCreation,
    CircleCreation
  };
  enum ScaleBarUnits
  {
    UseOSDefault,
    Metric,
    Imperial
  };

//Constructors / Destructors
  COSMCtrl();

//static methods
  static CString GetTileCachePath(const CString& sCacheDirectory, int nZoom, int nTileX, int nTileY, BOOL bOld);
#ifdef COSMCTRL_NOD2D
  static BOOL    PositionToClient(const COSMCtrlPosition& position, const COSMCtrlPosition& centerPosition, double fZoom, const CRect& rClient, Gdiplus::PointF& point);
  static BOOL    ClientToPosition(const Gdiplus::PointF& point, const COSMCtrlPosition& centerPosition, double fZoom, const CRect& rClient, COSMCtrlPosition& position);
#else
  static BOOL    PositionToClient(const COSMCtrlPosition& position, const COSMCtrlPosition& centerPosition, double fZoom, const CRect& rClient, CD2DPointF& point);
  static BOOL    ClientToPosition(const CD2DPointF& point, const COSMCtrlPosition& centerPosition, double fZoom, const CRect& rClient, COSMCtrlPosition& position);
#endif

//Methods
  virtual BOOL Create(LPCTSTR lpszText, DWORD dwStyle,	const RECT& rect, CWnd* pParentWnd, UINT nID = 0xFFFF);
#ifdef COSMCTRL_NOD2D
  virtual BOOL Draw(CDC* pDC, const CRect& rSource, const CRect* prDest = NULL, BOOL bDrawScrollRose = TRUE, BOOL bDrawZoomBar = TRUE, BOOL bDrawScaleBar = TRUE, BOOL bDrawMarkers = TRUE, BOOL bDrawPolylines = TRUE, BOOL bDrawPolygons = TRUE, BOOL bDrawCircles = TRUE, BOOL bDrawGPSTracks = TRUE);
  virtual BOOL Draw(Gdiplus::Graphics& graphics, const CRect& rClient, const CRect* prClip = NULL, BOOL bDrawScrollRose = TRUE, BOOL bDrawZoomBar = TRUE, BOOL bDrawScaleBar = TRUE, BOOL bDrawMarkers = TRUE, BOOL bDrawPolylines = TRUE, BOOL bDrawPolygons = TRUE, BOOL bDrawCircles = TRUE, BOOL bDrawGPSTracks = TRUE);
#else
  HRESULT CreateImageFromWICBitmap(IWICBitmapSource* WICBitmapSource, ATL::CImage& image);
  virtual HRESULT Draw(CDC* pDC, const CRect& rSource, const CRect* prDest = NULL, BOOL bDrawScrollRose = TRUE, BOOL bDrawZoomBar = TRUE, BOOL bDrawScaleBar = TRUE, BOOL bDrawMarkers = TRUE, BOOL bDrawPolylines = TRUE, BOOL bDrawPolygons = TRUE, BOOL bDrawCircles = TRUE, BOOL bDrawGPSTracks = TRUE);
  virtual HRESULT Draw(CRenderTarget* pRenderTarget, const CRect& rClient, const CRect* prClip = NULL, BOOL bDrawScrollRose = TRUE, BOOL bDrawZoomBar = TRUE, BOOL bDrawScaleBar = TRUE, BOOL bDrawMarkers = TRUE, BOOL bDrawPolylines = TRUE, BOOL bDrawPolygons = TRUE, BOOL bDrawCircles = TRUE, BOOL bDrawGPSTracks = TRUE, BOOL bResetDPI = TRUE, BOOL bUseInMemoryCache = TRUE);
#endif
  double GetZoom() const { return m_fZoom; };
  BOOL SetZoom(double fZoom, BOOL bAnimation);
  BOOL SetCenter(const COSMCtrlPosition& position, BOOL bAnimation);
  BOOL SetCenterAndZoom(const COSMCtrlPosition& position, double fZoom, BOOL bAnimation);
  COSMCtrlPosition GetCenter() const { return m_CenterPosition; };
  BOOL ZoomToBounds(const COSMCtrlPosition& position1, const COSMCtrlPosition& position2, BOOL bAnimation);
  CString GetCacheDirectory() const { return m_sCacheDirectory; };
  void SetCacheDirectory(const CString& sCacheDirectory, BOOL bInvalidateCache = FALSE);
  BOOL GetDownloadTiles() const { return m_bDownloadTiles; };
  void SetDownloadTiles(BOOL bDownloadTiles);
  BOOL GetIfModifiedSinceHeader() const { return m_bUseIfModifiedSinceHeader; };
  void SetIfModifiedSinceHeader(BOOL bUseIfModifiedSinceHeader);
  void Refresh();
  void SetDrawCenterCrossHairs(BOOL bDrawCenterCrossHairs);
  BOOL GetDrawCenterCrossHairs() const { return m_bDrawCenterCrossHairs; };
  void SetMapMode(MapMode mode);
  MapMode GetMapMode() const { return m_MapMode; };
  BOOL GetDrawScrollRose() const { return m_bDrawScrollRose; };
  BOOL SetDrawScrollRose(BOOL bDrawScrollRose);
  BOOL GetScrollRoseAnchorPosition() const { return m_ScrollRoseAnchorPosition; };
  BOOL SetScrollRoseAnchorPosition(ControlAnchorPosition controlAnchorPosition);
  CPoint GetScrollRoseAnchorPositionOffset() const { return m_ptOffsetScrollRose; };
  BOOL SetScrollRoseAnchorPositionOffset(CPoint ptOffset);
  BOOL GetDrawZoomBar() const { return m_bDrawZoomBar; };
  BOOL SetDrawZoomBar(BOOL bDrawZoomBar);
  BOOL GetZoomBarAnchorPosition() const { return m_ZoomBarAnchorPosition; };
  BOOL SetZoomBarAnchorPosition(ControlAnchorPosition controlAnchorPosition);
  CPoint GetZoomBarAnchorPositionOffset() const { return m_ptOffsetZoomBar; };
  BOOL SetZoomBarAnchorPositionOffset(CPoint ptOffset);
  BOOL GetUseTransparencyForZoomBar() const { return m_bTransparencyForZoomBar; };
  void SetUseTransparencyForZoomBar(BOOL bUseTransparencyForZoomBar);
  BOOL GetDrawZoomBarAsSlider() const { return m_bDrawZoomBarAsSlider; };
  BOOL SetDrawZoomBarAsSlider(BOOL bDrawZoomBarAsSlider);
  BOOL GetDrawScaleBar() const { return m_bDrawScaleBar; };
  void SetDrawScaleBar(BOOL bDrawScaleBar);
  BOOL GetScaleBarAnchorPosition() const { return m_ScaleBarAnchorPosition; };
  void SetScaleBarAnchorPosition(ControlAnchorPosition controlAnchorPosition);
  CPoint GetScaleBarAnchorPositionOffset() const { return m_ptOffsetScaleBar; };
  void SetScaleBarAnchorPositionOffset(CPoint ptOffset);
  void SetScaleBarUnits(ScaleBarUnits units) { m_ScaleBarUnits = units; };
  ScaleBarUnits GetScaleBarUnits() const { return m_ScaleBarUnits; };
  BOOL GetDrawCopyright() const { return m_bDrawCopyright; };
  BOOL SetDrawCopyright(BOOL bDrawCopyright);
  BOOL GetCopyrightAnchorPosition() const { return m_CopyrightAnchorPosition; };
  BOOL SetCopyrightAnchorPosition(ControlAnchorPosition controlAnchorPosition);
  CPoint GetCopyrightAnchorPositionOffset() const { return m_ptOffsetCopyright; };
  BOOL SetCopyrightAnchorPositionOffset(CPoint ptOffset);
  BOOL GetAllowDrag() const { return m_bAllowDrag; };
  void SetAllowDrag(BOOL bAllowDrag) { m_bAllowDrag = bAllowDrag; };
  BOOL GetDeltaMode() const { return m_bDeltaMode; };
  void SetDeltaMode(BOOL bDeltaMode);
  UINT GetDeltaModeTimerInterval() const { return m_nDeltaTimerInterval; };
  void SetDeltaModeTimerInterval(UINT nDeltaTimerInterval) { m_nDeltaTimerInterval = nDeltaTimerInterval; };
  BOOL GetAllowKeyboard() const { return m_bAllowKeyboard; };
  void SetAllowKeyboard(BOOL bAllowKeyboard) { m_bAllowKeyboard = bAllowKeyboard; };
  BOOL GetAllowMouseZoom() const { return m_bAllowMouseZoom; };
  void SetAllowMouseZoom(BOOL bAllowMouseZoom) { m_bAllowMouseZoom = bAllowMouseZoom; };
  BOOL GetAllowDoubleClickZoom() const { return m_bAllowDoubleClickZoom; };
  void SetAllowDoubleClickZoom(BOOL bAllowDoubleClickZoom) { m_bAllowDoubleClickZoom = bAllowDoubleClickZoom; };
  BOOL GetAllowUsePreviousZoomStretch() const { return m_bAllowPreviousZoomStretch; };
  void SetAllowUsePreviousZoomStretch(BOOL bAllowPreviousZoomStretch) { m_bAllowPreviousZoomStretch = bAllowPreviousZoomStretch; };
  BOOL GetAllowUseNextZoomSqueeze() const { return m_bAllowNextZoomSqueeze; };
  void SetAllowUseNextZoomSqueeze(BOOL bAllowNextZoomSqueeze) { m_bAllowNextZoomSqueeze = bAllowNextZoomSqueeze; };
  CString GetUserAgent() const { return m_sUserAgent; };
  void SetUserAgent(const CString& sUserAgent) { m_sUserAgent = sUserAgent; };
#ifndef COSMCTRL_NOANIMATION
  BOOL GetDoAnimations() const { return m_bAnimations; };
  void SetDoAnimations(BOOL bAnimations) { m_bAnimations = bAnimations; };
#endif
  DownloadOrder GetDownloadOrder() const { return m_DownloadOrder; };
  void SetDownloadOrder(DownloadOrder downloadOrder) { m_DownloadOrder = downloadOrder; };
  BOOL GetDrawTileOutlines() const { return m_bDrawTileOutlines; };
  void SetDrawTileOutlines(BOOL bDrawTileOutlines);
  MapItem HitTest(const CPoint& point, INT_PTR& nItem, INT_PTR& nSubItem, const CRect& rClient) const;
  BOOL GetBoundingRect(const COSMCtrlMarker& marker, CRect& rBounds, const CRect& rClient) const;
  BOOL GetBoundingRect(const COSMCtrlPolyline& polyline, CRect& rBounds, const CRect& rClient) const;
  BOOL GetBoundingRect(const COSMCtrlPolygon& polygon, CRect& rBounds, const CRect& rClient) const;
  BOOL GetBoundingRect(const COSMCtrlCircle& circle, CRect& rBounds, const CRect& rClient) const;
  BOOL GetBoundingRectGPSTrack(CRect& rBounds, const CRect& rClient) const;
  BOOL GetBoundingNodeRect(const COSMCtrlPolyline& polyline, INT_PTR nIndex, CRect& rBounds, const CRect& rClient) const;
  BOOL GetBoundingNodeRect(const COSMCtrlPolygon& polygon, INT_PTR nIndex, CRect& rBounds, const CRect& rClient) const;
  void GetBoundingRectMarkers(BOOL bOnlySelectedMarkers, COSMCtrlPosition& topLeft, COSMCtrlPosition& bottomRight) const;
  void GetBoundingRectPolylines(BOOL bOnlySelectedPolylines, COSMCtrlPosition& topLeft, COSMCtrlPosition& bottomRight) const;
  void GetBoundingRectPolygons(BOOL bOnlySelectedPolygons, COSMCtrlPosition& topLeft, COSMCtrlPosition& bottomRight) const;
  void GetBoundingRectCircles(BOOL bOnlySelectedCircles, COSMCtrlPosition& topLeft, COSMCtrlPosition& bottomRight, const CRect& rClient) const;
  void GetBoundingRectAllItems(BOOL bOnlySelectedItems, COSMCtrlPosition& topLeft, COSMCtrlPosition& bottomRight, const CRect& rClient) const;
#ifdef COSMCTRL_NOD2D
  BOOL ClientToPosition(const Gdiplus::PointF& point, const CRect& rClient, COSMCtrlPosition& position) const;
  BOOL PositionToClient(const COSMCtrlPosition& position, const CRect& rClient, Gdiplus::PointF& point) const;
#else
  BOOL ClientToPosition(const CD2DPointF& point, const CRect& rClient, COSMCtrlPosition& position) const;
  BOOL PositionToClient(const COSMCtrlPosition& position, const CRect& rClient, CD2DPointF& point) const;
#endif
  int GetDownlodTilesEdgeCount() const { return m_nDownlodTilesEdgeCount; };
  void SetDownloadTilesEdgeCount(int nDownloadTilesEdgeCount) { m_nDownlodTilesEdgeCount = nDownloadTilesEdgeCount; };
  virtual void ScrollTileToNorth();
  virtual void ScrollTileToSouth();
  virtual void ScrollTileToWest();
  virtual void ScrollTileToEast();
  virtual BOOL ScrollToNorth();
  virtual BOOL ScrollToSouth();
  virtual BOOL ScrollToWest();
  virtual BOOL ScrollToEast();
  void DeselectAllItems();
  void SelectAllItems();
  void SelectMarker(INT_PTR nIndex);
  void SelectPolyline(INT_PTR nIndex);
  void SelectPolylineNode(INT_PTR nIndex, INT_PTR nSubItemIndex);
  void SelectPolygon(INT_PTR nIndex);
  void SelectPolygonNode(INT_PTR nIndex, INT_PTR nSubItemIndex);
  void SelectCircle(INT_PTR nIndex);
  int  DeleteSelectedItems();
  INT_PTR GetFirstSelectedPolyline() const;
  INT_PTR GetFirstSelectedPolygon() const;
  INT_PTR GetFirstSelectedMarker() const;
  INT_PTR GetFirstSelectedCircle() const;
  INT_PTR ShowOperationsDialog(COSMCtrlMapOperationsDlg& dialog);
  BOOL DecimatePolyline(COSMCtrlPolyline& polyline, int nNewNodesPerWay, const CRect& rClient);
  BOOL DecimatePolygon(COSMCtrlPolygon& polyline, int nNewNodesPerWay, const CRect& rClient);
  void DeleteInMemoryCache();
  void SetMaxTilesInMemoryCache(INT_PTR nMaxTiles);
  INT_PTR GetMaxTilesInMemoryCache() const { return m_nMaxTilesInMemoryCache; };
  void SetScrollPixels(int nPixels) { m_nScrollPixels = nPixels; };
  int GetScrollPixels() const { return m_nScrollPixels; };
  void SetMaxGPSTracks(int nGPSTracks);
  int GetMaxGPSTracks() const { return m_nMaxGPSTracks; };
  virtual void AddGPSTrack(BOOL bGPSFix, double dLongitude, double dLatitude, double dBearing, double dSpeed, BOOL bBearingValid, BOOL bSpeedValid);
  IOSMCtrlHandler* GetEventHandler() const { return m_pEventHandler; };
  void SetEventHandler(IOSMCtrlHandler* pHandler) { m_pEventHandler = pHandler; };
  IOSMCtrlTileProvider* GetTileProvider() const { return m_pTileProvider; };
  void SetTileProvider(IOSMCtrlTileProvider* pTileProvider);

//Virtual Methods
  virtual HRESULT DownloadTile(HINTERNET hConnection, const CString& sObject, BOOL bUseIfModifiedSinceHeader, BOOL bForcedRefresh, int nZoom, int nTileX, int nTileY, const CString& sFile);
  virtual HRESULT DownloadPage(HINTERNET hConnection, const CString& sObject, BOOL bForcedRefresh, CStringA& sResponse);
  
//Member variables  
  CArray<COSMCtrlMarker, COSMCtrlMarker&>           m_Markers;                       //The marker array associated with this map
  CArray<COSMCtrlPolygon, COSMCtrlPolygon&>         m_Polygons;                      //The polygons array associated with this map
  CArray<COSMCtrlPolyline, COSMCtrlPolyline&>       m_Polylines;                     //The polylines array associated with this map
  CArray<COSMCtrlCircle, COSMCtrlCircle&>           m_Circles;                       //The circles array associated with this map
  CArray<COSMCtrlIcon*, COSMCtrlIcon*>              m_Icons;                         //The icon array associated with this map
  COSMCtrlPolygon                                   m_SelectionPolygon;              //The selection polygon
  COSMCtrlPolyline                                  m_GPSTrack;                      //The GPS track on the map
#ifdef COSMCTRL_NOD2D
  Gdiplus::Color                                    m_colorGPSTrackPointer;          //The color to use for the GPS track pointer
  Gdiplus::Color                                    m_colorGPSTrackNoBearingPointer; //The color to use for the GPS track pointer when we do not have a bearing
#else
  D2D1::ColorF                                      m_colorGPSTrackPointer;          //The color to use for the GPS track pointer
  D2D1::ColorF                                      m_colorGPSTrackNoBearingPointer; //The color to use for the GPS track pointer when we do not have a bearing
#endif
  BOOL                                              m_bGPSFix;                       //Do we currently have a GPS fix

protected:
//Message handlers
  afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
  afx_msg void OnDestroy();
  afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
  afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
  afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
  afx_msg void OnMouseMove(UINT nFlags, CPoint point);
  afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
  afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
  afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
  afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
  afx_msg BOOL OnToolTipText(UINT nID, NMHDR* pNMHDR, LRESULT* pResult);
  afx_msg void OnNMClickCopyright(NMHDR* pNMHDR, LRESULT* pResult);
  afx_msg LRESULT OnCreateChildControls(WPARAM wParam, LPARAM lParam);
  afx_msg void OnSize(UINT nType, int cx, int cy);
  afx_msg void OnTimer(UINT_PTR nIDEvent);
#ifndef COSMCTRL_NOD2D
  afx_msg LRESULT OnDraw2D(WPARAM wParam, LPARAM lParam);
  afx_msg LRESULT OnRecreatedResources(WPARAM wParam, LPARAM lParam);
#endif
  
  DECLARE_MESSAGE_MAP()

//Methods
  virtual INT_PTR  OnToolHitTest(CPoint point, TOOLINFO* pTI) const;
  static UINT      _DownloadThread(LPVOID pParam);
  virtual CPoint   GetControlPosition(ControlAnchorPosition anchorPosition, const CPoint& ptOffset, const CRect& rClient);
  virtual HRESULT  CreateSession(HINTERNET& hSession);
  virtual HRESULT  CreateConnection(HINTERNET hSession, const CString& sServer, INTERNET_PORT nPort, HINTERNET& hConnection);
  virtual HRESULT  CreateRequest(HINTERNET hConnection, const CString& sObject, BOOL bForcedRefresh, HINTERNET& hFile);
  virtual UINT     DownloadThread();
  virtual BOOL     CreateDownloadThread();
  virtual void     DestroyDownloadThread();
  virtual CString  FormIfModifiedSinceHeader(const SYSTEMTIME& st);
  virtual BOOL     CreateZoomBarSlider();
  virtual BOOL     CreateCopyrightLinkCtrl();
  virtual CString  GetDeltaFile(const CString& sFile);
  virtual BOOL     HitTest(const CPoint& point, const COSMCtrlPolyline& polyline, const CRect& rClient) const;
  virtual BOOL     HitTest(const CPoint& point, const COSMCtrlPolygon& polygon, const CRect& rClient) const;
  virtual MapItem  HitTest(const CPoint& point, const COSMCtrlCircle& circle, const CRect& rClient) const;
#ifdef COSMCTRL_NOD2D
  virtual BOOL     DrawTileNotAvailable(Gdiplus::Graphics& graphics, const Gdiplus::RectF& rTile);
  virtual BOOL     DrawTile(Gdiplus::Graphics& graphics, const Gdiplus::RectF& rTile, int nTileX, int nTileY);
  virtual BOOL     DrawTileOutline(Gdiplus::Graphics& graphics, const Gdiplus::RectF& rTile);
  virtual BOOL     DrawScrollRose(Gdiplus::Graphics& graphics, const CRect& rClip, const CRect& rClient);
  virtual BOOL     DrawZoomBar(Gdiplus::Graphics& graphics, const CRect& rClip, const CRect& rClient);
  virtual BOOL     DrawScaleBar(Gdiplus::Graphics& graphics, const CRect& rClip, const CRect& rClient);
  virtual BOOL     DrawCenterCrossHairs(Gdiplus::Graphics& graphics, const CRect& rClip, const CRect& rClient);
  virtual int      DrawPolylines(Gdiplus::Graphics& graphics, const CRect& rClip, const CRect& rClient);
  virtual BOOL     DrawPolyline(Gdiplus::Graphics& path, const COSMCtrlPolyline& polyline, const CRect& rClient);
  virtual int      DrawPolygons(Gdiplus::Graphics& graphics, const CRect& rClip, const CRect& rClient);
  virtual BOOL     DrawPolygon(Gdiplus::Graphics& graphics, const COSMCtrlPolygon& polygon, const CRect& rClient);
  virtual BOOL     DrawPolygonOutline(Gdiplus::GraphicsPath& path, const COSMCtrlPolygon& polygon, const CRect& rClient) const;
  virtual BOOL     DrawPolygonInternal(Gdiplus::GraphicsPath& path, const COSMCtrlPolygon& polygon, const CRect& rClient) const;
  virtual BOOL     DrawCircle(Gdiplus::Graphics& graphics, const COSMCtrlCircle& circle, const CRect& rClient) const;
  virtual BOOL     DrawCircle(Gdiplus::GraphicsPath& path, const COSMCtrlCircle& circle, BOOL bCircumference, const CRect& rClient) const;
  virtual int      DrawCircles(Gdiplus::Graphics& graphics, const CRect& rClip, const CRect& rClient);
  virtual int      DrawMarkers(Gdiplus::Graphics& graphics, const CRect& rClip, const CRect& rClient);
  virtual BOOL     DrawMarker(Gdiplus::Graphics& graphics, const COSMCtrlMarker& marker, const CRect& rClient);
  virtual BOOL     DrawGPSTrack(Gdiplus::Graphics& graphics, const CRect& rClient);
  virtual BOOL     DrawGPSTrackTriangle(Gdiplus::Graphics& graphics, const CRect& rClient);
  BOOL             Draw3dRect(Gdiplus::Graphics& graphics, const Gdiplus::RectF& r, const Gdiplus::Color& clrTopLeft, const Gdiplus::Color& clrBottomRight);
  virtual Gdiplus::CachedBitmap* GetCachedBitmap(Gdiplus::Graphics& graphics, const CStringW& sFile, int nTileX, int nTileY, int nZoom, Gdiplus::Bitmap*& pBitmap);
#else
  virtual BOOL     DrawTileNotAvailable(CRenderTarget* pRenderTarget, const CD2DRectF& rTile);
  virtual BOOL     DrawTileFromCache(CRenderTarget* pRenderTarget, const CD2DRectF& rTile, int nTileX, int nTileY);
  virtual BOOL     DrawTileWithoutCache(CRenderTarget* pRenderTarget, const CD2DRectF& rTile, int nTileX, int nTileY);
  virtual BOOL     DrawTileOutline(CRenderTarget* pRenderTarget, const CD2DRectF& rTile);
  virtual BOOL     DrawScrollRose(CRenderTarget* pRenderTarget, const CRect& rClip, const CRect& rClient);
  virtual BOOL     DrawZoomBar(CRenderTarget* pRenderTarget, const CRect& rClip, const CRect& rClient);
  virtual BOOL     DrawScaleBar(CRenderTarget* pRenderTarget, const CRect& rClip, const CRect& rClient);
  virtual BOOL     DrawCenterCrossHairs(CRenderTarget* pRenderTarget, const CRect& rClip, const CRect& rClient);
  virtual int      DrawPolylines(CRenderTarget* pRenderTarget, const CRect& rClip, const CRect& rClient);
  virtual BOOL     DrawPolyline(CRenderTarget* pRenderTarget, const COSMCtrlPolyline& polyline, const CRect& rClient);
  virtual int      DrawPolygons(CRenderTarget* pRenderTarget, const CRect& rClip, const CRect& rClient);
  virtual BOOL     DrawPolygon(CRenderTarget* pRenderTarget, const COSMCtrlPolygon& polygon, const CRect& rClient);
  virtual BOOL     DrawCircle(CRenderTarget* pRenderTarget, const COSMCtrlCircle& circle, const CRect& rClient) const;
  virtual int      DrawCircles(CRenderTarget* pRenderTarget, const CRect& rClip, const CRect& rClient);
  virtual int      DrawMarkers(CRenderTarget* pRenderTarget, const CRect& rClip, BOOL bUseInMemoryCache, const CRect& rClient);
  virtual BOOL     DrawMarkerInternal(CRenderTarget* pRenderTarget, const COSMCtrlMarker& marker, CD2DBitmap* pBitmap, CD2DPointF& ptMarker);
  virtual BOOL     DrawMarkerWithoutCache(CRenderTarget* pRenderTarget, const COSMCtrlMarker& marker, const CRect& rClient);
  virtual BOOL     DrawMarkerFromCache(CRenderTarget* pRenderTarget, const COSMCtrlMarker& marker, const CRect& rClient);
  virtual BOOL     DrawGPSTrack(CRenderTarget* pRenderTarget, const CRect& rClient);
  virtual BOOL     DrawGPSTrackTriangle(CRenderTarget* pRenderTarget, const CRect& rClient);
  BOOL             Draw3dRect(CRenderTarget* pRenderTarget, const CD2DRectF& r, const D2D1::ColorF& clrTopLeft, const D2D1::ColorF& clrBottomRight);
  virtual CD2DBitmap* GetCachedBitmap(CRenderTarget* pRenderTarget, const CString& sFile, int nTileX, int nTileY, int nZoom);
#endif
  virtual CString  FormScaleBarText(double fScaleDistance, BOOL bMetric);
  virtual CString  FormCopyrightText();
  virtual void     PerformInMemoryGPSTrackMaintenance(INT_PTR nMaxTracks);
  virtual void     EnsureBoundingRectForGPSTrackTriangle(const COSMCtrlPosition& position, CRect& rBounds, const CRect& rClient) const;
  virtual BOOL     UseMetric();
  virtual void     StartDrag(const CPoint& point, MapItem item, INT_PTR nItem, INT_PTR nSubItem);
  virtual void     CalculateScaleBar(double& fDistance, double& fScaleDistance, BOOL& bMetric, int& nScaleLength);
#ifdef COSMCTRL_NOD2D
  virtual CStringW GetFileForMemoryCache(const CStringW& sFile);
#else
  virtual CString  GetFileForMemoryCache(const CString& sFile);
#endif
  virtual void     PerformInMemoryCacheMaintenance(INT_PTR nMaxTiles);
  virtual void     InitializeNewPolyline(COSMCtrlPolyline& polyline);
  virtual void     InitializeNewPolygon(COSMCtrlPolygon& polygon);
  virtual void     InitializeNewSelectionPolygon(COSMCtrlPolygon& polygon);
  virtual void     InitializeNewMarker(COSMCtrlMarker& marker);
  virtual void     InitializeNewCircle(COSMCtrlCircle& circle);
  virtual void     HandleLButtonDownStandard(UINT nFlags, CPoint point, MapItem item, INT_PTR nItem, INT_PTR nSubItem);
  virtual void     HandleLButtonDownPolylineCreation(UINT nFlags, CPoint point, MapItem item, INT_PTR nItem, INT_PTR nSubItem);
  virtual void     HandleLButtonDownPolygonCreation(UINT nFlags, CPoint point, MapItem item, INT_PTR nItem, INT_PTR nSubItem);
  virtual void     HandleLButtonDownPolylineCreationWithNode(UINT nFlags, CPoint point, MapItem item, INT_PTR nItem, INT_PTR nSubItem);
  virtual void     HandleLButtonDownPolygonCreationWithNode(UINT nFlags, CPoint point, MapItem item, INT_PTR nItem, INT_PTR nSubItem);
  virtual void     HandleLButtonDownMarkerCreation(UINT nFlags, CPoint point, MapItem item, INT_PTR nItem, INT_PTR nSubItem);
  virtual void     HandleLButtonDownCircleCreation(UINT nFlags, CPoint point, MapItem item, INT_PTR nItem, INT_PTR nSubItem);
  virtual void     HandleLButtonDownCircleCircumference(UINT nFlags, CPoint point, MapItem item, INT_PTR nItem, INT_PTR nSubItem);
  virtual void     HandleLButtonDownSelection(UINT nFlags, CPoint point, MapItem item, INT_PTR nItem, INT_PTR nSubItem);
  virtual void     HandleLButtonDownPolygonNode(UINT nFlags, CPoint point, MapItem item, INT_PTR nItem, INT_PTR nSubItem);
  virtual void     HandleLButtonDownPolygon(UINT nFlags, CPoint point, MapItem item, INT_PTR nItem, INT_PTR nSubItem);
  virtual void     HandleLButtonDownPolylineNode(UINT nFlags, CPoint point, MapItem item, INT_PTR nItem, INT_PTR nSubItem);
  virtual void     HandleLButtonDownPolyline(UINT nFlags, CPoint point, MapItem item, INT_PTR nItem, INT_PTR nSubItem);
  virtual void     HandleLButtonDownMarker(UINT nFlags, CPoint point, MapItem item, INT_PTR nItem, INT_PTR nSubItem);
  virtual void     HandleLButtonDownCircle(UINT nFlags, CPoint point, MapItem item, INT_PTR nItem, INT_PTR nSubItem);
  virtual void     HandleLButtonDownMap(UINT nFlags, CPoint point, MapItem item, INT_PTR nItem, INT_PTR nSubItem);
  virtual void     HandleMouseMoveSelection(UINT nFlags, CPoint point);
  virtual void     HandleMouseMoveCircleCreation(UINT nFlags, CPoint point);
  virtual void     HandleMouseMovePolylineCreation(UINT nFlags, CPoint point);
  virtual void     HandleMouseMovePolygonCreation(UINT nFlags, CPoint point);
  virtual void     HandleMouseMovePolygonCreationWithNode(UINT nFlags, CPoint point);
  virtual void     HandleMouseMoveMap(UINT nFlags, CPoint point);
  virtual void     HandleMouseMoveMarker(UINT nFlags, CPoint point);
  virtual void     HandleMouseMoveCircle(UINT nFlags, CPoint point);
  virtual void     HandleMouseMovePolylineNode(UINT nFlags, CPoint point);
  virtual void     HandleMouseMovePolyline(UINT nFlags, CPoint point);
  virtual void     HandleMouseMovePolygonNode(UINT nFlags, CPoint point);
  virtual void     HandleMouseMovePolygon(UINT nFlags, CPoint point);
  virtual void     HandleLButtonDblClickPolyline(UINT nFlags, CPoint point, MapItem item, INT_PTR nItem, INT_PTR nSubItem);
  virtual void     HandleLButtonDblClickPolygon(UINT nFlags, CPoint point, MapItem item, INT_PTR nItem, INT_PTR nSubItem);
  virtual void     HandleLButtonDblClickMap(UINT nFlags, CPoint point, MapItem item, INT_PTR nItem, INT_PTR nSubItem);
  virtual void     HandleLButtonDblClickMarker(UINT nFlags, CPoint point, MapItem item, INT_PTR nItem, INT_PTR nSubItem);
  virtual void     HandleLButtonDblClickCircle(UINT nFlags, CPoint point, MapItem item, INT_PTR nItem, INT_PTR nSubItem);
#ifndef COSMCTRL_NOANIMATION
  virtual HRESULT  CreateTransition(double fOldZoom, double fNewZoom, double fOldLongitude, double fNewLongitude, double fOldLatitude, double fNewLatitude);
  virtual void     HandleAnimationTimerEvent();
#endif
  
//Member variables
  double                                      m_fZoom;                           //The current zoom level
  double                                      m_fFinalZoom;                      //The final zoom level (used for animations)
  COSMCtrlPosition                            m_CenterPosition;                  //The center point of the map
  COSMCtrlPosition                            m_FinalCenterPosition;             //The final center position (used for animations)
  CString                                     m_sCacheDirectory;                 //The directory where downloaded OSM tiles will be cached
  IOSMCtrlTileProvider*                       m_pTileProvider;                   //The tileprovider for this map
  COSMCtrlMapnikTileProvider                  m_DefaultTileProvider;             //The default tile provider (Mapnik)
  BOOL                                        m_bDownloadTiles;                  //Should we download tiles
  CCriticalSection                            m_csData;                          //critical section used to serialize access to our configuration settings
  CCriticalSection                            m_csThread;                        //critical section used to serialize access to our worker thread
  CWinThread*                                 m_pDownloadThread;                 //The pointer for the download thread
  CEvent                                      m_DownloadTerminateEvent;          //Event using to terminate the thread
  BOOL                                        m_bMouseCapturedForDrag;           //Is the mouse currently captured for dragging the map
  BOOL                                        m_bMouseCapturedForZoom;           //Is the mouse currently captured for zooming the map
  double                                      m_dLatitudeMouseCapture;           //The latitude coordinates where the mouse was captured
  double                                      m_dLongitudeMouseCapture;          //The longitude coordinates where the mouse was captured
  CPoint                                      m_pointMouseCapture;               //the client coordinates where we started dragging from
  BOOL                                        m_bUseIfModifiedSinceHeader;       //Should we use a If-Modified-Since header in the HTTP requests for tiles
  volatile LONG                               m_lForcedRefresh;                  //Used to force a refresh of cached tiles
  BOOL                                        m_bDrawTileOutlines;               //Should we draw outlines around all the tiles
  CRect                                       m_rClientForThread;                //The client rect which the download thread will operate against
  BOOL                                        m_bDrawScrollRose;                 //Should we draw the scroll rose
  ControlAnchorPosition                       m_ScrollRoseAnchorPosition;        //The anchor position for the scroll rose
  CPoint                                      m_ptOffsetScrollRose;              //the offset position for the scroll rose
  BOOL                                        m_bDrawZoomBar;                    //Should we draw the zoom bar
  ControlAnchorPosition                       m_ZoomBarAnchorPosition;           //The anchor position for the Zoom bar
  CPoint                                      m_ptOffsetZoomBar;                 //the offset position for the Zoom bar
  BOOL                                        m_bTransparencyForZoomBar;         //Should we use transparency to draw the zoom bar
  BOOL                                        m_bDrawScaleBar;                   //Should we draw the scale bar
  ControlAnchorPosition                       m_ScaleBarAnchorPosition;          //The anchor position for the Scale bar
  CPoint                                      m_ptOffsetScaleBar;                //the offset position for the Scale bar
  BOOL                                        m_bAllowDrag;                      //Is the map allowed to be dragged
  BOOL                                        m_bAllowKeyboard;                  //Is the map allowed to be scrolled/zoomed via the keyboard
  BOOL                                        m_bDeltaMode;                      //Is "Delta" mode enabled
  UINT                                        m_nDeltaTimerInterval;             //The delta timer interval in ms
  UINT_PTR                                    m_nDeltaTimerID;                   //The ID of the delta timer
  BOOL                                        m_bAllowMouseZoom;                 //Is the map allowed to be zoomed via the mouse wheel
  BOOL                                        m_bAllowDoubleClickZoom;           //Is the map allowed to be zoomed via a double click
  BOOL                                        m_bAllowPreviousZoomStretch;       //Do we allow the previous zoom level image to be stretched
  BOOL                                        m_bAllowNextZoomSqueeze;           //Do we allow the next zoom level image to be squeezed
  CString                                     m_sUserAgent;                      //The HTTP_USER_AGENT header to use
  DownloadOrder                               m_DownloadOrder;                   //The order in which we will download tiles
  int                                         m_nDownlodTilesEdgeCount;          //The number of rows or columns of tiles to cache which are outside of the visible client area
  CRect                                       m_rNorthScrollRose;                //Used for hittesting
  CRect                                       m_rSouthScrollRose;                //Used for hittesting
  CRect                                       m_rEastScrollRose;                 //Used for hittesting
  CRect                                       m_rWestScrollRose;                 //Used for hittesting
  CRect                                       m_rZoomInZoomBar;                  //Used for hittesting
  CRect                                       m_rZoomOutZoomBar;                 //Used for hittesting
  CRect                                       m_rZoomBar;                        //Used for hittesting
  HINTERNET                                   m_hSession;                        //The Wininet session handle we are using for downloads
  CSliderCtrl                                 m_ctrlZoomBar;                     //The Zoom bar if we are using a native slider control
  BOOL                                        m_bDrawZoomBarAsSlider;            //Should we use a slider control for the zoom bar
  TCHAR                                       m_szCurrentToolTipText[1024];      //The current tooltip text
  MapItem                                     m_MouseDragItem;                   //What we are dragging
  MapItem                                     m_ToolHitItem;                     //What we hit for tooltips
  INT_PTR                                     m_nMouseDragItemIndex;             //The index value associated with what we are dragging
  INT_PTR                                     m_nMouseDragSubItemIndex;          //The node index value associated with what we are dragging
  CSize                                       m_DragOffset;                      //Used to implement smooth / non jumpy dragging of markers, polylines and polygons
  CArray<COSMCtrlNode, COSMCtrlNode&>         m_OriginalDragNodes; //The nodes we are dragging from (used for dragging of polylines and polygons)
  BOOL                                        m_bModifyingCircleRadius;          //Ised to implement circle editing
  MapMode                                     m_MapMode;                         //The mapping mode currently active
  BOOL                                        m_bDrawCenterCrossHairs;           //Should cross hairs be shown on the map
  BOOL                                        m_bDrawCopyright;                  //Should we draw the copyright on the map
  ControlAnchorPosition                       m_CopyrightAnchorPosition;         //The anchor position for the copyright message
  CPoint                                      m_ptOffsetCopyright;               //the offset position for the copyright message
  ScaleBarUnits                               m_ScaleBarUnits;                   //What units should we use for drawing in the scale bar
	CLinkCtrl                                   m_ctrlCopyright;                   //The copyright hyperlink control
	CSortedArrayEx<COSMCtrlCachedTile, CompareCOSMCtrlCachedTile, COSMCtrlCachedTile&> m_CachedTiles; //The cache of tiles we have
	INT_PTR                                     m_nMaxTilesInMemoryCache;          //The maximum number of tiles which are allowed in "m_CachedTiles"
	int                                         m_nInMemoryCachedTilesInsertionCounter; //The insertion counter value for the next item to be added to "m_CachedTiles"
	int                                         m_nScrollPixels;                   //The amount to scroll in pixels 
	int                                         m_nMaxGPSTracks;                   //The maximum number of GPS tracks to display on the map
  IOSMCtrlHandler*                            m_pEventHandler;                   //The event handler instance
#ifndef COSMCTRL_NOANIMATION
	ATL::CComPtr<IUIAnimationManager>           m_pAnimationManager;               //The Windows Animation manager
  ATL::CComPtr<IUIAnimationTimer>             m_pAnimationTimer;                 //The Windows Animation timer     
  ATL::CComPtr<IUIAnimationTransitionLibrary> m_pTransitionLibrary;              //The Windows Transaction Library
  ATL::CComPtr<IUIAnimationVariable>          m_pZoomLevelAnimationVariable;     //The animation variable for zooming
  ATL::CComPtr<IUIAnimationVariable>          m_pLatitudeAnimationVariable;      //The animation variable for latitude
  ATL::CComPtr<IUIAnimationVariable>          m_pLongitudeAnimationVariable;     //The animation variable for zooming
  BOOL                                        m_bAnimations;                     //Should we do animations
#endif

  friend class COSMCtrlMapOperationsDlg;
  friend class COSMCtrlTimerEventHandler;
};


#endif //__OSMCTRL_H__
