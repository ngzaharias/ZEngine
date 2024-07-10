#include <Catch2/catch.hpp>

#include "Math/Common.h"
#include "Math/Math.h"
#include "Math/Matrix.h"
#include "Math/Quaternion.h"

TEST_CASE("Quaternion")
{
	{
		INFO("Quaternion::AxisX");
		CHECK(Quaternion::AxisX == Quaternion(1.f, 0.f, 0.f, 0.f));
	}

	{
		INFO("Quaternion::AxisY");
		CHECK(Quaternion::AxisY == Quaternion(0.f, 1.f, 0.f, 0.f));
	}

	{
		INFO("Quaternion::AxisZ");
		CHECK(Quaternion::AxisZ == Quaternion(0.f, 0.f, 1.f, 0.f));
	}

	{
		INFO("Quaternion::AxisW");
		CHECK(Quaternion::AxisW == Quaternion(0.f, 0.f, 0.f, 1.f));
	}

	{
		INFO("Quaternion::Identity");
		CHECK(Quaternion::Identity == Quaternion(0.f, 0.f, 0.f, 1.f));
	}

	{
		INFO("Quaternion::Zero");
		CHECK(Quaternion::Zero == Quaternion(0.f, 0.f, 0.f, 0.f));
	}

	{
		INFO("Quaternion(float, float, float, float)");
		Quaternion quaternion(1.f, 2.f, 3.f, 4.f);
		CHECK(quaternion.x == 1.f);
		CHECK(quaternion.y == 2.f);
		CHECK(quaternion.z == 3.f);
		CHECK(quaternion.w == 4.f);
	}

	{
		INFO("Quaternion(Vector3f)");
		Quaternion quaternion(Vector3f(1.f, 2.f, 3.f), 4.f);
		CHECK(quaternion.x == 1.f);
		CHECK(quaternion.y == 2.f);
		CHECK(quaternion.z == 3.f);
		CHECK(quaternion.w == 4.f);
	}

	{
		INFO("Quaternion(Vector4f)");
		Quaternion quaternion(Vector4f(1.f, 2.f, 3.f, 4.f));
		CHECK(quaternion.x == 1.f);
		CHECK(quaternion.y == 2.f);
		CHECK(quaternion.z == 3.f);
		CHECK(quaternion.w == 4.f);
	}

	{
		INFO("operator==(Quaternion)");
		CHECK(Quaternion(0.f, 0.f, 0.f, 0.f) == Quaternion(0.f, 0.f, 0.f, 0.f));
		CHECK_FALSE(Quaternion(0.f, 0.f, 0.f, 0.f) == Quaternion(1.f, 1.f, 1.f, 1.f));
	}

	{
		INFO("operator!=(Quaternion)");
		CHECK(Quaternion(0.f, 0.f, 0.f, 0.f) != Quaternion(1.f, 1.f, 1.f, 1.f));
		CHECK_FALSE(Quaternion(0.f, 0.f, 0.f, 0.f) != Quaternion(0.f, 0.f, 0.f, 0.f));
	}

	{
		INFO("operator+(Quaternion)");
		const Quaternion quaternion =
			Quaternion(1.f, 1.f, 1.f, 1.f) +
			Quaternion(2.f, 2.f, 2.f, 2.f);
		CHECK(quaternion == Quaternion(3.f, 3.f, 3.f, 3.f));
	}

	{
		INFO("operator-(Quaternion)");
		const Quaternion quaternion =
			Quaternion(3.f, 3.f, 3.f, 3.f) -
			Quaternion(2.f, 2.f, 2.f, 2.f);
		CHECK(quaternion == Quaternion(1.f, 1.f, 1.f, 1.f));
	}

	{
		INFO("operator*(Quaternion)");
		const Quaternion quaternion =
			Quaternion(2.f, 2.f, 2.f, 2.f) *
			Quaternion(3.f, 3.f, 3.f, 3.f);
		CHECK(quaternion == Quaternion(12.f, 12.f, 12.f, -12.f));
	}

	{
		INFO("operator+=(Quaternion)");
		Quaternion quaternion = Quaternion(1.f, 1.f, 1.f, 1.f);
		quaternion += Quaternion(2.f, 2.f, 2.f, 2.f);
		CHECK(quaternion == Quaternion(3.f, 3.f, 3.f, 3.f));
	}

	{
		INFO("operator-=(Quaternion)");
		Quaternion quaternion = Quaternion(3.f, 3.f, 3.f, 3.f);
		quaternion -= Quaternion(2.f, 2.f, 2.f, 2.f);
		CHECK(quaternion == Quaternion(1.f, 1.f, 1.f, 1.f));
	}

	{
		INFO("operator*=(Quaternion)");
		Quaternion quaternion = Quaternion(2.f, 2.f, 2.f, 2.f);
		quaternion *= Quaternion(3.f, 3.f, 3.f, 3.f);
		CHECK(quaternion == Quaternion(12.f, 12.f, 12.f, -12.f));
	}

	{
		INFO("operator/(float)");
		const Quaternion quaternion = Quaternion(6.f, 6.f, 6.f, 6.f) / 3.f;
		CHECK(quaternion == Quaternion(2.f, 2.f, 2.f, 2.f));
	}

	{
		INFO("operator*(float)");
		const Quaternion quaternion = Quaternion(2.f, 2.f, 2.f, 2.f) * 3.f;
		CHECK(quaternion == Quaternion(6.f, 6.f, 6.f, 6.f));
	}

	{
		INFO("operator/=(float)");
		Quaternion quaternion = Quaternion(6.f, 6.f, 6.f, 6.f);
		quaternion /= 3.f;
		CHECK(quaternion == Quaternion(2.f, 2.f, 2.f, 2.f));
	}

	{
		INFO("operator*=(float)");
		Quaternion quaternion = Quaternion(2.f, 2.f, 2.f, 2.f);
		quaternion *= 3.f;
		CHECK(quaternion == Quaternion(6.f, 6.f, 6.f, 6.f));
	}

	{
		INFO("operator-()");
		const Quaternion quaternion = -Quaternion(1.f, 1.f, 1.f, 1.f);
		CHECK(quaternion == Quaternion(-1.f, -1.f, -1.f, -1.f));
	}

	{
		INFO("Length");
		CHECK(Quaternion(0.f, 0.f, 0.f, 0.f).Length() == 0.f);

		CHECK(Quaternion(1.f, 0.f, 0.f, 0.f).Length() == 1.f);
		CHECK(Quaternion(10.f, 0.f, 0.f, 0.f).Length() == 10.f);

		CHECK(Quaternion(-1.f, 0.f, 0.f, 0.f).Length() == 1.f);
		CHECK(Quaternion(-10.f, 0.f, 0.f, 0.f).Length() == 10.f);
	}

	{
		INFO("LengthSqr");
		CHECK(Quaternion(0.f, 0.f, 0.f, 0.f).LengthSqr() == 0.f);
		CHECK(Quaternion(1.f, 0.f, 0.f, 0.f).LengthSqr() == 1.f);
		CHECK(Quaternion(10.f, 0.f, 0.f, 0.f).LengthSqr() == 100.f);
	}

	{
		INFO("Inverse");
		Quaternion quaternionA = Quaternion::Zero;
		quaternionA.Inverse();
		CHECK(quaternionA == Quaternion::Zero);

		Quaternion quaternionB = Quaternion::Identity;
		quaternionB.Inverse();
		CHECK(quaternionB == Quaternion::Identity);

		Quaternion quaternionC = Quaternion(1.f, 1.f, 1.f, 1.f);
		quaternionC.Inverse();
		CHECK(quaternionC == Quaternion(-1.f, -1.f, -1.f, 1.f));
	}

	{
		INFO("Normalize");
		Quaternion quaternionA = Quaternion::AxisX;
		quaternionA.Normalize();
		CHECK(quaternionA.Length() == 1.f);
		CHECK(quaternionA == Quaternion::AxisX);

		Quaternion quaternionB = Quaternion::AxisY;
		quaternionB.Normalize();
		CHECK(quaternionB.Length() == 1.f);
		CHECK(quaternionB == Quaternion::AxisY);

		Quaternion quaternionC = Quaternion::AxisZ;
		quaternionC.Normalize();
		CHECK(quaternionC.Length() == 1.f);
		CHECK(quaternionC == Quaternion::AxisZ);

		Quaternion quaternionD = Quaternion::AxisW;
		quaternionD.Normalize();
		CHECK(quaternionD.Length() == 1.f);
		CHECK(quaternionD == Quaternion::AxisW);

		//////////////////////////////////////////////////////////////////////////

		Quaternion quaternionE = Quaternion::AxisX * 10.f;
		quaternionE.Normalize();
		CHECK(quaternionE.Length() == 1.f);
		CHECK(quaternionE == Quaternion::AxisX);

		Quaternion quaternionF = Quaternion::AxisY * 10.f;
		quaternionF.Normalize();
		CHECK(quaternionF.Length() == 1.f);
		CHECK(quaternionF == Quaternion::AxisY);

		Quaternion quaternionG = Quaternion::AxisZ * 10.f;
		quaternionG.Normalize();
		CHECK(quaternionG.Length() == 1.f);
		CHECK(quaternionG == Quaternion::AxisZ);

		Quaternion quaternionH = Quaternion::AxisW * 10.f;
		quaternionH.Normalize();
		CHECK(quaternionH.Length() == 1.f);
		CHECK(quaternionH == Quaternion::AxisW);

		//////////////////////////////////////////////////////////////////////////

		Quaternion quaternionI = Quaternion::Identity;
		quaternionI.Normalize();
		CHECK(quaternionI.Length() == 1.f);
		CHECK(quaternionI == Quaternion::Identity);

		Quaternion quaternionJ = Quaternion::Zero;
		quaternionJ.Normalize();
		CHECK(quaternionJ.Length() == 0.f);
		CHECK(quaternionJ == Quaternion::Zero);
	}

	{
		INFO("NormalizeUnsafe");
		Quaternion quaternionA = Quaternion::AxisX;
		quaternionA.NormalizeUnsafe();
		CHECK(quaternionA.Length() == 1.f);
		CHECK(quaternionA == Quaternion::AxisX);

		Quaternion quaternionB = Quaternion::AxisY;
		quaternionB.NormalizeUnsafe();
		CHECK(quaternionB.Length() == 1.f);
		CHECK(quaternionB == Quaternion::AxisY);

		Quaternion quaternionC = Quaternion::AxisZ;
		quaternionC.NormalizeUnsafe();
		CHECK(quaternionC.Length() == 1.f);
		CHECK(quaternionC == Quaternion::AxisZ);

		Quaternion quaternionD = Quaternion::AxisW;
		quaternionD.NormalizeUnsafe();
		CHECK(quaternionD.Length() == 1.f);
		CHECK(quaternionD == Quaternion::AxisW);

		//////////////////////////////////////////////////////////////////////////

		Quaternion quaternionE = Quaternion::AxisX * 10.f;
		quaternionE.NormalizeUnsafe();
		CHECK(quaternionE.Length() == 1.f);
		CHECK(quaternionE == Quaternion::AxisX);

		Quaternion quaternionF = Quaternion::AxisY * 10.f;
		quaternionF.NormalizeUnsafe();
		CHECK(quaternionF.Length() == 1.f);
		CHECK(quaternionF == Quaternion::AxisY);

		Quaternion quaternionG = Quaternion::AxisZ * 10.f;
		quaternionG.NormalizeUnsafe();
		CHECK(quaternionG.Length() == 1.f);
		CHECK(quaternionG == Quaternion::AxisZ);

		Quaternion quaternionH = Quaternion::AxisW * 10.f;
		quaternionH.NormalizeUnsafe();
		CHECK(quaternionH.Length() == 1.f);
		CHECK(quaternionH == Quaternion::AxisW);

		//////////////////////////////////////////////////////////////////////////

		Quaternion quaternionI = Quaternion::Identity;
		quaternionI.NormalizeUnsafe();
		CHECK(quaternionI.Length() == 1.f);
		CHECK(quaternionI == Quaternion::Identity);
	}

	{
		INFO("Inversed");
		const Quaternion quaternionA = Quaternion::Zero.Inversed();
		CHECK(quaternionA == Quaternion::Zero);

		const Quaternion quaternionB = Quaternion::Identity.Inversed();
		CHECK(quaternionB == Quaternion::Identity);

		Quaternion quaternionC = Quaternion(1.f, 1.f, 1.f, 1.f).Inversed();
		CHECK(quaternionC == Quaternion(-1.f, -1.f, -1.f, 1.f));
	}

	{
		INFO("Normalized");
		const Quaternion quaternionA = Quaternion::AxisX.Normalized();
		CHECK(quaternionA.Length() == 1.f);
		CHECK(quaternionA == Quaternion::AxisX);

		const Quaternion quaternionB = Quaternion::AxisY.Normalized();
		CHECK(quaternionB.Length() == 1.f);
		CHECK(quaternionB == Quaternion::AxisY);

		const Quaternion quaternionC = Quaternion::AxisZ.Normalized();
		CHECK(quaternionC.Length() == 1.f);
		CHECK(quaternionC == Quaternion::AxisZ);

		const Quaternion quaternionD = Quaternion::AxisW.Normalized();
		CHECK(quaternionD.Length() == 1.f);
		CHECK(quaternionD == Quaternion::AxisW);

		//////////////////////////////////////////////////////////////////////////

		const Quaternion quaternionE = (Quaternion::AxisX * 10.f).Normalized();
		CHECK(quaternionE.Length() == 1.f);
		CHECK(quaternionE == Quaternion::AxisX);

		const Quaternion quaternionF = (Quaternion::AxisY * 10.f).Normalized();
		CHECK(quaternionF.Length() == 1.f);
		CHECK(quaternionF == Quaternion::AxisY);

		const Quaternion quaternionG = (Quaternion::AxisZ * 10.f).Normalized();
		CHECK(quaternionG.Length() == 1.f);
		CHECK(quaternionG == Quaternion::AxisZ);

		const Quaternion quaternionH = (Quaternion::AxisW * 10.f).Normalized();
		CHECK(quaternionH.Length() == 1.f);
		CHECK(quaternionH == Quaternion::AxisW);

		//////////////////////////////////////////////////////////////////////////

		const Quaternion quaternionI = Quaternion::Identity.Normalized();
		CHECK(quaternionI.Length() == 1.f);
		CHECK(quaternionI == Quaternion::Identity);

		const Quaternion quaternionJ = Quaternion::Zero.Normalized();
		CHECK(quaternionJ.Length() == 0.f);
		CHECK(quaternionJ == Quaternion::Zero);
	}

	{
		INFO("NormalizedUnsafe");
		const Quaternion quaternionA = Quaternion::AxisX.NormalizedUnsafe();
		CHECK(quaternionA.Length() == 1.f);
		CHECK(quaternionA == Quaternion::AxisX);

		const Quaternion quaternionB = Quaternion::AxisY.NormalizedUnsafe();
		CHECK(quaternionB.Length() == 1.f);
		CHECK(quaternionB == Quaternion::AxisY);

		const Quaternion quaternionC = Quaternion::AxisZ.NormalizedUnsafe();
		CHECK(quaternionC.Length() == 1.f);
		CHECK(quaternionC == Quaternion::AxisZ);

		const Quaternion quaternionD = Quaternion::AxisW.NormalizedUnsafe();
		CHECK(quaternionD.Length() == 1.f);
		CHECK(quaternionD == Quaternion::AxisW);

		//////////////////////////////////////////////////////////////////////////

		const Quaternion quaternionE = (Quaternion::AxisX * 10.f).NormalizedUnsafe();
		CHECK(quaternionE.Length() == 1.f);
		CHECK(quaternionE == Quaternion::AxisX);

		const Quaternion quaternionF = (Quaternion::AxisY * 10.f).NormalizedUnsafe();
		CHECK(quaternionF.Length() == 1.f);
		CHECK(quaternionF == Quaternion::AxisY);

		const Quaternion quaternionG = (Quaternion::AxisZ * 10.f).NormalizedUnsafe();
		CHECK(quaternionG.Length() == 1.f);
		CHECK(quaternionG == Quaternion::AxisZ);

		const Quaternion quaternionH = (Quaternion::AxisW * 10.f).NormalizedUnsafe();
		CHECK(quaternionH.Length() == 1.f);
		CHECK(quaternionH == Quaternion::AxisW);

		//////////////////////////////////////////////////////////////////////////

		const Quaternion quaternionI = Quaternion::Identity.NormalizedUnsafe();
		CHECK(quaternionI.Length() == 1.f);
		CHECK(quaternionI == Quaternion::Identity);
	}

	{
		INFO("ToAxisAngle");
		Quaternion quaternionA = Quaternion(0.707107008f, 0.f, 0.f, 0.707106531f);
		Vector3f axisA; float angleA;
		quaternionA.ToAxisAngle(axisA, angleA);
		CHECK(axisA.x == Approx(1.f));
		CHECK(axisA.y == Approx(0.f));
		CHECK(axisA.z == Approx(0.f));
		CHECK(angleA  == Approx(PI_HALF));

		Quaternion quaternionB = Quaternion(0.f, 0.707107008f, 0.f, 0.707106531f);
		Vector3f axisB; float angleB;
		quaternionB.ToAxisAngle(axisB, angleB);
		CHECK(axisB.x == Approx(0.f));
		CHECK(axisB.y == Approx(1.f));
		CHECK(axisB.z == Approx(0.f));
		CHECK(angleB  == Approx(PI_HALF));

		Quaternion quaternionC = Quaternion(0.f, 0.f, 0.707107008f, 0.707106531f);
		Vector3f axisC; float angleC;
		quaternionC.ToAxisAngle(axisC, angleC);
		CHECK(axisC.x == Approx(0.f));
		CHECK(axisC.y == Approx(0.f));
		CHECK(axisC.z == Approx(1.f));
		CHECK(angleC  == Approx(PI_HALF));

		Quaternion quaternionD = Quaternion(0.191341698f, 0.461939931f, 0.191341698f, 0.844623029f);
		Vector3f axisD; float angleD;
		quaternionD.ToAxisAngle(axisD, angleD);
		CHECK(axisD.x == Approx(0.35741f));
		CHECK(axisD.y == Approx(0.86286f));
		CHECK(axisD.z == Approx(0.35741f));
		CHECK(angleD  == Approx(1.12987f));
	}

	{
		INFO("Angle");
	}

	{
		INFO("Dot");
	}

	{
		INFO("Lerp");
	}

	{
		INFO("Slerp");
	}

	{
		INFO("FromAxisAngle");

		{
			const Quaternion quaternion = Quaternion::FromRotator(Rotator::Zero);
			CHECK(quaternion == Quaternion::Identity);
		}

		{
			const Quaternion quaternionA = Quaternion::FromAxisAngle(Vector3f::AxisX, math::ToRadians(90.f));
			CHECK(quaternionA.x == Approx(0.70711f));
			CHECK(quaternionA.y == 0.f);
			CHECK(quaternionA.z == 0.f);
			CHECK(quaternionA.w == Approx(0.70711f));

			const Quaternion quaternionB = Quaternion::FromAxisAngle(Vector3f::AxisY, math::ToRadians(90.f));
			CHECK(quaternionB.x == 0.f);
			CHECK(quaternionB.y == Approx(0.70711f));
			CHECK(quaternionB.z == 0.f);
			CHECK(quaternionB.w == Approx(0.70711f));

			const Quaternion quaternionC = Quaternion::FromAxisAngle(Vector3f::AxisZ, math::ToRadians(90.f));
			CHECK(quaternionC.x == 0.f);
			CHECK(quaternionC.y == 0.f);
			CHECK(quaternionC.z == Approx(0.70711f));
			CHECK(quaternionC.w == Approx(0.70711f));
		}
	}

	{
		INFO("FromMatrix(Matrix3x3");
		const Quaternion quaternion = Quaternion::FromMatrix(Matrix3x3::Identity);
		CHECK(quaternion == Quaternion::Identity);
	}

	{
		INFO("FromMatrix(Matrix4x4");
		const Quaternion quaternion = Quaternion::FromMatrix(Matrix4x4::Identity);
		CHECK(quaternion == Quaternion::Identity);
	}

	{
		INFO("FromRotator");

		{
			const Quaternion quaternion = Quaternion::FromRotator(Rotator::Zero);
			CHECK(quaternion == Quaternion::Identity);
		}

		{
			const Quaternion quaternionA = Quaternion::FromRotator({ 90.f, 0.f, 0.f });
			CHECK(quaternionA.x == Approx(0.70711f));
			CHECK(quaternionA.y == 0.f);
			CHECK(quaternionA.z == 0.f);
			CHECK(quaternionA.w == Approx(0.70711f));

			const Quaternion quaternionB = Quaternion::FromRotator({ 0.f, 90.f, 0.f });
			CHECK(quaternionB.x == 0.f);
			CHECK(quaternionB.y == Approx(0.70711f));
			CHECK(quaternionB.z == 0.f);
			CHECK(quaternionB.w == Approx(0.70711f));

			const Quaternion quaternionC = Quaternion::FromRotator({ 0.f, 0.f, 90.f });
			CHECK(quaternionC.x == 0.f);
			CHECK(quaternionC.y == 0.f);
			CHECK(quaternionC.z == Approx(0.70711f));
			CHECK(quaternionC.w == Approx(0.70711f));
		}
	}

	{
		INFO("operator*(Vector2f, Quaternion)");

		{
			INFO("X-Axis");
			const Quaternion quaternion = Quaternion::FromRotator(Rotator(90.f, 0.f, 0.f));

			const Vector2f vectorA = Vector2f::Zero * quaternion;
			CHECK(IsNearly(vectorA, Vector2f::Zero));

			const Vector2f vectorB = Vector2f::AxisX * quaternion;
			CHECK(IsNearly(vectorB, Vector2f::AxisX));

			const Vector2f vectorC = Vector2f::AxisY * quaternion;
			CHECK(IsNearly(vectorC, Vector2f::Zero));
		}

		{
			INFO("Y-Axis");
			const Quaternion quaternion = Quaternion::FromRotator(Rotator(0.f, 90.f, 0.f));

			const Vector2f vectorA = Vector2f::Zero * quaternion;
			CHECK(IsNearly(vectorA, Vector2f::Zero));

			const Vector2f vectorB = Vector2f::AxisX * quaternion;
			CHECK(IsNearly(vectorB, Vector2f::Zero));

			const Vector2f vectorC = Vector2f::AxisY * quaternion;
			CHECK(IsNearly(vectorC, Vector2f::AxisY));
		}

		{
			INFO("Z-Axis");
			const Quaternion quaternion = Quaternion::FromRotator(Rotator(0.f, 0.f, 90.f));

			const Vector2f vectorA = Vector2f::Zero * quaternion;
			CHECK(IsNearly(vectorA, Vector2f::Zero));

			const Vector2f vectorB = Vector2f::AxisX * quaternion;
			CHECK(IsNearly(vectorB, Vector2f::AxisY));

			const Vector2f vectorC = Vector2f::AxisY * quaternion;
			CHECK(IsNearly(vectorC, -Vector2f::AxisX));
		}
	}

	{
		INFO("operator*(Quaternion)");
		{
			INFO("X-Axis");
			const Vector3f vectorA = Vector3f::AxisX * Quaternion::FromRotator(Rotator(90.f, 0.f, 0.f));
			CHECK(IsNearly(vectorA, Vector3f::AxisX));

			const Vector3f vectorB = Vector3f::AxisX * Quaternion::FromRotator(Rotator(-90.f, 0.f, 0.f));
			CHECK(IsNearly(vectorB, Vector3f::AxisX));

			const Vector3f vectorC = Vector3f::AxisY * Quaternion::FromRotator(Rotator(90.f, 0.f, 0.f));
			CHECK(IsNearly(vectorC, Vector3f::AxisZ));

			const Vector3f vectorD = Vector3f::AxisY * Quaternion::FromRotator(Rotator(-90.f, 0.f, 0.f));
			CHECK(IsNearly(vectorD, -Vector3f::AxisZ));

			const Vector3f vectorE = Vector3f::AxisZ * Quaternion::FromRotator(Rotator(90.f, 0.f, 0.f));
			CHECK(IsNearly(vectorE, -Vector3f::AxisY));

			const Vector3f vectorF = Vector3f::AxisZ * Quaternion::FromRotator(Rotator(-90.f, 0.f, 0.f));
			CHECK(IsNearly(vectorF, Vector3f::AxisY));
		}

		{
			INFO("Y-Axis");
			const Vector3f vectorA = Vector3f::AxisX * Quaternion::FromRotator(Rotator(0.f, 90.f, 0.f));
			CHECK(IsNearly(vectorA, -Vector3f::AxisZ));

			const Vector3f vectorB = Vector3f::AxisX * Quaternion::FromRotator(Rotator(0.f, -90.f, 0.f));
			CHECK(IsNearly(vectorB, Vector3f::AxisZ));

			const Vector3f vectorC = Vector3f::AxisY * Quaternion::FromRotator(Rotator(0.f, 90.f, 0.f));
			CHECK(IsNearly(vectorC, Vector3f::AxisY));

			const Vector3f vectorD = Vector3f::AxisY * Quaternion::FromRotator(Rotator(0.f, -90.f, 0.f));
			CHECK(IsNearly(vectorD, Vector3f::AxisY));

			const Vector3f vectorE = Vector3f::AxisZ * Quaternion::FromRotator(Rotator(0.f, 90.f, 0.f));
			CHECK(IsNearly(vectorE, Vector3f::AxisX));

			const Vector3f vectorF = Vector3f::AxisZ * Quaternion::FromRotator(Rotator(0.f, -90.f, 0.f));
			CHECK(IsNearly(vectorF, -Vector3f::AxisX));
		}

		{
			INFO("Z-Axis");
			const Vector3f vectorA = Vector3f::AxisX * Quaternion::FromRotator(Rotator(0.f, 0.f, 90.f));
			CHECK(IsNearly(vectorA, Vector3f::AxisY));

			const Vector3f vectorB = Vector3f::AxisX * Quaternion::FromRotator(Rotator(0.f, 0.f, -90.f));
			CHECK(IsNearly(vectorB, -Vector3f::AxisY));

			const Vector3f vectorC = Vector3f::AxisY * Quaternion::FromRotator(Rotator(0.f, 0.f, 90.f));
			CHECK(IsNearly(vectorC, -Vector3f::AxisX));

			const Vector3f vectorD = Vector3f::AxisY * Quaternion::FromRotator(Rotator(0.f, 0.f, -90.f));
			CHECK(IsNearly(vectorD, Vector3f::AxisX));

			const Vector3f vectorE = Vector3f::AxisZ * Quaternion::FromRotator(Rotator(0.f, 0.f, 90.f));
			CHECK(IsNearly(vectorE, Vector3f::AxisZ));

			const Vector3f vectorF = Vector3f::AxisZ * Quaternion::FromRotator(Rotator(0.f, 0.f, -90.f));
			CHECK(IsNearly(vectorF, Vector3f::AxisZ));
		}

		{
			INFO("XYZ-Axis");
			const Quaternion quaternion = Quaternion::FromRotator(Rotator(90.f));
			const Vector3f vector = Vector3f(1.f) * quaternion;
			CHECK(IsNearly(vector, Vector3f(1.f, 1.f, -1.f)));
		}
	}
}
