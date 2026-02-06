#pragma once

enum class AppState
{
    Booking,     // Seat selection, purchase keys enabled
    Entering,    // People walking to seats
    Projection,  // Film playing
    Exiting,     // People leaving
    Reset        // Clear and restart
};
