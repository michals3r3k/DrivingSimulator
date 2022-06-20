#ifndef MYCAR_H_INCLUDED
#define MYCAR_H_INCLUDED

#include <glm/glm.hpp>
#include <iostream>
#include <math.h>
#include "constants.h"

//   x,     z
glm::vec3 autoCoords2[] = {
    glm::vec3(-32, 0, -29),
    glm::vec3(-32, 0, 31),
    glm::vec3(30, 0, 35),
    glm::vec3(35, 0, 20),
    glm::vec3(-11, 0, -7),
    glm::vec3(-14, 0, -22),
    glm::vec3(0, 0, -22),
    glm::vec3(16, 0, -10),
    glm::vec3(34, 0, -14),
    glm::vec3(32, 0, -34),
    glm::vec3(-22, 0, -38),
    glm::vec3(30, 0, -34)
};



glm::vec3 autoCoords[] = {
    glm::vec3(-34	,	0	,	-28),
    glm::vec3(-35	,	0	,	22),
    glm::vec3(-35	,	0	,	28),
    glm::vec3(-31	,	0	,	33),
    glm::vec3(-26	,	0	,	35),
    glm::vec3(-13	,	0	,	38),
    glm::vec3(26	,	0	,	38),
    glm::vec3(31	,	0	,	34),
    glm::vec3(32	,	0	,	28),
    glm::vec3(32	,	0	,	20),
    glm::vec3(27	,	0	,	15),
    glm::vec3(-2	,	0	,	1),
    glm::vec3(-14	,	0	,	-5),
    glm::vec3(-17	,	0	,	-15),
    glm::vec3(-13	,	0	,	-22),
    glm::vec3(-6	,	0	,	-24),
    glm::vec3(1	,	0	,	-23),
    glm::vec3(9	,	0	,	-13),
    glm::vec3(15	,	0	,	-7),
    glm::vec3(26	,	0	,	-6),
    glm::vec3(34	,	0	,	-11),
    glm::vec3(36	,	0	,	-18),
    glm::vec3(36	,	0	,	-29),
    glm::vec3(33	,	0	,	-36),
    glm::vec3(27	,	0	,	-40),
    glm::vec3(4	,	0	,	-40),
    glm::vec3(-17	,	0	,	-39),
    glm::vec3(-24	,	0	,	-34),
    glm::vec3(-28	,	0	,	-29)
};

enum Direction {
    FORWARD,
    LEFT,
    RIGHT,
    BACKWARD
};

enum CarElem {
    FRONT,
    RIGHT_SIDE,
    LEFT_SIDE,
    BACK
};

class MyCar {
private:
    glm::vec3 HitBox[4] = {
        glm::vec3(Position + Front * 3.0f),
        glm::vec3(Position + Front * 3.0f),
        glm::vec3(Position + Front * 3.0f),
        glm::vec3(Position + Front * 3.0f)
    };

    float AutonomousSpeed;
    float DefaultTurningSpeed;

    glm::vec3 Position;
    glm::vec3 Front;

    float TurningSpeed;
    float Speed;
    float Angle;
    bool Autonomous;

    int CurrentPoint;
    bool InitialIteration;

    float Acceleration;
    float NeutralSlowingDown;
    float BreaksSlow;
    float MaxSpeed;
    float RearMaxSpeed;

    bool Breaks;
    bool Gas;
    bool RearGas;

public:
    MyCar(float defaultSpeed, float defaultTurningSpeed, glm::vec3 startPosition, bool autonomous, float acceleration, float maxSpeed)
    {
        AutonomousSpeed = defaultSpeed;
        DefaultTurningSpeed = defaultTurningSpeed;
        Autonomous = autonomous;

        Acceleration = acceleration;
        NeutralSlowingDown = 0.5f;
        BreaksSlow = 2.4f;
        MaxSpeed = maxSpeed;
        RearMaxSpeed = -5.0f;

        Speed = 0.0f;
        TurningSpeed = 0.0f;
        Angle = 180.0f;
        CurrentPoint = 0;
        Front = glm::vec3(0,0,0);

        InitialIteration = true;
        NeutralSlowingDown = 2.0f;

        Breaks = false;
        Gas = autonomous;
        RearGas = false;
        
        Position = startPosition + glm::vec3(0.0f, 0.5f, 0.0f);
        updateFront();
    }

