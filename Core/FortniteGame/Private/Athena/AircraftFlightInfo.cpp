#include "pch.h"
#include "FortniteGame/Public/Athena/AircraftFlightInfo.h"

FVector FAircraftFlightInfo::GetFlightEnd()
{
    FVector Direction = FlightStartRotation.Vector();

    const double LenSq = Direction.SizeSquared();

    if (LenSq != 1.0)
    {
        if (LenSq < SMALL_NUMBER)
        {
            Direction = FVector::ZeroVector;
        }
        else
        {
            Direction /= FMath::Sqrt(LenSq);
        }
    }

    const double Distance = (double)(FlightSpeed * TimeTillFlightEnd);

    return FlightStartLocation + Direction * Distance;
}