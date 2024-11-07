#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>

using namespace cv;

int main(int argc, char** argv) {
  cv::Mat image, labelImage;
  int width, height;
  int nobjects = 0;
  int obj_with_holes = 0;
  int obj_without_holes = 0;

  cv::Point p;
  labelImage = cv::Mat::zeros(height, width, CV_16U);
  //image = cv::imread(argv[1], cv::IMREAD_GRAYSCALE);

  if (!image.data) {
    std::cout << "imagem nao carregou corretamente\n";
    return (-1);
  }

  width = image.cols;
  height = image.rows;
  std::cout << width << "x" << height << std::endl;

  labelImage = cv::Mat::zeros(height, width, CV_16U);
  //p.x = 0;
  //p.y = 0;
 
  // Preencher as bordas para eliminar objetos tocando nelas
  for (int i = 0; i < height; i++) {
      if (image.at<uchar>(i, 0) == 255) cv::floodFill(image, cv::Point(0, i), 0);
      if (image.at<uchar>(i, width - 1) == 255) cv::floodFill(image, cv::Point(width - 1, i), 0);
  }
  for (int j = 0; j < width; j++) {
      if (image.at<uchar>(0, j) == 255) cv::floodFill(image, cv::Point(j, 0), 0);
      if (image.at<uchar>(height - 1, j) == 255) cv::floodFill(image, cv::Point(j, height - 1), 0);
  }
  // Passo 1 - Buscar objetos
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      if (image.at<uchar>(i, j) == 255) {
        // achou um objeto
        nobjects++;
        // para o floodfill as coordenadas
        // x e y são trocadas.
        p.x = j;
        p.y = i;
        // preenche o objeto com o contador
        cv::floodFill(image, p, 0);              // Preencher a bolha para evitar contá-la novamente
        cv::floodFill(labelImage, p, nobjects);  // Preenche na imagem de rótulos para contar as bolhas
      }
    }
  }

  // Passo 2 - Detectar e rotular bolhas com e sem buracos
  for (int label = 1; label < nobjects; label++) {
    cv::Mat mask = (labelImage == label);
    cv::mat inverted;
    bitwise_not(mask, inverted);                // Inverter cores para procurar buracos

    int holeCount = 0;
    for (int i = 0; i < height; i++) {
      for (int j = 0; j < width; j++) {
        if (inverted.at<uchar>(i,ij) == 255) {
          holeCount++;
          p.x = j;
          p.y = i;
          cv::floodFill(inverted, p, 0);        // Preenche o buraco para não contar novamente
        }
      }
    }

    if (holeCount > 0) {
      obj_with_holes++;
    } else {
      obj_without_holes++;
    }
  
  std::cout << "a figura tem " << nobjects << " bolhas\n";
  std::cout << obj_with_holes << " bolhas com buracos\n";
  std::cout << obj_without_holes << " bolhas sem buracos\n";
  
  cv::imshow("image", image);
  cv::imwrite("labeling.png", image);
  cv::waitKey();
  return 0;
}
