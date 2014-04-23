#include "MyThread.h"
#include "app.h"
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/nonfree/features2d.hpp"
#include "opencv2/opencv.hpp"
#include <vector>
#include "wx/msgdlg.h"
#include "frame.h"

using namespace cv;

void* MyThread::Entry()
{
    Mat img_scene = imread(string(m_name.c_str()));
    Mat img_object= imread("./template/1.jpg");
    wxGetApp().frame->ClearRC();
    if(img_object.empty())
    {
        wxMessageBox(wxT("无法读取餐盘模板图像，请确认在当前路径下的template文件夹中有名为1.jpg的模板图像。"));
        return NULL;
    }

    if(img_scene.empty())
    {
        wxMessageBox(wxT("无法读取餐盘图像，请确认当前查看的图像为餐盘图像。"));
        return NULL;
    }

    wxGetApp().frame->SetStatusText(wxT("餐盘检测..."));
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
    {
        double dist = matches[i].distance;
        if( dist < min_dist ) min_dist = dist;
    }

    //-- Draw only "good" matches (i.e. whose distance is less than 3*min_dist )
    std::vector< DMatch > good_matches;

    for( int i = 0; i < descriptors_object.rows; i++ )
    {
        if( matches[i].distance < 3*min_dist )
        {
            good_matches.push_back( matches[i]);
        }
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


    for(int i = 0; i < Vx.size(); i++)
    {
        if(Vx[i] - Vx[i+1] < 10 && Vx[i+1] - Vx[i+2] < 10
                && Vx[i+2] - Vx[i+3] < 10)
        {
            x2 = Vx[i];
            break;
        }
    }
    for(int i = Vx.size()-1; i>0; i--)
    {
        if(Vx[i-1] - Vx[i] < 7 && Vx[i-2] - Vx[i-1] < 7
                && Vx[i-3]-Vx[i-2] < 7)
        {
            x1 = Vx[i];
            break;
        }
    }
    for(int i = Vy.size()-1; i>0; i--)
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
    if(rc.area() < 10000)
    {
        wxMessageBox(wxT("该图不是餐盘图像。"));
        wxGetApp().frame->SetStatusText(wxT("餐盘检测结束"));
        return NULL;
    }
    imwrite("tmp1.png",img_scene(rc));
    for(int ii=0; ii<img_scene.rows; ++ii)
        for(int jj=0; jj<img_scene.cols; ++jj)
    {
        if(rc.y < ii && ii < rc.y+rc.height &&
           rc.x < jj && jj < rc.x+rc.width)
        {
            img_scene.at<Vec3b>(ii,jj)[0]=0;
            img_scene.at<Vec3b>(ii,jj)[1]=0;
            img_scene.at<Vec3b>(ii,jj)[2]=0;
        }
    }
    imwrite("tmp1_1.png",img_scene);
    wxString w1;w1.Printf("%d",400);
    wxString h1;h1.Printf("%d",img_scene.rows*400/img_scene.cols);
    wxString w2;w2.Printf("%d",rc.width*400/img_scene.cols);
    wxString h2;h2.Printf("%d",rc.height*400/img_scene.cols);

    wxString html=wxT("<h1 align=\"center\">餐盘检测</h1><br />"
                      "<table border=\"1\" align=\"center\"><tr><th>处理前</th><th>处理后</th></tr>"
                      "<tr><td rowspan=\"2\"><img src=\""+m_name+"\" width="+w1+" height="+h1+" /></td>"
                      "<td align=\"center\"><img src=\"tmp1.png\" width="+w2+" height=" +h2+" /></td></tr>"
                      "<tr><td><img src=\"tmp1_1.png\" width="+w1+" height="+h1+" /></td></tr></table>");
    wxGetApp().frame->UpdateShow2(html,"tmp1.png");
    wxGetApp().frame->SetStatusText(wxT("餐盘检测结束"));
    wxGetApp().frame->SetAntiZero(true);
    return NULL;
}

void* PayThread::Entry()
{
    wxGetApp().frame->PlateRecognition();
    wxGetApp().frame->AntiZeroThreshold();
    wxGetApp().frame->CloseOperation1();
    wxGetApp().frame->RiceRecognition();
    wxGetApp().frame->PlateBlacked();
    wxGetApp().frame->FoodWhited();
    wxGetApp().frame->CloseOperation2();
    wxGetApp().frame->FoodErode();
    wxGetApp().frame->FoodRecognition();
    wxGetApp().frame->Pay();
}
