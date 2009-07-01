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
#include <wx/bmpbuttn.h>
#include <wx/button.h>
#include <wx/sizer.h>
#include <wx/choice.h>
#include <wx/listctrl.h>
#include <wx/richtext/richtextctrl.h>
#include <wx/textctrl.h>
#include <wx/gbsizer.h>
#include <wx/panel.h>
#include <wx/aui/auibook.h>
#include <wx/statusbr.h>
#include <wx/frame.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class MainForm
///////////////////////////////////////////////////////////////////////////////
class MainForm : public wxFrame
{
	private:

	protected:
		wxAuiNotebook* m_mainnb;
		wxPanel* m_pTasks;
		wxStaticText* m_stActiveTasks;

		wxBitmapButton* m_bpStartTask;
		wxBitmapButton* m_bpPauseTask;
		wxBitmapButton* m_bpCancelTask;
		wxStaticText* m_stTaskLog;

		wxStaticText* m_stLogLevel;
		wxChoice* m_chLogLevel;
		wxListCtrl* m_lstActiveTask;
		wxRichTextCtrl* m_rtTask;
		wxStaticText* m_stTaskList;

		wxBitmapButton* m_bpTaskGo;
		wxBitmapButton* m_bpTaskNew;
		wxBitmapButton* m_bpTaskDel;
		wxStaticText* m_stProfile;
		wxListCtrl* m_lstTaskList;
		wxStaticText* m_stBaseUrl;
		wxTextCtrl* m_txtBaseURL;
		wxPanel* m_pReports;
		wxPanel* m_pSettings;
		wxStaticText* m_stConVers;
		wxStaticText* m_stConVersData;
		wxStaticText* m_stServer;
		wxChoice* m_chServers;
		wxBitmapButton* m_bpConnect;
		wxBitmapButton* m_bpServerNew;
		wxBitmapButton* m_bpSrvEdit;
		wxBitmapButton* m_bpServerDel;
		wxStaticText* m_stSrvVers;
		wxStaticText* m_stSrvVersData;
		wxStaticText* m_stLang;
		wxChoice* m_chLangs;
		wxBitmapButton* m_bpLangApply;
		wxStaticText* m_stLangRestart;
		wiStatBar* m_statusBar;

		// Virtual event handlers, overide them in your derived class
		virtual void OnClose( wxCloseEvent& event ){ event.Skip(); }
		virtual void OnConnect( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnAddServer( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnEditServer( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnDelServer( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnLangChange( wxCommandEvent& event ){ event.Skip(); }


	public:
		MainForm( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("WebInventory"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 1070,660 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );
		~MainForm();

};

///////////////////////////////////////////////////////////////////////////////
/// Class wiServDialog
///////////////////////////////////////////////////////////////////////////////
class wiServDialog : public wxDialog
{
	private:

	protected:
		wxStaticText* m_stServName;
		wxStaticText* m_stPort;
		wxStdDialogButtonSizer* m_sdbSizer1;
		wxButton* m_sdbSizer1OK;
		wxButton* m_sdbSizer1Cancel;

		// Virtual event handlers, overide them in your derived class
		virtual void OnOK( wxCommandEvent& event ){ EndModal(wxOK); }


	public:
		wxTextCtrl* m_txtHostname;
		wxTextCtrl* m_txtSrvPort;
		wiServDialog( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Connection"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 294,126 ), long style = wxDEFAULT_DIALOG_STYLE );
		~wiServDialog();

};

#endif //__wiGuiData__