    glm::mat4 getModelMatrix()
    {
    return glm::translate(glm::mat4(1.0f), Position) 
        * glm::rotate(glm::mat4(1.0f), PI, glm::vec3(0.0f, 1.0f, 0.0f))
        * glm::rotate(glm::mat4(1.0f), glm::radians(Angle), glm::vec3(0.0f, 1.0f, 0.0f));
    }

    glm::mat4 getCameraMatrix()
    {
        return glm::lookAt(
            glm::vec3(0, 4, -6),
            glm::vec3(1, 1, 0),
            glm::vec3(0, 1, 0)) * glm::inverse(getModelMatrix());
    }

    void updateFront()
    {
        glm::vec3 front;
        front.x = -sin(glm::radians(Angle));
        front.y = 0.0f;
        front.z = -cos(glm::radians(Angle));
        Front = glm::normalize(front);
    }

    int getNextPointIndex()
    {
        int size = sizeof(autoCoords) / sizeof(glm::vec3);
        float nextIndex = CurrentPoint + 1;
        if (nextIndex + 1 >= size)
        {
            nextIndex = 0;
        }
        return nextIndex;
    }

    void updateAutonomousFront()
    {
        glm::vec3 nextPosition = autoCoords[getNextPointIndex()];
        glm::vec3 newFront;
        newFront.x = nextPosition.x - Position.x;
        newFront.y = 0.0f;
        newFront.z = nextPosition.z - Position.z;
        Front = glm::normalize(newFront);
    }

    bool updateCurrentPoint(float deltaTime)
    {
        int nextIndex = getNextPointIndex();
        glm::vec3 nextPosition = autoCoords[nextIndex];
        float nextX = nextPosition.x;
        float nextZ = nextPosition.z;
        //std::cout << "nextX: " << nextX << " nextZ: " << nextZ << "\n";
        bool reachedPoint = Position.x >= nextX - 1.5
            && Position.x <= nextX + 1.5
            && Position.z >= nextZ - 1.5
            && Position.z <= nextZ + 1.5;
        if (reachedPoint)
        {
            CurrentPoint = nextIndex;
            return true;
        }
        return false;
    }

    void goAutonomous(float deltaTime)
    {
        glm::vec3 oldFront;
        oldFront.x = Front.x;
        oldFront.y = Front.y;
        oldFront.z = Front.z;

        bool updated = updateCurrentPoint(deltaTime);
        applyDrivingPhysics();
        updateAutonomousFront();
        updateFrontHitBox();
        Position += Front * Speed * deltaTime;
        if (updated)
        {
            bool right = glm::cross(oldFront, Front).y < 0;

            float dot = glm::dot(oldFront, Front);
            float verticesLenght = glm::length(oldFront) * glm::length(Front);
            float angle = glm::acos(dot/verticesLenght);
            angle = glm::degrees(angle);
            angle = !right ? angle : 360 - angle;
            Angle += angle;
            //std::cout << " Angle:" << Angle << " angle:" << angle << " right:" << right << "\n";
        }
    }

