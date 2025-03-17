#include <Catch2/catch.hpp>

#include "Math/Matrix.h"
#include "Math/Vector.h"
#include "Math/VectorMath.h"

TEST_CASE("Vector3f. Alignment.")
{
	CHECK(alignof(Vector3f) == 4);
}

TEST_CASE("Vector3f. Vector3f(float).")
{
	const Vector3f vector(1.f);
	CHECK(vector.x == 1.f);
	CHECK(vector.y == 1.f);
	CHECK(vector.z == 1.f);
}

TEST_CASE("Vector3f. Vector3f(float, float, float).")
{
	const Vector3f vector(1.f, 2.f, 3.f);
	CHECK(vector.x == 1.f);
	CHECK(vector.y == 2.f);
	CHECK(vector.z == 3.f);
}

TEST_CASE("Vector3f. Vector3f(Vector2f, float).")
{
	const Vector3f vector(Vector2f(1.f, 2.f), 3.f);
	CHECK(vector.x == 1.f);
	CHECK(vector.y == 2.f);
	CHECK(vector.z == 3.f);
}

TEST_CASE("Vector3f. operator==(Vector3f).")
{
	CHECK(Vector3f(0.f) == Vector3f(0.f));
	CHECK_FALSE(Vector3f(0.f) == Vector3f(1.f));
}

TEST_CASE("Vector3f. operator!=(Vector3f).")
{
	CHECK(Vector3f(0.f) != Vector3f(1.f));
	CHECK_FALSE(Vector3f(0.f) != Vector3f(0.f));
}

TEST_CASE("Vector3f. operator+(Vector3f).")
{
	Vector3f vector = Vector3f(1.f) + Vector3f(2.f);
	CHECK(vector == Vector3f(3.f));
}

TEST_CASE("Vector3f. operator-(Vector3f).")
{
	INFO("operator-(Vector3f)");
	Vector3f vector = Vector3f(3.f) - Vector3f(2.f);
	CHECK(vector == Vector3f(1.f));
}

TEST_CASE("Vector3f. operator+=(Vector3f).")
{
	Vector3f vector = Vector3f(1.f);
	vector += Vector3f(2.f);
	CHECK(vector == Vector3f(3.f));
}

TEST_CASE("Vector3f. operator-=(Vector3f).")
{
	Vector3f vector = Vector3f(3.f);
	vector -= Vector3f(2.f);
	CHECK(vector == Vector3f(1.f));
}

TEST_CASE("Vector3f. operator*(float).")
{
	Vector3f vector = Vector3f(2.f) * 3.f;
	CHECK(vector == Vector3f(6.f));
}

TEST_CASE("Vector3f. operator/(float).")
{
	Vector3f vector = Vector3f(6.f) / 3.f;
	CHECK(vector == Vector3f(2.f));
}

TEST_CASE("Vector3f. operator*=(float).")
{
	Vector3f vector = Vector3f(2.f);
	vector *= 3.f;
	CHECK(vector == Vector3f(6.f));
}

TEST_CASE("Vector3f. operator/=(float).")
{
	Vector3f vector = Vector3f(6.f);
	vector /= 3.f;
	CHECK(vector == Vector3f(2.f));
}

TEST_CASE("Vector3f. operator*(Matrix4x4) - Translate.")
{
	const Vector3f vectorA = Vector3f(2.f) * Matrix4x4::FromTranslate(Vector3f(4.f));
	CHECK(vectorA == Vector3f(6.f));

	const Vector3f vectorB = Vector3f(2.f) * Matrix4x4::FromTranslate(Vector3f(-4.f));
	CHECK(vectorB == Vector3f(-2.f));
}

