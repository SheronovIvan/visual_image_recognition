#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;

int main() {
    std::system("chcp 65001 > nul"); // ��������� ��������� UTF-8 � ������� Windows

    // ���� � �����������
    std::string image_path = "C:/Users/isher/Downloads/cmake_asadagar.jpg";

    // �������� �����������
    Mat img = imread(image_path, IMREAD_COLOR);
    if (img.empty()) {
        std::cout << "������ ��� �������� �����������!" << std::endl;
        return -1;
    }

    // ����������� �������� ������
    int width = img.cols / 2;
    int height = img.rows / 2;

    // ������� ����� ��� ����������
    Mat result = img.clone();

   
    Mat topLeft = result(Rect(0, 0, width, height));        // ��� ���������
    Mat topRight = result(Rect(width, 0, width, height));   // �������� ������
    Mat bottomLeft = result(Rect(0, height, width, height)); // �����-�����
    Mat bottomRight = result(Rect(width, height, width, height)); // ���������� ������

    // ���������� ���������
    bitwise_not(topRight, topRight); // ����������� �����

    Mat grayPart;
    cvtColor(bottomLeft, grayPart, COLOR_BGR2GRAY); // ��������� � �� (�������������)
    cvtColor(grayPart, grayPart, COLOR_GRAY2BGR);   // ����������� ������� � 3 ������
    grayPart.copyTo(bottomLeft); // �������� ������� � ROI

    bottomRight.setTo(Scalar(0, 255, 0)); // �������� ������� ������

    // ����������� �����������
    imshow("������������ �����������", img);
    imshow("������������ �����������", result);

    waitKey(0);
    destroyAllWindows();

    return 0;
}