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
    Mat big_map2 = cv::imread( "../map_control/big_floor_test.png", IMREAD_COLOR );
    Mat big_map3 = cv::imread( "../map_control/big_floor_test2.png", IMREAD_COLOR );
    Mat small_map = cv::imread( "../map_control/floor_plan.png", IMREAD_COLOR );

    Mat src = big_map.clone();
    print_map( src, "Source" );

    Map *m = new Map();
    Mat imgBrushfire = m->brushfire_img(src);

    threshold( imgBrushfire, imgBrushfire, 6, 255, THRESH_BINARY );

    print_map( imgBrushfire, "Fields" );

    waitKey(0);
    return 0;
}
