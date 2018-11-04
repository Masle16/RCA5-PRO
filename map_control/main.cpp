#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/ximgproc.hpp"

#include <iostream>
#include "Map.h"
#include "path_planning.h"

using namespace std;
using namespace cv;

Point point_before_obstacle;
int pixel_before_obstacle;

struct obstacle{
    bool up = false;
    bool left = false;
    bool right = false;
    bool down = false;
};

void printMap(Mat &map) {
    Mat resizeMap;
    resize(map,resizeMap,map.size()*10,0,0,INTER_NEAREST);
    imshow("Map", resizeMap);
}

void draw_pixel_red(vector<Point> &v, Mat &img) {
    for (size_t i = 0; i < v.size(); i++) {
        Vec3b color(0, 0, 255);
        img.at<Vec3b>(v[i].y, v[i].x) = color;
    }
}

void draw_pixel(Point p, Mat &img, Vec3b color) {
    img.at<Vec3b>(p) = color;
}

void binarize_img(Mat &img) {
    Mat gray;
    cvtColor(img, gray, CV_RGB2GRAY);
    threshold(gray, img, 128.0, 255.0, THRESH_BINARY);
    img = img > 128;
}

vector<Point> get_points(LineIterator &it) {
    vector<Point> v(it.count);
    for (int i = 0; i < it.count; i++, it++) {
        v[i] = it.pos();
    }
    return v;
}

Point get_p_before_obstacle(vector<Point> &v, Mat &img) {
    Point p;
    for (size_t i = 0; i < v.size(); i++) {
        if ( (int)img.at<uchar>(v[i]) == 0 ) {
            p = v[i-1];
            break;
        }
    }
    return p;
}

bool obstacle_detected(Point start, Point goal, Mat &img) {
    LineIterator it(img, start, goal, 8);
    vector<Point> v = get_points(it);
    for (size_t i = 0; i < v.size(); i++) {
        if ( (int)img.at<uchar>(v[i])==0 ) {
            cout << v[i] << endl;
            return true;
        }
    }
    return false;
}

void go_left(Point &p, Mat &img) {
    Point left(p.x-1, p.y);
    Point left_up(p.x-1, p.y-1);
    Point left_down(p.x-1, p.y+1);
    Point right(p.x+1, p.y);
    Point right_up(p.x+1, p.y-1);
    Point right_down(p.x+1, p.y+1);
    Point up(p.x, p.y-1);
    Point down(p.x, p.y+1);

    int left_val = (int)img.at<uchar>(left);
    int right_val = (int)img.at<uchar>(right);
    int up_val = (int)img.at<uchar>(up);
    int down_val = (int)img.at<uchar>(down);
    int right_down_val = (int)img.at<uchar>(right_down);
    int left_down_val = (int)img.at<uchar>(left_down);
    int right_up_val = (int)img.at<uchar>(right_up);
    int left_up_val = (int)img.at<uchar>(left_up);

    if ( left_up_val==0 && left_val==0 && up_val==0){
        /*****
         * ##
         * #X
         ****/
        p = down;
    }
    else if ( left_down_val==0 && left_val==0 && down_val==0 ) {
        /*****
         * #X
         * ##
         ****/
        p = right;
    }
    else if ( right_up_val==0 && right_val==0 && up_val==0) {
        /*****
         * ##
         * X#
         ****/
        p = left;
    }
    else if ( right_down_val==0 && right_val==0 && down_val==0) {
        /*****
         * X#
         * ##
         ****/
        p = up;
    }
    else if ( left_val==0 ) {
        /*****
         * #
         * #X
         * #
         ****/
        p = down;
    }
    else if ( up_val==0 ) {
        /*****
         * ###
         *  X
         ****/
        p = left;
    }
    else if ( right_val==0 ) {

        /*****
         *  #
         * X#
         *  #
         ****/
        p = up;
    }
    else if ( down_val==0 ) {
        /*****
         *  X
         * ###
         ****/
        p = right;
    }
    else if ( left_up_val==0 ) {
        /*****
         * #
         *  X
         ****/
        p = left;
    }
    else if ( left_down_val==0) {
        /*****
         *  X
         * #
         ****/
        p = down;
    }
    else if ( right_up_val==0 ) {
        /*****
         *  #
         * X
         ****/
        p = up;
    }
    else if ( right_down_val==0 ) {
        /*****
         * X
         *  #
         ****/
        p = right;
    }
}

