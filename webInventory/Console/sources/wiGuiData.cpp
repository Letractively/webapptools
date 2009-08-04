///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Apr 16 2008)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "wiStatBar.h"

#include "wiGuiData.h"

#include "../images/btnAdd.xpm"
#include "../images/btnApply16.xpm"
#include "../images/btnDel.xpm"
#include "../images/btnEdit.xpm"
#include "../images/btnStop.xpm"
#include "../images/date.xpm"
#include "../images/filter.xpm"
#include "../images/flsave.xpm"
#include "../images/flstatus.xpm"
#include "../images/panReports.xpm"
#include "../images/panSettings.xpm"
#include "../images/panTasks.xpm"
#include "../images/reload.xpm"
#include "../images/start.xpm"

///////////////////////////////////////////////////////////////////////////

MainForm::MainForm( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxSize( 800,600 ), wxDefaultSize );

	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxVERTICAL );

	m_mainnb = new wxAuiNotebook( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxAUI_NB_SCROLL_BUTTONS|wxAUI_NB_TAB_FIXED_WIDTH );
	m_pTasks = new wxPanel( m_mainnb, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxFlexGridSizer* fgSizer1;
	fgSizer1 = new wxFlexGridSizer( 1, 2, 0, 0 );
	fgSizer1->AddGrowableCol( 1 );
	fgSizer1->AddGrowableRow( 0 );
	fgSizer1->SetFlexibleDirection( wxBOTH );
	fgSizer1->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	wxBoxSizer* bSizer121;
	bSizer121 = new wxBoxSizer( wxVERTICAL );

	wxBoxSizer* bSizer4;
	bSizer4 = new wxBoxSizer( wxHORIZONTAL );

	m_stTaskList = new wxStaticText( m_pTasks, wxID_ANY, _("Task List"), wxDefaultPosition, wxDefaultSize, 0 );
	m_stTaskList->Wrap( -1 );
	m_stTaskList->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 92, false, wxEmptyString ) );

	bSizer4->Add( m_stTaskList, 0, wxALIGN_BOTTOM|wxALL, 5 );


	bSizer4->Add( 0, 0, 10, wxEXPAND, 5 );

	m_toolBarTask = new wxToolBar( m_pTasks, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTB_FLAT|wxTB_HORIZONTAL|wxTB_NODIVIDER );
	m_toolBarTask->SetToolBitmapSize( wxSize( 20,20 ) );
	m_toolBarTask->AddTool( wxID_TOOLGO, wxEmptyString, wxBitmap( start_xpm ), wxNullBitmap, wxITEM_NORMAL, _("Start task"), wxEmptyString );
	m_toolBarTask->AddTool( wxID_TOOLSTOP, wxEmptyString, wxBitmap( btnStop_xpm ), wxNullBitmap, wxITEM_NORMAL, _("Cancel task"), wxEmptyString );
	m_toolBarTask->AddSeparator();
	m_toolBarTask->AddTool( wxID_TOOLNEW, wxEmptyString, wxBitmap( btnAdd_xpm ), wxNullBitmap, wxITEM_NORMAL, _("Add new task"), wxEmptyString );
	m_toolBarTask->AddTool( wxID_TOOLDEL, wxEmptyString, wxBitmap( btnDel_xpm ), wxNullBitmap, wxITEM_NORMAL, _("Delete task"), wxEmptyString );
	m_toolBarTask->Realize();

	bSizer4->Add( m_toolBarTask, 0, wxEXPAND, 0 );

	bSizer121->Add( bSizer4, 0, wxEXPAND, 5 );

	m_lstTaskList = new wxListCtrl( m_pTasks, wxID_ANY, wxDefaultPosition, wxSize( 300,-1 ), wxLC_HRULES|wxLC_REPORT|wxLC_SINGLE_SEL );
	bSizer121->Add( m_lstTaskList, 1, wxBOTTOM|wxLEFT, 5 );

	fgSizer1->Add( bSizer121, 1, wxEXPAND, 5 );

	wxFlexGridSizer* fgSizer6;
	fgSizer6 = new wxFlexGridSizer( 5, 1, 0, 0 );
	fgSizer6->AddGrowableCol( 0 );
	fgSizer6->AddGrowableRow( 4 );
	fgSizer6->SetFlexibleDirection( wxBOTH );
	fgSizer6->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	wxBoxSizer* bSizer111;
	bSizer111 = new wxBoxSizer( wxHORIZONTAL );

	m_stProfile = new wxStaticText( m_pTasks, wxID_ANY, _("Task settings"), wxDefaultPosition, wxDefaultSize, 0 );
	m_stProfile->Wrap( -1 );
	m_stProfile->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 92, false, wxEmptyString ) );

	bSizer111->Add( m_stProfile, 0, wxALIGN_BOTTOM|wxALL, 5 );


	bSizer111->Add( 0, 0, 1, wxEXPAND, 5 );

	m_btnApply = new wxButton( m_pTasks, wxID_ANY, _("Apply task settings"), wxDefaultPosition, wxDefaultSize, wxNO_BORDER );
	bSizer111->Add( m_btnApply, 0, wxALL, 5 );

	fgSizer6->Add( bSizer111, 0, wxEXPAND, 5 );

	m_panTaskOpts = new wxScrolledWindow( m_pTasks, wxID_ANY, wxDefaultPosition, wxSize( -1,200 ), wxHSCROLL|wxVSCROLL );
	m_panTaskOpts->SetScrollRate( 5, 5 );
	m_panTaskOpts->SetMinSize( wxSize( -1,200 ) );
	m_panTaskOpts->SetMaxSize( wxSize( -1,300 ) );

	wxBoxSizer* bSizer11;
	bSizer11 = new wxBoxSizer( wxVERTICAL );

	wxGridBagSizer* gbSizer2;
	gbSizer2 = new wxGridBagSizer( 0, 0 );
	gbSizer2->AddGrowableCol( 4 );
	gbSizer2->AddGrowableRow( 10 );
	gbSizer2->SetFlexibleDirection( wxBOTH );
	gbSizer2->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_stTaskName = new wxStaticText( m_panTaskOpts, wxID_ANY, _("Task name"), wxDefaultPosition, wxDefaultSize, 0 );
	m_stTaskName->Wrap( -1 );
	gbSizer2->Add( m_stTaskName, wxGBPosition( 0, 0 ), wxGBSpan( 1, 1 ), wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_txtTaskName = new wxTextCtrl( m_panTaskOpts, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_txtTaskName->SetMinSize( wxSize( 200,-1 ) );

	gbSizer2->Add( m_txtTaskName, wxGBPosition( 0, 1 ), wxGBSpan( 1, 2 ), wxALIGN_CENTER_VERTICAL|wxALL|wxEXPAND, 5 );

	m_stBaseURL = new wxStaticText( m_panTaskOpts, wxID_ANY, _("Base URL"), wxDefaultPosition, wxDefaultSize, 0 );
	m_stBaseURL->Wrap( -1 );
	gbSizer2->Add( m_stBaseURL, wxGBPosition( 1, 0 ), wxGBSpan( 1, 1 ), wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_txtBaseURL = new wxTextCtrl( m_panTaskOpts, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_txtBaseURL->SetMinSize( wxSize( 200,-1 ) );

	gbSizer2->Add( m_txtBaseURL, wxGBPosition( 1, 1 ), wxGBSpan( 1, 2 ), wxALL|wxEXPAND, 5 );

	m_stLogLevel = new wxStaticText( m_panTaskOpts, wxID_ANY, _("Log level"), wxDefaultPosition, wxDefaultSize, 0 );
	m_stLogLevel->Wrap( -1 );
	gbSizer2->Add( m_stLogLevel, wxGBPosition( 2, 0 ), wxGBSpan( 1, 1 ), wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	wxString m_chLogLevelChoices[] = { _("Trace"), _("Debug"), _("Info"), _("Warnings"), _("Errors"), _("Fatal") };
	int m_chLogLevelNChoices = sizeof( m_chLogLevelChoices ) / sizeof( wxString );
	m_chLogLevel = new wxChoice( m_panTaskOpts, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_chLogLevelNChoices, m_chLogLevelChoices, 0 );
	m_chLogLevel->SetSelection( 0 );
	m_chLogLevel->SetMinSize( wxSize( 150,-1 ) );

	gbSizer2->Add( m_chLogLevel, wxGBPosition( 2, 1 ), wxGBSpan( 1, 2 ), wxALL|wxEXPAND, 5 );

	wxString m_rbDepthChoices[] = { _("Stay in dir"), _("Stay in host"), _("Stay in domain") };
	int m_rbDepthNChoices = sizeof( m_rbDepthChoices ) / sizeof( wxString );
	m_rbDepth = new wxRadioBox( m_panTaskOpts, wxID_ANY, _("Depth mode"), wxDefaultPosition, wxDefaultSize, m_rbDepthNChoices, m_rbDepthChoices, 1, wxRA_SPECIFY_COLS );
	m_rbDepth->SetSelection( 0 );
	gbSizer2->Add( m_rbDepth, wxGBPosition( 3, 0 ), wxGBSpan( 3, 2 ), wxALL|wxEXPAND, 5 );

	m_stDepth = new wxStaticText( m_panTaskOpts, wxID_ANY, _("Scan depth"), wxDefaultPosition, wxDefaultSize, 0 );
	m_stDepth->Wrap( -1 );
	gbSizer2->Add( m_stDepth, wxGBPosition( 3, 2 ), wxGBSpan( 1, 1 ), wxALIGN_BOTTOM|wxBOTTOM|wxLEFT|wxRIGHT, 5 );

	m_txtDepth = new wxTextCtrl( m_panTaskOpts, wxID_ANY, _("-1"), wxDefaultPosition, wxDefaultSize, 0 );
	gbSizer2->Add( m_txtDepth, wxGBPosition( 4, 2 ), wxGBSpan( 1, 1 ), wxALIGN_TOP|wxBOTTOM|wxLEFT|wxRIGHT, 5 );

	m_cbInvent = new wxCheckBox( m_panTaskOpts, wxID_ANY, _("Inventory only"), wxDefaultPosition, wxDefaultSize, 0 );
	m_cbInvent->SetValue(true);

	m_cbInvent->Enable( false );

	gbSizer2->Add( m_cbInvent, wxGBPosition( 5, 2 ), wxGBSpan( 1, 1 ), wxALL, 5 );

	wxStaticBoxSizer* sbSizer1;
	sbSizer1 = new wxStaticBoxSizer( new wxStaticBox( m_panTaskOpts, wxID_ANY, _("Inventory options") ), wxVERTICAL );

	m_chIgnoreParams = new wxCheckBox( m_panTaskOpts, wxID_ANY, _("Ignore URL parameters"), wxDefaultPosition, wxDefaultSize, 0 );

	sbSizer1->Add( m_chIgnoreParams, 0, wxALL, 5 );

	gbSizer2->Add( sbSizer1, wxGBPosition( 6, 0 ), wxGBSpan( 3, 2 ), wxEXPAND, 5 );

	m_stThreads = new wxStaticText( m_panTaskOpts, wxID_ANY, _("Number of threads"), wxDefaultPosition, wxDefaultSize, 0 );
	m_stThreads->Wrap( -1 );
	m_stThreads->SetToolTip( _("Number of thread for transport operations") );

	gbSizer2->Add( m_stThreads, wxGBPosition( 6, 2 ), wxGBSpan( 1, 1 ), wxALL, 5 );

	m_spinCtrl1 = new wxSpinCtrl( m_panTaskOpts, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 50, 10 );
	m_spinCtrl1->SetToolTip( _("Number of thread for transport operations") );

	gbSizer2->Add( m_spinCtrl1, wxGBPosition( 7, 2 ), wxGBSpan( 1, 1 ), wxALL, 5 );


	gbSizer2->Add( 0, 0, wxGBPosition( 3, 4 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );

	bSizer11->Add( gbSizer2, 0, wxEXPAND, 0 );

	m_panTaskOpts->SetSizer( bSizer11 );
	m_panTaskOpts->Layout();
	fgSizer6->Add( m_panTaskOpts, 0, wxEXPAND | wxALL, 5 );

	m_staticline1 = new wxStaticLine( m_pTasks, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	fgSizer6->Add( m_staticline1, 0, wxEXPAND | wxALL, 5 );

	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxHORIZONTAL );

	m_stTaskLog = new wxStaticText( m_pTasks, wxID_ANY, _("Task execution log"), wxDefaultPosition, wxDefaultSize, 0 );
	m_stTaskLog->Wrap( -1 );
	m_stTaskLog->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 92, false, wxEmptyString ) );

	bSizer2->Add( m_stTaskLog, 0, wxALIGN_BOTTOM|wxALL, 5 );


	bSizer2->Add( 0, 0, 1, wxEXPAND, 5 );

	fgSizer6->Add( bSizer2, 0, wxEXPAND, 5 );

	m_rtTask = new wxRichTextCtrl( m_pTasks, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( -1,-1 ), wxTE_AUTO_URL|wxTE_READONLY|wxHSCROLL|wxVSCROLL|wxWANTS_CHARS );
	fgSizer6->Add( m_rtTask, 1, wxBOTTOM|wxEXPAND|wxLEFT|wxRIGHT, 5 );

	fgSizer1->Add( fgSizer6, 1, wxEXPAND, 5 );

	m_pTasks->SetSizer( fgSizer1 );
	m_pTasks->Layout();
	fgSizer1->Fit( m_pTasks );
	m_mainnb->AddPage( m_pTasks, _("Tasks"), false, wxBitmap( panTasks_xpm ) );
	m_pReports = new wxPanel( m_mainnb, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer13;
	bSizer13 = new wxBoxSizer( wxVERTICAL );

	m_split = new wxSplitterWindow( m_pReports, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_3D );
	m_split->SetSashGravity( 0.66 );
	m_split->Connect( wxEVT_IDLE, wxIdleEventHandler( MainForm::m_splitOnIdle ), NULL, this );
	m_panRepTree = new wxPanel( m_split, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer14;
	bSizer14 = new wxBoxSizer( wxVERTICAL );

	m_toolBarFilter = new wxToolBar( m_panRepTree, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTB_FLAT|wxTB_HORIZONTAL );
	m_toolBarFilter->SetToolBitmapSize( wxSize( 22,22 ) );
	m_toolBarFilter->AddTool( wxID_TLREFRESH, _("tool"), wxBitmap( reload_xpm ), wxNullBitmap, wxITEM_NORMAL, _("Refresh reports list"), wxEmptyString );
	m_toolBarFilter->AddTool( wxID_TLFILTER, _("tool"), wxBitmap( filter_xpm ), wxNullBitmap, wxITEM_CHECK, _("Apply filter"), wxEmptyString );
	m_stToolTask = new wxStaticText( m_toolBarFilter, wxID_ANY, _("Task: "), wxDefaultPosition, wxDefaultSize, 0 );
	m_stToolTask->Wrap( -1 );
	m_toolBarFilter->AddControl( m_stToolTask );
	wxArrayString m_chTaskFilterChoices;
	m_chTaskFilter = new wxChoice( m_toolBarFilter, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_chTaskFilterChoices, 0 );
	m_chTaskFilter->SetSelection( 0 );
	m_toolBarFilter->AddControl( m_chTaskFilter );
	m_toolBarFilter->AddTool( wxID_ANY, _("tool"), wxBitmap( date_xpm ), wxNullBitmap, wxITEM_NORMAL, _("Date filter mode"), wxEmptyString );
	m_dateFilter = new wxDatePickerCtrl( m_toolBarFilter, wxID_ANY, wxDefaultDateTime, wxDefaultPosition, wxDefaultSize, wxDP_DEFAULT|wxDP_DROPDOWN|wxDP_SHOWCENTURY );
	m_toolBarFilter->AddControl( m_dateFilter );
	m_toolBarFilter->AddTool( wxID_TLSTATUS, _("tool"), wxBitmap( flstatus_xpm ), wxNullBitmap, wxITEM_NORMAL, _("Filter by scan status"), wxEmptyString );
	m_toolBarFilter->AddTool( wxID_TLSAVE, _("tool"), wxBitmap( flsave_xpm ), wxNullBitmap, wxITEM_NORMAL, _("Save and load filters"), wxEmptyString );
	m_toolBarFilter->Realize();

	bSizer14->Add( m_toolBarFilter, 0, wxEXPAND, 5 );

	m_treeScans = new wxTreeCtrl( m_panRepTree, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTR_FULL_ROW_HIGHLIGHT|wxTR_HAS_BUTTONS|wxTR_HIDE_ROOT|wxTR_LINES_AT_ROOT|wxTR_SINGLE );
	bSizer14->Add( m_treeScans, 1, wxALL|wxEXPAND, 0 );

	m_panRepTree->SetSizer( bSizer14 );
	m_panRepTree->Layout();
	bSizer14->Fit( m_panRepTree );
	m_panRepData = new wxPanel( m_split, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer15;
	bSizer15 = new wxBoxSizer( wxVERTICAL );

	m_richText2 = new wxRichTextCtrl( m_panRepData, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0|wxVSCROLL|wxHSCROLL|wxNO_BORDER|wxWANTS_CHARS );
	bSizer15->Add( m_richText2, 1, wxEXPAND | wxALL, 0 );

	m_panRepData->SetSizer( bSizer15 );
	m_panRepData->Layout();
	bSizer15->Fit( m_panRepData );
	m_split->SplitVertically( m_panRepTree, m_panRepData, 400 );
	bSizer13->Add( m_split, 1, wxEXPAND, 5 );

	m_pReports->SetSizer( bSizer13 );
	m_pReports->Layout();
	bSizer13->Fit( m_pReports );
	m_mainnb->AddPage( m_pReports, _("Reports"), true, wxBitmap( panReports_xpm ) );
	m_pSettings = new wxPanel( m_mainnb, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer10;
	bSizer10 = new wxBoxSizer( wxVERTICAL );

	wxFlexGridSizer* fgSizer2;
	fgSizer2 = new wxFlexGridSizer( 2, 2, 0, 0 );
	fgSizer2->SetFlexibleDirection( wxBOTH );
	fgSizer2->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_stConVers = new wxStaticText( m_pSettings, wxID_ANY, _("Console version:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_stConVers->Wrap( -1 );
	fgSizer2->Add( m_stConVers, 0, wxALL, 5 );

	m_stConVersData = new wxStaticText( m_pSettings, wxID_ANY, _("unknown"), wxDefaultPosition, wxDefaultSize, 0 );
	m_stConVersData->Wrap( -1 );
	fgSizer2->Add( m_stConVersData, 0, wxALL, 5 );

	m_stServer = new wxStaticText( m_pSettings, wxID_ANY, _("Scanner address:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_stServer->Wrap( -1 );
	fgSizer2->Add( m_stServer, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	wxBoxSizer* bSizer6;
	bSizer6 = new wxBoxSizer( wxHORIZONTAL );

	wxString m_chServersChoices[] = { _("hostname[:port]") };
	int m_chServersNChoices = sizeof( m_chServersChoices ) / sizeof( wxString );
	m_chServers = new wxChoice( m_pSettings, wxID_ANY, wxDefaultPosition, wxSize( 250,-1 ), m_chServersNChoices, m_chServersChoices, 0 );
	m_chServers->SetSelection( 0 );
	bSizer6->Add( m_chServers, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_toolBarSrv = new wxToolBar( m_pSettings, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTB_FLAT|wxTB_HORIZONTAL|wxTB_NODIVIDER );
	m_toolBarSrv->SetToolBitmapSize( wxSize( 20,20 ) );
	m_toolBarSrv->AddTool( wxID_TLCONNECT, wxEmptyString, wxBitmap( start_xpm ), wxNullBitmap, wxITEM_NORMAL, _("Connect"), wxEmptyString );
	m_toolBarSrv->AddTool( wxID_TLNEW, wxEmptyString, wxBitmap( btnAdd_xpm ), wxNullBitmap, wxITEM_NORMAL, _("Add new connection"), wxEmptyString );
	m_toolBarSrv->AddTool( wxID_TLEDIT, _("tool"), wxBitmap( btnEdit_xpm ), wxNullBitmap, wxITEM_NORMAL, _("Edit connection"), wxEmptyString );
	m_toolBarSrv->AddTool( wxID_TLDELETE, _("tool"), wxBitmap( btnDel_xpm ), wxNullBitmap, wxITEM_NORMAL, _("Delete connection"), wxEmptyString );
	m_toolBarSrv->Realize();

	bSizer6->Add( m_toolBarSrv, 0, wxALIGN_CENTER_VERTICAL, 5 );

	fgSizer2->Add( bSizer6, 1, wxEXPAND, 5 );

	m_stSrvVers = new wxStaticText( m_pSettings, wxID_ANY, _("Scanner version:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_stSrvVers->Wrap( -1 );
	fgSizer2->Add( m_stSrvVers, 0, wxALL, 5 );

	m_stSrvVersData = new wxStaticText( m_pSettings, wxID_ANY, _("unknown"), wxDefaultPosition, wxDefaultSize, 0 );
	m_stSrvVersData->Wrap( -1 );
	fgSizer2->Add( m_stSrvVersData, 0, wxALL, 5 );

	m_stLang = new wxStaticText( m_pSettings, wxID_ANY, _("Interface Language:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_stLang->Wrap( -1 );
	fgSizer2->Add( m_stLang, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	wxBoxSizer* bSizer7;
	bSizer7 = new wxBoxSizer( wxHORIZONTAL );

	wxString m_chLangsChoices[] = { _("English (default)") };
	int m_chLangsNChoices = sizeof( m_chLangsChoices ) / sizeof( wxString );
	m_chLangs = new wxChoice( m_pSettings, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_chLangsNChoices, m_chLangsChoices, 0 );
	m_chLangs->SetSelection( 0 );
	m_chLangs->SetMinSize( wxSize( 250,-1 ) );

	bSizer7->Add( m_chLangs, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_toolBarLang = new wxToolBar( m_pSettings, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTB_FLAT|wxTB_HORIZONTAL|wxTB_NODIVIDER );
	m_toolBarLang->SetToolBitmapSize( wxSize( 20,20 ) );
	m_toolBarLang->AddTool( wxID_TLLANGAPPLY, _("tool"), wxBitmap( btnApply16_xpm ), wxNullBitmap, wxITEM_NORMAL, _("Apply (need restart)"), wxEmptyString );
	m_toolBarLang->Realize();

	bSizer7->Add( m_toolBarLang, 0, wxALIGN_CENTER_VERTICAL, 5 );

	m_stLangRestart = new wxStaticText( m_pSettings, wxID_ANY, _("Need to restart to apply changes"), wxDefaultPosition, wxDefaultSize, 0 );
	m_stLangRestart->Wrap( -1 );
	m_stLangRestart->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 92, false, wxEmptyString ) );
	m_stLangRestart->SetForegroundColour( wxColour( 255, 70, 70 ) );
	m_stLangRestart->Hide();

	bSizer7->Add( m_stLangRestart, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	fgSizer2->Add( bSizer7, 1, wxEXPAND, 5 );


	fgSizer2->Add( 120, 0, 1, wxEXPAND, 5 );

	bSizer10->Add( fgSizer2, 0, wxEXPAND, 5 );

	m_staticline2 = new wxStaticLine( m_pSettings, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	bSizer10->Add( m_staticline2, 0, wxEXPAND | wxALL, 5 );

	m_pnServer = new wxPanel( m_pSettings, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxFlexGridSizer* fgSizer61;
	fgSizer61 = new wxFlexGridSizer( 2, 2, 0, 0 );
	fgSizer61->SetFlexibleDirection( wxBOTH );
	fgSizer61->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_staticText18 = new wxStaticText( m_pnServer, wxID_ANY, _("Scanner storage:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText18->Wrap( -1 );
	fgSizer61->Add( m_staticText18, 0, wxALL, 5 );

	wxBoxSizer* bSizer12;
	bSizer12 = new wxBoxSizer( wxHORIZONTAL );

	wxArrayString m_chStorageChoices;
	m_chStorage = new wxChoice( m_pnServer, wxID_ANY, wxDefaultPosition, wxSize( -1,-1 ), m_chStorageChoices, 0 );
	m_chStorage->SetSelection( 0 );
	m_chStorage->SetMinSize( wxSize( 250,-1 ) );

	bSizer12->Add( m_chStorage, 0, wxALL, 5 );

	m_bpStorageApply = new wxBitmapButton( m_pnServer, wxID_ANY, wxBitmap( btnApply16_xpm ), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
	m_bpStorageApply->SetToolTip( _("Apply") );

	m_bpStorageApply->SetToolTip( _("Apply") );

	bSizer12->Add( m_bpStorageApply, 0, wxBOTTOM|wxTOP, 5 );


	bSizer12->Add( 0, 0, 1, wxEXPAND, 5 );

	m_bpPlgRefresh = new wxBitmapButton( m_pnServer, wxID_ANY, wxBitmap( reload_xpm ), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
	m_bpPlgRefresh->SetToolTip( _("Refresh") );

	m_bpPlgRefresh->SetToolTip( _("Refresh") );

	bSizer12->Add( m_bpPlgRefresh, 0, wxBOTTOM|wxTOP, 5 );

	fgSizer61->Add( bSizer12, 1, wxEXPAND, 5 );

	m_staticText19 = new wxStaticText( m_pnServer, wxID_ANY, _("Scanner plugins:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText19->Wrap( -1 );
	fgSizer61->Add( m_staticText19, 0, wxALL, 5 );

	m_pluginsDock = new wxScrolledWindow( m_pnServer, wxID_ANY, wxDefaultPosition, wxSize( 520,300 ), wxHSCROLL|wxVSCROLL );
	m_pluginsDock->SetScrollRate( 5, 5 );
	m_pluginsDock->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_INFOBK ) );
	m_pluginsDock->SetMinSize( wxSize( 520,300 ) );

	wxBoxSizer* bSizer9;
	bSizer9 = new wxBoxSizer( wxVERTICAL );

	bSizer9->SetMinSize( wxSize( 500,300 ) );
	m_gbPluginsGrid = new wxGridBagSizer( 0, 0 );
	m_gbPluginsGrid->AddGrowableCol( 1 );
	m_gbPluginsGrid->SetFlexibleDirection( wxBOTH );
	m_gbPluginsGrid->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_gbPluginsGrid->SetMinSize( wxSize( 500,-1 ) );
	m_btPluginIcon = new wxButton( m_pluginsDock, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 24,-1 ), wxNO_BORDER );
	m_gbPluginsGrid->Add( m_btPluginIcon, wxGBPosition( 0, 0 ), wxGBSpan( 1, 1 ), wxALL|wxEXPAND, 0 );

	m_btPluginDesc = new wxButton( m_pluginsDock, wxID_ANY, _("Description"), wxDefaultPosition, wxDefaultSize, wxNO_BORDER );
	m_gbPluginsGrid->Add( m_btPluginDesc, wxGBPosition( 0, 1 ), wxGBSpan( 1, 1 ), wxALL|wxEXPAND, 0 );

	m_btPluginIface = new wxButton( m_pluginsDock, wxID_ANY, _("Interface"), wxDefaultPosition, wxSize( 120,-1 ), wxNO_BORDER );
	m_gbPluginsGrid->Add( m_btPluginIface, wxGBPosition( 0, 2 ), wxGBSpan( 1, 1 ), wxALL|wxEXPAND, 0 );

	m_btPluginSetup = new wxButton( m_pluginsDock, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxNO_BORDER );
	m_gbPluginsGrid->Add( m_btPluginSetup, wxGBPosition( 0, 3 ), wxGBSpan( 1, 1 ), wxALL|wxEXPAND, 0 );

	bSizer9->Add( m_gbPluginsGrid, 1, wxEXPAND, 5 );

	m_pluginsDock->SetSizer( bSizer9 );
	m_pluginsDock->Layout();
	fgSizer61->Add( m_pluginsDock, 1, wxEXPAND|wxLEFT, 5 );


	fgSizer61->Add( 120, 0, 1, wxEXPAND, 5 );

	m_pnServer->SetSizer( fgSizer61 );
	m_pnServer->Layout();
	fgSizer61->Fit( m_pnServer );
	bSizer10->Add( m_pnServer, 1, wxEXPAND | wxALL, 0 );

	m_pSettings->SetSizer( bSizer10 );
	m_pSettings->Layout();
	bSizer10->Fit( m_pSettings );
	m_mainnb->AddPage( m_pSettings, _("Settings"), false, wxBitmap( panSettings_xpm ) );

	bSizer1->Add( m_mainnb, 1, wxEXPAND | wxALL, 0 );

	this->SetSizer( bSizer1 );
	this->Layout();
    m_statusBar = new wiStatBar( this );
    SetStatusBar(m_statusBar);

	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( MainForm::OnClose ) );
	this->Connect( wxID_TOOLGO, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( MainForm::OnRunTask ) );
	this->Connect( wxID_TOOLSTOP, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( MainForm::OnCancelTask ) );
	this->Connect( wxID_TOOLNEW, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( MainForm::OnAddTask ) );
	this->Connect( wxID_TOOLDEL, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( MainForm::OnDelTask ) );
	m_lstTaskList->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( MainForm::OnTaskKillFocus ), NULL, this );
	m_lstTaskList->Connect( wxEVT_COMMAND_LIST_COL_CLICK, wxListEventHandler( MainForm::OnSortItems ), NULL, this );
	m_lstTaskList->Connect( wxEVT_COMMAND_LIST_ITEM_SELECTED, wxListEventHandler( MainForm::OnTaskSelected ), NULL, this );
	m_btnApply->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainForm::OnTaskApply ), NULL, this );
	this->Connect( wxID_TLREFRESH, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( MainForm::OnReportsRefresh ) );
	this->Connect( wxID_TLFILTER, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( MainForm::OnReportsFilter ) );
	m_chTaskFilter->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( MainForm::OnReportTskFilter ), NULL, this );
	this->Connect( wxID_ANY, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( MainForm::OnReportsDate ) );
	m_dateFilter->Connect( wxEVT_DATE_CHANGED, wxDateEventHandler( MainForm::OnReportDateFilter ), NULL, this );
	this->Connect( wxID_TLSTATUS, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( MainForm::OnReportsStatus ) );
	this->Connect( wxID_TLSAVE, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( MainForm::OnReportsSave ) );
	this->Connect( wxID_TLCONNECT, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( MainForm::OnConnect ) );
	this->Connect( wxID_TLNEW, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( MainForm::OnAddServer ) );
	this->Connect( wxID_TLEDIT, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( MainForm::OnEditServer ) );
	this->Connect( wxID_TLDELETE, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( MainForm::OnDelServer ) );
	this->Connect( wxID_TLLANGAPPLY, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( MainForm::OnLangChange ) );
	m_bpStorageApply->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainForm::OnStorageChange ), NULL, this );
	m_bpPlgRefresh->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainForm::OnPlgRefresh ), NULL, this );
}

MainForm::~MainForm()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( MainForm::OnClose ) );
	this->Disconnect( wxID_TOOLGO, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( MainForm::OnRunTask ) );
	this->Disconnect( wxID_TOOLSTOP, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( MainForm::OnCancelTask ) );
	this->Disconnect( wxID_TOOLNEW, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( MainForm::OnAddTask ) );
	this->Disconnect( wxID_TOOLDEL, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( MainForm::OnDelTask ) );
	m_lstTaskList->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( MainForm::OnTaskKillFocus ), NULL, this );
	m_lstTaskList->Disconnect( wxEVT_COMMAND_LIST_COL_CLICK, wxListEventHandler( MainForm::OnSortItems ), NULL, this );
	m_lstTaskList->Disconnect( wxEVT_COMMAND_LIST_ITEM_SELECTED, wxListEventHandler( MainForm::OnTaskSelected ), NULL, this );
	m_btnApply->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainForm::OnTaskApply ), NULL, this );
	this->Disconnect( wxID_TLREFRESH, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( MainForm::OnReportsRefresh ) );
	this->Disconnect( wxID_TLFILTER, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( MainForm::OnReportsFilter ) );
	m_chTaskFilter->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( MainForm::OnReportTskFilter ), NULL, this );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( MainForm::OnReportsDate ) );
	m_dateFilter->Disconnect( wxEVT_DATE_CHANGED, wxDateEventHandler( MainForm::OnReportDateFilter ), NULL, this );
	this->Disconnect( wxID_TLSTATUS, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( MainForm::OnReportsStatus ) );
	this->Disconnect( wxID_TLSAVE, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( MainForm::OnReportsSave ) );
	this->Disconnect( wxID_TLCONNECT, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( MainForm::OnConnect ) );
	this->Disconnect( wxID_TLNEW, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( MainForm::OnAddServer ) );
	this->Disconnect( wxID_TLEDIT, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( MainForm::OnEditServer ) );
	this->Disconnect( wxID_TLDELETE, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( MainForm::OnDelServer ) );
	this->Disconnect( wxID_TLLANGAPPLY, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( MainForm::OnLangChange ) );
	m_bpStorageApply->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainForm::OnStorageChange ), NULL, this );
	m_bpPlgRefresh->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainForm::OnPlgRefresh ), NULL, this );
}

ServDialog::ServDialog( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxBoxSizer* bSizer8;
	bSizer8 = new wxBoxSizer( wxVERTICAL );

	wxFlexGridSizer* fgSizer3;
	fgSizer3 = new wxFlexGridSizer( 2, 2, 0, 0 );
	fgSizer3->SetFlexibleDirection( wxBOTH );
	fgSizer3->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_stName = new wxStaticText( this, wxID_ANY, _("Scanner name:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_stName->Wrap( -1 );
	fgSizer3->Add( m_stName, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_txtName = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_txtName->SetMinSize( wxSize( 200,-1 ) );

	fgSizer3->Add( m_txtName, 0, wxALL, 5 );

	m_stServName = new wxStaticText( this, wxID_ANY, _("Scanner host:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_stServName->Wrap( -1 );
	fgSizer3->Add( m_stServName, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_txtHostname = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_txtHostname->SetMinSize( wxSize( 200,-1 ) );

	fgSizer3->Add( m_txtHostname, 0, wxALL, 5 );

	m_stPort = new wxStaticText( this, wxID_ANY, _("Scanner port:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_stPort->Wrap( -1 );
	fgSizer3->Add( m_stPort, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_txtSrvPort = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_txtSrvPort->SetMinSize( wxSize( 200,-1 ) );

	fgSizer3->Add( m_txtSrvPort, 0, wxALL, 5 );

	bSizer8->Add( fgSizer3, 1, wxEXPAND, 5 );

	m_sdbSizer1 = new wxStdDialogButtonSizer();
	m_sdbSizer1OK = new wxButton( this, wxID_OK );
	m_sdbSizer1->AddButton( m_sdbSizer1OK );
	m_sdbSizer1Cancel = new wxButton( this, wxID_CANCEL );
	m_sdbSizer1->AddButton( m_sdbSizer1Cancel );
	m_sdbSizer1->Realize();
	bSizer8->Add( m_sdbSizer1, 0, wxALL|wxEXPAND, 5 );

	this->SetSizer( bSizer8 );
	this->Layout();

	this->Centre( wxBOTH );

	// Connect Events
	m_sdbSizer1OK->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ServDialog::OnOK ), NULL, this );
}

ServDialog::~ServDialog()
{
	// Disconnect Events
	m_sdbSizer1OK->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ServDialog::OnOK ), NULL, this );
}
