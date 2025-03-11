#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;


string detectShape(const vector<Point>& contour) {
    double epsilon = 0.02 * arcLength(contour, true);
    vector<Point> approx;
    approxPolyDP(contour, approx, epsilon, true);

    if (approx.size() == 3) {
        return "Triangle";
    }
    else if (approx.size() == 4) {
        Rect rect = boundingRect(approx);
        double aspectRatio = static_cast<double>(rect.width) / rect.height;
        return (aspectRatio >= 0.9 && aspectRatio <= 1.1) ? "Square" : "Rectangle";
    }
    else if (approx.size() == 5) {
        return "Pentagon";
    }
    else if (approx.size() == 6) {
        return "Hexagon";
    }
    else {
        return "Circle";
    }
}

int main() {
    system("chcp 65001 > nul");

    string video_path = "C:/Users/isher/Downloads/video.mp4";
    VideoCapture cap(video_path);
    if (!cap.isOpened()) {
        cout << "Ошибка при открытии видео!" << endl;
        return -1;
    }

    Mat frame;
    while (cap.read(frame)) {
        Mat gray, blurred, edges;
        cvtColor(frame, gray, COLOR_BGR2GRAY);
        GaussianBlur(gray, blurred, Size(5, 5), 1.5);
        Canny(blurred, edges, 50, 150);

        vector<vector<Point>> contours;
        vector<Vec4i> hierarchy;
        findContours(edges, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);

        Mat contourOutput = frame.clone();
        for (size_t i = 0; i < contours.size(); i++) {
            if (hierarchy[i][2] == -1) continue; 
            if (contourArea(contours[i]) < 500) continue; 

            string shape = detectShape(contours[i]);
            drawContours(contourOutput, contours, (int)i, Scalar(0, 255, 0), 2);

            Moments M = moments(contours[i]);
            if (M.m00 != 0) {
                int cx = static_cast<int>(M.m10 / M.m00);
                int cy = static_cast<int>(M.m01 / M.m00);
                putText(contourOutput, shape, Point(cx - 25, cy), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(200, 100, 0), 2);
            }
        }

        imshow("Video Tracking", contourOutput);
        if (waitKey(30) >= 0) break;
    }

    cap.release();
    destroyAllWindows();
    return 0;
}
