#include "frame.h"
#include "wx/grid.h"
#include "wx/msgdlg.h"
#include "wx/menu.h"
#include "sqlite3.h"
#include "wx/filedlg.h"
#include <iostream>
#include <string>
#include "dialog.h"
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/nonfree/features2d.hpp"
#include "MyThread.h"
#include <wx/utils.h>
#include <wx/toolbar.h>
#include <wx/bitmap.h>
#include <wx/artprov.h>
#include <wx/aui/auibar.h>

using namespace std;
using namespace cv;

#include "sample.xpm"

MyFrame::MyFrame(wxWindow* parent,
                 wxWindowID id,
                 const wxString& title,
                 const wxPoint& pos,
                 const wxSize& size,
                 long style)
        : wxFrame(parent, id, title, pos, size, style),m_mode(true)
{
    m_mgr.SetManagedWindow(this);

    wxImage::AddHandler(new wxPNGHandler);
    wxImage::AddHandler(new wxJPEGHandler);
    // create menu
    wxMenuBar* mb = new wxMenuBar;

    wxMenu* file_menu = new wxMenu;
    file_menu->Append(ID_FILE_OPEN, wxT("打开餐盘图像..."));
    file_menu->AppendSeparator();
    file_menu->Append(wxID_EXIT,wxT("退出"));

    wxMenu* help_menu = new wxMenu;
    help_menu->Append(wxID_ABOUT,wxT("关于"));

    wxMenu* edit_menu = new wxMenu;
//    edit_menu->AppendRadioItem(ID_ANY,wxT("请按步骤执行"));
    edit_menu->Append(ID_PLATE_RECOGNITION,wxT("餐盘检测"));
    edit_menu->Append(ID_ANTIZERO_THRESHOLD, wxT("反零阈值处理"));
    edit_menu->Append(ID_CLOSE_OPERATION1, wxT("闭运算处理1"));
    edit_menu->Append(ID_RICE_RECOGNITION, wxT("白饭检测"));
    edit_menu->Append(ID_PLATE_BLACKED, wxT("餐盘二值化处理"));
    edit_menu->Append(ID_FOOD_WHITED, wxT("菜肴二值化处理"));
    edit_menu->Append(ID_CLOSE_OPERATION2, wxT("闭运算处理2"));
    edit_menu->Append(ID_FOOD_ERODE, wxT("腐蚀处理"));
    edit_menu->Append(ID_FOOD_RECOGNITION, wxT("菜肴检测"));


    wxMenu* pay_menu = new wxMenu;
    pay_menu->Append(ID_PAY, wxT("计价"));

    wxMenu* mode_menu = new wxMenu;
    mode_menu->AppendRadioItem(ID_MANUAL_MODE, wxT("手动"));
    mode_menu->AppendRadioItem(ID_AUTO_MODE, wxT("自动"));

    mb->Append(file_menu, wxT("文件"));
    mb->Append(edit_menu, wxT("识别"));
    mb->Append(pay_menu, wxT("计费"));
    mb->Append(mode_menu, wxT("模式"));
    mb->Append(help_menu, wxT("帮助"));

    SetMenuBar(mb);

    InitEditMenu();
    SetIcon(wxIcon(sample_xpm));
    CreateStatusBar();
    GetStatusBar()->SetStatusText(wxT("准备好了"));

//    wxToolBar* toolBar = CreateToolBar();
//    wxBitmap bitmap;
//    bitmap.LoadFile("./bitmap/open.bmp",wxBITMAP_TYPE_BMP);
//
//    toolBar->AddTool(ID_FILE_OPEN,wxT("打开餐盘图像文件"),
//                     bitmap,wxNullBitmap,wxITEM_NORMAL,
//                     wxT("打开"),wxT("打开餐盘文件"));
//    toolBar->Realize();
//    SetToolBar(toolBar);

    wxAuiToolBar* tb1 = new wxAuiToolBar(this,wxID_ANY,wxDefaultPosition,wxDefaultSize,
                                         wxAUI_TB_DEFAULT_STYLE);
    tb1->SetToolBitmapSize(wxSize(48,48));
    tb1->AddTool(ID_FILE_OPEN, wxT("打开餐盘图像文件"), wxArtProvider::GetBitmap(wxART_FILE_OPEN));
    tb1->AddSeparator();

    wxBitmap bitmap;

    bitmap.LoadFile("./bitmap/manul.png",wxBITMAP_TYPE_PNG);
    tb1->AddTool(ID_MANUAL_MODE,wxT("手动模式")
                 ,bitmap,wxT("手动模式")
                 ,wxITEM_RADIO);
    bitmap.LoadFile("./bitmap/auto.png",wxBITMAP_TYPE_PNG);
    tb1->AddTool(ID_AUTO_MODE,wxT("自动模式")
                 ,bitmap,wxT("自动模式")
                 ,wxITEM_RADIO);
    tb1->AddSeparator();

    bitmap.LoadFile("./bitmap/1.png",wxBITMAP_TYPE_PNG);
    tb1->AddTool(ID_PLATE_RECOGNITION,wxT("餐盘检测"),bitmap,wxT("餐盘检测"));
    bitmap.LoadFile("./bitmap/2.png",wxBITMAP_TYPE_PNG);
    tb1->AddTool(ID_ANTIZERO_THRESHOLD,wxT("反零阈值处理"),bitmap,wxT("反零阈值处理"));
    bitmap.LoadFile("./bitmap/3.png",wxBITMAP_TYPE_PNG);
    tb1->AddTool(ID_CLOSE_OPERATION1,wxT("闭运算处理1"),bitmap,wxT("闭运算处理1"));
    bitmap.LoadFile("./bitmap/4.png",wxBITMAP_TYPE_PNG);
    tb1->AddTool(ID_RICE_RECOGNITION,wxT("白饭检测"),bitmap,wxT("白饭检测"));
    bitmap.LoadFile("./bitmap/5.png",wxBITMAP_TYPE_PNG);
    tb1->AddTool(ID_PLATE_BLACKED,wxT("餐盘二值化处理"),bitmap,wxT("餐盘二值化处理"));
    bitmap.LoadFile("./bitmap/6.png",wxBITMAP_TYPE_PNG);
    tb1->AddTool(ID_FOOD_WHITED,wxT("菜肴二值化处理"),bitmap,wxT("菜肴二值化处理"));
    bitmap.LoadFile("./bitmap/7.png",wxBITMAP_TYPE_PNG);
    tb1->AddTool(ID_CLOSE_OPERATION2,wxT("闭运算处理2"),bitmap,wxT("闭运算处理2"));
    bitmap.LoadFile("./bitmap/8.png",wxBITMAP_TYPE_PNG);
    tb1->AddTool(ID_FOOD_ERODE,wxT("菜肴腐蚀处理"),bitmap,wxT("菜肴腐蚀处理"));
    bitmap.LoadFile("./bitmap/9.png",wxBITMAP_TYPE_PNG);
    tb1->AddTool(ID_FOOD_RECOGNITION,wxT("菜肴检测"),bitmap,wxT("菜肴检测"));
    tb1->AddSeparator();

    bitmap.LoadFile("./bitmap/jia.png",wxBITMAP_TYPE_PNG);
    tb1->AddTool(ID_PAY,wxT("计价"),bitmap,wxT("计价"));

    tb1->Realize();

    m_mgr.AddPane(tb1, wxAuiPaneInfo().
                  Name(wxT("tb")).Caption(wxT("Toolbar (Horizontal)")).
                  ToolbarPane().Top().Row(1));

    SetFont(GetFont().MakeLarger());
    InitInterface();
}