TEST_CASE("Vector3f. operator*(Matrix4x4) - Rotate.")
{
	{
		INFO("X-Axis");
		const Quaternion quaternion = Quaternion::FromAxisAngle(Vector3f::AxisX, math::ToRadians(90.f));

		const Vector3f vectorA = Vector3f(1.f, 0.f, 0.f) * Matrix4x4::FromRotate(quaternion);
		const Vector3f vectorB = Vector3f(0.f, 1.f, 0.f) * Matrix4x4::FromRotate(quaternion);
		const Vector3f vectorC = Vector3f(0.f, 0.f, 1.f) * Matrix4x4::FromRotate(quaternion);

		CHECK(math::IsNearly(vectorA, Vector3f(1.f, 0.f, 0.f)));
		CHECK(math::IsNearly(vectorB, Vector3f(0.f, 0.f, 1.f)));
		CHECK(math::IsNearly(vectorC, Vector3f(0.f, -1.f, 0.f)));
	}

	{
		INFO("Y-Axis");
		const Quaternion quaternion = Quaternion::FromAxisAngle(Vector3f::AxisY, math::ToRadians(90.f));

		const Vector3f vectorA = Vector3f(1.f, 0.f, 0.f) * Matrix4x4::FromRotate(quaternion);
		const Vector3f vectorB = Vector3f(0.f, 1.f, 0.f) * Matrix4x4::FromRotate(quaternion);
		const Vector3f vectorC = Vector3f(0.f, 0.f, 1.f) * Matrix4x4::FromRotate(quaternion);

		CHECK(math::IsNearly(vectorA, Vector3f(0.f, 0.f, -1.f)));
		CHECK(math::IsNearly(vectorB, Vector3f(0.f, 1.f, 0.f)));
		CHECK(math::IsNearly(vectorC, Vector3f(1.f, 0.f, 0.f)));
	}

	{
		INFO("Z-Axis");
		const Quaternion quaternion = Quaternion::FromAxisAngle(Vector3f::AxisZ, math::ToRadians(90.f));

		const Vector3f vectorA = Vector3f(1.f, 0.f, 0.f) * Matrix4x4::FromRotate(quaternion);
		const Vector3f vectorB = Vector3f(0.f, 1.f, 0.f) * Matrix4x4::FromRotate(quaternion);
		const Vector3f vectorC = Vector3f(0.f, 0.f, 1.f) * Matrix4x4::FromRotate(quaternion);

		CHECK(math::IsNearly(vectorA, Vector3f(0.f, 1.f, 0.f)));
		CHECK(math::IsNearly(vectorB, Vector3f(-1.f, 0.f, 0.f)));
		CHECK(math::IsNearly(vectorC, Vector3f(0.f, 0.f, 1.f)));
	}

	{
		INFO("XYZ-Axis");
		const Quaternion quaternion = Quaternion::FromRotator({ 90.f });
		const Vector3f vector = Vector3f(1.f) * Matrix4x4::FromRotate(quaternion);
		CHECK(math::IsNearly(vector, Vector3f(1.f, 1.f, -1.f)));
	}
}

TEST_CASE("Vector3f. operator*(Matrix4x4) - Scale.")
{
	const Vector3f vectorA = Vector3f(2.f) * Matrix4x4::FromScale({ 3.f });
	const Vector3f vectorB = Vector3f(6.f) * Matrix4x4::FromScale({ 0.5f });
	const Vector3f vectorC = Vector3f(2.f) * Matrix4x4::FromScale({ -3.f });
	const Vector3f vectorD = Vector3f(2.f) * Matrix4x4::FromScale(3.f);
	const Vector3f vectorE = Vector3f(6.f) * Matrix4x4::FromScale(0.5f);
	const Vector3f vectorF = Vector3f(2.f) * Matrix4x4::FromScale(-3.f);
	CHECK(vectorA == Vector3f(6.f));
	CHECK(vectorB == Vector3f(3.f));
	CHECK(vectorC == Vector3f(-6.f));
	CHECK(vectorD == Vector3f(6.f));
	CHECK(vectorE == Vector3f(3.f));
	CHECK(vectorF == Vector3f(-6.f));
}

TEST_CASE("Vector3f. operator*(Matrix4x4) - Translate & Rotate.")
{
	const Quaternion quaternion = Quaternion::FromAxisAngle(Vector3f::AxisZ, math::ToRadians(90.f));
	const Vector3f vector = Vector3f(2.f) * Matrix4x4::FromTransform(Vector3f(1.f), quaternion, 1.f);
	CHECK(math::IsNearly(vector, Vector3f(-1.f, 3.f, 3.f)));
}

TEST_CASE("Vector3f. operator*(Matrix4x4) - Translate & Scale.")
{
	const Vector3f vector = Vector3f(2.f) * Matrix4x4::FromTransform(Vector3f(1.f), Quaternion::Identity, 2.f);
	CHECK(vector == Vector3f(5.f));
}

TEST_CASE("Vector3f. operator*(Matrix4x4) - Rotate & Scale.")
{
	const Quaternion quaternion = Quaternion::FromAxisAngle(Vector3f::AxisZ, math::ToRadians(90.f));
	const Vector3f vector = Vector3f(2.f) * Matrix4x4::FromTransform(Vector3f(0.f), quaternion, 2.f);
	CHECK(math::IsNearly(vector, Vector3f(-4.f, 4.f, 4.f)));
}

