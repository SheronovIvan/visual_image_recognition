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

    // Определение размеров частей
    int width = img.cols / 2;
    int height = img.rows / 2;

    // Создаем копию для результата
    Mat result = img.clone();

   
    Mat topLeft = result(Rect(0, 0, width, height));        // Без изменений
    Mat topRight = result(Rect(width, 0, width, height));   // Инверсия цветов
    Mat bottomLeft = result(Rect(0, height, width, height)); // Черно-белое
    Mat bottomRight = result(Rect(width, height, width, height)); // Заполнение цветом

    // Применение обработки
    bitwise_not(topRight, topRight); // Инвертируем цвета

    Mat grayPart;
    cvtColor(bottomLeft, grayPart, COLOR_BGR2GRAY); // Переводим в ЧБ (одноканальный)
    cvtColor(grayPart, grayPart, COLOR_GRAY2BGR);   // Преобразуем обратно в 3 канала
    grayPart.copyTo(bottomLeft); // Копируем обратно в ROI

    bottomRight.setTo(Scalar(0, 255, 0)); // Заливаем зеленым цветом

    // Отображение изображений
    imshow("Оригинальное изображение", img);
    imshow("Обработанное изображение", result);

    waitKey(0);
    destroyAllWindows();

    return 0;
}