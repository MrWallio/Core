#include "pch.h"
#include "../../Public/Math/UnrealMathUtility.h"
#include "../../Public/Math/Vector.h"
#include "../../Public/Math/Vector2D.h"
#include "../../Public/Math/Vector4.h"
#include "../../Public/Math/Quat.h"
#include "../../Public/Math/Rotator.h"
#include "../../Public/Math/Matrix.h"
#include "../../Public/Math/TransformNonVectorized.h"

FQuat FRotator::Quaternion() const
{
	FQuat& (*QuaternionInternal)(const FRotator*, FQuat&) = decltype(QuaternionInternal)(ImageBase + Finder::FindFRotator_Quaternion());
	FQuat Result = FQuat();
	QuaternionInternal(this, Result);
	return Result;
}

// ---- FRotator -------------------------------------------------------------

FVector FRotator::Vector() const
{
	float CP, SP, CY, SY;
	FMath::SinCos(&SP, &CP, FMath::DegreesToRadians(Pitch));
	FMath::SinCos(&SY, &CY, FMath::DegreesToRadians(Yaw));

	return FVector(CP * CY, CP * SY, SP);
}

FVector FRotator::Euler() const
{
	return FVector(Roll, Pitch, Yaw);
}

FVector FRotator::RotateVector(const FVector& V) const
{
	return Quaternion().RotateVector(V);
}

FVector FRotator::UnrotateVector(const FVector& V) const
{
	return Quaternion().UnrotateVector(V);
}

FRotator FRotator::GetInverse() const
{
	return Quaternion().Inverse().Rotator();
}

// ---- FVector --------------------------------------------------------------

FRotator FVector::Rotation() const
{
	return ToOrientationRotator();
}

FRotator FVector::ToOrientationRotator() const
{
	FRotator R;

	R.Yaw = FMath::RadiansToDegrees(FMath::Atan2(Y, X));
	R.Pitch = FMath::RadiansToDegrees(FMath::Atan2(Z, FMath::Sqrt(X * X + Y * Y)));
	R.Roll = 0.f;

	return R;
}

FQuat FVector::ToOrientationQuat() const
{
	const float YawRad = FMath::Atan2(Y, X);
	const float PitchRad = FMath::Atan2(Z, FMath::Sqrt(X * X + Y * Y));

	const float DIVIDE_BY_2 = 0.5f;
	float SP, SY;
	float CP, CY;

	FMath::SinCos(&SP, &CP, PitchRad * DIVIDE_BY_2);
	FMath::SinCos(&SY, &CY, YawRad * DIVIDE_BY_2);

	FQuat RotationQuat;
	RotationQuat.X = SP * SY;
	RotationQuat.Y = -SP * CY;
	RotationQuat.Z = CP * SY;
	RotationQuat.W = CP * CY;
	return RotationQuat;
}

// ---- FQuat ----------------------------------------------------------------

FRotator FQuat::Rotator() const
{
	const float SingularityTest = Z * X - W * Y;
	const float YawY = 2.f * (W * Z + X * Y);
	const float YawX = (1.f - 2.f * (FMath::Square(Y) + FMath::Square(Z)));

	const float SINGULARITY_THRESHOLD = 0.4999995f;
	const float RAD_TO_DEG = (180.f) / PI;
	FRotator RotatorFromQuat;

	if (SingularityTest < -SINGULARITY_THRESHOLD)
	{
		RotatorFromQuat.Pitch = -90.f;
		RotatorFromQuat.Yaw = FMath::Atan2(YawY, YawX) * RAD_TO_DEG;
		RotatorFromQuat.Roll = FRotator::NormalizeAxis(-RotatorFromQuat.Yaw - (2.f * FMath::Atan2(X, W) * RAD_TO_DEG));
	}
	else if (SingularityTest > SINGULARITY_THRESHOLD)
	{
		RotatorFromQuat.Pitch = 90.f;
		RotatorFromQuat.Yaw = FMath::Atan2(YawY, YawX) * RAD_TO_DEG;
		RotatorFromQuat.Roll = FRotator::NormalizeAxis(RotatorFromQuat.Yaw - (2.f * FMath::Atan2(X, W) * RAD_TO_DEG));
	}
	else
	{
		RotatorFromQuat.Pitch = FMath::FastAsin(2.f * SingularityTest) * RAD_TO_DEG;
		RotatorFromQuat.Yaw = FMath::Atan2(YawY, YawX) * RAD_TO_DEG;
		RotatorFromQuat.Roll = FMath::Atan2(-2.f * (W * X + Y * Z), (1.f - 2.f * (FMath::Square(X) + FMath::Square(Y)))) * RAD_TO_DEG;
	}

	return RotatorFromQuat;
}

FVector FQuat::Euler() const
{
	return Rotator().Euler();
}

// ---- FMatrix --------------------------------------------------------------

