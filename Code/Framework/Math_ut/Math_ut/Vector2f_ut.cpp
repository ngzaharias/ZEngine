#include "MathPCH.h"

#include "Math/Matrix.h"
#include "Math/Vector.h"
#include "Math/VectorMath.h"

#define CLASS_TEST_CASE(name) TEST_CASE("Vector2f. " name, "[Vector2f]")

CLASS_TEST_CASE("Alignment.")
{
	CHECK(alignof(Vector2f) == 4);
}

CLASS_TEST_CASE("Vector2f().")
{
	constexpr Vector2f vector;
	CHECK(vector.x == 0.f);
	CHECK(vector.y == 0.f);
}

CLASS_TEST_CASE("Vector2f(float).")
{
	constexpr Vector2f vector(1.f);
	CHECK(vector.x == 1.f);
	CHECK(vector.y == 1.f);
}

CLASS_TEST_CASE("Vector2f(float, float).")
{
	constexpr Vector2f vector(1.f, 2.f);
	CHECK(vector.x == 1.f);
	CHECK(vector.y == 2.f);
}

CLASS_TEST_CASE("operator==(Vector2f).")
{
	CHECK(Vector2f::Zero == Vector2f::Zero);
	CHECK_FALSE(Vector2f::Zero == Vector2f(1.f));
}

CLASS_TEST_CASE("operator!=(Vector2f).")
{
	CHECK(Vector2f::Zero != Vector2f(1.f));
	CHECK_FALSE(Vector2f::Zero != Vector2f::Zero);
}

CLASS_TEST_CASE("operator+(Vector2f).")
{
	constexpr Vector2f vector = Vector2f(1.f) + Vector2f(2.f);
	CHECK(vector == Vector2f(3.f));
}

CLASS_TEST_CASE("operator-(Vector2f).")
{
	constexpr Vector2f vector = Vector2f(3.f) - Vector2f(2.f);
	CHECK(vector == Vector2f(1.f));
}

CLASS_TEST_CASE("operator+=(Vector2f).")
{
	Vector2f vector = Vector2f(1.f);
	vector += Vector2f(2.f);
	CHECK(vector == Vector2f(3.f));
}

CLASS_TEST_CASE("operator-=(Vector2f).")
{
	Vector2f vector = Vector2f(3.f);
	vector -= Vector2f(2.f);
	CHECK(vector == Vector2f(1.f));
}

CLASS_TEST_CASE("operator*(float).")
{
	constexpr Vector2f vector = Vector2f(2.f) * 3.f;
	CHECK(vector == Vector2f(6.f));
}

CLASS_TEST_CASE("operator/(float).")
{
	constexpr Vector2f vector = Vector2f(6.f) / 3.f;
	CHECK(vector == Vector2f(2.f));
}

CLASS_TEST_CASE("operator*=(float).")
{
	Vector2f vector = Vector2f(2.f);
	vector *= 3.f;
	CHECK(vector == Vector2f(6.f));
}

CLASS_TEST_CASE("operator/=(float).")
{
	Vector2f vector = Vector2f(6.f);
	vector /= 3.f;
	CHECK(vector == Vector2f(2.f));
}

CLASS_TEST_CASE("operator*(Matrix4x4)-Translate.")
{
	constexpr Vector2f vectorA = Vector2f(2.f) * Matrix4x4::FromTranslate(Vector3f(4.f));
	constexpr Vector2f vectorB = Vector2f(2.f) * Matrix4x4::FromTranslate(Vector3f(-4.f));

	CHECK(vectorA == Vector2f(6.f));
	CHECK(vectorB == Vector2f(-2.f));
}

