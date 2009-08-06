///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Apr 16 2008)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __wiGuiData__
#define __wiGuiData__

#include <wx/intl.h>

class wiStatBar;

#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/toolbar.h>
#include <wx/sizer.h>
#include <wx/listctrl.h>
#include <wx/choice.h>
#include <wx/listbox.h>
#include <wx/bmpbuttn.h>
#include <wx/button.h>
#include <wx/radiobox.h>
#include <wx/textctrl.h>
#include <wx/checkbox.h>
#include <wx/spinctrl.h>
#include <wx/statbox.h>
#include <wx/gbsizer.h>
#include <wx/scrolwin.h>
#include <wx/panel.h>
#include <wx/richtext/richtextctrl.h>
#include <wx/splitter.h>
#include <wx/datectrl.h>
#include <wx/dateevt.h>
#include <wx/treectrl.h>
#include <wx/statline.h>
#include <wx/aui/auibook.h>
#include <wx/statusbr.h>
#include <wx/frame.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////

#define wxID_TOOLNEW 1000
#define wxID_TOOLEDIT 1001
#define wxID_TOOLDEL 1002
#define wxID_TLPROFNEW 1003
#define wxID_TLPROFCLONE 1004
#define wxID_TLPROFDEL 1005
#define wxID_TLPROFSAVE 1006
#define wxID_TOOLGO 1007
#define wxID_TOOLPAUSE 1008
#define wxID_TOOLSTOP 1009
#define wxID_TLREFRESH 1010
#define wxID_TLFILTER 1011
#define wxID_TLDATE 1012
#define wxID_TLSTATUS 1013
#define wxID_TLSAVE 1014
#define wxID_TLCONNECT 1015
#define wxID_TLNEW 1016
#define wxID_TLEDIT 1017
#define wxID_TLDELETE 1018
#define wxID_TLLANGAPPLY 1019

///////////////////////////////////////////////////////////////////////////////
/// Class MainForm
///////////////////////////////////////////////////////////////////////////////
class MainForm : public wxFrame 
{
	private:
	
	protected:
		wxAuiNotebook* m_mainnb;
		wxPanel* m_pTasks;
		wxSplitterWindow* m_splitTasks;
		wxPanel* m_panObjects;
		wxToolBar* m_toolBarObject;
		wxListCtrl* m_lstObjectList;
		wxToolBar* m_toolBar6;
		wxChoice* m_chProfile;
		wxScrolledWindow* m_panTaskOpts;
		wxStaticText* m_stTransports;
		wxListBox* m_listBox1;
		wxBitmapButton* m_bpAddTransp;
		wxBitmapButton* m_bpDelTrasp;
		wxStaticText* m_stLogLevel;
		wxChoice* m_chLogLevel;
		wxRadioBox* m_rbDepth;
		wxStaticText* m_stDepth;
		wxTextCtrl* m_txtDepth;
		wxCheckBox* m_cbInvent;
		wxCheckBox* m_chIgnoreParams;
		wxStaticText* m_stThreads;
		wxSpinCtrl* m_spinCtrl1;
		
		wxPanel* m_panTasks;
		wxToolBar* m_toolBarTasks;
		wxListCtrl* m_lstTaskList;
		wxRichTextCtrl* m_rtTask;
		wxPanel* m_pReports;
		wxSplitterWindow* m_split;
		wxPanel* m_panRepTree;
		wxToolBar* m_toolBarFilter;
		wxChoice* m_chTaskFilter;
		wxDatePickerCtrl* m_dateFilter;
		wxTreeCtrl* m_treeScans;
		wxPanel* m_panRepData;
		wxRichTextCtrl* m_richText2;
		wxPanel* m_pSettings;
		wxStaticText* m_stConVers;
		wxStaticText* m_stConVersData;
		wxStaticText* m_stServer;
		wxChoice* m_chServers;
		wxToolBar* m_toolBarSrv;
		wxStaticText* m_stSrvVers;
		wxStaticText* m_stSrvVersData;
		wxStaticText* m_stLang;
		wxChoice* m_chLangs;
		wxToolBar* m_toolBarLang;
		wxStaticText* m_stLangRestart;
		
