#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;

int main()
{
    std::system("chcp 65001 > nul");

    // Путь к изображению
    std::string image_path = "C:/Users/isher/Downloads/cmake_asadagar.jpg";

    // Загрузка изображения
    Mat image = imread(image_path, IMREAD_COLOR);

    // Проверка, удалось ли загрузить изображение
    if (image.empty())
    {
        std::cout << "Ошибка при загрузке изображения!" << std::endl;
        return -1;
    }

    int x1 = 2, y1 = 2;
    int x2 = 150, y2 = 150;
    int x = 150, y = 150;
    int x3 = 200, y3 = 50;
    int radius = 50;
    int a = 100, b = 50;
    int angle = 45, startAngle = 20, endAngle = 380;
    int thickness = 2;
    bool isClosed = true;
    double fontScale = 1.0;

    int B = 20, G = 255, R = 20;





    // Рисование фигур и текста
    line(image, Point(x1, y1+25), Point(x2, y2+50), Scalar(B, G, R), thickness);

    rectangle(image, Point(x1, y1), Point(x2, y2), Scalar(B, G, R), thickness);
    circle(image, Point(x+500, y), radius, Scalar(B, G, R), thickness);
    ellipse(image, Point(x + 400, y + 100), Size(a, b), angle, startAngle, endAngle, Scalar(B, G, R), thickness);
    std::vector<Point> points = { {x1+300, y1+300}, {x2+300, y2+30}, {x3+100, y3+100} };
    polylines(image, points, isClosed, Scalar(B, G, R), thickness);
    putText(image, "Hello, OpenCV!", Point(x, y + 200), FONT_HERSHEY_SIMPLEX, fontScale, Scalar(B, G, R), thickness);

    // Показ изображения в окне
    imshow("Окно с изображением", image);

    // Ожидание нажатия клавиши
    int k = waitKey(0);

    // Закрытие всех окон
    destroyAllWindows();

    return 0;
}