#include "cMain.h"

wxBEGIN_EVENT_TABLE(cMain, wxFrame)
	EVT_BUTTON(4000, create_bttn_click)
	EVT_BUTTON(4001, reset_bttn_click)
	EVT_BUTTON(4002, run_sim_bttn_click)
wxEND_EVENT_TABLE()


cMain::cMain() : wxFrame(nullptr, wxID_ANY, "Qbit Simulator", wxPoint(150,50), wxSize(800,600))
{
	std::cout << "Build...\n";
	pnl_1->SetBackgroundColour( wxColor(180,180,180) );
	pnl_2->SetBackgroundColour(wxColor(80, 80, 80));

	wxStaticBox* box_cols = new wxStaticBox(pnl_1, wxID_ANY, "Gate #", wxPoint(20, 20), wxSize(50,45));
	wxStaticBoxSizer* box_cols_sz = new wxStaticBoxSizer(box_cols, wxHORIZONTAL);
	spncntrl_cols = new wxSpinCtrl(box_cols, 9000, "1", wxPoint(5, 15), wxSize(45, 25), wxSP_ARROW_KEYS, 1, 100, 1);
	box_cols_sz->Add(spncntrl_cols);

	wxStaticBox* box_rows = new wxStaticBox(pnl_1, wxID_ANY, "Qbit #", wxPoint(80, 20), wxSize(50, 45));
	wxStaticBoxSizer* box_rows_sz = new wxStaticBoxSizer(box_rows, wxHORIZONTAL);
	spncntrl_rows = new wxSpinCtrl(box_rows, 9001, "1", wxPoint(5, 15), wxSize(45, 25), wxSP_ARROW_KEYS, 1, 100, 1);
	box_rows_sz->Add(spncntrl_rows);

	create_bttn = new wxButton(pnl_1, 4000, "Create", wxPoint(170, 35));
	reset_bttn = new wxButton(pnl_1, 4001, "Reset", wxPoint(250, 35)); reset_bttn->Enable(false);
	run_sim_bttn = new wxButton(pnl_1, 4002, "Run", wxPoint(330, 35)); run_sim_bttn->Enable(false);

	final_output = new wxListBox(pnl_1, wxID_ANY, wxPoint(170, 60), wxSize(900, 25));

	scrwin = new wxScrolledWindow(pnl_2, 10001, wxPoint(0, 0), wxSize(155, 155), wxALWAYS_SHOW_SB, "123");

	wxBoxSizer* grdszr_main = new wxBoxSizer(wxVERTICAL);
	grdszr_btn = new wxFlexGridSizer(1, 1, 0 ,0);
	
	wxBoxSizer* grdszr_pnl_2 = new wxBoxSizer(wxHORIZONTAL);
	grdszr_pnl_2->Add(scrwin, 1, wxEXPAND | wxALL);
	pnl_2->SetSizer(grdszr_pnl_2);
	grdszr_pnl_2->Layout();

	grdszr_main->Add(pnl_1, 0, wxEXPAND | wxDOWN, 20);
	grdszr_main->Add(pnl_2, 5, wxEXPAND);

	this->SetSizer(grdszr_main);
	grdszr_main->Layout();

	scrwin->SetScrollRate(5, 5);
}

cMain::~cMain() 
{
	Clear();
}


void cMain::create_bttn_click(wxCommandEvent& evt) {
	create_bttn->Enable(false);
	reset_bttn->Enable(true);
	run_sim_bttn->Enable(true);

	width = spncntrl_cols->GetValue();
	height = spncntrl_rows->GetValue();

	cmb = new wxChoice * [width * height];
	starting_bits = new wxChoice * [height];

	grdszr_btn->SetRows( 2 * height + 2 );
	grdszr_btn->SetCols( width + 2 );

	for (int i = 0; i < width + 1; i++) {
		topside_numbering.push_back(new wxListBox(scrwin, wxID_ANY, wxPoint(400, 35), wxSize(70, 25)));
		topside_numbering[topside_numbering.size() - 1]->Insert(std::to_string(i), 0);
		grdszr_btn->Add(topside_numbering[topside_numbering.size() - 1], 1, wxUP, 5);
	}
	topside_numbering.push_back(new wxListBox(scrwin, wxID_ANY, wxPoint(400, 35), wxSize(70, 25)));
	topside_numbering[topside_numbering.size() - 1]->Insert("END", 0);
	grdszr_btn->Add(topside_numbering[topside_numbering.size() - 1], 1, wxUP, 5);

	for (int x = 0; x < height; x++) {
		starting_bits[x] = new wxChoice(scrwin, 30 + x, wxDefaultPosition, wxDefaultSize, 2, avlb_Qbit);
		starting_bits[x]->SetSelection(0);
		grdszr_btn->Add(starting_bits[x], 1, wxUP, 5);

		for (int y = 0; y < width; y++) {
			cmb[y * height + x] = new wxChoice(scrwin, 20 + y * height + x, wxDefaultPosition, wxDefaultSize, 7, avlb_gates);
			cmb[y * height + x]->SetSelection(0);
			grdszr_btn->Add(cmb[y * height + x], 1, wxUP, 5);
		}
		Output.push_back(new wxListBox(scrwin, wxID_ANY, wxPoint(400, 35), wxSize(110, 25))); 
		Output[Output.size() - 1]->Show(false);
		grdszr_btn->Add(Output[Output.size() - 1], 1, wxRIGHT, 15);

		fake_angls.push_back(new wxSpinCtrlDouble(scrwin, 11 + x, "0", wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 360, 0, 0.001));
		grdszr_btn->Add(fake_angls[fake_angls.size() - 1], 1, wxDOWN, 20);
		fake_angls[fake_angls.size() - 1]->Enable(false); fake_angls[fake_angls.size() - 1]->Show(false);
		for (int y = 0; y < width; y++) {
			angls.push_back(new wxSpinCtrlDouble(scrwin, 10 + y * height + x, "0", wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 360, 0, 0.001));
			grdszr_btn->Add(angls[angls.size() - 1], 1, wxDOWN, 20);
		}

		fake_angls.push_back(new wxSpinCtrlDouble(scrwin, 11 + x + 1, "0", wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 360, 0, 0.001));
		grdszr_btn->Add(fake_angls[fake_angls.size() - 1], 1, wxDOWN, 20);
		fake_angls[fake_angls.size() - 1]->Enable(false); fake_angls[fake_angls.size() - 1]->Show(false);
	}
	scrwin->SetSizer(grdszr_btn);
	grdszr_btn->Layout();


	evt.Skip();
}