    void applyDrivingPhysics()
    {
        if (Breaks)
        {
            if (Speed > 0 && Speed > BreaksSlow)
            {
                Speed -= BreaksSlow;
                return;
            }
            if (Speed < 0 && Speed < -BreaksSlow)
            {
                Speed += BreaksSlow;
                return;
            }
            Speed = 0;
            return;
        }

        if (Gas)
        {
            if (Speed + Acceleration < MaxSpeed)
            {
                Speed += Acceleration;
                return;
            }
            Speed = MaxSpeed;
            return;
        }

        if (RearGas)
        {
            if (Speed > 0)
            {
                Speed -= BreaksSlow;
                return;
            }
            if (Speed > RearMaxSpeed)
            {
                Speed -= Acceleration / 2;
                return;
            }
            Speed = RearMaxSpeed;
            return;
        }

        if (Speed > NeutralSlowingDown)
        {
            Speed -= NeutralSlowingDown;
            return;
        }
        if (Speed > 0)
        {
             Speed = 0;
             return;
        }
        if (Speed < -NeutralSlowingDown)
        {
            Speed += NeutralSlowingDown;
            return;
        }
        Speed = 0;
    }

    void updatePosition(float deltaTime)
    {
        updateFront();
        updateFrontHitBox();
        applyDrivingPhysics();
        //std::cout<< "Speed: "<<Speed << " MaxSpeed: "<<MaxSpeed<<" Gas: "<<Gas<<" Breaks: "<<Breaks<<"\n";
        Position += Front * Speed * deltaTime;
        if (Speed != 0)
        {
            Angle += TurningSpeed * deltaTime;
        }
    }

    void turnRight()
    {
        TurningSpeed = -DefaultTurningSpeed;
    }

    void turnLeft()
    {
        TurningSpeed = DefaultTurningSpeed;
    }

    void goForward()
    {
        if (!Breaks)
        {
            Gas = true;
        }
    }

    void goBackWards()
    {
        if (!Gas && !Breaks)
        {
            RearGas = true;
        }
    }

    void stopGoingForward()
    {
        Gas = false;
    }

    void stopTurning()
    {
        TurningSpeed = 0.0f;
    }

    void startBreak()
    {
        Breaks = true;
    }

    void stopBreaking()
    {
        Breaks = false;
    }

    void stopGoingBackward()
    {
        RearGas = false;
    }

    float iloczyn_wektorowy(std::pair<float, float> X, std::pair<float, float> Y, std::pair<float, float> Z)
    {
        float x1 = Z.first - X.first, y1 = Z.second - X.second;
        float x2 = Y.first - X.first, y2 = Y.second - X.second;
        return x1 * y2 - x2 * y1;
    }
    bool sprawdz(std::pair<float, float> X, std::pair<float, float> Y, std::pair<float, float> Z)
    {
        return std::min(X.first, Y.first) <= Z.first && Z.first <= std::max(X.first, Y.first)
            && std::min(X.second, Y.second) <= Z.second && Z.second <= std::max(X.second, Y.second);
    }

    bool czyPrzecinaja(
        std::pair<std::pair<float, float>, std::pair<float, float>> A,
        std::pair<std::pair<float, float>, std::pair<float, float>> B)
    {
        float v1 = iloczyn_wektorowy(B.first, B.second, A.first),
            v2 = iloczyn_wektorowy(B.first, B.second, A.second),
            v3 = iloczyn_wektorowy(A.first, A.second, B.first),
            v4 = iloczyn_wektorowy(A.first, A.second, B.second);

        return ((v1 > 0 && v2 < 0 || v1 < 0 && v2>0) && (v3 > 0 && v4 < 0 || v3 < 0 && v4>0))
            || (v1 == 0 && sprawdz(B.first, B.second, A.first))
            || (v2 == 0 && sprawdz(B.first, B.second, A.second))
            || (v3 == 0 && sprawdz(A.first, A.second, B.first))
            || (v4 == 0 && sprawdz(A.first, A.second, B.first));
    }

