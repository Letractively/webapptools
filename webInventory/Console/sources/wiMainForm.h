/*
    webInventory is the web-application audit programm
    Copyright (C) 2009 Andrew Abramov stinger911@gmail.com

    This file is part of webInventory

    webInventory is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    webInventory is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with webInventory.  If not, see <http://www.gnu.org/licenses/>.
*/
/***************************************************************
 * @file      wiMainForm.h
 * @brief     Declaration of the MainForm class
 * @author    Andrew "Stinger" Abramov (stinger911@gmail.com)
 * @date      30.06.2009
 **************************************************************/
#ifndef __wiMainForm__
#define __wiMainForm__

#include "wiGuiData.h"
#include <wx/imaglist.h>
#include "Config.h"
#include "wiTcpClient.h"

/**
 * @class   wiMainForm
 * @brief   Implementing MainForm
 */
class wiMainForm : public MainForm
{
public:
	/** Constructor */
	wiMainForm( wxWindow* parent );

	void LoadConnections();

protected:
    // Virtual event handlers, overide them in your derived class
    virtual void OnTimer( wxTimerEvent& event );
    virtual void OnClose( wxCloseEvent& event );
    virtual void OnConnect( wxCommandEvent& event );
    virtual void OnAddServer( wxCommandEvent& event );
    virtual void OnEditServer( wxCommandEvent& event );
    virtual void OnDelServer( wxCommandEvent& event );
    virtual void OnLangChange( wxCommandEvent& event );
    virtual void OnAddTask( wxCommandEvent& event );
    virtual void OnDelTask( wxCommandEvent& event );
    virtual void OnRunTask( wxCommandEvent& event );
    virtual void OnCancelTask( wxCommandEvent& event );
    virtual void OnTaskSelected( wxListEvent& event );
    virtual void OnSortItems( wxListEvent& event );

    void ProcessTaskList(const wxString& criteria = wxT(""));
    void Disconnected(bool mode = true);
    void Connected(bool mode = true);
    void SelectTask(int id = -1);

    CConfigEngine m_cfgEngine;
    wiTcpClient* m_client;
    wxTimer m_timer;
    wxImageList m_lstImages;
    bool connStatus;
    int m_selectedTask;
    int m_selectedActive;
};

#endif // __wiMainForm__