TEST_CASE("Vector3f. operator*(Matrix4x4) - Translate, Rotate & Scale.")
{
	const Quaternion quaternion = Quaternion::FromAxisAngle(Vector3f::AxisZ, math::ToRadians(90.f));
	const Vector3f vector = Vector3f(2.f) * Matrix4x4::FromTransform(Vector3f(2.f), quaternion, 2.f);
	CHECK(math::IsNearly(vector, Vector3f(-2.f, 6.f, 6.f)));
}

TEST_CASE("Vector3f. operator-().")
{
	CHECK(-Vector3f(1.f) == Vector3f(-1.f));
	CHECK(-Vector3f(-1.f) == Vector3f(1.f));
}

TEST_CASE("Vector3f. Length.")
{
	CHECK(Vector3f::Zero.Length() == 0.f);
	CHECK(Vector3f::AxisX.Length() == 1.f);
	CHECK(Vector3f::AxisY.Length() == 1.f);
	CHECK(Vector3f::AxisZ.Length() == 1.f);
	CHECK(Vector3f(10.f, 0.f, 0.f).Length() == 10.f);
	CHECK(Vector3f(0.f, 10.f, 0.f).Length() == 10.f);
	CHECK(Vector3f(0.f, 0.f, 10.f).Length() == 10.f);
}

TEST_CASE("Vector3f. LengthXY.")
{
	CHECK(Vector3f::Zero.LengthXY() == 0.f);
	CHECK(Vector3f::AxisX.LengthXY() == 1.f);
	CHECK(Vector3f::AxisY.LengthXY() == 1.f);
	CHECK(Vector3f::AxisZ.LengthXY() == 0.f);
	CHECK(Vector3f(10.f, 0.f, 0.f).LengthXY() == 10.f);
	CHECK(Vector3f(0.f, 10.f, 0.f).LengthXY() == 10.f);
	CHECK(Vector3f(0.f, 0.f, 10.f).LengthXY() == 0.f);
}

TEST_CASE("Vector3f. LengthXZ.")
{
	CHECK(Vector3f::Zero.LengthXZ() == 0.f);
	CHECK(Vector3f::AxisX.LengthXZ() == 1.f);
	CHECK(Vector3f::AxisY.LengthXZ() == 0.f);
	CHECK(Vector3f::AxisZ.LengthXZ() == 1.f);
	CHECK(Vector3f(10.f, 0.f, 0.f).LengthXZ() == 10.f);
	CHECK(Vector3f(0.f, 10.f, 0.f).LengthXZ() == 0.f);
	CHECK(Vector3f(0.f, 0.f, 10.f).LengthXZ() == 10.f);
}

TEST_CASE("Vector3f. LengthSqr.")
{
	CHECK(Vector3f::Zero.LengthSqr() == 0.f);
	CHECK(Vector3f::AxisX.LengthSqr() == 1.f);
	CHECK(Vector3f::AxisY.LengthSqr() == 1.f);
	CHECK(Vector3f::AxisZ.LengthSqr() == 1.f);
	CHECK(Vector3f(10.f, 0.f, 0.f).LengthSqr() == 100.f);
	CHECK(Vector3f(0.f, 10.f, 0.f).LengthSqr() == 100.f);
	CHECK(Vector3f(0.f, 0.f, 10.f).LengthSqr() == 100.f);
}

TEST_CASE("Vector3f. LengthXYSqr.")
{
	CHECK(Vector3f::Zero.LengthXYSqr() == 0.f);
	CHECK(Vector3f::AxisX.LengthXYSqr() == 1.f);
	CHECK(Vector3f::AxisY.LengthXYSqr() == 1.f);
	CHECK(Vector3f::AxisZ.LengthXYSqr() == 0.f);
	CHECK(Vector3f(10.f, 0.f, 0.f).LengthXYSqr() == 100.f);
	CHECK(Vector3f(0.f, 10.f, 0.f).LengthXYSqr() == 100.f);
	CHECK(Vector3f(0.f, 0.f, 10.f).LengthXYSqr() == 0.f);
}

TEST_CASE("Vector3f. LengthXZSqr.")
{
	CHECK(Vector3f::Zero.LengthXZSqr() == 0.f);
	CHECK(Vector3f::AxisX.LengthXZSqr() == 1.f);
	CHECK(Vector3f::AxisY.LengthXZSqr() == 0.f);
	CHECK(Vector3f::AxisZ.LengthXZSqr() == 1.f);
	CHECK(Vector3f(10.f, 0.f, 0.f).LengthXZSqr() == 100.f);
	CHECK(Vector3f(0.f, 10.f, 0.f).LengthXZSqr() == 0.f);
	CHECK(Vector3f(0.f, 0.f, 10.f).LengthXZSqr() == 100.f);
}

