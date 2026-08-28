#include "MathPCH.h"

#include "Math/Vector.h"

#define CLASS_TEST_CASE(name) TEST_CASE("Vector4f. " name, "[Vector4f]")

CLASS_TEST_CASE("Alignment.")
{
	CHECK(alignof(Vector4f) == 4);
}

CLASS_TEST_CASE("Vector4f().")
{
	constexpr Vector4f vector;
	CHECK(vector.x == 0.f);
	CHECK(vector.y == 0.f);
	CHECK(vector.z == 0.f);
	CHECK(vector.w == 0.f);
}

CLASS_TEST_CASE("Vector4f(Vector3f, float).")
{
	Vector4f vector(Vector3f(1.f, 2.f, 3.f), 4.f);
	CHECK(vector.x == 1.f);
	CHECK(vector.y == 2.f);
	CHECK(vector.z == 3.f);
	CHECK(vector.w == 4.f);
}

CLASS_TEST_CASE("Vector4f(float, float, float, float).")
{
	Vector4f vector(1.f, 2.f, 3.f, 4.f);
	CHECK(vector.x == 1.f);
	CHECK(vector.y == 2.f);
	CHECK(vector.z == 3.f);
	CHECK(vector.w == 4.f);
}

CLASS_TEST_CASE("operator==(Vector4f).")
{
	CHECK(Vector4f(0.f, 0.f, 0.f, 0.f) == Vector4f(0.f, 0.f, 0.f, 0.f));
	CHECK_FALSE(Vector4f(0.f, 0.f, 0.f, 0.f) == Vector4f(1.f, 1.f, 1.f, 1.f));
}

CLASS_TEST_CASE("operator!=(Vector4f).")
{
	CHECK(Vector4f(0.f, 0.f, 0.f, 0.f) != Vector4f(1.f, 1.f, 1.f, 1.f));
	CHECK_FALSE(Vector4f(0.f, 0.f, 0.f, 0.f) != Vector4f(0.f, 0.f, 0.f, 0.f));
}

CLASS_TEST_CASE("operator+(Vector4f).")
{
	Vector4f vector = Vector4f(1.f, 1.f, 1.f, 1.f) + Vector4f(2.f, 2.f, 2.f, 2.f);
	CHECK(vector == Vector4f(3.f, 3.f, 3.f, 3.f));
}

CLASS_TEST_CASE("operator-(Vector4f).")
{
	Vector4f vector = Vector4f(3.f, 3.f, 3.f, 3.f) - Vector4f(2.f, 2.f, 2.f, 2.f);
	CHECK(vector == Vector4f(1.f, 1.f, 1.f, 1.f));
}

CLASS_TEST_CASE("operator+=(Vector4f).")
{
	Vector4f vector = Vector4f(1.f, 1.f, 1.f, 1.f);
	vector += Vector4f(2.f, 2.f, 2.f, 2.f);
	CHECK(vector == Vector4f(3.f, 3.f, 3.f, 3.f));
}

CLASS_TEST_CASE("operator-=(Vector4f).")
{
	Vector4f vector = Vector4f(3.f, 3.f, 3.f, 3.f);
	vector -= Vector4f(2.f, 2.f, 2.f, 2.f);
	CHECK(vector == Vector4f(1.f, 1.f, 1.f, 1.f));
}

CLASS_TEST_CASE("operator*(Vector4f).")
{
	Vector4f vector = Vector4f(2.f, 2.f, 2.f, 2.f) * 3.f;
	CHECK(vector == Vector4f(6.f, 6.f, 6.f, 6.f));
}

CLASS_TEST_CASE("operator/(Vector4f).")
{
	Vector4f vector = Vector4f(6.f, 6.f, 6.f, 6.f) / 3.f;
	CHECK(vector == Vector4f(2.f, 2.f, 2.f, 2.f));
}

CLASS_TEST_CASE("operator*=(float).")
{
	Vector4f vector = Vector4f(2.f, 2.f, 2.f, 2.f);
	vector *= 3.f;
	CHECK(vector == Vector4f(6.f, 6.f, 6.f, 6.f));
}

CLASS_TEST_CASE("operator/=(float).")
{
	Vector4f vector = Vector4f(6.f, 6.f, 6.f, 6.f);
	vector /= 3.f;
	CHECK(vector == Vector4f(2.f, 2.f, 2.f, 2.f));
}

CLASS_TEST_CASE("operator-().")
{
	const Vector4f vector = -Vector4f(1.f, 1.f, 1.f, 1.f);
	CHECK(vector == Vector4f(-1.f, -1.f, -1.f, -1.f));
}

CLASS_TEST_CASE("Length.")
{
	CHECK(Vector4f(0.f, 0.f, 0.f, 0.f).Length() == 0.f);

	CHECK(Vector4f(1.f, 0.f, 0.f, 0.f).Length() == 1.f);
	CHECK(Vector4f(10.f, 0.f, 0.f, 0.f).Length() == 10.f);

	CHECK(Vector4f(-1.f, 0.f, 0.f, 0.f).Length() == 1.f);
	CHECK(Vector4f(-10.f, 0.f, 0.f, 0.f).Length() == 10.f);
}

