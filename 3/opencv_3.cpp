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

    // 1. Преобразование в оттенки серого
    cv::Mat gray;
    cv::cvtColor(img2, gray, cv::COLOR_BGR2GRAY);

    // 2. Сглаживание для уменьшения шума
    cv::GaussianBlur(gray, gray, cv::Size(9, 9), 2, 2);

    // 3. Обнаружение окружностей методом HoughCircles
    std::vector<cv::Vec3f> circles;
    cv::HoughCircles(gray, circles, cv::HOUGH_GRADIENT, 1, gray.rows / 8, 100, 30, 0, 0);

    // 4. Отображение найденных окружностей
    for (size_t i = 0; i < circles.size(); i++) {
        cv::Vec3i c = circles[i];
        cv::Point center(c[0], c[1]);
        int radius = c[2];
        // Рисуем центр окружности
        cv::circle(img2, center, 3, cv::Scalar(0, 255, 0), -1, cv::LINE_AA);
        // Рисуем саму окружность
        cv::circle(img2, center, radius, cv::Scalar(255, 0, 0), 2, cv::LINE_AA);
    }


    // 2. Выделение границ с помощью Canny
    cv::Mat edges;
    cv::Canny(gray, edges, 50, 150, 3);

    // 3. Обнаружение линий методом преобразования Хафа
    std::vector<cv::Vec2f> lines;
    cv::HoughLines(edges, lines, 1, CV_PI / 180, 150);

    // 4. Отображение найденных линий
    for (size_t i = 0; i < lines.size(); i++) {
        float rho = lines[i][0];
        float theta = lines[i][1];
        double a = cos(theta), b = sin(theta);
        double x0 = a * rho, y0 = b * rho;
        // Вычисляем две точки, достаточно далеко лежащие вдоль линии
        cv::Point pt1(cvRound(x0 + 1000 * (-b)), cvRound(y0 + 1000 * (a)));
        cv::Point pt2(cvRound(x0 - 1000 * (-b)), cvRound(y0 - 1000 * (a)));
        cv::line(img2, pt1, pt2, cv::Scalar(0, 0, 255), 2, cv::LINE_AA);
    }


    Mat grayPart;
    cvtColor(img, grayPart, COLOR_BGR2GRAY); 

    Mat hsvImage;
    cvtColor(img, hsvImage, COLOR_BGR2HSV);

    Mat labImage;
    cvtColor(img, labImage, COLOR_BGR2Lab);

    Mat yuvImage;
    cvtColor(img, yuvImage, COLOR_BGR2YUV);

    Mat xyzImage;
    cvtColor(img, xyzImage, COLOR_BGR2XYZ);
 
    // Отображение изображений
    imshow("gray", grayPart);
    imshow("original", img);
    imshow("hsv", hsvImage);
    imshow("Lab", labImage);
    imshow("yuv", yuvImage);
    imshow("xyz", xyzImage);
    imshow("circle", img2);

    waitKey(0);
    destroyAllWindows();

    return 0;
}





  