CLASS_TEST_CASE("operator*(Matrix4x4)-Rotate.")
{
	{
		INFO("X-Axis");
		const Quaternion quaternion = Quaternion::FromAxisAngle(Vector3f::AxisX, math::ToRadians(90.f));

		const Vector2f vectorA = Vector2f::Zero * Matrix4x4::FromRotate(quaternion);
		const Vector2f vectorB = Vector2f::AxisX * Matrix4x4::FromRotate(quaternion);
		const Vector2f vectorC = Vector2f::AxisY * Matrix4x4::FromRotate(quaternion);

		CHECK(math::IsNearly(vectorA, Vector2f::Zero));
		CHECK(math::IsNearly(vectorB, Vector2f::AxisX));
		CHECK(math::IsNearly(vectorC, Vector2f::Zero));
	}

	{
		INFO("Y-Axis");
		const Quaternion quaternion = Quaternion::FromAxisAngle(Vector3f::AxisY, math::ToRadians(90.f));

		const Vector2f vectorA = Vector2f::Zero * Matrix4x4::FromRotate(quaternion);
		const Vector2f vectorB = Vector2f::AxisX * Matrix4x4::FromRotate(quaternion);
		const Vector2f vectorC = Vector2f::AxisY * Matrix4x4::FromRotate(quaternion);

		CHECK(math::IsNearly(vectorA, Vector2f::Zero));
		CHECK(math::IsNearly(vectorB, Vector2f::Zero));
		CHECK(math::IsNearly(vectorC, Vector2f::AxisY));
	}

	{
		INFO("Z-Axis");
		const Quaternion quaternion = Quaternion::FromAxisAngle(Vector3f::AxisZ, math::ToRadians(90.f));

		const Vector2f vectorA = Vector2f::Zero * Matrix4x4::FromRotate(quaternion);
		const Vector2f vectorB = Vector2f::AxisX * Matrix4x4::FromRotate(quaternion);
		const Vector2f vectorC = Vector2f::AxisY * Matrix4x4::FromRotate(quaternion);

		CHECK(math::IsNearly(vectorA, Vector2f::Zero));
		CHECK(math::IsNearly(vectorB, Vector2f::AxisY));
		CHECK(math::IsNearly(vectorC, -Vector2f::AxisX));
	}
}

CLASS_TEST_CASE("operator*(Matrix4x4)-Scale.")
{
	constexpr Vector2f vectorA = Vector2f(2.f) * Matrix4x4::FromScale(Vector3f(4.f, 3.f, 2.f));
	constexpr Vector2f vectorB = Vector2f(8.f) * Matrix4x4::FromScale(Vector3f(0.5f, 0.25f, 0.125f));
	constexpr Vector2f vectorC = Vector2f(2.f) * Matrix4x4::FromScale(Vector3f(-4.f, -3.f, -2.f));
	constexpr Vector2f vectorD = Vector2f(2.f) * Matrix4x4::FromScale(3.f);
	constexpr Vector2f vectorE = Vector2f(6.f) * Matrix4x4::FromScale(0.5f);
	constexpr Vector2f vectorF = Vector2f(2.f) * Matrix4x4::FromScale(-3.f);

	CHECK(vectorA == Vector2f(8.f, 6.f));
	CHECK(vectorB == Vector2f(4.f, 2.f));
	CHECK(vectorC == Vector2f(-8.f, -6.f));
	CHECK(vectorD == Vector2f(6.f));
	CHECK(vectorE == Vector2f(3.f));
	CHECK(vectorF == Vector2f(-6.f));
}

CLASS_TEST_CASE("operator*(Matrix4x4)-Translate & Scale.")
{
	const Vector2f vector = Vector2f(2.f) * Matrix4x4::FromTransform(Vector3f(1.f), Quaternion::Identity, 2.f);
	CHECK(vector == Vector2f(5.f));
}

CLASS_TEST_CASE("operator*(Matrix4x4)-Translate & Rotate.")
{
	const Quaternion quaternion = Quaternion::FromAxisAngle(Vector3f::AxisZ, math::ToRadians(90.f));
	const Vector2f vector = Vector2f(2.f) * Matrix4x4::FromTransform(Vector3f(1.f), quaternion, 1.f);
	CHECK(math::IsNearly(vector, Vector2f(-1.f, 3.f)));
}

CLASS_TEST_CASE("operator*(Matrix4x4)-Rotate & Scale.")
{
	const Quaternion quaternion = Quaternion::FromAxisAngle(Vector3f::AxisZ, math::ToRadians(90.f));
	const Vector2f vector = Vector2f(2.f) * Matrix4x4::FromTransform(Vector3f(0.f), quaternion, 2.f);
	CHECK(math::IsNearly(vector, Vector2f(-4.f, 4.f)));
}

CLASS_TEST_CASE("operator*(Matrix4x4)-Translate, Rotate& Scale.")
{
	const Quaternion quaternion = Quaternion::FromAxisAngle(Vector3f::AxisZ, math::ToRadians(90.f));
	const Vector2f vector = Vector2f(2.f) * Matrix4x4::FromTransform(Vector3f(2.f), quaternion, 2.f);
	CHECK(math::IsNearly(vector, Vector2f(-2.f, 6.f)));
}

