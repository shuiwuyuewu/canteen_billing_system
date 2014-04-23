#ifndef MYTHREAD_H
#define MYTHREAD_H

#include "wx/thread.h"
#include "wx/string.h"

class MyThread:public wxThread
{
    public:
        MyThread(wxString name):m_name(name){};
        virtual void * Entry();

    private:
        wxString m_name;
};

class PayThread:public wxThread
{
    public:
        PayThread(){};
        virtual void * Entry();

    private:

};
#endif // MYTHREAD_H
