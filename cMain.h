#pragma once

#include "wx/wx.h"
#include <wx/spinctrl.h>
#include <wx/listbox.h>
#include <wx/choice.h>
#include <wx/log.h>

#include "Qbit_sim.h"

#include <string> 
#include <vector>
#include <iostream>
#include <iomanip>
#include <math.h>

class cMain : public wxFrame
{
public:
	cMain();
	~cMain();

	wxDECLARE_EVENT_TABLE();

	void create_bttn_click( wxCommandEvent &evt );
	void reset_bttn_click(wxCommandEvent& evt);
	void run_sim_bttn_click(wxCommandEvent& evt);
private:
	Q_sim Q_simulator;

	const wxString avlb_gates[7] = { "I", "H", "CNOT", "X", "Y", "Z", "phase" };
	const wxString avlb_Qbit[2] = { "0", "1" };

	wxScrolledWindow* scrwin = nullptr;
	wxPanel *pnl_1 = new wxPanel(this, 0000, wxPoint(0,0), wxSize(300, 100));
	wxPanel* pnl_2 = new wxPanel(this, 0000, wxPoint(300, 300), wxSize(300, 300));
	
	wxButton* create_bttn = nullptr;
	wxButton* reset_bttn = nullptr;
	wxButton* run_sim_bttn = nullptr;

	wxSpinCtrl* spncntrl_cols = nullptr;
	wxSpinCtrl* spncntrl_rows = nullptr;
	wxListBox* final_output = nullptr;

	wxFlexGridSizer* grdszr_btn = nullptr;

	int width = 5;
	int height = 3;
	wxChoice** cmb = nullptr;//new wxChoice * [width * height];
	wxChoice** starting_bits = nullptr;//new wxChoice* [height];
	//wxSpinCtrl** btn = new wxSpinCtrl * [width * height];
	std::vector< wxSpinCtrlDouble* > angls;
	std::vector< wxSpinCtrlDouble* > fake_angls;
	std::vector< wxListBox* > topside_numbering;
	std::vector< wxListBox* > Output;

private:
	void Clear();
	std::string num_to_bin_str( int num, int digit_num );
};

std::string prune_str_end(std::string str, int num_of_del);