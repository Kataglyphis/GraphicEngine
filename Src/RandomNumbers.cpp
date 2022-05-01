#include "RandomNumbers.h"

RandomNumbers::RandomNumbers()
{
}

std::shared_ptr<GLfloat[]> RandomNumbers::generate_random_numbers()
{
    std::shared_ptr<GLfloat[]> random_number_data;
    random_number_data = std::shared_ptr<GLfloat[]>(new GLfloat[MAX_RESOLUTION_X * MAX_RESOLUTION_Y * 4]); // we only store one float in the red channel

    std::mt19937_64 gen64(25121995);
    std::uniform_real_distribution<float> dis(0, 1);

    for (int i = 0; i < MAX_RESOLUTION_X; i++) {

        for (int k = 0; k < MAX_RESOLUTION_Y; k++) {

            GLfloat random_offset[4] = { dis(gen64), dis(gen64), dis(gen64), dis(gen64) };

            GLuint index = (MAX_RESOLUTION_Y * i + k) * 4;

            *(random_number_data.get() + index) = random_offset[0];//random_offset[0];//
            *(random_number_data.get() + index + 1) = random_offset[1];//
            *(random_number_data.get() + index + 2) = random_offset[2];//
            *(random_number_data.get() + index + 3) = random_offset[3];//
        }
    }

    return random_number_data;
}

RandomNumbers::~RandomNumbers()
{
}