MyFrame::~MyFrame()
{
    m_mgr.UnInit();
}

wxHtmlWindow* MyFrame::CreateHTMLCtrl(wxWindow* parent)
{
    if (!parent)
        parent = this;

    wxHtmlWindow* ctrl = new wxHtmlWindow(this,wxID_ANY);
    wxString html;
    html = "<html><body>"
            wxT("<h1>欢迎使用食堂餐盘自动计费系统</h1>")
            "</body></html>";
    ctrl->SetPage(html);
    return ctrl;
}

void MyFrame::InitInterface()
{
    m_LCplate = CreateListCtrl(ID_PLATE);
    m_LCplate->AppendColumn(wxT("餐盘名"));
    m_LCplate->AppendColumn(wxT("餐盘路径"));

    wxSize size = m_LCplate->GetMinSize();
    size.SetWidth(170);
    m_LCplate->SetSize(size);

    UpdatePlate();
    m_mgr.AddPane(m_LCplate, wxLEFT, wxT("餐盘库"));

//    m_mgr.GetPane(m_LCplate).MinSize(size);

    m_LCfood = CreateListCtrl(ID_FOOD);
    m_LCfood->AppendColumn(wxT("菜肴名"));
    m_LCfood->AppendColumn(wxT("菜肴路径"));
    m_LCfood->AppendColumn(wxT("菜肴价格"));
    m_LCfood->AppendColumn(wxT("分类器训练文件"));

    UpdateFood();
    m_mgr.AddPane(m_LCfood, wxRIGHT, wxT("菜肴库"));

    m_sBmpFileName = "";
    m_HWshow = CreateHTMLCtrl(NULL);
    m_mgr.AddPane(m_HWshow, wxCENTER,wxT("显示窗"));

//    m_LCresult = CreateListCtrl(ID_TRAIN);
//    m_LCresult->AppendColumn(wxT("序号"));
//    m_LCresult->AppendColumn(wxT("菜肴名"));
//    m_LCresult->AppendColumn(wxT("价格"));
//
//    m_mgr.AddPane(m_LCresult, wxBOTTOM, wxT("计价结果"));

    m_mgr.Update();
}

void MyFrame::OnExit(wxCommandEvent& WXUNUSED(event))
{
    Close(true);
}

void MyFrame::OnAbout(wxCommandEvent& WXUNUSED(event))
{
    wxMessageBox(wxT("食堂餐盘自动计费系统\n制作者：\t张子文\n年级班级：2010级电气工程及其自动化2班\n学号：\t201006022236"), wxT("关于食堂餐盘自动计费系统"), wxOK, this);
}

wxListCtrl* MyFrame::CreateListCtrl(MyFrame::id id)
{
    wxListCtrl* listCtrl = new wxListCtrl(this, id,
                                          wxDefaultPosition,
                                          wxDefaultSize,
                                          wxLC_REPORT);
    return listCtrl;
}

void MyFrame::UpdateShow(wxString sFileName)
{
    wxString html;
    m_sBmpFileName = sFileName;
    html = "<html><body>"
            "<img src=\"" + m_sBmpFileName +
            "\" />"
            "</body></html>";

    m_HWshow->SetPage( html );
}

void MyFrame::UpdateShow2(wxString sHtml, wxString name)
{
    wxString html;
    m_sBmpFileName = name;

    html = "<html>"
//            "<head><style type=\"text/css\">"
//            "#final {color:rgb(255,0,0);font-size:3em}"
//            "</style></head>"
            "<body>"+sHtml+"</body></html>";

    m_HWshow->SetPage( html );
}

void MyFrame::OnRCPlate(wxListEvent& evt)
{
    m_index_plate = evt.GetIndex();

    wxMenu* popMenu = new wxMenu;
    popMenu->Append(ID_VIEW_PLATE,wxT("查看"));
    popMenu->Append(ID_ADD_PLATE,wxT("添加"));
    popMenu->Append(ID_MODIFY_PLATE,wxT("修改"));
    popMenu->Append(ID_DELETE_PLATE,wxT("删除"));

    PopupMenu(popMenu);
}

void MyFrame::OnRCFood(wxListEvent& evt)
{
    m_index_food = evt.GetIndex();

    wxMenu* popMenu = new wxMenu;
    popMenu->Append(ID_VIEW_FOOD,wxT("查看"));
    popMenu->Append(ID_ADD_FOOD,wxT("添加"));
    popMenu->Append(ID_MODIFY_FOOD,wxT("修改"));
    popMenu->Append(ID_DELETE_FOOD,wxT("删除"));

    PopupMenu(popMenu);
}

