/*
Module : OSMCtrlPolyline.cpp
Purpose: Implementation for the COSMCtrlPolyline class
Created: PJN / 10-04-2011
History: PJN / None
                                                    
Copyright (c) 2011 by PJ Naughter (Web: www.naughter.com, Email: pjna@naughter.com)

All rights reserved.

Copyright / Usage Details:

You are allowed to include the source code in any product (commercial, shareware, freeware or otherwise) 
when your product is released in binary form. You are allowed to modify the source code in any way you want 
except you cannot modify the copyright details at the top of each module. If you want to distribute source 
code with your application, then you are only allowed to distribute versions released by the author. This is 
to maintain a single distribution point for the source code. 

*/


/////////////////////////////////  Includes  //////////////////////////////////

#include "stdafx.h"
#include "OSMCtrlPolyline.h"
#include "OSMCtrlHelper.h"


///////////////////////////////// Implementation //////////////////////////////


COSMCtrlPolyline::COSMCtrlPolyline() : m_fLinePenWidth(2),
									   m_bSelected(FALSE),
                                       m_fNodeWidth(4),
									   relatedBranch(-1),
                                       m_colorNode(0, 0, 0),
                                       m_fSelectionNodeWidth(4),
                                       m_colorSelectionNode(255, 0, 0),
                                       m_DrawingStyle(LinesOnly),
                                     #ifdef COSMCTRL_NOD2D                                     
                                       m_colorPen(128, 0, 0, 255),
                                       m_DashCap(Gdiplus::DashCapFlat),
                                       m_DashStyle(Gdiplus::DashStyleSolid),
                                       m_EndCap(Gdiplus::LineCapRound),
                                       m_StartCap(Gdiplus::LineCapRound),
                                       m_LineJoin(Gdiplus::LineJoinRound),
                                     #else
                                       m_colorPen(0, 0, 255, 128),
                                       m_DashCap(D2D1_CAP_STYLE_FLAT),
                                       m_DashStyle(D2D1_DASH_STYLE_SOLID),
                                       m_fMiterLimit(1),
                                       m_EndCap(D2D1_CAP_STYLE_ROUND),
                                       m_StartCap(D2D1_CAP_STYLE_ROUND),
                                       m_LineJoin(D2D1_LINE_JOIN_ROUND),
                                     #endif
                                       m_fDashOffset(0),
                                       m_bDraggable(FALSE),
                                       m_bVisible(TRUE),
                                       m_bEditable(FALSE),
                                       m_bHitTest(TRUE),
                                       m_nMinZoomLevel(COSMCtrlHelper::OSMMinZoom),
                                       m_nMaxZoomLevel(COSMCtrlHelper::OSMMaxZoom)
{
}

COSMCtrlPolyline::COSMCtrlPolyline(const COSMCtrlPolyline& polyline)
                                                                  #ifndef COSMCTRL_NOD2D
                                                                    : m_colorPen(D2D1::ColorF::Black),
                                                                      m_colorNode(D2D1::ColorF::Black),
                                                                      m_colorSelectionNode(D2D1::ColorF::Black)
                                                                  #endif
{
  *this = polyline;
}

COSMCtrlPolyline& COSMCtrlPolyline::operator=(const COSMCtrlPolyline& polyline)
{
  m_Nodes.Copy(polyline.m_Nodes);
  m_fLinePenWidth       = polyline.m_fLinePenWidth;
  relatedBranch			= polyline.relatedBranch;
  m_colorPen            = polyline.m_colorPen;
  m_bSelected			= polyline.m_bSelected;
  m_colorNode           = polyline.m_colorNode;
  m_colorSelectionNode  = polyline.m_colorSelectionNode;
  m_DashCap             = polyline.m_DashCap;
  m_DashStyle           = polyline.m_DashStyle;
  m_EndCap              = polyline.m_EndCap;
  m_StartCap            = polyline.m_StartCap;
  m_LineJoin            = polyline.m_LineJoin;
  m_fDashOffset         = polyline.m_fDashOffset;
#ifndef COSMCTRL_NOD2D
  m_fMiterLimit         = polyline.m_fMiterLimit;
#endif
  m_fNodeWidth          = polyline.m_fNodeWidth;
  m_fSelectionNodeWidth = polyline.m_fSelectionNodeWidth;
  m_DrawingStyle        = polyline.m_DrawingStyle;
  m_sToolTipText        = polyline.m_sToolTipText;
  m_bDraggable          = polyline.m_bDraggable;
  m_bVisible            = polyline.m_bVisible;
  m_bEditable           = polyline.m_bEditable;
  m_bHitTest            = polyline.m_bHitTest;
  m_nMinZoomLevel       = polyline.m_nMinZoomLevel;
  m_nMaxZoomLevel       = polyline.m_nMaxZoomLevel;
  
  return *this;
}

