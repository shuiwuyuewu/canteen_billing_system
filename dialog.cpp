#include "dialog.h"
#include "wx/sizer.h"
#include "wx/control.h"
#include "wx/stattext.h"
#include "wx/textctrl.h"
#include "wx/button.h"
#include "wx/valgen.h"


PlateDialog::PlateDialog(wxWindow* parent,wxString caption):wxDialog(parent, wxID_ANY, caption,
      wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE),m_name(""),m_path("")
{
    wxBoxSizer* sizer  = new wxBoxSizer(wxVERTICAL);
    wxFlexGridSizer* sizer1 = new wxFlexGridSizer(2,2,0,0);
    wxBoxSizer* sizer3 = new wxBoxSizer(wxHORIZONTAL);
    wxStaticText* STname = new wxStaticText(this,wxID_STATIC,
                                            wxT("餐盘文件名："));
    wxTextCtrl* TCname = new wxTextCtrl(this,ID_NAME,wxT(""));

    wxStaticText* STpath = new wxStaticText(this,wxID_STATIC,
                                            wxT("餐盘文件路径："));
    wxTextCtrl* TCpath = new wxTextCtrl(this,ID_PATH,wxT(""));

    sizer1->Add(STname,0,wxALIGN_LEFT|wxALL,5);
    sizer1->Add(TCname,0,wxALIGN_LEFT|wxALL,5);
    sizer1->Add(STpath,0,wxALIGN_LEFT|wxALL,5);
    sizer1->Add(TCpath,0,wxALIGN_LEFT|wxALL,5);

    wxButton* Bok = new wxButton(this,wxID_OK,
                                 wxT("确定"));
    wxButton* Bcancel = new wxButton(this,wxID_CANCEL,
                                 wxT("取消"));
    sizer3->Add(Bok,0,wxALL,5);
    sizer3->Add(5,5,2,wxALIGN_CENTER|wxALL,5);
    sizer3->Add(Bcancel,0,wxALL,5);

    sizer->Add(sizer1,0,wxALL,5);
    sizer->Add(sizer3,0,wxALIGN_CENTER|wxALL,5);

    SetSizerAndFit(sizer);

    FindWindow(ID_NAME)->SetValidator(wxGenericValidator(&m_name));
    FindWindow(ID_PATH)->SetValidator(wxGenericValidator(&m_path));
}

FoodDialog::FoodDialog(wxWindow* parent,wxString caption):wxDialog(parent, wxID_ANY, caption,
      wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE),
      m_FoodName(""),m_Path(""),m_Price(""),m_Train("")
{
    wxBoxSizer* sizer  = new wxBoxSizer(wxVERTICAL);
    wxFlexGridSizer* sizer1 = new wxFlexGridSizer(6,2,0,0);
    wxBoxSizer* sizer3 = new wxBoxSizer(wxHORIZONTAL);

    wxStaticText* STfoodname = new wxStaticText(this,wxID_STATIC,
                                            wxT("菜肴名："));
    wxTextCtrl* TCfoodname = new wxTextCtrl(this,ID_FOOD_NAME,wxT(""));

    wxStaticText* STpath = new wxStaticText(this,wxID_STATIC,
                                            wxT("菜肴文件路径："));
    wxTextCtrl* TCpath = new wxTextCtrl(this,ID_PATH,wxT(""));

    wxStaticText* STprice = new wxStaticText(this,wxID_STATIC,
                                            wxT("菜肴价格："));
    wxTextCtrl* TCprice = new wxTextCtrl(this,ID_PRICE,wxT(""));

    wxStaticText* STtrain = new wxStaticText(this,wxID_STATIC,
                                            wxT("训练器文件名："));
    wxTextCtrl* TCtrain = new wxTextCtrl(this,ID_TRAIN,wxT(""));

    wxStaticText* STtrainpath = new wxStaticText(this,wxID_STATIC,
                                            wxT("菜肴训练文件路径："));
    wxTextCtrl* TCtrainpath = new wxTextCtrl(this,ID_TRAIN_PATH,wxT(""));

    wxStaticText* STname = new wxStaticText(this,wxID_STATIC,
                                            wxT("菜肴文件名："));
    wxTextCtrl* TCname = new wxTextCtrl(this,ID_NAME,wxT(""));

    sizer1->Add(STfoodname,0,wxALIGN_LEFT|wxALL,5);
    sizer1->Add(TCfoodname,0,wxALIGN_LEFT|wxALL,5);
    sizer1->Add(STpath,0,wxALIGN_LEFT|wxALL,5);
    sizer1->Add(TCpath,0,wxALIGN_LEFT|wxALL,5);
    sizer1->Add(STprice,0,wxALIGN_LEFT|wxALL,5);
    sizer1->Add(TCprice,0,wxALIGN_LEFT|wxALL,5);
    sizer1->Add(STtrain,0,wxALIGN_LEFT|wxALL,5);
    sizer1->Add(TCtrain,0,wxALIGN_LEFT|wxALL,5);
    sizer1->Add(STtrainpath,0,wxALIGN_LEFT|wxALL,5);
    sizer1->Add(TCtrainpath,0,wxALIGN_LEFT|wxALL,5);
    sizer1->Add(STname,0,wxALIGN_LEFT|wxALL,5);
    sizer1->Add(TCname,0,wxALIGN_LEFT|wxALL,5);

    wxButton* Bok = new wxButton(this,wxID_OK,
                                 wxT("确定"));
    wxButton* Bcancel = new wxButton(this,wxID_CANCEL,
                                 wxT("取消"));
    sizer3->Add(Bok,0,wxALL,5);
    sizer3->Add(5,5,2,wxALIGN_CENTER|wxALL,5);
    sizer3->Add(Bcancel,0,wxALL,5);

    sizer->Add(sizer1,0,wxALL,5);
    sizer->Add(sizer3,0,wxALIGN_CENTER|wxALL,5);

    SetSizerAndFit(sizer);

    FindWindow(ID_FOOD_NAME)->SetValidator(wxGenericValidator(&m_FoodName));
    FindWindow(ID_PATH)->SetValidator(wxGenericValidator(&m_Path));
    FindWindow(ID_PRICE)->SetValidator(wxGenericValidator(&m_Price));
    FindWindow(ID_TRAIN)->SetValidator(wxGenericValidator(&m_Train));
    FindWindow(ID_TRAIN_PATH)->SetValidator(wxGenericValidator(&m_TrainPath));
    FindWindow(ID_NAME)->SetValidator(wxGenericValidator(&m_Name));
}

BEGIN_EVENT_TABLE(PlateDialog,wxDialog)

END_EVENT_TABLE()

BEGIN_EVENT_TABLE(FoodDialog,wxDialog)

END_EVENT_TABLE()
