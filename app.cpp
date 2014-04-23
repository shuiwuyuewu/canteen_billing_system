#include "app.h"
#include "frame.h"

bool MyApp::OnInit()
{
    if ( !wxApp::OnInit() )
        return false;

    frame = new MyFrame(NULL,
                                 wxID_ANY,
                                 wxT("食堂餐盘自动计费系统"),
                                 wxDefaultPosition,
                                 wxSize(800, 600));
    frame->Maximize();
    frame->Show();

    return true;
}
