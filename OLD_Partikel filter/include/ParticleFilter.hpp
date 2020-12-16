#pragma once

#include <opencv2/core/core.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include "Particle.hpp"

#include <cmath>
#include <vector>
#include <random>
#include <iostream>

class ParticleFilter
{
public:
    ParticleFilter(cv::Mat &_map, cv::Point _topLeftCorner, cv::Point _buttomRightCorner)
    {
        // This is the default constructor, primary purpose is to calculate the
        // scaling factor between the Gazebo map and the picture Map 
        // The variable for this is called mapScale

        map = _map;
        gzTL = _topLeftCorner;
        gzBR = _buttomRightCorner;

        cv::Point gzMapVector = gzBR - gzTL;
        float gzMapSize = std::sqrt(std::pow(gzMapVector.x,2)) * std::sqrt(std::pow(gzMapVector.y,2));
        float MapSize = map.rows * map.cols;

        mapScale = std::sqrt(MapSize / gzMapSize); // The scale factor between the gazebo map and the picture map
        lidarMapLength = 10 * mapScale;
        cv::line(map, cv::Point(0,50), cv::Point(11.31 * 10, 50), cv::Scalar(0,0,255), 5, cv::LINE_8);
    };

    void CreateParticles(int lidarLength)
    {
        cv::Mat animation = map.clone();    // Create / Clear map for animation

        if(weights.empty()) // If no particles has been created yet, create X random ones
        {
            // Generate X random points in a random look direction
            std::default_random_engine generator(std::time(0));
            for(int i = 0 ; i < 10; i++)    // Choose how many particles used in program
                                            // Problems working with over 10, but is unreliable
                                            // Should ideally be 200
            {
                Particle part;
                
                while(true) //Keep looking through random points until one is inside map
                            // To avoid segmentation error.
                {
                    std::uniform_int_distribution<int> xDistribution(0,map.cols);
                    std::uniform_int_distribution<int> yDistribution(0,map.rows);
                    std::uniform_int_distribution<int> rotateXDistribution(0,360);
                    std::uniform_int_distribution<int> rotateYDistribution(0,360);
                    int x = xDistribution(generator);
                    int y = yDistribution(generator);
                    int degree = rotateXDistribution(generator);

                    // Create a particle
                    cv::Point StartRotation = cv::Point(lidarMapLength, 0);
                        // Create rotation vector based on rotation matrix
                    cv::Point RandomRotation = cv::Point(StartRotation.x * std::cos(degree) - StartRotation.y * std::sin(degree), 
                                                        StartRotation.x * std::sin(degree) + StartRotation.y * std::cos(degree));
                    //Particle part(cv::Point(x,y), RandomRotation);
                    part.Position = cv::Point(x,y);
                    part.LookDirection = RandomRotation;

                    if(CheckInsideMap(part.Position + part.LookDirection))  // If particle is inside map
                    {
                        break;
                    }
                }

                // Find lidar accuracy value for each particle
                cv::Point collisionPoint;
                // draw particle
                cv::circle(animation, part.Position, 5, cv::Scalar(0, 0, 255), 1, cv::LINE_8);
                cv::line(animation, part.Position, part.Position + part.LookDirection, cv::Scalar(0,0,255), 2, cv::LINE_8);
                
                // Find point of collision between point and its lidar
                if(FindCollision(collisionPoint, part.Position, part.Position + part.LookDirection))
                {
                    cv::circle(animation, collisionPoint, 5, cv::Scalar(255, 0, 0), 1, cv::LINE_8);
                    part.weight = DistanceBetweenPoints(part.Position, collisionPoint) / (lidarLength * mapScale);
                    //cv::putText(animation, std::to_string(part.weight), part.Position, cv::FONT_HERSHEY_COMPLEX_SMALL, 1, cv::Scalar(0,0,colorChange), 1, cv::LINE_8);
                }
                else if(lidarLength == 10) // If lidar length is 10 there is no collision
                {
                    part.weight = 1;
                }
                else    // If no collision, but value is below 10
                {
                    part.weight = lidarMapLength / (lidarLength * mapScale);
                }

                // Add to weights
                weights.push_back(part);
                
                // Sort weights by size, most accurate lowest
                SortWeight(weights.size() - 1);
            }

            PrintWeights(); // Print weights (for debugging)
        }
        else    // If partikles has already been created one
        {
            // Remove the lowest 50% accurate particles
            weights.erase(weights.begin() + (weights.size()/2), weights.end());
            PrintWeights(); // Print for debugging

            // Create new particles with noise
            std::vector<Particle> weightsCopy = weights; // use a copy of particles to avoid segmentation
            for(int i = 0; i < weightsCopy.size(); i++) // Create a new particle for each particle already existing
            {
                while(true) // Loop is again used to avoid segmentation fault in case noise is outside map
                {
                    // Generate a new particle around the particles with highest weight from last time step
                    Particle part;
                    cv::Point newPoint = weightsCopy[i].Position + GenerateNoise(-40, 40); // The new particle can be offset with -40-40
                    int degree = RandomNumber(-180, 180); // New particle will have a random rotation
                    cv::Point RandomRotation = cv::Point(weightsCopy[i].LookDirection.x * std::cos(degree) - weightsCopy[i].LookDirection.y * std::sin(degree), 
                                                            weightsCopy[i].LookDirection.x * std::sin(degree) + weightsCopy[i].LookDirection.y * std::cos(degree));
                    part.Position = newPoint;
                    part.LookDirection = RandomRotation;

                    if(CheckInsideMap(part.Position + part.LookDirection)) // if the new particle is inside map, add to existing particle
                    {
                        weights.push_back(part);
                        break;
                    }
                }
            }

            // Recalculate all particles weight
            weightsCopy = weights;
            for(int i = 0; i < weightsCopy.size(); i++)
            {
                cv::Point collisionPoint;
                cv::circle(animation, weightsCopy[i].Position, 5, cv::Scalar(0, 0, 255), 1, cv::LINE_8);
                cv::line(animation, weightsCopy[i].Position, weightsCopy[i].Position + weightsCopy[i].LookDirection, cv::Scalar(0,0,255), 2, cv::LINE_8);
                if(FindCollision(collisionPoint, weightsCopy[i].Position, weightsCopy[i].Position + weightsCopy[i].LookDirection))
                {
                    cv::circle(animation, collisionPoint, 5, cv::Scalar(255, 0, 0), 1, cv::LINE_8); // Debug, show the point of collision
                    weights[i].weight = DistanceBetweenPoints(weightsCopy[i].Position, collisionPoint) / (lidarLength * mapScale);
                }
                else if(lidarLength == 10)
                {
                    weights[i].weight = 1;
                }
                else
                {
                    weights[i].weight = lidarMapLength / (lidarLength * mapScale);
                }
                
                SortWeight(i);
            }


            PrintWeights(); // Debug
        }

        // Draw particles
        cv::imshow("animation", animation);
    }

