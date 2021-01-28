#include "Util.h"

bool Util::Init()
{
    //Seeds random so we can use it
    srand(time(NULL));

    return true;
}

bool Util::CheckNumBetween(int num, int min, int max)
{
    //Is the num greater than the minimum
    //Is it less than the maximum?
    //It's inbetween then
    return (num >= min && num <= max);
}

bool Util::CheckNumBetween(float num, float min, float max)
{
    //Is the num greater than the minimum
    //Is it less than the maximum?
    //It's inbetween then
    return (num >= min && num <= max);
}

bool Util::CheckNumBetween(glm::vec2 num, glm::vec2 min, glm::vec2 max)
{
    //Calls the float version on individual components
    bool x = CheckNumBetween(num.x, min.x, max.x);
    bool y = CheckNumBetween(num.y, min.y, max.y);

    //If it's between for both components, that means it's true
    //if not, it's false
    return (x && y);
}

bool Util::CheckNumBetween(glm::vec3 num, glm::vec3 min, glm::vec3 max)
{
    //Calls the float version on individual components
    bool x = CheckNumBetween(num.x, min.x, max.x);
    bool y = CheckNumBetween(num.y, min.y, max.y);
    bool z = CheckNumBetween(num.z, min.z, max.z);

    //If it's between for all 3 components, that means it's true
    //if not, it's false
    return (x && y && z);
}

bool Util::CheckNumBetween(glm::vec4 num, glm::vec4 min, glm::vec4 max)
{
    //Calls the float version on individual components
    bool x = CheckNumBetween(num.x, min.x, max.x);
    bool y = CheckNumBetween(num.y, min.y, max.y);
    bool z = CheckNumBetween(num.z, min.z, max.z);
    bool w = CheckNumBetween(num.w, min.w, max.w);

    //If it's between for all 4 components, that means it's true
    //if not, it's false
    return (x && y && z && w);
}

int Util::GetRandomNumberBetween(int from, int to, int avoidFrom, int avoidTo)
{
    //Just the typical random number generation within range
    int randomNum = (rand() % (to - from)) + from;

    if (!CheckNumBetween(randomNum, avoidFrom, avoidTo))
    {
        return randomNum;
    }
    else
    {
        GetRandomNumberBetween(from, to, avoidFrom, avoidTo);
    }
}

float Util::GetRandomNumberBetween(float from, float to, float avoidFrom, float avoidTo)
{
    //DO NOT DIVIDE BY Z    if (to == 0.0f || from == 0.0f)
    {
        //Just add very small number to make sure it isn't zero
        to += 0.0001f;
        from += 0.0001f;
    }

    //Uses static casting to convert rand to a float to allow us
    //to divide it by RAND_MAX (which has been modified to suit our range)
    //in order to convert it into a float range
    float randomNum = from + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (to - from)));

    if (!CheckNumBetween(randomNum, avoidFrom, avoidTo))
    {
        return randomNum;
    }
    else
    {
        GetRandomNumberBetween(from, to, avoidFrom, avoidTo);
    }
}

glm::vec2 Util::GetRandomNumberBetween(glm::vec2 from, glm::vec2 to, glm::vec2 avoidFrom, glm::vec2 avoidTo)
{
    //Calls the float version on individual components
    glm::vec2 randomNum;
    randomNum.x = GetRandomNumberBetween(from.x, to.x);
    randomNum.y = GetRandomNumberBetween(from.y, to.y);

    if (!CheckNumBetween(randomNum, avoidFrom, avoidTo))
    {
        return randomNum;
    }
    else
    {
        GetRandomNumberBetween(from, to, avoidFrom, avoidTo);
    }
}

glm::vec3 Util::GetRandomNumberBetween(glm::vec3 from, glm::vec3 to, glm::vec3 avoidFrom, glm::vec3 avoidTo)
{
    //Calls the float version on individual components
    glm::vec3 randomNum;
    randomNum.x = GetRandomNumberBetween(from.x, to.x);
    randomNum.y = GetRandomNumberBetween(from.y, to.y);
    randomNum.z = GetRandomNumberBetween(from.z, to.z);
    
    if (!CheckNumBetween(randomNum, avoidFrom, avoidTo))
    {
        return randomNum;
    }
    else
    {
        GetRandomNumberBetween(from, to, avoidFrom, avoidTo);
    }
}

glm::vec3 Util::GetRandomNumberBetween(glm::vec4 from, glm::vec4 to, glm::vec4 avoidFrom, glm::vec4 avoidTo)
{
    //Calls the float version on individual components
    glm::vec4 randomNum;
    randomNum.x = GetRandomNumberBetween(from.x, to.x);
    randomNum.y = GetRandomNumberBetween(from.y, to.y);
    randomNum.z = GetRandomNumberBetween(from.z, to.z);
    randomNum.w = GetRandomNumberBetween(from.w, to.w);
    
    if (!CheckNumBetween(randomNum, avoidFrom, avoidTo))
    {
        return randomNum;
    }
    else
    {
        GetRandomNumberBetween(from, to, avoidFrom, avoidTo);
    }
}
