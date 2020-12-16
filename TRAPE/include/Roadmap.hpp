#include <opencv2/core/core.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

bool checkArea(cv::Point position, cv::Mat image)
{
    int whitePixelCount = 0;
    for(int i = -1 ; i < 2; i++)
    {
        for(int j = -1 ; j < 2; j++)
        {
            cv::Vec3b value = image.at<cv::Vec3b>(cv::Point(position.x + j, position.y + i));
            
            if(value.val[0] == 255 && value.val[1] == 255 && value.val[255])
            {
                whitePixelCount++;
            }
            
            //std::cout << (int)value.val[0] << " " << (int)value.val[1] << " " << (int)value.val[2] << "   ,   ";
        }

        //std::cout << std::endl;
    }

    if(whitePixelCount > 3)
    {
        return true;
    }
    return false;
}

template<typename T>
void swap(std::vector<T> &array, int pointer1, int pointer2)
{
    T element1 = array[pointer1];
    T element2 = array[pointer2];
    array[pointer1] = element2;
    array[pointer2] = element1;
}

void ModBubbleSort(std::vector<cv::Point> &points, std::vector<int> &numbers)
{
    for(int i = 0 ; i < numbers.size() - 1 ; i++)
    {
        bool swapped = false;
        for(int j = 0 ; j < numbers.size() - i - 1 ; j++)
        {
            if(numbers[j] > numbers[j+1])
            {
                swap(numbers, j, j+1);
                swap(points, j, j+1);
                swapped = true;
            }
        }
        if(!swapped)
        {
            return;
        }
    }
}