    void hasColision(MyCar* enemy) {
        int size = sizeof(enemy->HitBox) / sizeof(enemy->HitBox[0]);
        std::pair <float, float> enemyPoints [] = {
            std::pair <float, float> (enemy->HitBox[0].x, enemy->HitBox[0].z),
            std::pair <float, float>(enemy->HitBox[1].x, enemy->HitBox[1].z),
            std::pair <float, float>(enemy->HitBox[2].x, enemy->HitBox[2].z),
            std::pair <float, float>(enemy->HitBox[3].x, enemy->HitBox[3].z)
        };
        std::pair <float, float> myPoints[4] = {
                    std::pair <float, float>(HitBox[0].x, HitBox[0].z),
                    std::pair <float, float>(HitBox[1].x, HitBox[1].z),
                    std::pair <float, float>(HitBox[2].x, HitBox[2].z),
                    std::pair <float, float>(HitBox[3].x, HitBox[3].z)
        };
        std::pair <std::pair <float, float>, std::pair <float, float>> mojeOdcinki [] = {
            std::pair <std::pair <float, float>, std::pair <float, float>>(myPoints[0], myPoints[1]), //Przód
            std::pair <std::pair <float, float>, std::pair <float, float>>(myPoints[1], myPoints[3]), //Prawy bok
            std::pair <std::pair <float, float>, std::pair <float, float>>(myPoints[0], myPoints[2]), //Lewy bok
            std::pair <std::pair <float, float>, std::pair <float, float>>(myPoints[2], myPoints[3]) //Ty³
        };
        std::pair <std::pair <float, float>, std::pair <float, float>> przeciwnikaOdcinki[] = {
            std::pair <std::pair <float, float>, std::pair <float, float>>(enemyPoints[0], enemyPoints[1]), //Przód
            std::pair <std::pair <float, float>, std::pair <float, float>>(enemyPoints[1], enemyPoints[3]), //Prawy bok
            std::pair <std::pair <float, float>, std::pair <float, float>>(enemyPoints[0], enemyPoints[2]), //Lewy bok
            std::pair <std::pair <float, float>, std::pair <float, float>>(enemyPoints[2], enemyPoints[3]) //Ty³
        };

        bool kolizja = czyPrzecinaja(mojeOdcinki[FRONT], przeciwnikaOdcinki[BACK])
            || czyPrzecinaja(mojeOdcinki[FRONT], przeciwnikaOdcinki[LEFT_SIDE])
            || czyPrzecinaja(mojeOdcinki[FRONT], przeciwnikaOdcinki[RIGHT_SIDE])
            || czyPrzecinaja(mojeOdcinki[FRONT], przeciwnikaOdcinki[BACK]);
        
        if(kolizja)
        {
            Speed = 0;
        }
    }

    void updateFrontHitBox()
    {
        glm::vec3 toLeft = Front;
        toLeft.x = -sin(glm::radians(Angle + 90.0f));
        toLeft.y = 0.0f;
        toLeft.z = -cos(glm::radians(Angle + 90.0f));
        toLeft = glm::normalize(toLeft);
        glm::vec3 toRight = Front;
        toRight.x = -sin(glm::radians(Angle - 90.0f));
        toRight.y = 0.0f;
        toRight.z = -cos(glm::radians(Angle - 90.0f));
        toRight = glm::normalize(toRight);
        //std::cout << Front.x << " " << Front.z <<" "<< toLeft.x << " " << toLeft.z << " "<< Angle<< "\n";
        HitBox[0] = glm::vec3(Position + Front * 1.0f + toLeft * 1.0f);
        HitBox[1] = glm::vec3(Position + Front * 1.0f + toRight * 1.0f);
        HitBox[2] = glm::vec3(Position - Front * 1.0f + toLeft * 1.0f);
        HitBox[3] = glm::vec3(Position - Front * 1.0f + toRight * 1.0f);
        //std::cout << Position.x << " " << Position.z << " | " << HitBox[0].x << " " << HitBox[0].z << " | " << HitBox[1].x << " " << HitBox[1].z << " | " << HitBox[2].x << " " << HitBox[2].z << " | " << HitBox[3].x << " " << HitBox[3].z << "\n";
    }

};
#endif 