void go_right(Point &p, Mat &img) {
    Point left(p.x-1, p.y);
    Point left_up(p.x-1, p.y-1);
    Point left_down(p.x-1, p.y+1);
    Point right(p.x+1, p.y);
    Point right_up(p.x+1, p.y-1);
    Point right_down(p.x+1, p.y+1);
    Point up(p.x, p.y-1);
    Point down(p.x, p.y+1);

    int left_val = (int)img.at<uchar>(left);
    int right_val = (int)img.at<uchar>(right);
    int up_val = (int)img.at<uchar>(up);
    int down_val = (int)img.at<uchar>(down);
    int right_down_val = (int)img.at<uchar>(right_down);
    int left_down_val = (int)img.at<uchar>(left_down);
    int right_up_val = (int)img.at<uchar>(right_up);
    int left_up_val = (int)img.at<uchar>(left_up);

    if ( left_up_val==0 && left_val==0 && up_val==0){
        /*****
         * ##
         * #X
         *****/
        p = right;
    }
    else if ( left_down_val==0 && left_val==0 && down_val==0 ) {
        /******
         * #X
         * ##
         * ***/
        p = up;
    }
    else if ( right_up_val==0 && right_val==0 && up_val==0) {
        /*****
         * ##
         * X#
         *****/
        p = down;
    }
    else if ( right_down_val==0 && right_val==0 && down_val==0) {
        /******
         * X#
         * ##
         *****/
        p = left;
    }
    else if ( left_val==0 ) {
        /*****
         * #
         * #X
         * #
         *****/
        p = up;
    }
    else if ( up_val==0 ) {
        /******
         * ###
         *  X
         *****/
        p = right;
    }
    else if ( right_val==0 ) {
        /****
         *  #
         * X#
         *  #
         ****/
        p = down;
    }
    else if ( down_val==0 ) {
        /****
         *  X
         * ###
         ****/
        p = left;
    }
    else if ( left_up_val==0 ) {
        /****
         * #
         *  X
         ****/
        p = up;
    }
    else if ( left_down_val==0) {
        /****
         *  X
         * #
         ****/
        p = left;
    }
    else if ( right_up_val==0 ) {
        /****
         *  #
         * X
         ****/
        p = right;
    }
    else if ( right_down_val==0 ) {
        /****
         * X
         *  #
         ****/
        p = down;
    }
}

int main(int argc, char** argv)
{
    const char* default_file = "../map_control/floor_plan.png";
//    const char* default_file = "../map_control/big_floor_plan.png";
    const char* filename = argc >=2 ? argv[1] : default_file;
    Mat src = cv::imread(filename, IMREAD_COLOR);

<<<<<<< HEAD
    Map grid;
    Mat img = grid.bushfire_img(src);
    vector<Point> mid_points = grid.find_centers(img);

    draw_pixel_red(mid_points, src);
    printMap(src);
    // Loads an image
    Mat smallWorld = imread( filename, IMREAD_GRAYSCALE );
    Map smallMap(smallWorld);

    vector<Point> detectedCorners = smallMap.cornerDetection();
    smallMap.trapezoidalLines(detectedCorners);

    smallMap.printMap();
    smallMap.drawNShowPoints("Detected Corners", detectedCorners);
    /* SHOWS UPPER AND LOWER SEPARATELY
    smallMap.drawNShowPoints("Upper Goals", smallMap.getUpperTrapezoidalGoals());
    smallMap.drawNShowPoints("Lower Goals", smallMap.getLowerTrapezoidalGoals());
    */
    //SHOWS UPPER AND LOWER TOGETHER
    Mat tempMap = smallWorld;
    cvtColor(smallWorld, tempMap, COLOR_GRAY2BGR);
    vector<Point> upper = smallMap.getUpperTrapezoidalGoals();
    vector<Point> lower = smallMap.getLowerTrapezoidalGoals();

    // DRAWS IN SAME MAP

    for(int i = 0; i < upper.size(); i++) // Upper goal green
    {
        cout << "Upper x: " << upper[i].x << " y: " << upper[i].y << endl;
        tempMap.at<Vec3b>(upper[i].y,upper[i].x)[1] = 255;
    }
    for(int i = 0; i < lower.size(); i++)
    {
        cout << "Lower x: " << lower[i].x << " y: " << lower[i].y << endl;
        tempMap.at<Vec3b>(lower[i].y,lower[i].x)[2] = 255; // Lower goal red
    }

    // COUT COORDINATES FROM PIXELS TO GAZEBO
    vector<Point_<double>> gazeboGoals = smallMap.convertToGazeboCoordinatesTrapezoidal(upper, lower);
    for(int i = 0; i < gazeboGoals.size(); i++)
    {
        cout << "Goal " << i << " x: " << gazeboGoals[i].x << " y: " << gazeboGoals[i].y << endl;
    }


    resize(tempMap,tempMap,tempMap.size()*10,0,0,INTER_NEAREST);
    imshow("Goals", tempMap);
=======
    Path_planning path;
    Point start(2,2), goal(16,2);
    cout << path.way_around_obstacle(start, goal, src) << endl;
>>>>>>> way_around_obstacle

    waitKey(0);
    return 0;
}
