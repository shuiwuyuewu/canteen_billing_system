#ifndef FRAME_H_INCLUDED
#define FRAME_H_INCLUDED

#include <vector>
#include "wx/frame.h"
#include "wx/grid.h"
#include "wx/aui/aui.h"
#include "wx/html/htmlwin.h"
#include "wx/listctrl.h"
#include "opencv2/opencv.hpp"
#include "wx/webview.h"

class MyFrame : public wxFrame
{
    typedef enum
    {
        ID_PLATE = wxID_HIGHEST+1,
        ID_FOOD,
        ID_TRAIN,
        ID_VIEW_PLATE,
        ID_ADD_PLATE,
        ID_MODIFY_PLATE,
        ID_DELETE_PLATE,
        ID_VIEW_FOOD,
        ID_ADD_FOOD,
        ID_MODIFY_FOOD,
        ID_DELETE_FOOD,
        ID_PLATE_RECOGNITION,
        ID_ANTIZERO_THRESHOLD,
        ID_CLOSE_OPERATION1,
        ID_RICE_RECOGNITION,
        ID_PLATE_BLACKED,
        ID_FOOD_WHITED,
        ID_CLOSE_OPERATION2,
        ID_FOOD_ERODE,
        ID_FOOD_RECOGNITION,
        ID_PAY,
        ID_MANUAL_MODE,
        ID_AUTO_MODE,
        ID_FILE_OPEN,
        ID_ANY
    }id;

public:
    MyFrame(wxWindow* parent,
            wxWindowID id,
            const wxString& title,
            const wxPoint& pos = wxDefaultPosition,
            const wxSize& size = wxDefaultSize,
            long style = wxDEFAULT_FRAME_STYLE | wxSUNKEN_BORDER);

    ~MyFrame();
    void InitInterface();
    void UpdateShow(wxString sFileName);
    void UpdateShow2(wxString sHtml,wxString name);
    void ClearRC(){m_rc.clear();}
    void SetAntiZero(bool val){m_bAntiZero = val;}
public:


public:
    wxListCtrl* CreateListCtrl(MyFrame::id id);
    wxHtmlWindow* CreateHTMLCtrl(wxWindow* parent = NULL);

    void UpdatePlate();
    void UpdateFood();
    bool isSilver(int r, int g, int b);
    cv::Mat hist(cv::Mat mImg);
    void PlateRecognition();
    void PlateDetection();
    void AntiZeroThreshold();
    void CloseOperation1();
    void RiceRecognition();
    void PlateBlacked();
    void FoodWhited();
    void CloseOperation2();
    void FoodErode();
    void FoodRecognition();
    void InitEditMenu();
    void Pay();

private:
    void OnOpenFile(wxCommandEvent& evt);
    void OnExit(wxCommandEvent& evt);
    void OnAbout(wxCommandEvent& evt);
    void OnRCPlate(wxListEvent& evt);
    void OnRCFood(wxListEvent& evt);
    void OnViewPlate(wxCommandEvent& evt);
    void OnAddPlate(wxCommandEvent& evt);
    void OnDeletePlate(wxCommandEvent& evt);
    void OnModifyPlate(wxCommandEvent& evt);
    void OnViewFood(wxCommandEvent& evt);
    void OnAddFood(wxCommandEvent& evt);
    void OnModifyFood(wxCommandEvent& evt);
    void OnDeleteFood(wxCommandEvent& evt);

    void OnPlateRecognition(wxCommandEvent& evt);
    void OnAntiZeroThreshold(wxCommandEvent& evt);
    void OnCloseOperation1(wxCommandEvent& evt);
    void OnRiceRecognition(wxCommandEvent& evt);
    void OnPlateBlacked(wxCommandEvent& evt);
    void OnFoodWhited(wxCommandEvent& evt);
    void OnCloseOperation2(wxCommandEvent& evt);
    void OnFoodErode(wxCommandEvent& evt);
    void OnFoodRecognition(wxCommandEvent& evt);
    void OnPay(wxCommandEvent& evt);
    void OnManual(wxCommandEvent& evt);
    void OnAuto(wxCommandEvent& evt);

    void OnPlateRecognitionUI(wxUpdateUIEvent& evt);
    void OnAntiZeroThresholdUI(wxUpdateUIEvent& evt);
    void OnCloseOperation1UI(wxUpdateUIEvent& evt);
    void OnRiceRecognitionUI(wxUpdateUIEvent& evt);
    void OnPlateBlackedUI(wxUpdateUIEvent& evt);
    void OnFoodWhitedUI(wxUpdateUIEvent& evt);
    void OnCloseOperation2UI(wxUpdateUIEvent& evt);
    void OnFoodErodeUI(wxUpdateUIEvent& evt);
    void OnFoodRecognitionUI(wxUpdateUIEvent& evt);
    void OnFoodPayUI(wxUpdateUIEvent& evt);
    void OnAutoUI(wxUpdateUIEvent& evt);
    void OnManualUI(wxUpdateUIEvent& evt);

private:
    wxAuiManager m_mgr;
    wxString m_sBmpFileName;
    wxListCtrl* m_LCplate;
    wxListCtrl* m_LCfood;
    wxListCtrl* m_LCresult;
    wxHtmlWindow* m_HWshow;
    int m_index_plate;
    int m_index_food;
    bool m_mode;
    std::vector<cv::Rect> m_rc;
    bool m_bplateDetection;
    bool m_bAntiZero;
    bool m_bCloseOperation1;
    bool m_bRiceRecognition;
    bool m_bPlateBlacked;
    bool m_bFoodWhited;
    bool m_bCloseOperation2;
    bool m_bFoodErode;
    bool m_bFoodDetection;
    bool m_bCount;

    DECLARE_EVENT_TABLE()
};

#endif // FRAME_H_INCLUDED
