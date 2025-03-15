#pragma once

#include "wx/wx.h"
#include "cMain.h"

class Handler : public wxApp
{
public:
	Handler();
	~Handler();

	virtual bool OnInit();

private:
	cMain* frame_1 = nullptr;
};