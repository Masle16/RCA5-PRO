#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/ximgproc.hpp"

#include <iostream>
#include <fstream>
#include <vector>
#include <Map.h>
#include <Cell.h>
#include <Cellpoint.h>
#include <vector>

#include "Map.h"
#include "path_planning.h"
#include "Voronoi_Diagram.h"
#include "A_Star.h"

using namespace std;
using namespace cv;

void print_map(Mat &map, string s) {
    Mat resizeMap;
    resize( map, resizeMap, map.size()*10, 0, 0, INTER_NEAREST);
    imshow(s, resizeMap);
}

void draw_pixel_red(vector<Point> &v, Mat &img) {
    for (size_t i = 0; i < v.size(); i++) {
        Vec3b color(0, 0, 255);
        img.at<Vec3b>(v[i].y, v[i].x) = color;
    }
}

Vec3b red(0,0,255), black(0,0,0), white(255,255,255), blue(255,0,0);

int main( ) {
    Mat big_map = cv::imread( "../map_control/big_floor_plan.png", IMREAD_COLOR);
    Mat small_map = cv::imread( "../map_control/floor_plan.png", IMREAD_COLOR );

    Mat src = big_map.clone(), dst;
    Voronoi_Diagram *v_d = new Voronoi_Diagram();
    v_d->get_voronoi_img( src, dst );
    std::vector<Point> points;
    for (int y = 0; y < dst.rows; y++)
        for (int x = 0; x < dst.cols; x++)
            if ( (int)dst.at<uchar>(y,x) == 255 )
                points.push_back( Point(x,y) );
    for ( auto& p : points )
        src.at<Vec3b>( p ) = red;
//    print_map( src, "Voronoi Diagram" );

    Mat img = big_map.clone();
    Path_planning *path = new Path_planning();
    Mat dst1 = path->make_visibility_map( img, points );
//    print_map( dst1, "Visibility Voronoi" );

    Mat src1 = big_map.clone();
    cvtColor( src1, src1, CV_BGR2GRAY );
    Map map(src1);
    map.trapezoidalLines( map.cornerDetection() );
    Mat img_beo = map.drawCellsPath( "Beosthrupdon", map.calculateCells( map.getUpperTrapezoidalGoals(), map.getLowerTrapezoidalGoals() ) );
    points.clear();
    for (int y = 0; y < img_beo.rows; y++)
        for (int x = 0; x < img_beo.cols; x++)
            if ( img_beo.at<Vec3b>(y,x) == Vec3b(0,0,255) )
                points.push_back( Point(x,y) );
    Mat src2 = big_map.clone();
    for ( auto& p : points )
        src2.at<Vec3b>( p ) = red;
//    print_map( src2, "Beosthruphdon" );

    Mat img1 = big_map.clone();
    Mat dst2 = path->make_visibility_map( img1, points );
//    print_map( dst2, "Visibility Beosthruphdon" );

    waitKey(0);
    return 0;
}
