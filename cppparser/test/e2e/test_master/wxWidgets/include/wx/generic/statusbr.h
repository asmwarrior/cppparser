/////////////////////////////////////////////////////////////////////////////
// Name:        wx/generic/statusbr.h
// Purpose:     wxStatusBarGeneric class
// Author:      Julian Smart
// Modified by: VZ at 05.02.00 to derive from wxStatusBarBase
// Created:     01/02/97
// Copyright:   (c) Julian Smart
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////
#ifndef _WX_GENERIC_STATUSBR_H_
#  define _WX_GENERIC_STATUSBR_H_
#  include "wx/defs.h"
#  if  wxUSE_STATUSBAR
#    include "wx/pen.h"
#    include "wx/arrstr.h"
// ----------------------------------------------------------------------------
// wxStatusBarGeneric
// ----------------------------------------------------------------------------
class WXDLLIMPEXP_CORE wxStatusBarGeneric : public wxStatusBarBase
{
public:
  wxStatusBarGeneric()
  {
    Init();
  }
  wxStatusBarGeneric(wxWindow* parent, wxWindowID winid = wxID_ANY, long style = wxSTB_DEFAULT_STYLE, const wxString& name = wxASCII_STR(wxStatusBarNameStr))
  {
    Init();
    Create(parent, winid, style, name);
  }
  virtual ~wxStatusBarGeneric();
  bool Create(wxWindow* parent, wxWindowID winid = wxID_ANY, long style = wxSTB_DEFAULT_STYLE, const wxString& name = wxASCII_STR(wxStatusBarNameStr));
    // implement base class methods
  void SetStatusWidths(int n, const int widths_field[]) override;
  bool GetFieldRect(int i, wxRect& rect) const override;
  void SetMinHeight(int height) override;
  int GetBorderX() const override
  {
    return m_borderX;
  }
  int GetBorderY() const override
  {
    return m_borderY;
  }
    // implementation only (not part of wxStatusBar public API):
  int GetFieldFromPoint(const wxPoint& point) const;
protected:
  void DoUpdateStatusText(int number) override;
    // event handlers
  void OnPaint(wxPaintEvent& event);
  void OnSize(wxSizeEvent& event);
  void OnLeftDown(wxMouseEvent& event);
  void OnRightDown(wxMouseEvent& event);
    // Responds to colour changes
  void OnSysColourChanged(wxSysColourChangedEvent& event);
protected:
  virtual void DrawFieldText(wxDC& dc, const wxRect& rc, int i, int textHeight);
  virtual void DrawField(wxDC& dc, int i, int textHeight);
  void SetBorderX(int x);
  void SetBorderY(int y);
  virtual void InitColours();
    // true if the status bar shows the size grip: for this it must have
    // wxSTB_SIZEGRIP style and the window it is attached to must be resizable
    // and not maximized (note that currently size grip is only used in wxGTK)
  bool ShowsSizeGrip() const;
    // returns the position and the size of the size grip
  wxRect GetSizeGripRect() const;
    // common part of all ctors
  void Init();
    // the last known size, fields widths must be updated whenever it's out of
    // date
  wxSize m_lastClientSize;
    // the absolute widths of the status bar panes in pixels
  wxArrayInt m_widthsAbs;
  int m_borderX;
  int m_borderY;
  wxPen m_mediumShadowPen;
  wxPen m_hilightPen;
  wxSize DoGetBestSize() const override;
private:
    // Update m_lastClientSize and m_widthsAbs from the current size.
  void DoUpdateFieldWidths();
  wxDECLARE_EVENT_TABLE();
  wxDECLARE_DYNAMIC_CLASS_NO_COPY(wxStatusBarGeneric);
};
#  endif
#endif
    // _WX_GENERIC_STATUSBR_H_