TEST_CASE("Vector3f. Normalize.")
{
	Vector3f vectorA = Vector3f::Zero;
	vectorA.Normalize();
	CHECK(std::isnan(vectorA.Length()));

	Vector3f vectorB = Vector3f::AxisX;
	vectorB.Normalize();
	CHECK(vectorB.Length() == 1.f);
	CHECK(vectorB == Vector3f::AxisX);

	Vector3f vectorC = Vector3f::AxisY;
	vectorC.Normalize();
	CHECK(vectorC.Length() == 1.f);
	CHECK(vectorC == Vector3f::AxisY);

	Vector3f vectorD = Vector3f::AxisZ;
	vectorD.Normalize();
	CHECK(vectorD.Length() == 1.f);
	CHECK(vectorD == Vector3f::AxisZ);

	Vector3f vectorF = Vector3f::AxisX * 10.f;
	vectorF.Normalize();
	CHECK(vectorF.Length() == 1.f);
	CHECK(vectorF == Vector3f::AxisX);

	Vector3f vectorG = Vector3f::AxisY * 10.f;
	vectorG.Normalize();
	CHECK(vectorG.Length() == 1.f);
	CHECK(vectorG == Vector3f::AxisY);

	Vector3f vectorH = Vector3f::AxisZ * 10.f;
	vectorH.Normalize();
	CHECK(vectorH.Length() == 1.f);
	CHECK(vectorH == Vector3f::AxisZ);
}

TEST_CASE("Vector3f. Normalized.")
{
	const Vector3f vectorA = (Vector3f::Zero).Normalized();
	CHECK(std::isnan(vectorA.Length()));

	const Vector3f vectorB = (Vector3f::AxisX).Normalized();
	CHECK(vectorB.Length() == 1.f);
	CHECK(vectorB == Vector3f::AxisX);

	const Vector3f vectorC = (Vector3f::AxisY).Normalized();
	CHECK(vectorC.Length() == 1.f);
	CHECK(vectorC == Vector3f::AxisY);

	const Vector3f vectorD = (Vector3f::AxisZ).Normalized();
	CHECK(vectorD.Length() == 1.f);
	CHECK(vectorD == Vector3f::AxisZ);

	const Vector3f vectorF = (Vector3f::AxisX * 10.f).Normalized();
	CHECK(vectorF.Length() == 1.f);
	CHECK(vectorF == Vector3f::AxisX);

	const Vector3f vectorG = (Vector3f::AxisY * 10.f).Normalized();
	CHECK(vectorG.Length() == 1.f);
	CHECK(vectorG == Vector3f::AxisY);

	const Vector3f vectorH = (Vector3f::AxisZ * 10.f).Normalized();
	CHECK(vectorH.Length() == 1.f);
	CHECK(vectorH == Vector3f::AxisZ);
}

TEST_CASE("Vector3f. XY.")
{
	CHECK(Vector3f(1.f, 2.f, 3.f).XY() == Vector2f(1.f, 2.f));
}

TEST_CASE("Vector3f. XZ.")
{
	CHECK(Vector3f(1.f, 2.f, 3.f).XZ() == Vector2f(1.f, 3.f));
}

TEST_CASE("Vector3f. XY0.")
{
	CHECK(Vector3f(1.f, 2.f, 3.f).XY0() == Vector3f(1.f, 2.f, 0.f));
}

TEST_CASE("Vector3f. X0Z.")
{
	CHECK(Vector3f(1.f, 2.f, 3.f).X0Z() == Vector3f(1.f, 0.f, 3.f));
}

TEST_CASE("Vector3f. AxisX.")
{
	CHECK(Vector3f::AxisX == Vector3f(1.f, 0.f, 0.f));
}

TEST_CASE("Vector3f. AxisY.")
{
	CHECK(Vector3f::AxisY == Vector3f(0.f, 1.f, 0.f));
}

TEST_CASE("Vector3f. AxisZ.")
{
	CHECK(Vector3f::AxisZ == Vector3f(0.f, 0.f, 1.f));
}

TEST_CASE("Vector3f. Zero.")
{
	CHECK(Vector3f::Zero == Vector3f(0.f));
}