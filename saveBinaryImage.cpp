#include <fstream>
#include <iostream>
#include <sstream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc_c.h>
#include <opencv2/imgproc/imgproc.hpp>

int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		std::cout << "ファイル名を入力してください" << std::endl;
		return 1;
	}
	int w = 178;
	int h = 128;
	std::stringstream ss;
	if (argc > 7)
	{
		
		ss << argv[3];
		ss >> w;
	}
	

	/*if (w % 8 != 0)
	{
		std::cout << "画像の幅は8で割れる大きさにしてください。" << std::endl;
		return 1;
	}*/

	if (argc > 7)
	{
		ss = std::stringstream();
		ss << argv[4];
		ss >> h;
	}

	int data_w = 192;
	int data_h = 128;

	if (argc > 7)
	{
		ss = std::stringstream();
		ss << argv[5];
		ss >> data_w;
	}
	if (argc > 7)
	{
		ss = std::stringstream();
		ss << argv[6];
		ss >> data_h;
	}
	
	cv::Mat color_img = cv::imread(argv[1]);
	
	cv::Mat resize_img;
	cv::resize(color_img, resize_img, cv::Size(w, h));

	
	cv::Mat gray_img;
	cv::cvtColor(resize_img, gray_img, CV_RGB2GRAY);

	//cv::Mat not_image;
	//cv::bitwise_not(gray_img, not_image);

	cv::Mat result_image;
	cv::threshold(gray_img, result_image, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);
	
	std::string output_filename;
	if (argc <= 2)
	{
		
		std::string input_filename = argv[1];
		std::string::size_type pos;
		if ((pos = input_filename.find_last_of(".")) == std::string::npos){
			output_filename = input_filename + ".dat";
		}
		else
		{
			output_filename = input_filename.substr(0, pos) + ".dat";
		}
		std::cout << output_filename <<  std::endl;
	}
	else
	{
		output_filename = argv[2];
	}

	std::ofstream ofs(output_filename, std::ios::out | std::ios::binary | std::ios::trunc);
	//int num = h*(w/8);
	int num = data_h * data_w;
	unsigned char *temp_data = new unsigned char[num];

	//memcpy(&temp_data[0], 0x00, num);
	for (int i = 0; i < h; i++)
	{
		for (int j = 0; j < w; j++)
		{
			//std::cout << i*w + j + (data_w - w)*i << "\t" << i << "\t" << j << std::endl;
			temp_data[i*data_w+j] = result_image.data[i*w+j];
		}
	}
	
	char *data = new char[num/8];
	for (int i = 0; i < num/8; i++)
	{
		data[i] = 0x00;
		for (int j = 0; j < 8; j++)
		{
				if (temp_data[i * 8 + j] == 0x00)
				{
					data[i] += 0x01 << j;

				}
		}
	}
	
	ofs.write((char*)&data[0], sizeof(unsigned char)*num/8);
	ofs.close();

	

	cv::namedWindow("view", CV_WINDOW_AUTOSIZE | CV_WINDOW_FREERATIO);
	cv::imshow("view", result_image);
	cv::waitKey(0);

	delete data;
	delete temp_data;
	return 0;
}