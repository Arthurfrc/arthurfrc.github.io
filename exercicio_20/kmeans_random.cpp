#include <cstdlib>
#include <opencv2/opencv.hpp>
#include <string>

int main(int argc, char** argv) {
  int nClusters = 8; // Número de clusters fixo
  int nRodadas = 1;  // Apenas uma rodada por execução

  cv::Mat rotulos, centros;

  if (argc != 3) {
    std::cout << "kmeans_random entrada.jpg saida_base\n";
    exit(0);
  }

  cv::Mat img = cv::imread(argv[1], cv::IMREAD_COLOR);
  if (img.empty()) {
    std::cerr << "Erro ao abrir a imagem.\n";
    exit(1);
  }

  cv::Mat samples(img.rows * img.cols, 3, CV_32F);

  // Converter a imagem em uma matriz de amostras
  for (int y = 0; y < img.rows; y++) {
    for (int x = 0; x < img.cols; x++) {
      for (int z = 0; z < 3; z++) {
        samples.at<float>(y + x * img.rows, z) = img.at<cv::Vec3b>(y, x)[z];
      }
    }
  }

  for (int rodada = 1; rodada <= 10; rodada++) {
    cv::kmeans(samples, nClusters, rotulos, 
               cv::TermCriteria(cv::TermCriteria::EPS | cv::TermCriteria::COUNT, 10000, 0.0001), 
               nRodadas, cv::KMEANS_RANDOM_CENTERS, centros);

    cv::Mat rotulada(img.size(), img.type());

    // Criar a imagem rotulada
    for (int y = 0; y < img.rows; y++) {
      for (int x = 0; x < img.cols; x++) {
        int indice = rotulos.at<int>(y + x * img.rows, 0);
        rotulada.at<cv::Vec3b>(y, x)[0] = (uchar)centros.at<float>(indice, 0);
        rotulada.at<cv::Vec3b>(y, x)[1] = (uchar)centros.at<float>(indice, 1);
        rotulada.at<cv::Vec3b>(y, x)[2] = (uchar)centros.at<float>(indice, 2);
      }
    }

    // Salvar a imagem rotulada
    std::string nomeArquivo = argv[2] + std::to_string(rodada) + ".jpg";
    cv::imwrite(nomeArquivo, rotulada);
    std::cout << "Imagem rodada " << rodada << " salva como " << nomeArquivo << "\n";
  }

  return 0;
}
