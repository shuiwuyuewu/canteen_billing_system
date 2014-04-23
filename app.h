#ifndef APP_H_INCLUDED
#define APP_H_INCLUDED

#include "wx/app.h"
class MyFrame;

class MyApp : public wxApp
{
public:
    bool OnInit();
    MyFrame* frame;
};

DECLARE_APP(MyApp)

#endif // APP_H_INCLUDED