    std::vector<Particle> weights;

    double mapScale;
private:
    cv::Mat map;
    cv::Point gzTL;
    cv::Point gzBR;
    float lidarMapLength;
    
    // Recursive function to sort weights in weight array
    void SortWeight(int sort)
    {
        if(sort < 1 || (1- weights[sort - 1].weight) < (1 - weights[sort].weight))
        {
            return;
        }

        std::swap(weights[sort-1], weights[sort]);
        SortWeight(sort - 1);
    }
    
    // Print weight for debuggin
    void PrintWeights()
    {
        std::cout << std::endl << std::endl << "Weights: ";
        for(int i = 0; i < weights.size(); i++)
        {
            //cv::putText(map, std::to_string(i), weights[i].Position, cv::FONT_HERSHEY_COMPLEX_SMALL, 1, cv::Scalar(255,0,0), 1, cv::LINE_8);
            std::cout << "(" << i << ", " << weights[i].weight << ") "; 
        }
    }

    // Create a random number between the to numbers
    int RandomNumber(int from, int to)
    {
        std::default_random_engine generator(std::time(0));
        std::uniform_int_distribution<int> range(from, to);
        return range(generator);
    }

    // Generate a random point between the two numbers
    cv::Point GenerateNoise(int from, int to)
    {
        std::default_random_engine generator(std::time(0));
        std::uniform_int_distribution<int> xDistribution(from, to);
        std::uniform_int_distribution<int> yDistribution(from, to);
        int x = xDistribution(generator);
        int y = yDistribution(generator);
        return cv::Point(x,y);
    }

    // Check if point is inside map
    bool CheckInsideMap(cv::Point p1)
    {
        if(p1.x < map.cols && 0 < p1.x && p1.y < map.cols && p1.y > 0)
        {
            return true;
        }
        return false;
    }

    // Calculate distance between two points
    float DistanceBetweenPoints(cv::Point from, cv::Point to)
    {
        return std::sqrt(std::pow( to.x - from.x , 2 ) + std::pow( to.y - from.y , 2 ));
    }

    // Check if the is a collision between two points. 
    bool FindCollision(cv::Point &collision, cv::Point from, cv::Point to)
    {
        cv::Point currentPos;
        for(float t = 0; t < 1; t += 0.01)
        {
            currentPos = (1-t)*from + t*to;
            cv::Vec3b tempColor = map.at<cv::Vec3b>(currentPos);
            if((int)tempColor.val[0] == 0 && (int)tempColor.val[1] == 0 && (int)tempColor.val[2] == 0)
            {
                collision = currentPos;
                return true;
            }
        }
        return false;
    }
};