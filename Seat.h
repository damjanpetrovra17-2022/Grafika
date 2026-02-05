#pragma once
#include "Rectangle.h"

class Seat
{
public:
    enum SeatState { FREE, RESERVED, PURCHASED };

    Rectangle rect;
    SeatState state = FREE;
    bool isHovered = false;

    Seat(float x, float y, float w, float h, Shader* shader)
        : rect(x, y, w, h, shader)
    {
    }

    void toggleReserve()
    {
        if (state == PURCHASED) return;     // kupljeno se više ne dira

        if (state == FREE) state = RESERVED;
        else if (state == RESERVED) state = FREE;
    }

    void purchase()
    {
        if (state == RESERVED)
            state = PURCHASED;
    }

    void draw()
    {
        if (state == PURCHASED)
            rect.setColor(1.0f, 0.0f, 0.0f);          // CRVENO – kupljeno
        else if (state == RESERVED)
            rect.setColor(1.0f, 1.0f, 0.0f);          // ŽUTO – rezervisano
        else if (isHovered)
            rect.setColor(0.2f, 0.6f, 1.0f);          // Hover – svetlo plavo
        else
            rect.setColor(0.0f, 0.2f, 1.0f);          // Slobodno – plavo

        rect.draw();
    }
    void buy()
    {
        if (state == 0)    // samo slobodno može da se kupi
        {
            state = PURCHASED;
            rect.setColor(1.0f, 0.0f, 0.0f);   // crveno
        }
    }


};
