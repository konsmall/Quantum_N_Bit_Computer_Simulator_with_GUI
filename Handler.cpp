#include "Handler.h"

wxIMPLEMENT_APP(Handler);

Handler::Handler() {

}

Handler::~Handler() {

}

bool Handler::OnInit()
{
	frame_1 = new cMain();
	frame_1->Show();

	return true;
}