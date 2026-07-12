#pragma once
#include "pch.h"

#include "UnrealMathUtility.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"

struct FColor;

struct FLinearColor
{
public:
	DefineUnrealStruct(FLinearColor);
public:
	float R;
	float G;
	float B;
	float A;
public:
	static const FLinearColor White;
	static const FLinearColor Gray;
	static const FLinearColor Black;
	static const FLinearColor Transparent;
	static const FLinearColor Red;
	static const FLinearColor Green;
	static const FLinearColor Blue;
	static const FLinearColor Yellow;
public:
	FORCEINLINE FLinearColor() : R(0.f), G(0.f), B(0.f), A(0.f) {}

	FORCEINLINE FLinearColor(float InR, float InG, float InB, float InA = 1.0f)
		: R(InR), G(InG), B(InB), A(InA)
	{
	}

	FLinearColor(const FColor& Color);

public:
	FORCEINLINE FLinearColor operator+(const FLinearColor& C) const { return FLinearColor(R + C.R, G + C.G, B + C.B, A + C.A); }
	FORCEINLINE FLinearColor operator-(const FLinearColor& C) const { return FLinearColor(R - C.R, G - C.G, B - C.B, A - C.A); }
	FORCEINLINE FLinearColor operator*(const FLinearColor& C) const { return FLinearColor(R * C.R, G * C.G, B * C.B, A * C.A); }
	FORCEINLINE FLinearColor operator*(float Scalar) const { return FLinearColor(R * Scalar, G * Scalar, B * Scalar, A * Scalar); }
	FORCEINLINE FLinearColor operator/(float Scalar) const { const float Inv = 1.0f / Scalar; return FLinearColor(R * Inv, G * Inv, B * Inv, A * Inv); }

	FORCEINLINE bool operator==(const FLinearColor& C) const { return R == C.R && G == C.G && B == C.B && A == C.A; }
	FORCEINLINE bool operator!=(const FLinearColor& C) const { return R != C.R || G != C.G || B != C.B || A != C.A; }

public:
	FORCEINLINE bool Equals(const FLinearColor& C, float Tolerance = KINDA_SMALL_NUMBER) const
	{
		return FMath::Abs(R - C.R) < Tolerance && FMath::Abs(G - C.G) < Tolerance && FMath::Abs(B - C.B) < Tolerance && FMath::Abs(A - C.A) < Tolerance;
	}

	FORCEINLINE float GetMax() const { return FMath::Max(FMath::Max(FMath::Max(R, G), B), A); }
	FORCEINLINE float GetMin() const { return FMath::Min(FMath::Min(FMath::Min(R, G), B), A); }
	FORCEINLINE float GetLuminance() const { return R * 0.3f + G * 0.59f + B * 0.11f; }

	FColor ToFColor(bool bSRGB) const;

	static float Dist(const FLinearColor& V1, const FLinearColor& V2)
	{
		return FMath::Sqrt(FMath::Square(V2.R - V1.R) + FMath::Square(V2.G - V1.G) + FMath::Square(V2.B - V1.B) + FMath::Square(V2.A - V1.A));
	}

	static FLinearColor LerpUsingHSV(const FLinearColor& From, const FLinearColor& To, float Progress);
};

inline const FLinearColor FLinearColor::White(1.f, 1.f, 1.f, 1.f);
inline const FLinearColor FLinearColor::Gray(0.5f, 0.5f, 0.5f, 1.f);
inline const FLinearColor FLinearColor::Black(0.f, 0.f, 0.f, 1.f);
inline const FLinearColor FLinearColor::Transparent(0.f, 0.f, 0.f, 0.f);
inline const FLinearColor FLinearColor::Red(1.f, 0.f, 0.f, 1.f);
inline const FLinearColor FLinearColor::Green(0.f, 1.f, 0.f, 1.f);
inline const FLinearColor FLinearColor::Blue(0.f, 0.f, 1.f, 1.f);
inline const FLinearColor FLinearColor::Yellow(1.f, 1.f, 0.f, 1.f);

struct FColor
{
public:
	DefineUnrealStruct(FColor);
public:
	uint8 B;
	uint8 G;
	uint8 R;
	uint8 A;
public:
	static const FColor White;
	static const FColor Black;
	static const FColor Red;
	static const FColor Green;
	static const FColor Blue;
	static const FColor Yellow;
	static const FColor Transparent;
public:
	FORCEINLINE FColor() : B(0), G(0), R(0), A(0) {}

	FORCEINLINE FColor(uint8 InR, uint8 InG, uint8 InB, uint8 InA = 255)
		: B(InB), G(InG), R(InR), A(InA)
	{
	}

	explicit FColor(uint32 InColor) { DWColor() = InColor; }

public:
	FORCEINLINE uint32& DWColor() { return *((uint32*)this); }
	FORCEINLINE const uint32& DWColor() const { return *((uint32*)this); }

	FORCEINLINE bool operator==(const FColor& C) const { return DWColor() == C.DWColor(); }
	FORCEINLINE bool operator!=(const FColor& C) const { return DWColor() != C.DWColor(); }

	FORCEINLINE uint32 ToPackedARGB() const { return (A << 24) | (R << 16) | (G << 8) | (B << 0); }
	FORCEINLINE uint32 ToPackedABGR() const { return (A << 24) | (B << 16) | (G << 8) | (R << 0); }
	FORCEINLINE uint32 ToPackedRGBA() const { return (R << 24) | (G << 16) | (B << 8) | (A << 0); }

	FLinearColor ReinterpretAsLinear() const
	{
		return FLinearColor(R / 255.f, G / 255.f, B / 255.f, A / 255.f);
	}

	friend uint32 GetTypeHash(const FColor& Color)
	{
		return Color.DWColor();
	}
};

inline const FColor FColor::White(255, 255, 255, 255);
inline const FColor FColor::Black(0, 0, 0, 255);
inline const FColor FColor::Red(255, 0, 0, 255);
inline const FColor FColor::Green(0, 255, 0, 255);
inline const FColor FColor::Blue(0, 0, 255, 255);
inline const FColor FColor::Yellow(255, 255, 0, 255);
inline const FColor FColor::Transparent(0, 0, 0, 0);

FORCEINLINE FLinearColor::FLinearColor(const FColor& Color)
{
	R = Color.R / 255.f;
	G = Color.G / 255.f;
	B = Color.B / 255.f;
	A = Color.A / 255.f;
}

FORCEINLINE FColor FLinearColor::ToFColor(bool bSRGB) const
{
	const float ClampedR = FMath::Clamp(R, 0.f, 1.f);
	const float ClampedG = FMath::Clamp(G, 0.f, 1.f);
	const float ClampedB = FMath::Clamp(B, 0.f, 1.f);
	const float ClampedA = FMath::Clamp(A, 0.f, 1.f);

	return FColor(
		(uint8)FMath::RoundToInt(ClampedR * 255.f),
		(uint8)FMath::RoundToInt(ClampedG * 255.f),
		(uint8)FMath::RoundToInt(ClampedB * 255.f),
		(uint8)FMath::RoundToInt(ClampedA * 255.f));
}

static_assert(sizeof(FLinearColor) == 0x10, "FLinearColor layout broke: four floats expected (0x10)");
static_assert(sizeof(FColor) == 0x4, "FColor layout broke: BGRA bytes expected (0x4)");
