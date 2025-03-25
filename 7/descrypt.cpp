#include <opencv2/opencv.hpp>
#include <opencv2/features2d.hpp>
#include <iostream>

int main() {
    std::string image_path1 = "C:/Users/isher/Downloads/image1.png";
    std::string image_path2 = "C:/Users/isher/Downloads/image2.png";
    cv::Mat img1 = cv::imread(image_path1, cv::IMREAD_COLOR);
    cv::Mat img2 = cv::imread(image_path2, cv::IMREAD_COLOR);
    if (img1.empty() || img2.empty()) {
        std::cerr << "Ошибка: не удалось загрузить изображения!" << std::endl;
        return -1;
    }

    //ORB
    cv::Ptr<cv::ORB> orb = cv::ORB::create(5000);
    std::vector<cv::KeyPoint> keypoints1, keypoints2;
    cv::Mat descriptors1, descriptors2;
    orb->detectAndCompute(img1, cv::Mat(), keypoints1, descriptors1);
    orb->detectAndCompute(img2, cv::Mat(), keypoints2, descriptors2);

    //BFMatcher + Ratio Test
    cv::BFMatcher matcher(cv::NORM_HAMMING);
    std::vector<std::vector<cv::DMatch>> knn_matches;
    matcher.knnMatch(descriptors1, descriptors2, knn_matches, 2);

    std::vector<cv::DMatch> good_matches;
    for (const auto& match : knn_matches) {
        if (match[0].distance < 0.5 * match[1].distance) {
            good_matches.push_back(match[0]);
        }
    }

    std::sort(good_matches.begin(), good_matches.end(), [](const cv::DMatch& m1, const cv::DMatch& m2) {
        return m1.distance < m2.distance;
        });

    //filtered key point match
    cv::Mat img_matches_filtered;
    cv::drawMatches(img1, keypoints1, img2, keypoints2, good_matches, img_matches_filtered);
    cv::imshow("Filtered key point match", img_matches_filtered);
    cv::waitKey(0);


    //RANSAC
    std::vector<cv::Point2f> points1, points2;
    for (const auto& match : good_matches) {
        points1.push_back(keypoints1[match.queryIdx].pt);
        points2.push_back(keypoints2[match.trainIdx].pt);
    }

    cv::Mat H = cv::findHomography(points2, points1, cv::RANSAC);


    cv::Mat result;
    cv::warpPerspective(img2, result, H, cv::Size(img1.cols + img2.cols, img1.rows));

    img1.copyTo(result(cv::Rect(0, 0, img1.cols, img1.rows)));
    
    cv::imshow("Panorama", result);
    cv::waitKey(0);

    return 0;
}