CLASS_TEST_CASE("LengthSqr.")
{
	CHECK(Vector4f(0.f, 0.f, 0.f, 0.f).LengthSqr() == 0.f);
	CHECK(Vector4f(1.f, 0.f, 0.f, 0.f).LengthSqr() == 1.f);
	CHECK(Vector4f(10.f, 0.f, 0.f, 0.f).LengthSqr() == 100.f);
}

CLASS_TEST_CASE("Normalize.")
{
	Vector4f vectorA = Vector4f::AxisX;
	vectorA.Normalize();
	CHECK(vectorA.Length() == 1.f);
	CHECK(vectorA == Vector4f::AxisX);

	Vector4f vectorB = Vector4f::AxisY;
	vectorB.Normalize();
	CHECK(vectorB.Length() == 1.f);
	CHECK(vectorB == Vector4f::AxisY);

	Vector4f vectorC = Vector4f::AxisZ;
	vectorC.Normalize();
	CHECK(vectorC.Length() == 1.f);
	CHECK(vectorC == Vector4f::AxisZ);

	Vector4f vectorD = Vector4f::AxisW;
	vectorD.Normalize();
	CHECK(vectorD.Length() == 1.f);
	CHECK(vectorD == Vector4f::AxisW);

	//////////////////////////////////////////////////////////////////////////

	Vector4f vectorE = Vector4f::AxisX * 10.f;
	vectorE.Normalize();
	CHECK(vectorE.Length() == 1.f);
	CHECK(vectorE == Vector4f::AxisX);

	Vector4f vectorF = Vector4f::AxisY * 10.f;
	vectorF.Normalize();
	CHECK(vectorF.Length() == 1.f);
	CHECK(vectorF == Vector4f::AxisY);

	Vector4f vectorG = Vector4f::AxisZ * 10.f;
	vectorG.Normalize();
	CHECK(vectorG.Length() == 1.f);
	CHECK(vectorG == Vector4f::AxisZ);

	Vector4f vectorH = Vector4f::AxisW * 10.f;
	vectorH.Normalize();
	CHECK(vectorH.Length() == 1.f);
	CHECK(vectorH == Vector4f::AxisW);

	//////////////////////////////////////////////////////////////////////////

	Vector4f vectorI = Vector4f::Identity;
	vectorI.Normalize();
	CHECK(vectorI.Length() == 1.f);
	CHECK(vectorI == Vector4f::Identity);
}

CLASS_TEST_CASE("Normalized.")
{
	const Vector4f vectorA = Vector4f::AxisX.Normalized();
	CHECK(vectorA.Length() == 1.f);
	CHECK(vectorA == Vector4f::AxisX);

	const Vector4f vectorB = Vector4f::AxisY.Normalized();
	CHECK(vectorB.Length() == 1.f);
	CHECK(vectorB == Vector4f::AxisY);

	const Vector4f vectorC = Vector4f::AxisZ.Normalized();
	CHECK(vectorC.Length() == 1.f);
	CHECK(vectorC == Vector4f::AxisZ);

	const Vector4f vectorD = Vector4f::AxisW.Normalized();
	CHECK(vectorD.Length() == 1.f);
	CHECK(vectorD == Vector4f::AxisW);

	//////////////////////////////////////////////////////////////////////////

	const Vector4f vectorE = (Vector4f::AxisX * 10.f).Normalized();
	CHECK(vectorE.Length() == 1.f);
	CHECK(vectorE == Vector4f::AxisX);

	const Vector4f vectorF = (Vector4f::AxisY * 10.f).Normalized();
	CHECK(vectorF.Length() == 1.f);
	CHECK(vectorF == Vector4f::AxisY);

	const Vector4f vectorG = (Vector4f::AxisZ * 10.f).Normalized();
	CHECK(vectorG.Length() == 1.f);
	CHECK(vectorG == Vector4f::AxisZ);

	const Vector4f vectorH = (Vector4f::AxisW * 10.f).Normalized();
	CHECK(vectorH.Length() == 1.f);
	CHECK(vectorH == Vector4f::AxisW);

	//////////////////////////////////////////////////////////////////////////

	const Vector4f vectorI = Vector4f::Identity.Normalized();
	CHECK(vectorI.Length() == 1.f);
	CHECK(vectorI == Vector4f::Identity);
}

CLASS_TEST_CASE("AxisW.")
{
	CHECK(Vector4f::AxisW == Vector4f(0.f, 0.f, 0.f, 1.f));
}

CLASS_TEST_CASE("AxisX.")
{
	CHECK(Vector4f::AxisX == Vector4f(1.f, 0.f, 0.f, 0.f));
}

CLASS_TEST_CASE("AxisY.")
{
	CHECK(Vector4f::AxisY == Vector4f(0.f, 1.f, 0.f, 0.f));
}

CLASS_TEST_CASE("AxisZ.")
{
	CHECK(Vector4f::AxisZ == Vector4f(0.f, 0.f, 1.f, 0.f));
}

CLASS_TEST_CASE("Identity.")
{
	CHECK(Vector4f::Identity == Vector4f(0.f, 0.f, 0.f, 1.f));
}

CLASS_TEST_CASE("Zero.")
{
	CHECK(Vector4f::Zero == Vector4f(0.f, 0.f, 0.f, 0.f));
}