BOOL COSMCtrlPolyline::FullySelected() const
{
  //Check all the nodes
  for (INT_PTR i=0; i<m_Nodes.GetSize(); i++)
  {
    if (!m_Nodes.ElementAt(i).m_bSelected)
      return FALSE;
  }
  
  return TRUE;
}

BOOL COSMCtrlPolyline::AnySelected() const
{
  //Check all the nodes
  for (INT_PTR i=0; i<m_Nodes.GetSize(); i++)
  {
    if (m_Nodes.ElementAt(i).m_bSelected)
      return TRUE;
  }
  
  return FALSE;
}

void COSMCtrlPolyline::Select()
{
  //Mark all nodes as selected  
  for (INT_PTR i=0; i<m_Nodes.GetSize(); i++)
    m_Nodes.ElementAt(i).m_bSelected = TRUE;
   m_bSelected = TRUE;
}

void COSMCtrlPolyline::Deselect()
{
  //Mark all nodes as selected  
  for (INT_PTR i=0; i<m_Nodes.GetSize(); i++)
    m_Nodes.ElementAt(i).m_bSelected = FALSE;
  m_bSelected = FALSE;
}

double COSMCtrlPolyline::Length() const
{
  //What will be the return value from this function
  double dDistance = 0;
  
  //Accumulate the distance between all of the nodes in this polyline
  for (INT_PTR i=1; i<m_Nodes.GetSize(); i++)
    dDistance += COSMCtrlHelper::DistanceBetweenPoints(m_Nodes.ElementAt(i-1).m_Position, m_Nodes.ElementAt(i).m_Position, NULL, NULL);
  
  return dDistance;
}
  
double COSMCtrlPolyline::LastBearing() const
{
  double dBearing = 0;
  INT_PTR nNodes = m_Nodes.GetSize();
  COSMCtrlHelper::DistanceBetweenPoints(m_Nodes.ElementAt(nNodes-2).m_Position, m_Nodes.ElementAt(nNodes-1).m_Position, &dBearing, NULL);
  
  return dBearing;
}

BOOL COSMCtrlPolyline::GetBoundingRect(COSMCtrlPosition& topLeft, COSMCtrlPosition& bottomRight) const
{
  double fTopLatitude = -91;
  double fBottomLatitude = 91;
  double fLeftLongitude = 181;
  double fRightLongitude = -181;

  //Look through all the nodes to find the extreme values in latitude and longitude  
  INT_PTR nNodes = m_Nodes.GetSize();
  for (INT_PTR i=0; i<nNodes; i++)
  {
    const COSMCtrlNode& node = m_Nodes.ElementAt(i);
    if (node.m_Position.m_fLatitude > fTopLatitude)
      fTopLatitude = node.m_Position.m_fLatitude;
    if (node.m_Position.m_fLatitude < fBottomLatitude)
      fBottomLatitude = node.m_Position.m_fLatitude;
    if (node.m_Position.m_fLongitude < fLeftLongitude)
      fLeftLongitude = node.m_Position.m_fLongitude;
    if (node.m_Position.m_fLongitude > fRightLongitude)
      fRightLongitude = node.m_Position.m_fLongitude;
  }

  topLeft.m_fLatitude = fTopLatitude;
  topLeft.m_fLongitude = fLeftLongitude;
  bottomRight.m_fLatitude = fBottomLatitude;
  bottomRight.m_fLongitude = fRightLongitude;  
  
  return (nNodes != 0); //A polygon without no nodes fails this function
}