void MyFrame::UpdatePlate()
{
    sqlite3* db;
    sqlite3_stmt* stmt;

    int rc = sqlite3_open_v2("MyDB.db",&db,SQLITE_OPEN_READWRITE,NULL);
    if(rc)
    {
        cout << "Can't open database.\n";
        sqlite3_close(db);
        exit(-1);
    }
    m_LCplate->DeleteAllItems();
    char* sql = "select * from plate;";
    sqlite3_prepare_v2(db,sql,-1,&stmt,NULL);
    rc = sqlite3_step(stmt);
    int i=0;
    while(rc == SQLITE_ROW)
    {
        m_LCplate->InsertItem(i,sqlite3_column_text(stmt,1));
        m_LCplate->SetItem(i++,1,sqlite3_column_text(stmt,2));
        rc = sqlite3_step(stmt);
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
}

void MyFrame::UpdateFood()
{
    sqlite3* db;
    sqlite3_stmt* stmt;

    int rc = sqlite3_open_v2("MyDB.db",&db,SQLITE_OPEN_READWRITE,NULL);
    if(rc)
    {
        wxMessageBox("In UpdateFood:Can't open database.\n");
        sqlite3_close(db);
        exit(-1);
    }
    m_LCfood->DeleteAllItems();
    char* sql = "select * from food;";
    sqlite3_prepare_v2(db,sql,-1,&stmt,NULL);
    rc = sqlite3_step(stmt);
    int i=0;
    while(rc == SQLITE_ROW)
    {
        m_LCfood->InsertItem(i,sqlite3_column_text(stmt,5));
        m_LCfood->SetItem(i,1,sqlite3_column_text(stmt,2));
        m_LCfood->SetItem(i,2,sqlite3_column_text(stmt,6));
        m_LCfood->SetItem(i++,3,sqlite3_column_text(stmt,3));
        rc = sqlite3_step(stmt);
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
}

void MyFrame::OnViewPlate(wxCommandEvent& evt)
{
    m_sBmpFileName = m_LCplate->GetItemText(m_index_plate,1);
    UpdateShow(m_sBmpFileName);
//    m_LCresult->DeleteAllItems();
    InitEditMenu();
}

void MyFrame::OnAddPlate(wxCommandEvent& evt)
{
    sqlite3* db;
    sqlite3_stmt* stmt;
    wxString sql = "insert into plate (name,path) values('";
    wxString name,path;

    PlateDialog* dialog = new PlateDialog(this,wxT("添加餐盘记录"));
    dialog->CenterOnScreen();
    if(dialog->ShowModal()==wxID_OK)
    {
        name = dialog->GetName();
        path = dialog->GetPath();

        int rc = sqlite3_open_v2("MyDB.db",&db,SQLITE_OPEN_READWRITE,NULL);
        if(rc)
        {
            wxMessageBox("Can't open database.\n");
            sqlite3_close(db);
            exit(-2);
        }
        sql = sql + name +"','"+path+"');";
//        wxMessageBox(sql);

        sqlite3_prepare_v2(db,sql.c_str(),-1,&stmt,NULL);
        rc = sqlite3_step(stmt);
        if(rc!=SQLITE_DONE)
        {
            wxMessageBox(wxT("添加餐盘记录失败！"));
            sqlite3_close(db);
            exit(-3);
        }
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        UpdatePlate();
    }
}

void MyFrame::OnModifyPlate(wxCommandEvent& evt)
{
    sqlite3* db;
    sqlite3_stmt* stmt;
    wxString sql = "update plate set name = '";
    wxString name,path,nameOld,pathOld;
    nameOld = m_LCplate->GetItemText(m_index_plate,0);
    pathOld = m_LCplate->GetItemText(m_index_plate,1);

//    wxMessageBox(nameOld+"\n"+pathOld);
    PlateDialog* dialog = new PlateDialog(this,wxT("修改餐盘记录"));
    dialog->SetName(nameOld);
    dialog->SetPath(pathOld);
    dialog->CenterOnScreen();
    if(dialog->ShowModal()==wxID_OK)
    {
        name = dialog->GetName();
        path = dialog->GetPath();

        int rc = sqlite3_open_v2("MyDB.db",&db,SQLITE_OPEN_READWRITE,NULL);
        if(rc)
        {
            wxMessageBox("Can't open database.\n");
            sqlite3_close(db);
            exit(-2);
        }
        sql = sql + name +"', path='"+path+"' where name='"+nameOld+"';";
//        wxMessageBox(sql);

        sqlite3_prepare_v2(db,sql.c_str(),-1,&stmt,NULL);
        rc = sqlite3_step(stmt);
        if(rc!=SQLITE_DONE)
        {
            wxMessageBox(wxT("修改餐盘记录失败！"));
            sqlite3_close(db);
            exit(-3);
        }
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        UpdatePlate();
    }
}

void MyFrame::OnDeletePlate(wxCommandEvent& evt)
{
    sqlite3* db;
    sqlite3_stmt* stmt;
    wxString sql = "delete from plate where name='";
    wxString name;
    int rc = sqlite3_open_v2("MyDB.db",&db,SQLITE_OPEN_READWRITE,NULL);
    if(rc)
    {
        wxMessageBox("Can't open database.\n");
        sqlite3_close(db);
        exit(-2);
    }
    name = m_LCplate->GetItemText(m_index_plate,0);
    sql = sql + name + "';";
    sqlite3_prepare_v2(db,sql.c_str(),-1,&stmt,NULL);
    rc = sqlite3_step(stmt);
    if(rc!=SQLITE_DONE)
    {
        wxMessageBox(wxT("删除餐盘记录失败！"));
        sqlite3_close(db);
        exit(-3);
    }
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    UpdatePlate();
}

void MyFrame::OnViewFood(wxCommandEvent& evt)
{
    m_sBmpFileName = m_LCfood->GetItemText(m_index_food,1);
    UpdateShow(m_sBmpFileName);
//    m_LCresult->DeleteAllItems();
}

void MyFrame::OnAddFood(wxCommandEvent& evt)
{
    sqlite3* db;
    sqlite3_stmt* stmt;
    wxString sql = "insert into food (name,path,train_name,train_path,food_name,price) values('";
    wxString foodname,path,price,train,trainpath,name;

    FoodDialog* dialog = new FoodDialog(this,wxT("添加菜肴记录"));
    dialog->CenterOnScreen();
    if(dialog->ShowModal()==wxID_OK)
    {
        foodname = dialog->GetFoodName();
        path = dialog->GetPath();
        train = dialog->GetTrain();
        name = dialog->GetName();
        price = dialog->GetPrice();
        trainpath = dialog->GetTrainPath();

        int rc = sqlite3_open_v2("MyDB.db",&db,SQLITE_OPEN_READWRITE,NULL);
        if(rc)
        {
            wxMessageBox("Can't open database.\n");
            sqlite3_close(db);
            exit(-2);
        }
        sql = sql + name +"','"+path+"','"+train+"','"+
                trainpath + "','"+ foodname + "','"+price +"');";
//        wxMessageBox(sql);

        sqlite3_prepare_v2(db,sql.c_str(),-1,&stmt,NULL);
        rc = sqlite3_step(stmt);
        if(rc!=SQLITE_DONE)
        {
            wxMessageBox(wxT("添加菜肴记录失败！"));
            sqlite3_close(db);
            exit(-3);
        }
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        UpdateFood();
    }
}

void MyFrame::OnModifyFood(wxCommandEvent& evt)
{
    sqlite3* db;
    sqlite3_stmt* stmt;
    wxString sql = "select * from food where path='";
    wxString foodname,path,train,trainpath,name,price,
            foodnameOld,pathOld,trainOld,trainpathOld,
            nameOld,priceOld;

    pathOld = m_LCfood->GetItemText(m_index_food,1);
    sql = sql + pathOld + "';";

//    wxMessageBox(sql);

    FoodDialog* dialog = new FoodDialog(this,wxT("修改菜肴记录"));
    dialog->CenterOnScreen();
    int rc = sqlite3_open_v2("MyDB.db",&db,SQLITE_OPEN_READWRITE,NULL);
    if(rc)
    {
        wxMessageBox("Can't open database.\n");
        sqlite3_close(db);
        exit(-2);
    }
    sqlite3_prepare_v2(db,sql.c_str(),-1,&stmt,NULL);
    rc = sqlite3_step(stmt);

    if(rc == SQLITE_ROW)
    {
        nameOld = sqlite3_column_text(stmt,1);
        pathOld = sqlite3_column_text(stmt,2);
        trainOld= sqlite3_column_text(stmt,3);
        trainpathOld = sqlite3_column_text(stmt,4);
        foodnameOld  = sqlite3_column_text(stmt,5);
        priceOld = sqlite3_column_text(stmt,6);

        rc = sqlite3_step(stmt);
    }
    sqlite3_finalize(stmt);

    dialog->SetFoodName(foodnameOld);
    dialog->SetPath(pathOld);
    dialog->SetTrain(trainOld);
    dialog->SetTrainPath(trainpathOld);
    dialog->SetName(nameOld);
    dialog->SetPrice(priceOld);

    if(dialog->ShowModal()==wxID_OK)
    {
        foodname = dialog->GetFoodName();
        path = dialog->GetPath();
        train = dialog->GetTrain();
        name = dialog->GetName();
        price = dialog->GetPrice();
        trainpath = dialog->GetTrainPath();


        sql.Clear();

        sql = "update food set name = '"+name+"',path='"
            + path + "',train_name = '" + train + "',train_path = '"
            + trainpath + "', food_name = '"+foodname+"',price='"
            + price + "' where name = '" + nameOld + "';";
//        wxMessageBox(sql);

        sqlite3_prepare_v2(db,sql.c_str(),-1,&stmt,NULL);
        rc = sqlite3_step(stmt);
        if(rc!=SQLITE_DONE)
        {
            wxMessageBox(wxT("修改菜肴记录失败！"));
            sqlite3_close(db);
            exit(-3);
        }
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        UpdateFood();
    }
}

void MyFrame::OnDeleteFood(wxCommandEvent& evt)
{
    sqlite3* db;
    sqlite3_stmt* stmt;
    wxString sql = "delete from food where path='";
    wxString pathOld = m_LCfood->GetItemText(m_index_food,1);
    sql = sql + pathOld + "';";
    int rc = sqlite3_open_v2("MyDB.db",&db,SQLITE_OPEN_READWRITE,NULL);
    if(rc)
    {
        wxMessageBox("Can't open database.\n");
        sqlite3_close(db);
        exit(-2);
    }
    sqlite3_prepare_v2(db,sql.c_str(),-1,&stmt,NULL);
    rc = sqlite3_step(stmt);
    if(rc!=SQLITE_DONE)
    {
        wxMessageBox(wxT("删除菜肴记录失败！"));
        sqlite3_close(db);
        exit(-3);
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    UpdateFood();
}

void MyFrame::OnPlateRecognition(wxCommandEvent& evt)
{
    PlateDetection();
}

void MyFrame::PlateDetection()
{
    MyThread* thread = new MyThread(m_sBmpFileName);
    if( thread->Create() != wxTHREAD_NO_ERROR)
    {
        wxMessageBox(wxT("线程创建失败!"));
        return;
    }
    thread->Run();

//    wxMessageBox(wxT("餐盘检测需要较长时间，请稍等！"));

    if(m_mode)
        m_bplateDetection = false;
}
void MyFrame::PlateRecognition()
{
    Mat img_scene = imread(string(m_sBmpFileName.c_str()));
    Mat img_object= imread("./template/1.jpg");
    m_rc.clear();
    if(img_object.empty())
    {
        wxMessageBox(wxT("无法读取餐盘模板图像，请确认在当前路径下的template文件夹中有名为1.jpg的模板图像。"));
        return;
    }

    if(img_scene.empty())
    {
        wxMessageBox(wxT("无法读取餐盘图像，请确认当前查看的图像为餐盘图像。"));
        return;
    }

    SetStatusText(wxT("餐盘检测..."));
    //-- Step 1: Detect the keypoints using SURF Detector
  int minHessian = 400;

  SurfFeatureDetector detector( minHessian );

  std::vector<KeyPoint> keypoints_object, keypoints_scene;

  detector.detect( img_object, keypoints_object );
  detector.detect( img_scene, keypoints_scene );

  //-- Step 2: Calculate descriptors (feature vectors)
  SurfDescriptorExtractor extractor;

  Mat descriptors_object, descriptors_scene;

  extractor.compute( img_object, keypoints_object, descriptors_object );
  extractor.compute( img_scene, keypoints_scene, descriptors_scene );

  //-- Step 3: Matching descriptor vectors using FLANN matcher
  FlannBasedMatcher matcher;
  std::vector< DMatch > matches;
  matcher.match( descriptors_object, descriptors_scene, matches );

  double min_dist = 100;

  //-- Quick calculation of max and min distances between keypoints
  for( int i = 0; i < descriptors_object.rows; i++ )
  { double dist = matches[i].distance;
    if( dist < min_dist ) min_dist = dist;
  }

  //-- Draw only "good" matches (i.e. whose distance is less than 3*min_dist )
  std::vector< DMatch > good_matches;

  for( int i = 0; i < descriptors_object.rows; i++ )
  { if( matches[i].distance < 3*min_dist )
     { good_matches.push_back( matches[i]); }
  }

  //-- Localize the object
  std::vector<float> Vx;
  std::vector<float> Vy;
  float x1 = 0,x2 = 0,
        y1 = 0,y2 = 0;

  for( int i = 0; i < good_matches.size(); i++ )
  {
    //-- Get the keypoints from the good matches
    Vx.push_back( keypoints_scene[ good_matches[i].trainIdx ].pt.x );
    Vy.push_back( keypoints_scene[ good_matches[i].trainIdx ].pt.y );

  }

    std::sort(Vx.begin(),Vx.end(),std::greater<float>());
    std::sort(Vy.begin(),Vy.end(),std::greater<float>());


    for(int i = 0;i < Vx.size();i++)
    {
        if(Vx[i] - Vx[i+1] < 10 && Vx[i+1] - Vx[i+2] < 10
            && Vx[i+2] - Vx[i+3] < 10)
        {
            x2 = Vx[i];
            break;
        }
    }
    for(int i = Vx.size()-1;i>0;i--)
    {
        if(Vx[i-1] - Vx[i] < 7 && Vx[i-2] - Vx[i-1] < 7
           && Vx[i-3]-Vx[i-2] < 7)
        {
            x1 = Vx[i];
            break;
        }
    }
    for(int i = Vy.size()-1;i>0;i--)
    {
        if(Vy[i-1] - Vy[i] < 7 && Vy[i-2] - Vy[i-1] < 7
           && Vy[i-3]-Vy[i-2] < 7)
        {
            y1 = Vy[i];
            break;
        }
    }
    y2=Vy.front();
    Rect rc(x1,y1,x2-x1,y2-y1);

    imwrite("tmp1.png",img_scene(rc));
    UpdateShow("tmp1.png");
    SetStatusText(wxT("餐盘检测结束"));
    m_bAntiZero = true;
}

void MyFrame::OnAntiZeroThreshold(wxCommandEvent& evt)
{
    AntiZeroThreshold();
    m_bAntiZero = false;
    m_bCloseOperation1 = true;
}

void MyFrame::AntiZeroThreshold()
{
    Mat img = imread("tmp1.png");
    if(img.empty())
    {
        wxMessageBox(wxT("无法读取tmp1.png，请确认有先进行餐盘检测。"));
        return;
    }
    Mat src_gray;
    Mat dst;

    SetStatusText(wxT("反零阈值处理..."));
    /// 将图片转换成灰度图片
    cvtColor( img, src_gray, CV_RGB2GRAY );
    threshold( src_gray, dst, 144, 255,4 );
    cvtColor(dst,img,CV_GRAY2BGR);
    imwrite("tmp2.png",img);

    wxString h1;
    h1.Printf("%d",img.rows*400/img.cols);
    wxString html = wxT("<h1 align=\"center\">反零阈值处理</h1>"
                        "<table border=\"1\" align=\"center\">"
                        "<tr><td><h2>处理前</h2></td><td><h2>处理后</h2></td></tr>"
                        "<tr><td><img src=\"tmp1.png\" width=\"400\" height="+h1+" /></td>"
                        "<td><img src=\"tmp2.png\" width=\"400\" height="+h1+" /></td>"
                        "</tr>"
                        "</table>");
    UpdateShow2(html,"tmp2.png");
    SetStatusText(wxT("反零阈值处理结束"));
}

void MyFrame::OnCloseOperation1(wxCommandEvent& evt)
{
    CloseOperation1();
    m_bCloseOperation1 = false;
    m_bRiceRecognition = true;
}

void MyFrame::CloseOperation1()
{
    SetStatusText(wxT("闭运算处理1..."));
    Mat img = imread("tmp2.png");
    if(img.empty())
    {
        wxMessageBox(wxT("无法读取tmp2.png，请确认先进行反零阈值处理。"));
        SetStatusText(wxT("闭处理1失败"));
        return;
    }
    int s = 13;
    Mat element = getStructuringElement( 0, Size( 2*s + 1, 2*s+1 ), Point( s, s ) );

    /// 运行指定形态学操作
    morphologyEx( img, img, 3, element );
    imwrite("tmp3.png",img);

    wxString h1;
    h1.Printf("%d",img.rows*400/img.cols);
    wxString html = wxT("<h1 align=\"center\">闭运算处理1</h1>"
                        "<table border=\"1\" align=\"center\">"
                        "<tr><td><h2>处理前</h2></td><td><h2>处理后</h2></td></tr>"
                        "<tr><td><img src=\"tmp2.png\" width=\"400\" height="+h1+" /></td>"
                        "<td><img src=\"tmp3.png\" width=\"400\" height="+h1+" /></td>"
                        "</tr>"
                        "</table>");

    UpdateShow2(html,"tmp3.png");

    SetStatusText(wxT("闭运算处理1结束"));


}

void MyFrame::OnRiceRecognition(wxCommandEvent& evt)
{
    RiceRecognition();
    m_bRiceRecognition = false;
    m_bPlateBlacked = true;
}

void MyFrame::RiceRecognition()
{
    SetStatusText(wxT("白饭检测..."));

    Mat img = imread("tmp3.png");

    if(img.empty())
    {
        wxMessageBox(wxT("无法读取tmp3.png，请确认先进行闭运算。"));
        SetStatusText(wxT("白饭检测失败"));
        return;
    }

    Mat src_gray;

    /// Convert image to gray and blur it
    cvtColor( img, src_gray, CV_BGR2GRAY );
    blur( src_gray, src_gray, Size(3,3) );

    Mat canny_output;
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;

    /// Detect edges using canny
    Canny( src_gray, canny_output, 100, 100*2, 3 );
    /// Find contours
    findContours( canny_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

    for( int i = 0; i< contours.size(); i++ )
    {
        Rect r0= boundingRect(Mat(contours[i]));//boundingRect获取这个外接矩形
        if(r0.width*r0.height < 30000)
            continue;
        m_rc.push_back(r0);
        break;
    }
    if(m_rc.size()>0)
    {
//        rectangle(img,Rect(m_rc[0].x-3,m_rc[0].y-3,m_rc[0].width+6,m_rc[0].height+6),Scalar(255,0,0),3);
//        rectangle(img,m_rc[0],Scalar(0,255,0));
        imwrite("tmp4.png",img);
        int rows = img.rows;int cols = img.cols;
        img = imread("tmp1.png");
        imwrite("tmp4_1.png",img(m_rc[0]));
        for(int ii=0; ii<img.rows; ++ii)
            for(int jj=0; jj<img.cols; ++jj)
            {
                if(m_rc[0].y < ii && ii < m_rc[0].y+m_rc[0].height &&
                m_rc[0].x < jj && jj < m_rc[0].x+m_rc[0].width)
                {
                    img.at<Vec3b>(ii,jj)[0]=0;
                    img.at<Vec3b>(ii,jj)[1]=0;
                    img.at<Vec3b>(ii,jj)[2]=0;
                }
            }
        imwrite("tmp4_2.png",img);

        wxString h1;
        h1.Printf("%d",rows*400/cols);
        wxString w2,h2;
        w2.Printf("%d",m_rc[0].width*400/cols);
        h2.Printf("%d",m_rc[0].height*400/cols);
        wxString w3,h3;
        w3.Printf("%d",img.cols*400/cols);
        h3.Printf("%d",img.rows*400/cols);

        wxString html = wxT("<h1 align=\"center\">白饭检测</h1>"
                        "<table border=\"1\" align=\"center\">"
                        "<tr><td><h2>处理前</h2></td><td><h2>处理后</h2></td></tr>"
                        "<tr><td rowspan=\"2\"><img src=\"tmp3.png\" width=\"400\" height="+h1+" /></td>"
                        "<td align=\"center\"><img src=\"tmp4_1.png\" width="+w2+" height="+h2+" /></td>"
                        "</tr>"
                        "<tr><td align=\"center\"><img src=\"tmp4_2.png\" width="+w3+" height="+h3+" /></td></tr>"
                        "</table>");
        UpdateShow2(html,"tmp4.png");
    }

    SetStatusText(wxT("白饭检测结束"));


}

bool MyFrame::isSilver(int r, int g, int b)
{
    float y = 0.299*r + 0.587*g + 0.114*b;
    float cr = (r - y)*0.713 + 128;
    float cb = (b - y)*0.564 + 128;

    if(cr > 121 && cr < 148 &&
            cb > 94 && cb < 150)
        return true;
    return false;
}

void MyFrame::OnPlateBlacked(wxCommandEvent& evt)
{
    PlateBlacked();
    m_bPlateBlacked = false;
    m_bFoodWhited = true;
}

void MyFrame::PlateBlacked()
{
    SetStatusText(wxT("餐盘二值化处理..."));

    Mat img = imread("tmp1.png");
    if(img.empty())
    {
        wxMessageBox(wxT("无法读取tmp1.png，请确认先进行餐盘检测。"));
        SetStatusText(wxT("餐盘涂黑处理失败"));
        return;
    }

    for(int i = 0; i < img.rows; i++)
    for(int j = 0; j < img.cols; j++)
    {
        if(j > m_rc[0].x && j < m_rc[0].x+m_rc[0].width
           && i > m_rc[0].y && i < m_rc[0].y+m_rc[0].height)
        {
                img.at<Vec3b>(i,j)[0] = 0;
                img.at<Vec3b>(i,j)[1] = 0;
                img.at<Vec3b>(i,j)[2] = 0;
                continue;
        }
        int r = img.at<Vec3b>(i,j)[0];
        int g = img.at<Vec3b>(i,j)[1];
        int b = img.at<Vec3b>(i,j)[2];

        if( isSilver(r,g,b) )
        {
            img.at<Vec3b>(i,j)[0] = 0;
            img.at<Vec3b>(i,j)[1] = 0;
            img.at<Vec3b>(i,j)[2] = 0;
        }
    }

    imwrite("tmp5.png",img);

    wxString h1;
    h1.Printf("%d",img.rows*400/img.cols);
    wxString html = wxT("<h1 align=\"center\">餐盘二值化处理</h1>"
                        "<table border=\"1\" align=\"center\">"
                        "<tr><td><h2>处理前</h2></td><td><h2>处理后</h2></td></tr>"
                        "<tr><td><img src=\"tmp1.png\" width=\"400\" height="+h1+" /></td>"
                        "<td><img src=\"tmp5.png\" width=\"400\" height="+h1+" /></td>"
                        "</tr>"
                        "</table>");
    UpdateShow2(html,"tmp5.png");

    SetStatusText(wxT("餐盘二值化处理结束"));
}

void MyFrame::OnFoodWhited(wxCommandEvent& evt)
{
    FoodWhited();
    m_bFoodWhited = false;
    m_bCloseOperation2 = true;
}

void MyFrame::FoodWhited()
{
    SetStatusText(wxT("菜肴二值化处理..."));
    Mat src = imread("tmp5.png");
    if(src.empty())
    {
        SetStatusText(wxT("菜肴涂白失败"));
        wxMessageBox(wxT("无法读取tmp5.png，请确认先进行餐盘涂黑处理"));
        return;
    }

    for(int i=0;i<src.rows;++i)
        for(int j=0;j<src.cols;++j)
    {
        if(src.at<Vec3b>(i,j)[0] != 0 ||
           src.at<Vec3b>(i,j)[1] != 0 ||
           src.at<Vec3b>(i,j)[2] != 0)
        {
            src.at<Vec3b>(i,j)[0] = 255;
            src.at<Vec3b>(i,j)[1] = 255;
            src.at<Vec3b>(i,j)[2] = 255;
        }
    }

    imwrite("tmp6.png", src);

    wxString h1;
    h1.Printf("%d",src.rows*400/src.cols);
    wxString html = wxT("<h1 align=\"center\">菜肴二值化处理</h1>"
                        "<table border=\"1\" align=\"center\">"
                        "<tr><td><h2>处理前</h2></td><td><h2>处理后</h2></td></tr>"
                        "<tr><td><img src=\"tmp5.png\" width=\"400\" height="+h1+" /></td>"
                        "<td><img src=\"tmp6.png\" width=\"400\" height="+h1+" /></td>"
                        "</tr>"
                        "</table>");
    UpdateShow2(html,"tmp6.png");

    SetStatusText(wxT("菜肴二值化处理结束"));
}

void MyFrame::OnCloseOperation2(wxCommandEvent& evt)
{
    CloseOperation2();
    m_bCloseOperation2 = false;
    m_bFoodErode = true;
}

void MyFrame::CloseOperation2()
{
    SetStatusText(wxT("闭运算处理2..."));
    Mat img = imread("tmp6.png");
    if(img.empty())
    {
        wxMessageBox(wxT("无法读取tmp6.png，请确认先进行反零阈值处理。"));
        SetStatusText(wxT("闭处理2失败"));
        return;
    }
    int s = 4;
    Mat element = getStructuringElement( 0, Size( 2*s + 1, 2*s+1 ), Point( s, s ) );

    /// 运行指定形态学操作
    morphologyEx( img, img, 3, element );
    imwrite("tmp7.png",img);

    wxString h1;
    h1.Printf("%d",img.rows*400/img.cols);
    wxString html = wxT("<h1 align=\"center\">闭运算处理2</h1>"
                        "<table border=\"1\" align=\"center\">"
                        "<tr><td><h2>处理前</h2></td><td><h2>处理后</h2></td></tr>"
                        "<tr><td><img src=\"tmp6.png\" width=\"400\" height="+h1+" /></td>"
                        "<td><img src=\"tmp7.png\" width=\"400\" height="+h1+" /></td>"
                        "</tr>"
                        "</table>");
    UpdateShow2(html,"tmp7.png");

    SetStatusText(wxT("闭运算处理2结束"));
}

void MyFrame::OnFoodErode(wxCommandEvent& evt)
{
    FoodErode();
    m_bFoodErode = false;
    m_bFoodDetection = true;
}

void MyFrame::FoodErode()
{
    SetStatusText(wxT("腐蚀处理..."));

    Mat img = imread("tmp7.png");
    if(img.empty())
    {
        SetStatusText(wxT("腐蚀失败"));
        wxMessageBox(wxT("读取tmp7.png失败，请确认先进行闭运算2."));
        return;
    }

    Mat out;
    Mat element = getStructuringElement( MORPH_CROSS,
    Size( 2*21 + 1, 2*21+1 ),
    Point( 21,21 ) );

    /// 腐蚀操作
    erode( img, out, element );
    imwrite("tmp8.png",out);

    wxString h1;
    h1.Printf("%d",img.rows*400/img.cols);
    wxString html = wxT("<h1 align=\"center\">腐蚀处理</h1>"
                        "<table border=\"1\" align=\"center\">"
                        "<tr><td><h2>处理前</h2></td><td><h2>处理后</h2></td></tr>"
                        "<tr><td><img src=\"tmp7.png\" width=\"400\" height="+h1+" /></td>"
                        "<td><img src=\"tmp8.png\" width=\"400\" height="+h1+" /></td>"
                        "</tr>"
                        "</table>");
    UpdateShow2(html,"tmp8.png");
    SetStatusText(wxT("腐蚀结束"));
}

void MyFrame::OnFoodRecognition(wxCommandEvent& evt)
{
    FoodRecognition();
    m_bFoodDetection = false;
    m_bCount = true;
}

void MyFrame::FoodRecognition()
{
    SetStatusText(wxT("菜肴检测..."));

    Mat src = imread("tmp8.png");
    if(src.empty())
    {
        SetStatusText(wxT("菜肴检测失败"));
        wxMessageBox(wxT("读取tmp8.png失败，请确认先进行菜肴腐蚀。"));
        return;
    }
    Mat src_gray;

    /// 转化成灰度图像并进行平滑
    cvtColor( src, src_gray, CV_BGR2GRAY );
    blur( src_gray, src_gray, Size(3,3) );
    Mat threshold_output;
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;

    /// 使用Threshold检测边缘
    threshold( src_gray, threshold_output, 100, 255, THRESH_BINARY );
    /// 找到轮廓
    findContours( threshold_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
    vector<Rect> boundRect( contours.size() );
    vector<vector<Point> > contours_poly( contours.size() );
    for( int i = 0; i < contours.size(); i++ )
    {
        approxPolyDP( Mat(contours[i]), contours_poly[i], 3, true );
        boundRect[i] = boundingRect( Mat(contours_poly[i]) );

    }

    for( int i = 0; i< contours.size(); i++ )
    {
        if(boundRect[i].width*boundRect[i].height < 5000)
            continue;
        if(boundRect[i].width > 300)
        {
            m_rc.push_back(Rect(boundRect[i].x,
                              boundRect[i].y,
                              boundRect[i].width/2,
                              boundRect[i].height));
            m_rc.push_back(Rect(boundRect[i].x+boundRect[i].width/2,
                              boundRect[i].y,
                              boundRect[i].width/2,
                              boundRect[i].height));
            continue;
        }
        m_rc.push_back(boundRect[i]);
    }

    Mat img = imread("tmp1.png");
    if(img.empty())
    {
        SetStatusText(wxT("菜肴检测失败"));
        wxMessageBox(wxT("读取tmp1.png失败，请确认先进行餐盘检测。"));
        return;
    }


    for(int i=0; i<m_rc.size(); ++i)
    {
        rectangle(img,Rect(m_rc[i].x-3,m_rc[i].y-3,m_rc[i].width+6,m_rc[i].height+6),Scalar(255,0,0),3);
//        rectangle(img,m_rc[i],Scalar(0,255,0));
        switch(i)
        {
            case 0:imwrite("tmp10.png",img(m_rc[i]));break;
            case 1:imwrite("tmp11.png",img(m_rc[i]));break;
            case 2:imwrite("tmp12.png",img(m_rc[i]));break;
            case 3:imwrite("tmp13.png",img(m_rc[i]));break;
            default:break;
        }
    }

    imwrite("tmp9.png",img);

    wxString html(wxT("<div class=\"final\"><h1>"
                      "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"
                      "菜肴检测</h1>"
                  "<table border=\"1\" align=\"center\">"
                  "<tr><td><h2>处理前</h2></td><td><h2>处理后</h2></td></tr>"));
    wxString tmp;
    tmp.Printf("%d",img.rows*400/img.cols);
    switch(m_rc.size())
    {
        case 2:
            html = html + "<tr><td><img src=\"tmp1.png\" width=\"400\" height=\""
                + tmp+"\" /></td>"
                            "<td><img src=\"tmp11.png\" /></td></tr>";
            break;
        case 3:
            html = html + "<tr><td rowspan=\"2\"><img src=\"tmp1.png\" width=\"400\" height=\""
                +tmp +"\" /></td>"
                            "<td><img src=\"tmp11.png\" /></td></tr>"
                            "<tr><td><img src=\"tmp12.png\" /></td></tr>";
            break;
        case 4:
            html = html + "<tr><td rowspan=\"2\"><img src=\"tmp1.png\" width=\"400\" height=\""
                +tmp +"\" /></td>"
                            "<td><img src=\"tmp11.png\" /></td></tr>"
                            "<tr><td><img src=\"tmp12.png\" /></td></tr>"
                            "<tr><td><img src=\"tmp13.png\" /></td></tr>";
            break;
        default:
            break;
    }

    html = html + "</table></div>";
    UpdateShow2(html,"tmp9.png");

    SetStatusText(wxT("菜肴检测结束"));
}

Mat MyFrame::hist(Mat mImg)
{
    Mat mYCrCb;
    cvtColor(mImg,mYCrCb,CV_BGR2YCrCb);

    /// 分割成3个单通道图像 ( R, G 和 B )
    vector<Mat> rgb_planes;
    split( mYCrCb, rgb_planes );

    /// 设定bin数目
    int histSize = 256;

    /// 设定取值范围 ( R,G,B) )
    float range[] = { 0, 255 } ;
    const float* histRange = { range };

    bool uniform = true;
    bool accumulate = false;

    Mat r_hist, g_hist, b_hist;

/// 计算直方图:

    calcHist( &rgb_planes[1], 1, 0, Mat(), g_hist, 1, &histSize, &histRange, uniform, accumulate );
    calcHist( &rgb_planes[2], 1, 0, Mat(), b_hist, 1, &histSize, &histRange, uniform, accumulate );


    Mat mResult(2,256,CV_32FC1);
    int index=0;

    index=0;
    for(int i=0; i<256; ++i)
    {
        mResult.at<float>(0,index++) =
            g_hist.at<float>(i,0);
    }
    index=0;
    for(int i=0; i<256; ++i)
    {
        mResult.at<float>(1,index++) =
            b_hist.at<float>(i,0);
    }

    return mResult;
}

void MyFrame::OnPay(wxCommandEvent& evt)
{
    if(!m_mode)
    {
        PayThread* thread = new PayThread();
        if( thread->Create() != wxTHREAD_NO_ERROR)
        {
            wxMessageBox(wxT("线程创建失败!"));
            return;
        }
        thread->Run();
        return;
    }
    Pay();
}

void MyFrame::Pay()
{
    SetStatusText(wxT("计价..."));

    sqlite3* db;
    sqlite3_stmt* stmt;
    vector<string> xml,name,result_name,result_price;
    vector<double> price;
    double result = 0;

    int rc = sqlite3_open_v2("MyDB.db",&db,SQLITE_OPEN_READWRITE,NULL);
    if(rc)
    {
        wxMessageBox("In OnPay:Can't open database.\n");
        sqlite3_close(db);
        exit(-1);
    }
//    m_LCresult->DeleteAllItems();

    char* sql = "select train_path,food_name,price from food where train_path != '' order by train_path;";
    sqlite3_prepare_v2(db,sql,-1,&stmt,NULL);
    rc = sqlite3_step(stmt);

    while(rc == SQLITE_ROW)
    {
        xml.push_back(string((const char*)sqlite3_column_text(stmt,0)));
        name.push_back(string((const char*)sqlite3_column_text(stmt,1)));
        price.push_back(sqlite3_column_double(stmt,2));
        rc = sqlite3_step(stmt);
    }

//    wxMessageBox(name[0]+name[1]+name[2]+name[3]+name[4]+name[5]);

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    vector<Mat> trainMat;
    for(int k=0; k<m_rc.size(); ++k)
    {
        switch(k)
        {
        case 0:
            trainMat.push_back(imread("tmp10.png"));
            break;
        case 1:
            trainMat.push_back(imread("tmp11.png"));
            break;
        case 2:
            trainMat.push_back(imread("tmp12.png"));
            break;
        case 3:
            trainMat.push_back(imread("tmp13.png"));
            break;
        default:break;
        }
    }

    Mat mTest(1,2*256,CV_32FC1);
    int ii = 0,jj=1;
    wxString tmp;

    for(int i=0; i<trainMat.size(); ++i)
    {
        Mat img = hist(trainMat[i]);
        int index = 0;
        for(int n=0; n<img.rows; ++n)
            for(int m=0; m<img.cols; ++m)
        {
            mTest.at<float>(0,index++) = img.at<float>(n,m);
        }
        normalize(mTest,mTest,1,0,NORM_MINMAX);
        for(int j=0; j<xml.size(); ++j)
        {
            SVM svm;
            svm.load(xml[j].c_str());
            if(svm.predict(mTest) == 1)
            {
                result += price[j];
//                tmp.Printf("%d",jj++);
//                m_LCresult->InsertItem(ii,tmp);
//                m_LCresult->SetItem(ii,1,name[j]);
                tmp.Printf("%.1lf",price[j]);
//                m_LCresult->SetItem(ii++,2,tmp);
                result_name.push_back(name[j]);
                result_price.push_back(string(tmp.c_str()));
                break;
            }
        }
    }

//    m_LCresult->InsertItem(ii,wxT("总价"));
//    tmp.Printf("%lf", result);
//    m_LCresult->SetItem(ii,2,tmp);
    wxString sprice;
    sprice.Printf("%.1lf",result);

    tmp.Printf("%d",m_rc.size());
    wxString html(wxT("<h1 align=\"center\">总价:"+sprice+"</h1>"));
    html = html + wxT("<table border=\"1\">"
                      "<tr><td align=\"center\"><h2>餐盘图像</h2></td>"
                      "<td align=\"center\"><h2>菜肴图像</h2></td>"
                      "<td align=\"center\"><h2>菜肴名</h2></td>"
                      "<td align=\"center\"><h2>菜肴价格</h2></td></tr>"
                      "<tr><td rowspan=\"3\""
                      "><img src=\"tmp1.png\" width=\"400\" /></td>"
                      "<td><img src=\"tmp10.png\" /></td>"
                      "<td align=\"center\"><h2>")+result_name[0]+"</h2></td>"
                      "<td align=\"center\"><h2>"+result_price[0]+"</h2></td></tr>"
                      "<tr><td align=\"center\"><img src=\"tmp11.png\" /></td>"
                      "<td align=\"center\"><h2>"+result_name[1]+"</h2></td>"
                      "<td align=\"center\"><h2>"+result_price[1]+"</h2></td></tr>"
                      "<tr><td align=\"center\"><img src=\"tmp12.png\" /></td>"
                      "<td align=\"center\"><h2>"+result_name[2]+"</h2></td>"
                      "<td align=\"center\"><h2>"+result_price[2]+"</h2></td></tr>"
                      "</table>";
    UpdateShow2(html,"tmp9.png");
    SetStatusText(wxT("计价结束"));
}

void MyFrame::OnManual(wxCommandEvent& evt)
{
    InitEditMenu();
    m_mode = true;

}

void MyFrame::OnAuto(wxCommandEvent& evt)
{
//    InitEditMenu();
//    m_bplateDetection = false;
//    m_bCount = true;
    m_mode = false;

}

void MyFrame::OnPlateRecognitionUI(wxUpdateUIEvent& evt)
{
    if(m_mode && m_bplateDetection)
       evt.Enable(true);
    else
        evt.Enable(false);
}
void MyFrame::OnAntiZeroThresholdUI(wxUpdateUIEvent& evt)
{
    if(m_mode && m_bAntiZero)
        evt.Enable(true);
    else
        evt.Enable(false);
}
void MyFrame::OnCloseOperation1UI(wxUpdateUIEvent& evt)
{
    if(m_mode && m_bCloseOperation1)
        evt.Enable(true);
    else
        evt.Enable(false);
}
void MyFrame::OnRiceRecognitionUI(wxUpdateUIEvent& evt)
{
    if(m_mode && m_bRiceRecognition)
        evt.Enable(true);
    else
        evt.Enable(false);
}
void MyFrame::OnPlateBlackedUI(wxUpdateUIEvent& evt)
{
    if(m_mode && m_bPlateBlacked)
        evt.Enable(true);
    else
        evt.Enable(false);
}
void MyFrame::OnFoodWhitedUI(wxUpdateUIEvent& evt)
{
    if(m_mode && m_bFoodWhited)
        evt.Enable(true);
    else
        evt.Enable(false);
}
void MyFrame::OnCloseOperation2UI(wxUpdateUIEvent& evt)
{
    if(m_mode && m_bCloseOperation2)
        evt.Enable(true);
    else
        evt.Enable(false);
}
void MyFrame::OnFoodErodeUI(wxUpdateUIEvent& evt)
{
    if(m_mode && m_bFoodErode)
        evt.Enable(true);
    else
        evt.Enable(false);
}
void MyFrame::OnFoodRecognitionUI(wxUpdateUIEvent& evt)
{
    if(m_mode && m_bFoodDetection)
        evt.Enable(true);
    else
        evt.Enable(false);
}
void MyFrame::OnFoodPayUI(wxUpdateUIEvent& evt)
{
    if(!m_mode)
        evt.Enable(true);
    else
    {
        if(m_bCount)
            evt.Enable(true);
        else
            evt.Enable(false);
    }

}
void MyFrame::OnAutoUI(wxUpdateUIEvent& evt)
{
    if(!m_mode)
    {
        evt.Check(true);
    }
    else
        evt.Check(false);
}
void MyFrame::OnManualUI(wxUpdateUIEvent& evt)
{
    if(m_mode)
    {
        evt.Check(true);
    }
    else
        evt.Check(false);
}
void MyFrame::OnOpenFile(wxCommandEvent& evt)
{
    wxString caption = wxT("选择餐盘图像文件");
    wxString filter = wxT("JPEG file(*.jpg)|*.jpg|PNG file(*.png)|*.png|BMP file(*.bmp)|*.bmp");
    wxString DefaultDir = wxEmptyString;
    wxString DefaultName = wxEmptyString;
    wxFileDialog dialog(this,caption,DefaultDir,DefaultName,
                        filter,wxFD_OPEN);
    dialog.CenterOnScreen();
    if(dialog.ShowModal() == wxID_OK)
    {
//        wxMessageBox("Before updateshow!");
        UpdateShow(dialog.GetPath());
//        wxMessageBox("After updateshow!");
    }
    InitEditMenu();
//    m_LCresult->DeleteAllItems();
}

void MyFrame::InitEditMenu()
{
    m_bAntiZero = false;
    m_bCloseOperation1 = false;
    m_bCloseOperation2 = false;
    m_bCount = false;
    m_bFoodDetection = false;
    m_bFoodErode = false;
    m_bFoodWhited = false;
    m_bPlateBlacked = false;
    m_bplateDetection = true;
    m_bRiceRecognition = false;
}

BEGIN_EVENT_TABLE(MyFrame, wxFrame)
    EVT_MENU(wxID_EXIT, MyFrame::OnExit)
    EVT_MENU(wxID_ABOUT, MyFrame::OnAbout)
    EVT_MENU(ID_VIEW_PLATE, MyFrame::OnViewPlate)
    EVT_MENU(ID_ADD_PLATE, MyFrame::OnAddPlate)
    EVT_MENU(ID_MODIFY_PLATE, MyFrame::OnModifyPlate)
    EVT_MENU(ID_DELETE_PLATE, MyFrame::OnDeletePlate)
    EVT_MENU(ID_VIEW_FOOD, MyFrame::OnViewFood)
    EVT_MENU(ID_ADD_FOOD, MyFrame::OnAddFood)
    EVT_MENU(ID_MODIFY_FOOD, MyFrame::OnModifyFood)
    EVT_MENU(ID_DELETE_FOOD, MyFrame::OnDeleteFood)
    EVT_LIST_ITEM_RIGHT_CLICK(ID_PLATE, MyFrame::OnRCPlate)
    EVT_LIST_ITEM_RIGHT_CLICK(ID_FOOD, MyFrame::OnRCFood)
    EVT_MENU(ID_PLATE_RECOGNITION, MyFrame::OnPlateRecognition)
    EVT_MENU(ID_ANTIZERO_THRESHOLD, MyFrame::OnAntiZeroThreshold)
    EVT_MENU(ID_CLOSE_OPERATION1, MyFrame::OnCloseOperation1)
    EVT_MENU(ID_RICE_RECOGNITION, MyFrame::OnRiceRecognition)
    EVT_MENU(ID_PLATE_BLACKED, MyFrame::OnPlateBlacked)
    EVT_MENU(ID_FOOD_WHITED, MyFrame::OnFoodWhited)
    EVT_MENU(ID_CLOSE_OPERATION2, MyFrame::OnCloseOperation2)
    EVT_MENU(ID_FOOD_ERODE, MyFrame::OnFoodErode)
    EVT_MENU(ID_FOOD_RECOGNITION, MyFrame::OnFoodRecognition)
    EVT_MENU(ID_PAY, MyFrame::OnPay)
    EVT_MENU(ID_MANUAL_MODE, MyFrame::OnManual)
    EVT_MENU(ID_AUTO_MODE, MyFrame::OnAuto)
    EVT_UPDATE_UI(ID_PLATE_RECOGNITION, MyFrame::OnPlateRecognitionUI)
    EVT_UPDATE_UI(ID_ANTIZERO_THRESHOLD, MyFrame::OnAntiZeroThresholdUI)
    EVT_UPDATE_UI(ID_CLOSE_OPERATION1, MyFrame::OnCloseOperation1UI)
    EVT_UPDATE_UI(ID_RICE_RECOGNITION, MyFrame::OnRiceRecognitionUI)
    EVT_UPDATE_UI(ID_PLATE_BLACKED, MyFrame::OnPlateBlackedUI)
    EVT_UPDATE_UI(ID_FOOD_WHITED, MyFrame::OnFoodWhitedUI)
    EVT_UPDATE_UI(ID_CLOSE_OPERATION2, MyFrame::OnCloseOperation2UI)
    EVT_UPDATE_UI(ID_FOOD_ERODE, MyFrame::OnFoodErodeUI)
    EVT_UPDATE_UI(ID_FOOD_RECOGNITION, MyFrame::OnFoodRecognitionUI)
    EVT_UPDATE_UI(ID_PAY,MyFrame::OnFoodPayUI)
    EVT_UPDATE_UI(ID_AUTO_MODE,MyFrame::OnAutoUI)
    EVT_UPDATE_UI(ID_MANUAL_MODE,MyFrame::OnManualUI)
    EVT_MENU(ID_FILE_OPEN, MyFrame::OnOpenFile)
END_EVENT_TABLE()
