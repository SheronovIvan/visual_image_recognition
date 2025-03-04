#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;


std::string detectShape(const std::vector<cv::Point>& contour) {
    double epsilon = 0.01 * cv::arcLength(contour, true);
    std::vector<cv::Point> approx;
    cv::approxPolyDP(contour, approx, epsilon, true);

    if (approx.size() == 3) {
        return "Triangle";
    }
    else if (approx.size() == 4) {
        cv::Rect rect = cv::boundingRect(approx);
        double aspectRatio = static_cast<double>(rect.width) / rect.height;
        if (aspectRatio >= 0.9 && aspectRatio <= 1.1) {
            return "Square";
        }
        else {
            return "Rectangle";
        }
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
    std::system("chcp 65001 > nul"); 

    
    std::string image_path = "C:/Users/isher/Downloads/image.png";

    
    Mat img = imread(image_path, IMREAD_COLOR);
    if (img.empty()) {
        std::cout << "Ошибка при загрузке изображения!" << std::endl;
        return -1;
    }

    cv::Mat gray;
    cv::cvtColor(img, gray, cv::COLOR_BGR2GRAY);

   
    cv::Mat blurred;
    cv::GaussianBlur(gray, blurred, cv::Size(9, 9), 1.5);

   
    cv::Mat edges;
    cv::Canny(blurred, edges, 10, 50);

    
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(edges, contours, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);

    
    cv::Mat contourOutput = cv::Mat::zeros(img.size(), img.type());
    for (size_t i = 0; i < contours.size(); i++) {
        
        std::string shape = detectShape(contours[i]);

        
        cv::drawContours(contourOutput, contours, -1, cv::Scalar(0, 255, 0), 2);

        
        cv::Moments M = cv::moments(contours[i]);
        if (M.m00 != 0) {
            int cx = static_cast<int>(M.m10 / M.m00);
            int cy = static_cast<int>(M.m01 / M.m00);

            
            cv::putText(contourOutput, shape, cv::Point(cx - 25, cy), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(200, 100, 0), 2);
        }
    }

    cv::imshow("Contour shape", contourOutput);
    cv::waitKey(0);

    return 0;
}