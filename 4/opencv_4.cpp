#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;

int main() {
    std::system("chcp 65001 > nul"); // Установка кодировки UTF-8 в консоли Windows

    // Путь к изображению
    std::string image_path = "C:/Users/isher/Downloads/cmake_asadagar.jpg";

    // Загрузка изображения
    Mat img = imread(image_path, IMREAD_COLOR);
    if (img.empty()) {
        std::cout << "Ошибка при загрузке изображения!" << std::endl;
        return -1;
    }

    Mat img2 = img.clone();

    

    Mat hsvImage;
    cvtColor(img, hsvImage, COLOR_BGR2HSV);

    //red
    cv::Mat mask1, mask2, mask;
    cv::inRange(hsvImage, cv::Scalar(0, 100, 100), cv::Scalar(10, 255, 255), mask1);
    cv::inRange(hsvImage, cv::Scalar(160, 100, 100), cv::Scalar(180, 255, 255), mask2);
    cv::bitwise_or(mask1, mask2, mask);


    cv::Mat img3;
    cv::bitwise_and(img2, img2, img3, mask);
    
    //green
    cv::Mat mask10, mask20, mask0;
    cv::inRange(hsvImage, cv::Scalar(35, 50, 50), cv::Scalar(85, 255, 255), mask10); 
    cv::inRange(hsvImage, cv::Scalar(35, 50, 50), cv::Scalar(85, 255, 255), mask20); 
    cv::bitwise_or(mask10, mask20, mask0); 

    
    cv::Mat img4;
    cv::bitwise_and(img2, img2, img4, mask0);

    imshow("original1", img2);
    imshow("mask (red)", mask);
    imshow("final (red)", img3);
    imshow("mask (green)", mask0);
    imshow("final (green)", img4);

    waitKey(0);
    destroyAllWindows();

    return 0;
}
