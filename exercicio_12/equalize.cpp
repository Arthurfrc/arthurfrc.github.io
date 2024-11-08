#include <iostream>
#include <opencv2/opencv.hpp>
#include "camera.hpp"

int main(int argc, char** argv) {
  cv::Mat image, gray_image, equalized_image;
  int camera;
  cv::VideoCapture cap;
  cv:Mat hist;
  int nbins = 64;
  float range[] = {0, 255};
  const float *histrange = { range };
  bool uniform = true;
  bool acummulate = false;
  int key;

  camera = cameraEnumerator();
  cap.open(camera);

  if(!cap.isOpened() {
    std::sout << "Câmera indisponível!";
    return -1;
  }

  cap.set(cv::CAP_PROP_FRAME_WIDTH, 640);
  cap.set(cv::CAP_PROP_FRAME_HEIGHT, 480);
  width = cap.get(cv::CAP_PROP_FRAME_WIDTH);
  height = cap.get(cv::CAP_PROP_FRAME_HEIGHT);

  std::cout << "Largura = " << width << std::endl;
  std::cout << "Altura = " << height << std::endl;

  int histw = nbins, histh = nbins/2;
  cv::Mat histImg(histh, histw, CV_8UC3, cv::Scalar(0,0,0));

  while(true) {
    
  }
}
