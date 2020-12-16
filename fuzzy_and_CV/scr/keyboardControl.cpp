
int key = 0;

const int key_left = 81;
const int key_up = 82;
const int key_down = 84;
const int key_right = 83;
const int key_esc = 27;
const int key_w = 119;
const int key_a = 97;
const int key_s = 115;
const int key_d = 100;
const int key_space = 32;

// void keyboardControl(int key, float* dir, float* speed);

void keyboardControl(float* dir, float* speed)
{
    mutex.lock();
      key = cv::waitKey(1);
    mutex.unlock();

    if (((key == key_up)||(key == key_w)) && (*speed <= 1.2f))
      *speed += 0.05;
    else if (((key == key_down)||(key == key_s)) && (*speed >= -1.2f))
      *speed -= 0.05;
    else if (((key == key_right)||(key == key_d)) && (*dir <= 0.4f))
      *dir += 0.05;
    else if (((key == key_left)||(key == key_a)) && (*dir >= -0.4f))
      *dir -= 0.05;
    else if (key == key_space)
    {
      *dir = 0;
      *speed = 0;
    }
    else {
      // slow down - This is a heavy calculation and slows down the whole program!
      //*speed *= 0.001;
      //*dir *= 0.001;
    }
}

