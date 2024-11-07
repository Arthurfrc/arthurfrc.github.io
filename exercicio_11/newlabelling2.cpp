#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;

int main(int argc, char** argv) {
  cv::Mat image, realce;
  int width, height;
  int nobjects;
  int nobjects_holes;

  cv::Point p;
  image = cv::imread(argv[1], cv::IMREAD_GRAYSCALE);

  if (!image.data) {
    std::cout << "imagem nao carregou corretamente\n";
    return (-1);
  }

  width = image.cols;
  height = image.rows;
  std::cout << width << "x" << height << std::endl;

  p.x = 0;
  p.y = 0;
  
  // percorrer laterais e apagar bolhas que toquem

  for (int i = 0; i < height; i++) {
      if (image.at<uchar>(i, 0) == 255) {
        p.x = 0;p.y = i;
        cv::floodFill(image, p, 0)
      }
      if (image.at<uchar>(i,width-1) == 255){
        p.x = width-1;p.y = i;
        cv::floodFill(image, p, 0)
      }
  }

  for (int j = 0; j < width; j++) {
      if (image.at<uchar>(0, j) == 255) {
        p.x = j;p.y = 0;
        cv::floodFill(image, p, 0)
      }
      if (image.at<uchar>(height-1, j) == 255){
        p.x = j;p.y = height-1;
        cv::floodFill(image, p, 0)
      }
  }

  // busca objetos presentes
  nobjects = 0;

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
        cv::floodFill(image, p, nobjects);
      }
    }
  }
  std::cout << "a figura tem " << nobjects << " bolhas\n";
  cv::imshow("image", image);
  cv::imwrite("labeling.png", image);
  cv::waitKey();
  return 0;
}
