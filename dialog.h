#ifndef DIALOG_H_INCLUDED
#define DIALOG_H_INCLUDED

#include "wx/dialog.h"

class PlateDialog: public wxDialog
{
public:
    enum{
    ID_NAME = wxID_HIGHEST+1,
    ID_PATH};
public:
    PlateDialog(wxWindow* parent,wxString caption);

    void SetName(const wxString& name){m_name=name;}
    wxString GetName() const {return m_name;}
    void SetPath(const wxString& path){m_path=path;}
    wxString GetPath() const {return m_path;}

private:
    wxString m_name;
    wxString m_path;

    DECLARE_EVENT_TABLE()
};

class FoodDialog: public wxDialog
{
public:
    enum{
    ID_FOOD_NAME = wxID_HIGHEST+1,
    ID_PATH,
    ID_PRICE,
    ID_TRAIN,
    ID_TRAIN_PATH,
    ID_NAME};
public:
    FoodDialog(wxWindow* parent,wxString caption);

    void SetFoodName(const wxString& name ){m_FoodName = name;}
    void SetPath(const wxString& path ){m_Path = path;}
    void SetPrice(const wxString& price){m_Price = price;}
    void SetTrain(const wxString& train){m_Train = train;}
    void SetTrainPath(const wxString& trainPath){m_TrainPath = trainPath;}
    void SetName(const wxString& name){m_Name = name;}
    wxString GetFoodName() const {return m_FoodName;}
    wxString GetPath() const {return m_Path;}
    wxString GetPrice() const {return m_Price;}
    wxString GetTrain() const {return m_Train;}
    wxString GetTrainPath() const {return m_TrainPath;}
    wxString GetName() const {return m_Name;}
private:
    wxString m_FoodName;
    wxString m_Path;
    wxString m_Price;
    wxString m_Train;
    wxString m_TrainPath;
    wxString m_Name;

    DECLARE_EVENT_TABLE()
};
#endif // DIALOG_H_INCLUDED