		wxStaticLine* m_staticline2;
		wxPanel* m_pnServer;
		wxStaticText* m_staticText18;
		wxChoice* m_chStorage;
		wxBitmapButton* m_bpStorageApply;
		
		wxBitmapButton* m_bpPlgRefresh;
		wxStaticText* m_staticText19;
		wxScrolledWindow* m_pluginsDock;
		wxGridBagSizer* m_gbPluginsGrid;
		wxButton* m_btPluginIcon;
		wxButton* m_btPluginDesc;
		wxButton* m_btPluginIface;
		wxButton* m_btPluginSetup;
		
		wiStatBar* m_statusBar;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnClose( wxCloseEvent& event ){ event.Skip(); }
		virtual void OnAddObject( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnEditObject( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnDelObject( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnTaskKillFocus( wxFocusEvent& event ){ event.Skip(); }
		virtual void OnSelectObject( wxListEvent& event ){ event.Skip(); }
		virtual void OnTaskApply( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnRunTask( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnCancelTask( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnSortItems( wxListEvent& event ){ event.Skip(); }
		virtual void OnTaskSelected( wxListEvent& event ){ event.Skip(); }
		virtual void OnReportsRefresh( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnReportsFilter( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnReportTskFilter( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnReportsDate( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnReportDateFilter( wxDateEvent& event ){ event.Skip(); }
		virtual void OnReportsStatus( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnReportsSave( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnReportExpand( wxTreeEvent& event ){ event.Skip(); }
		virtual void OnReportSelected( wxTreeEvent& event ){ event.Skip(); }
		virtual void OnConnect( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnAddServer( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnEditServer( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnDelServer( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnLangChange( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnStorageChange( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnPlgRefresh( wxCommandEvent& event ){ event.Skip(); }
		
	
	public:
		MainForm( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("WebInventory"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 1070,660 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );
		~MainForm();
		void m_splitTasksOnIdle( wxIdleEvent& )
		{
		m_splitTasks->SetSashPosition( 0 );
		m_splitTasks->Disconnect( wxEVT_IDLE, wxIdleEventHandler( MainForm::m_splitTasksOnIdle ), NULL, this );
		}
		
		void m_splitOnIdle( wxIdleEvent& )
		{
		m_split->SetSashPosition( 400 );
		m_split->Disconnect( wxEVT_IDLE, wxIdleEventHandler( MainForm::m_splitOnIdle ), NULL, this );
		}
		
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class ObjDialog
///////////////////////////////////////////////////////////////////////////////
class ObjDialog : public wxDialog 
{
	private:
	
	protected:
		wxStdDialogButtonSizer* m_sdbSizer2;
		wxButton* m_sdbSizer2OK;
		wxButton* m_sdbSizer2Cancel;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnOK( wxCommandEvent& event ){ event.Skip(); }
		
	
	public:
		wxTextCtrl* m_txtObjName;
		wxTextCtrl* m_txtBaseURL;
		ObjDialog( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Object"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 279,130 ), long style = wxDEFAULT_DIALOG_STYLE );
		~ObjDialog();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class ServDialog
///////////////////////////////////////////////////////////////////////////////
class ServDialog : public wxDialog 
{
	private:
	
	protected:
		wxStaticText* m_stName;
		wxStaticText* m_stServName;
		wxStaticText* m_stPort;
		wxStdDialogButtonSizer* m_sdbSizer1;
		wxButton* m_sdbSizer1OK;
		wxButton* m_sdbSizer1Cancel;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnOK( wxCommandEvent& event ){ event.Skip(); }
		
	
	public:
		wxTextCtrl* m_txtName;
		wxTextCtrl* m_txtHostname;
		wxTextCtrl* m_txtSrvPort;
		ServDialog( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Connection"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 296,156 ), long style = wxDEFAULT_DIALOG_STYLE );
		~ServDialog();
	
};

#endif //__wiGuiData__
