﻿#include <filesystem>
#include <opencv2/opencv.hpp>
#include <iostream>
namespace fs = std::filesystem;

struct SampleData {
    std::string name;
    cv::Mat image;
    std::vector<cv::KeyPoint> keypoints;
    cv::Mat descriptors;
};

std::vector<cv::DMatch> ratio_test(const std::vector<std::vector<cv::DMatch>>& matches12, double ratio) {
    std::vector<cv::DMatch> good_matches;
    for (int i = 0; i < matches12.size(); i++) {
        if (matches12[i].size() >= 2 && matches12[i][0].distance < ratio * matches12[i][1].distance)
            good_matches.push_back(matches12[i][0]);
    }
    return good_matches;
}

int main() {
    std::vector<SampleData> samples;

    cv::Ptr<cv::SIFT> sift = cv::SIFT::create();

    std::string path("./cards");
    for (auto& p : fs::recursive_directory_iterator(path)) {
        if (p.path().extension() == ".png") {
            auto img_path = p.path().string();
            SampleData data;
            data.name = p.path().stem().string();
            data.image = cv::imread(img_path);
            if (data.image.empty()) {
                std::cout << "Failed to parse sample: " << img_path << std::endl;
                continue;
            }
            sift->detectAndCompute(data.image, cv::noArray(), data.keypoints, data.descriptors);
            samples.push_back(data);

            std::cout << "Parsed sample: " << img_path << std::endl;
        }
    }

    cv::Mat target = cv::imread("target.png");
    if (target.empty()) {
        std::cout << "Failed to parse target" << std::endl;
        return -1;
    }

    std::vector<cv::KeyPoint> targetKeypoints;
    cv::Mat targetDescriptors;
    sift->detectAndCompute(target, cv::noArray(), targetKeypoints, targetDescriptors);

    cv::Mat imgMatches = target.clone();

    cv::BFMatcher matcher(cv::NORM_L2);

    for (auto& sample : samples) {
        if (sample.descriptors.empty() || targetDescriptors.empty())
            continue;

        std::vector<std::vector<cv::DMatch>> matches;
        matcher.knnMatch(sample.descriptors, targetDescriptors, matches, 2);

        auto good_matches = ratio_test(matches, 0.75);

        if (good_matches.size() < 4)
            continue;

        std::vector<cv::Point2f> points_sample, points_target;
        for (int i = 0; i < good_matches.size(); i++) {
            points_sample.push_back(sample.keypoints[good_matches[i].queryIdx].pt);
            points_target.push_back(targetKeypoints[good_matches[i].trainIdx].pt);
        }

        cv::Mat H = cv::findHomography(points_sample, points_target, cv::RANSAC);

        if (H.empty())
            continue;

        std::vector<cv::Point2f> corners_sample = { {0, 0}, {static_cast<float>(sample.image.cols), 0},
                                                    {static_cast<float>(sample.image.cols), static_cast<float>(sample.image.rows)}, {0, static_cast<float>(sample.image.rows)} };
        std::vector<cv::Point2f> corners_target;

        cv::perspectiveTransform(corners_sample, corners_target, H);

        double area = cv::contourArea(corners_target);
        if (area < 1000) continue; 

        for (int i = 0; i < 4; i++) {
            cv::line(imgMatches, corners_target[i], corners_target[(i + 1) % 4], cv::Scalar(0, 255, 0), 3);
        }

        cv::Point2f center(0, 0);
        for (const auto& pt : corners_target)
            center += pt;
        center *= (1.0 / corners_target.size());
 
        int baseline = 0;
        cv::Size textSize = cv::getTextSize(sample.name, cv::FONT_HERSHEY_SIMPLEX, 0.7, 2, &baseline);
        cv::Point2f text_pos = center - cv::Point2f(textSize.width / 2.0, textSize.height / 2.0);
        cv::putText(imgMatches, sample.name, text_pos, cv::FONT_HERSHEY_SIMPLEX, 0.7, cv::Scalar(0, 0, 255), 2);

    }

    cv::imshow("Detected Cards", imgMatches);

    while ((cv::waitKey() & 0xEFFFFF) != 27);

    return 0;
}


