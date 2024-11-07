#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;

int main(int argc, char** argv) {
    cv::Mat image, realce;
    int width, height;
    int nobjects = 0, nholeObjects = 0;

    cv::Point p;
    image = cv::imread(argv[1], cv::IMREAD_GRAYSCALE);

    if (!image.data) {
        std::cout << "Imagem não carregou corretamente\n";
        return -1;
    }

    width = image.cols;
    height = image.rows;
    std::cout << width << "x" << height << std::endl;

    // Preenchendo bolhas na borda com um valor temporário
    for (int i = 0; i < width; i++) {
        if (image.at<uchar>(0, i) == 255) {
            floodFill(image, cv::Point(i, 0), 128);
        }
        if (image.at<uchar>(height - 1, i) == 255) {
            floodFill(image, cv::Point(i, height - 1), 128);
        }
    }
    for (int i = 0; i < height; i++) {
        if (image.at<uchar>(i, 0) == 255) {
            floodFill(image, cv::Point(0, i), 128);
        }
        if (image.at<uchar>(i, width - 1) == 255) {
            floodFill(image, cv::Point(width - 1, i), 128);
        }
    }

    // Detectando bolhas e separando com e sem buracos
    for (int i = 1; i < height - 1; i++) {
        for (int j = 1; j < width - 1; j++) {
            if (image.at<uchar>(i, j) == 255) {
                // Encontrou uma bolha
                nobjects++;
                p.x = j;
                p.y = i;
                // Marca a bolha com o número atual de objetos
                floodFill(image, p, nobjects);

                // Verifica se a bolha contém buracos
                bool hasHole = false;
                for (int y = 1; y < height - 1; y++) {
                    for (int x = 1; x < width - 1; x++) {
                        if (image.at<uchar>(y, x) == 0) {  // Um possível buraco dentro da bolha
                            if (floodFill(image, cv::Point(x, y), 200) > 0) { // Marca o buraco temporariamente
                                hasHole = true;
                            }
                        }
                    }
                }
                
                if (hasHole) {
                    nholeObjects++;
                }
            }
        }
    }

    std::cout << "A figura tem " << nobjects << " bolhas\n";
    std::cout << nholeObjects << " bolhas possuem buracos\n";

    // Restaurar os valores de borda temporários para branco (255) e buracos para preto (0)
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (image.at<uchar>(i, j) == 128) image.at<uchar>(i, j) = 255;
            if (image.at<uchar>(i, j) == 200) image.at<uchar>(i, j) = 0;
        }
    }

    cv::imshow("image", image);
    cv::imwrite("labeling.png", image);
    cv::waitKey();
    return 0;
}
