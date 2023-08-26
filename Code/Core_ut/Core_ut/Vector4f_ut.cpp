#include <Catch2/catch.hpp>

#include <Core/Vector.h>

TEST_CASE("Vector4f")
{
	SECTION("Alignment")
	{
		CHECK(alignof(Vector4f) == 16);
	}

	SECTION("Vector4f::AxisW")
	{
		CHECK(Vector4f::AxisW == Vector4f(0.f, 0.f, 0.f, 1.f));
	}

	SECTION("Vector4f::AxisX")
	{
		CHECK(Vector4f::AxisX == Vector4f(1.f, 0.f, 0.f, 0.f));
	}

	SECTION("Vector4f::AxisY")
	{
		CHECK(Vector4f::AxisY == Vector4f(0.f, 1.f, 0.f, 0.f));
	}

	SECTION("Vector4f::AxisZ")
	{
		CHECK(Vector4f::AxisZ == Vector4f(0.f, 0.f, 1.f, 0.f));
	}

	SECTION("Vector4f::Identity")
	{
		CHECK(Vector4f::Identity == Vector4f(0.f, 0.f, 0.f, 1.f));
	}

	SECTION("Vector4f::Zero")
	{
		CHECK(Vector4f::Zero == Vector4f(0.f, 0.f, 0.f, 0.f));
	}

	SECTION("Vector4f(Vector3f, float)")
	{
		Vector4f vector(Vector3f(1.f, 2.f, 3.f), 4.f);
		CHECK(vector.x == 1.f);
		CHECK(vector.y == 2.f);
		CHECK(vector.z == 3.f);
		CHECK(vector.w == 4.f);
	}

	SECTION("Vector4f(float, float, float, float)")
	{
		Vector4f vector(1.f, 2.f, 3.f, 4.f);
		CHECK(vector.x == 1.f);
		CHECK(vector.y == 2.f);
		CHECK(vector.z == 3.f);
		CHECK(vector.w == 4.f);
	}

	SECTION("operator==(Vector4f)")
	{
		CHECK(Vector4f(0.f, 0.f, 0.f, 0.f) == Vector4f(0.f, 0.f, 0.f, 0.f));
		CHECK_FALSE(Vector4f(0.f, 0.f, 0.f, 0.f) == Vector4f(1.f, 1.f, 1.f, 1.f));
	}

	SECTION("operator!=(Vector4f)")
	{
		CHECK(Vector4f(0.f, 0.f, 0.f, 0.f) != Vector4f(1.f, 1.f, 1.f, 1.f));
		CHECK_FALSE(Vector4f(0.f, 0.f, 0.f, 0.f) != Vector4f(0.f, 0.f, 0.f, 0.f));
	}

	SECTION("operator+(Vector4f)")
	{
		Vector4f vector = Vector4f(1.f, 1.f, 1.f, 1.f) + Vector4f(2.f, 2.f, 2.f, 2.f);
		CHECK(vector == Vector4f(3.f, 3.f, 3.f, 3.f));
	}

	SECTION("operator-(Vector4f)")
	{
		Vector4f vector = Vector4f(3.f, 3.f, 3.f, 3.f) - Vector4f(2.f, 2.f, 2.f, 2.f);
		CHECK(vector == Vector4f(1.f, 1.f, 1.f, 1.f));
	}

	SECTION("operator+=(Vector4f)")
	{
		Vector4f vector = Vector4f(1.f, 1.f, 1.f, 1.f);
		vector += Vector4f(2.f, 2.f, 2.f, 2.f);
		CHECK(vector == Vector4f(3.f, 3.f, 3.f, 3.f));
	}

	SECTION("operator-=(Vector4f)")
	{
		Vector4f vector = Vector4f(3.f, 3.f, 3.f, 3.f);
		vector -= Vector4f(2.f, 2.f, 2.f, 2.f);
		CHECK(vector == Vector4f(1.f, 1.f, 1.f, 1.f));
	}

	SECTION("operator*(Vector4f)")
	{
		Vector4f vector = Vector4f(2.f, 2.f, 2.f, 2.f) * 3.f;
		CHECK(vector == Vector4f(6.f, 6.f, 6.f, 6.f));
	}

	SECTION("operator/(Vector4f)")
	{
		Vector4f vector = Vector4f(6.f, 6.f, 6.f, 6.f) / 3.f;
		CHECK(vector == Vector4f(2.f, 2.f, 2.f, 2.f));
	}

	SECTION("operator*=(float)")
	{
		Vector4f vector = Vector4f(2.f, 2.f, 2.f, 2.f);
		vector *= 3.f;
		CHECK(vector == Vector4f(6.f, 6.f, 6.f, 6.f));
	}

	SECTION("operator/=(float)")
	{
		Vector4f vector = Vector4f(6.f, 6.f, 6.f, 6.f);
		vector /= 3.f;
		CHECK(vector == Vector4f(2.f, 2.f, 2.f, 2.f));
	}

	SECTION("operator-()")
	{
		const Vector4f vector = -Vector4f(1.f, 1.f, 1.f, 1.f);
		CHECK(vector == Vector4f(-1.f, -1.f, -1.f, -1.f));
	}

	SECTION("Length")
	{
		CHECK(Vector4f(0.f, 0.f, 0.f, 0.f).Length() == 0.f);

		CHECK(Vector4f(1.f, 0.f, 0.f, 0.f).Length() == 1.f);
		CHECK(Vector4f(10.f, 0.f, 0.f, 0.f).Length() == 10.f);

		CHECK(Vector4f(-1.f, 0.f, 0.f, 0.f).Length() == 1.f);
		CHECK(Vector4f(-10.f, 0.f, 0.f, 0.f).Length() == 10.f);
	}

	SECTION("LengthSqr")
	{
		CHECK(Vector4f(0.f, 0.f, 0.f, 0.f).LengthSqr() == 0.f);
		CHECK(Vector4f(1.f, 0.f, 0.f, 0.f).LengthSqr() == 1.f);
		CHECK(Vector4f(10.f, 0.f, 0.f, 0.f).LengthSqr() == 100.f);
	}

	SECTION("Normalize")
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

		Vector4f vectorJ = Vector4f::Zero;
		vectorJ.Normalize();
		CHECK(vectorJ.Length() == 0.f);
		CHECK(vectorJ == Vector4f::Zero);
	}

	SECTION("NormalizeUnsafe")
	{
		Vector4f vectorA = Vector4f::AxisX;
		vectorA.NormalizeUnsafe();
		CHECK(vectorA.Length() == 1.f);
		CHECK(vectorA == Vector4f::AxisX);

		Vector4f vectorB = Vector4f::AxisY;
		vectorB.NormalizeUnsafe();
		CHECK(vectorB.Length() == 1.f);
		CHECK(vectorB == Vector4f::AxisY);

		Vector4f vectorC = Vector4f::AxisZ;
		vectorC.NormalizeUnsafe();
		CHECK(vectorC.Length() == 1.f);
		CHECK(vectorC == Vector4f::AxisZ);

		Vector4f vectorD = Vector4f::AxisW;
		vectorD.NormalizeUnsafe();
		CHECK(vectorD.Length() == 1.f);
		CHECK(vectorD == Vector4f::AxisW);

		//////////////////////////////////////////////////////////////////////////

		Vector4f vectorE = Vector4f::AxisX * 10.f;
		vectorE.NormalizeUnsafe();
		CHECK(vectorE.Length() == 1.f);
		CHECK(vectorE == Vector4f::AxisX);

		Vector4f vectorF = Vector4f::AxisY * 10.f;
		vectorF.NormalizeUnsafe();
		CHECK(vectorF.Length() == 1.f);
		CHECK(vectorF == Vector4f::AxisY);

		Vector4f vectorG = Vector4f::AxisZ * 10.f;
		vectorG.NormalizeUnsafe();
		CHECK(vectorG.Length() == 1.f);
		CHECK(vectorG == Vector4f::AxisZ);

		Vector4f vectorH = Vector4f::AxisW * 10.f;
		vectorH.NormalizeUnsafe();
		CHECK(vectorH.Length() == 1.f);
		CHECK(vectorH == Vector4f::AxisW);

		//////////////////////////////////////////////////////////////////////////

		Vector4f vectorI = Vector4f::Identity;
		vectorI.NormalizeUnsafe();
		CHECK(vectorI.Length() == 1.f);
		CHECK(vectorI == Vector4f::Identity);
	}

	SECTION("Normalized")
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

		const Vector4f vectorJ = Vector4f::Zero.Normalized();
		CHECK(vectorJ.Length() == 0.f);
		CHECK(vectorJ == Vector4f::Zero);
	}

	SECTION("NormalizedUnsafe")
	{
		const Vector4f vectorA = Vector4f::AxisX.NormalizedUnsafe();
		CHECK(vectorA.Length() == 1.f);
		CHECK(vectorA == Vector4f::AxisX);

		const Vector4f vectorB = Vector4f::AxisY.NormalizedUnsafe();
		CHECK(vectorB.Length() == 1.f);
		CHECK(vectorB == Vector4f::AxisY);

		const Vector4f vectorC = Vector4f::AxisZ.NormalizedUnsafe();
		CHECK(vectorC.Length() == 1.f);
		CHECK(vectorC == Vector4f::AxisZ);

		const Vector4f vectorD = Vector4f::AxisW.NormalizedUnsafe();
		CHECK(vectorD.Length() == 1.f);
		CHECK(vectorD == Vector4f::AxisW);

		//////////////////////////////////////////////////////////////////////////

		const Vector4f vectorE = (Vector4f::AxisX * 10.f).NormalizedUnsafe();
		CHECK(vectorE.Length() == 1.f);
		CHECK(vectorE == Vector4f::AxisX);

		const Vector4f vectorF = (Vector4f::AxisY * 10.f).NormalizedUnsafe();
		CHECK(vectorF.Length() == 1.f);
		CHECK(vectorF == Vector4f::AxisY);

		const Vector4f vectorG = (Vector4f::AxisZ * 10.f).NormalizedUnsafe();
		CHECK(vectorG.Length() == 1.f);
		CHECK(vectorG == Vector4f::AxisZ);

		const Vector4f vectorH = (Vector4f::AxisW * 10.f).NormalizedUnsafe();
		CHECK(vectorH.Length() == 1.f);
		CHECK(vectorH == Vector4f::AxisW);

		//////////////////////////////////////////////////////////////////////////

		const Vector4f vectorI = Vector4f::Identity.NormalizedUnsafe();
		CHECK(vectorI.Length() == 1.f);
		CHECK(vectorI == Vector4f::Identity);
	}
}