FRotator FMatrix::Rotator() const
{
	const FVector XAxis = GetScaledAxis(EAxis::X);
	const FVector YAxis = GetScaledAxis(EAxis::Y);
	const FVector ZAxis = GetScaledAxis(EAxis::Z);
	const float RAD_TO_DEG = (180.f) / PI;

	FRotator Rotator = FRotator(
		FMath::Atan2(XAxis.Z, FMath::Sqrt(FMath::Square(XAxis.X) + FMath::Square(XAxis.Y))) * RAD_TO_DEG,
		FMath::Atan2(XAxis.Y, XAxis.X) * RAD_TO_DEG,
		0.f);

	const FVector SYAxis = FMatrix(FVector(FMath::Cos(FMath::DegreesToRadians(Rotator.Yaw)), FMath::Sin(FMath::DegreesToRadians(Rotator.Yaw)), 0.f),
		FVector(0, 0, 0), FVector(0, 0, 0), FVector(0, 0, 0)).GetScaledAxis(EAxis::X);

	Rotator.Roll = FMath::Atan2(ZAxis | SYAxis.operator^(XAxis).GetSafeNormal(), YAxis | SYAxis.operator^(XAxis).GetSafeNormal()) * RAD_TO_DEG;

	return Rotator;
}

// ---- FTransform -----------------------------------------------------------

FTransform::FTransform(const FRotator& InRotation, const FVector& InTranslation, const FVector& InScale3D)
	: Rotation(InRotation.Quaternion())
	, Translation(InTranslation)
	, Scale3D(InScale3D)
{
}

FRotator FTransform::Rotator() const
{
	return Rotation.Rotator();
}

// ---- FMath vector helpers -------------------------------------------------

FVector FMath::VInterpTo(const FVector& Current, const FVector& Target, float DeltaTime, float InterpSpeed)
{
	if (InterpSpeed <= 0.f)
	{
		return Target;
	}

	const FVector Dist = Target - Current;

	if (Dist.SizeSquared() < KINDA_SMALL_NUMBER)
	{
		return Target;
	}

	const FVector DeltaMove = Dist * Clamp<float>(DeltaTime * InterpSpeed, 0.f, 1.f);
	return Current + DeltaMove;
}

FVector FMath::VInterpConstantTo(const FVector& Current, const FVector& Target, float DeltaTime, float InterpSpeed)
{
	const FVector Delta = Target - Current;
	const float DeltaM = Delta.Size();
	const float MaxStep = InterpSpeed * DeltaTime;

	if (DeltaM > MaxStep)
	{
		if (MaxStep > 0.f)
		{
			const FVector DeltaN = Delta / DeltaM;
			return Current + DeltaN * MaxStep;
		}
		return Current;
	}

	return Target;
}

FRotator FMath::RInterpTo(const FRotator& Current, const FRotator& Target, float DeltaTime, float InterpSpeed)
{
	if (DeltaTime == 0.f || Current == Target)
	{
		return Current;
	}

	if (InterpSpeed <= 0.f)
	{
		return Target;
	}

	const float DeltaInterpSpeed = InterpSpeed * DeltaTime;

	const FRotator Delta = (Target - Current).GetNormalized();

	FRotator Result = Current;
	Result.Pitch += Clamp<float>(Delta.Pitch, -1.f, 1.f) == 0.f ? 0.f : Clamp<float>(Delta.Pitch * DeltaInterpSpeed, -Abs(Delta.Pitch), Abs(Delta.Pitch));
	Result.Yaw += Clamp<float>(Delta.Yaw * DeltaInterpSpeed, -Abs(Delta.Yaw), Abs(Delta.Yaw));
	Result.Roll += Clamp<float>(Delta.Roll * DeltaInterpSpeed, -Abs(Delta.Roll), Abs(Delta.Roll));
	return Result.GetNormalized();
}

FRotator FMath::RInterpConstantTo(const FRotator& Current, const FRotator& Target, float DeltaTime, float InterpSpeed)
{
	if (DeltaTime == 0.f || Current == Target)
	{
		return Current;
	}

	if (InterpSpeed <= 0.f)
	{
		return Target;
	}

	const float DeltaInterpSpeed = InterpSpeed * DeltaTime;
	const FRotator DeltaMove = (Target - Current).GetNormalized();

	FRotator Result = Current;
	Result.Pitch += Clamp<float>(DeltaMove.Pitch, -DeltaInterpSpeed, DeltaInterpSpeed);
	Result.Yaw += Clamp<float>(DeltaMove.Yaw, -DeltaInterpSpeed, DeltaInterpSpeed);
	Result.Roll += Clamp<float>(DeltaMove.Roll, -DeltaInterpSpeed, DeltaInterpSpeed);
	return Result.GetNormalized();
}

FVector FMath::VRand()
{
	FVector Result;
	float L;

	do
	{
		Result.X = FRand() * 2.f - 1.f;
		Result.Y = FRand() * 2.f - 1.f;
		Result.Z = FRand() * 2.f - 1.f;
		L = Result.SizeSquared();
	} while (L > 1.0f || L < KINDA_SMALL_NUMBER);

	return Result * (1.0f / Sqrt(L));
}