CLASS_TEST_CASE("operator+().")
{
	CHECK(+Vector2f(1.f) == Vector2f(1.f));
	CHECK(+Vector2f(-1.f) == Vector2f(-1.f));
}

CLASS_TEST_CASE("operator-().")
{
	CHECK(-Vector2f(1.f) == Vector2f(-1.f));
	CHECK(-Vector2f(-1.f) == Vector2f(1.f));
}

CLASS_TEST_CASE("Length.")
{
	CHECK(Vector2f::Zero.Length() == 0.f);
	CHECK(Vector2f::AxisX.Length() == 1.f);
	CHECK(Vector2f::AxisY.Length() == 1.f);
	CHECK(Vector2f(10.f, 0.f).Length() == 10.f);
	CHECK(Vector2f(0.f, 10.f).Length() == 10.f);
}

CLASS_TEST_CASE("LengthSqr.")
{
	CHECK(Vector2f::Zero.LengthSqr() == 0.f);
	CHECK(Vector2f::AxisX.LengthSqr() == 1.f);
	CHECK(Vector2f::AxisY.LengthSqr() == 1.f);
	CHECK(Vector2f(10.f, 0.f).LengthSqr() == 100.f);
	CHECK(Vector2f(0.f, 10.f).LengthSqr() == 100.f);
}

CLASS_TEST_CASE("Normalize.")
{
	Vector2f vectorA = Vector2f::Zero;
	vectorA.Normalize();
	CHECK(std::isnan(vectorA.Length()));

	Vector2f vectorB = Vector2f::AxisX;
	vectorB.Normalize();
	CHECK(vectorB.Length() == 1.f);
	CHECK(vectorB == Vector2f::AxisX);

	Vector2f vectorC = Vector2f::AxisY;
	vectorC.Normalize();
	CHECK(vectorC.Length() == 1.f);
	CHECK(vectorC == Vector2f::AxisY);

	Vector2f vectorD = Vector2f::AxisX * 10.f;
	vectorD.Normalize();
	CHECK(vectorD.Length() == 1.f);
	CHECK(vectorD == Vector2f::AxisX);

	Vector2f vectorE = Vector2f::AxisY * 10.f;
	vectorE.Normalize();
	CHECK(vectorE.Length() == 1.f);
	CHECK(vectorE == Vector2f::AxisY);
}

CLASS_TEST_CASE("Normalized.")
{
	const Vector2f vectorA = Vector2f::Zero.Normalized();
	CHECK(std::isnan(vectorA.Length()));

	const Vector2f vectorB = Vector2f::AxisX.Normalized();
	CHECK(vectorB.Length() == 1.f);
	CHECK(vectorB == Vector2f::AxisX);

	const Vector2f vectorC = Vector2f::AxisY.Normalized();
	CHECK(vectorC.Length() == 1.f);
	CHECK(vectorC == Vector2f::AxisY);

	const Vector2f vectorD = (Vector2f::AxisX * 10.f).Normalized();
	CHECK(vectorD.Length() == 1.f);
	CHECK(vectorD == Vector2f::AxisX);
	
	const Vector2f vectorE = (Vector2f::AxisY * 10.f).Normalized();
	CHECK(vectorE.Length() == 1.f);
	CHECK(vectorE == Vector2f::AxisY);
}

CLASS_TEST_CASE("XY0.")
{
	CHECK(Vector2f(1.f, 2.f).XY0() == Vector3f(1.f, 2.f, 0.f));
}

CLASS_TEST_CASE("X0Y.")
{
	CHECK(Vector2f(1.f, 2.f).X0Y() == Vector3f(1.f, 0.f, 2.f));
}

CLASS_TEST_CASE("AxisX.")
{
	CHECK(Vector2f::AxisX == Vector2f(1.f, 0.f));
}

CLASS_TEST_CASE("AxisY.")
{
	CHECK(Vector2f::AxisY == Vector2f(0.f, 1.f));
}

CLASS_TEST_CASE("Zero.")
{
	CHECK(Vector2f::Zero == Vector2f(0.f));
}

CLASS_TEST_CASE("One.")
{
	CHECK(Vector2f::One == Vector2f(1.f));
}