void cMain::reset_bttn_click(wxCommandEvent& evt) {
	Clear();
	create_bttn->Enable(true);
	run_sim_bttn->Enable(false);
	reset_bttn->Enable(false);
	evt.Skip();
}

void cMain::run_sim_bttn_click(wxCommandEvent& evt) {
	final_output->Clear();

	Q_simulator.init(height, width);
	gate_table temp_table;
	vector< vector< double > > temp_table_angles;
	temp_table.resize(height);
	temp_table_angles.resize(height);
	for (int x = 0; x < height; x++) {
		temp_table[x].resize(width);
		temp_table_angles[x].resize(width);
	}


	for (int x = 0; x < height; x++) {
		for (int y = 0; y < width; y++) {
			temp_table[x][y] =  Q_simulator.str_gate_to_enum_gate( (std::string)cmb[y * height + x]->GetString( cmb[y * height + x]->GetCurrentSelection() ) );
			temp_table_angles[x][y] = angls[y * height + x]->GetValue();
		}
	}
	Q_simulator.set_table( temp_table );
	Q_simulator.set_table_angles( temp_table_angles );

	for (int x = 0; x < height; x++) {
		if ( starting_bits[x]->GetString( starting_bits[x]->GetCurrentSelection() ) == "0" ) {
			Q_simulator.add_qbit( x, 1, 0, 0, 0 );
		}
		else {
			Q_simulator.add_qbit( x, 0, 0, 1, 0 );
		}
	}

	auto FLOPPA = Q_simulator.run(true);
	std:string fnl_out = "";
	/*for ( auto x : FLOPPA ) {
		fnl_out += "("+ prune_str_end( std::to_string(x.real()), 4) + ")+(" + prune_str_end(std::to_string(x.imag()), 4) + ")i" + "  +  ";
	}
	fnl_out = prune_str_end( fnl_out, 5 );*/
	for (int x = 0; x < pow(2, height); x++ ) {
		auto num = FLOPPA[x];
		std::string real_str = prune_str_end(std::to_string(num.real()), 4);
		std::string imag_str = prune_str_end(std::to_string(num.imag()), 4);
		if (num.real() == 0 && num.imag() == 0) { continue; }
		if (real_str == "1.00" && imag_str == "0.00") {
			fnl_out += "|" + num_to_bin_str(x,height) + ">  +  ";
		}
		else if (real_str != "0.00" && imag_str == "0.00") {
			fnl_out += "(" + real_str + ")" + "|" + num_to_bin_str(x, height) + ">  +  ";
		}
		else if (real_str == "0.00" && imag_str != "0,00") {
			fnl_out += "(" + imag_str + "i)" + "|" + num_to_bin_str(x, height) + ">  +  ";
		}
		else if (real_str != "0.00" && imag_str != "0.00") {
			fnl_out += "((" + real_str + ")" + " + " + "(" + imag_str + "i))" + "|" + num_to_bin_str(x, height) + ">  +  ";
		}
	}
	fnl_out = prune_str_end(fnl_out, 5);
	final_output->Append( fnl_out );

	evt.Skip();
}



void cMain::Clear() {

	final_output->Clear();

	for (int x = 0; x < height; x++) {
		for (int y = 0; y < width; y++) {
			cmb[y * height + x]->Destroy();
		}
	} delete[] cmb;

	for (int x = 0; x < height; x++) { starting_bits[x]->Destroy(); }
	delete[] starting_bits;

	for ( auto angls_ptr : angls ) {
		delete angls_ptr;
	}
	angls.clear();

	for (auto fake_anglss_ptr : fake_angls) {
		delete fake_anglss_ptr;
	}
	fake_angls.clear();

	for (auto topside_numbering_ptr : topside_numbering) {
		delete topside_numbering_ptr;
	}
	topside_numbering.clear();

	for (auto Output_ptr : Output) {
		delete Output_ptr;
	}
	Output.clear();
}

std::string cMain::num_to_bin_str(int num, int digit_num) {
	string binary("");
	int mask = 1;
	for (int i = 0; i < digit_num; i++)
	{
		if ((mask & num) >= 1)
			binary = "1" + binary;
		else
			binary = "0" + binary;
		mask <<= 1;
	}

	return binary;
}

std::string prune_str_end( std::string str, int num_of_del) {
	str.resize(str.size() - num_of_del);
	return str;
}