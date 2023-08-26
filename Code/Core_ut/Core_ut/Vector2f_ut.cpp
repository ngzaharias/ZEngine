#include <Catch2/catch.hpp>

#include <Core/Matrix.h>
#include <Core/Vector.h>
#include <Core/VectorHelpers.h>

TEST_CASE("Vector2f")
{
	SECTION("Alignment")
	{
		CHECK(alignof(Vector2f) == 4);
	}

	SECTION("Vector2f::AxisX")
	{
		CHECK(Vector2f::AxisX == Vector2f(1.f, 0.f));
	}

	SECTION("Vector2f::AxisY")
	{
		CHECK(Vector2f::AxisY == Vector2f(0.f, 1.f));
	}

	SECTION("Vector2f::Zero")
	{
		CHECK(Vector2f::Zero == Vector2f(0.f));
	}

	SECTION("Vector2f(float)")
	{
		constexpr Vector2f vector(1.f);
		CHECK(vector.x == 1.f);
		CHECK(vector.y == 1.f);
	}

	SECTION("Vector2f(float, float)")
	{
		constexpr Vector2f vector(1.f, 2.f);
		CHECK(vector.x == 1.f);
		CHECK(vector.y == 2.f);
	}

	SECTION("operator==(Vector2f)")
	{
		CHECK(Vector2f::Zero == Vector2f::Zero);
		CHECK_FALSE(Vector2f::Zero == Vector2f(1.f));
	}

	SECTION("operator!=(Vector2f)")
	{
		CHECK(Vector2f::Zero != Vector2f(1.f));
		CHECK_FALSE(Vector2f::Zero != Vector2f::Zero);
	}

	SECTION("operator+(Vector2f)")
	{
		const Vector2f vector = Vector2f(1.f) + Vector2f(2.f);
		CHECK(vector == Vector2f(3.f));
	}

	SECTION("operator-(Vector2f)")
	{
		const Vector2f vector = Vector2f(3.f) - Vector2f(2.f);
		CHECK(vector == Vector2f(1.f));
	}

	SECTION("operator+=(Vector2f)")
	{
		Vector2f vector = Vector2f(1.f);
		vector += Vector2f(2.f);
		CHECK(vector == Vector2f(3.f));
	}

	SECTION("operator-=(Vector2f)")
	{
		Vector2f vector = Vector2f(3.f);
		vector -= Vector2f(2.f);
		CHECK(vector == Vector2f(1.f));
	}

	SECTION("operator*(float)")
	{
		const Vector2f vector = Vector2f(2.f) * 3.f;
		CHECK(vector == Vector2f(6.f));
	}

	SECTION("operator/(float)")
	{
		const Vector2f vector = Vector2f(6.f) / 3.f;
		CHECK(vector == Vector2f(2.f));
	}

	SECTION("operator*=(float)")
	{
		Vector2f vector = Vector2f(2.f);
		vector *= 3.f;
		CHECK(vector == Vector2f(6.f));
	}

	SECTION("operator/=(float)")
	{
		Vector2f vector = Vector2f(6.f);
		vector /= 3.f;
		CHECK(vector == Vector2f(2.f));
	}

	SECTION("operator*(Matrix4x4)")
	{
		SECTION("Translate")
		{
			const Vector2f vectorA = Vector2f(2.f) * Matrix4x4::FromTranslate(Vector3f(4.f));
			CHECK(vectorA == Vector2f(6.f));

			const Vector2f vectorB = Vector2f(2.f) * Matrix4x4::FromTranslate(Vector3f(-4.f));
			CHECK(vectorB == Vector2f(-2.f));
		}

		SECTION("Rotate")
		{
			SECTION("X-Axis")
			{
				const Quaternion quaternion = Quaternion::FromAxisAngle(Vector3f::AxisX, math::ToRadians(90.f));

				const Vector2f vectorA = Vector2f::Zero * Matrix4x4::FromRotate(quaternion);
				CHECK(IsNearly(vectorA, Vector2f::Zero));

				const Vector2f vectorB = Vector2f::AxisX * Matrix4x4::FromRotate(quaternion);
				CHECK(IsNearly(vectorB, Vector2f::AxisX));

				const Vector2f vectorC = Vector2f::AxisY * Matrix4x4::FromRotate(quaternion);
				CHECK(IsNearly(vectorC, Vector2f::Zero));
			}

			SECTION("Y-Axis")
			{
				const Quaternion quaternion = Quaternion::FromAxisAngle(Vector3f::AxisY, math::ToRadians(90.f));

				const Vector2f vectorA = Vector2f::Zero * Matrix4x4::FromRotate(quaternion);
				CHECK(IsNearly(vectorA, Vector2f::Zero));

				const Vector2f vectorB = Vector2f::AxisX * Matrix4x4::FromRotate(quaternion);
				CHECK(IsNearly(vectorB, Vector2f::Zero));

				const Vector2f vectorC = Vector2f::AxisY * Matrix4x4::FromRotate(quaternion);
				CHECK(IsNearly(vectorC, Vector2f::AxisY));
			}

			SECTION("Z-Axis")
			{
				const Quaternion quaternion = Quaternion::FromAxisAngle(Vector3f::AxisZ, math::ToRadians(90.f));

				const Vector2f vectorA = Vector2f::Zero * Matrix4x4::FromRotate(quaternion);
				CHECK(IsNearly(vectorA, Vector2f::Zero));

				const Vector2f vectorB = Vector2f::AxisX * Matrix4x4::FromRotate(quaternion);
				CHECK(IsNearly(vectorB, Vector2f::AxisY));

				const Vector2f vectorC = Vector2f::AxisY * Matrix4x4::FromRotate(quaternion);
				CHECK(IsNearly(vectorC, -Vector2f::AxisX));
			}
		}

		SECTION("Scale(Vector3f)")
		{
			const Vector2f vectorA = Vector2f(2.f) * Matrix4x4::FromScale(Vector3f(4.f, 3.f, 2.f));
			CHECK(vectorA == Vector2f(8.f, 6.f));

			const Vector2f vectorB = Vector2f(8.f) * Matrix4x4::FromScale(Vector3f(0.5f, 0.25f, 0.125f));
			CHECK(vectorB == Vector2f(4.f, 2.f));

			const Vector2f vectorC = Vector2f(2.f) * Matrix4x4::FromScale(Vector3f(-4.f, -3.f, -2.f));
			CHECK(vectorC == Vector2f(-8.f, -6.f));
		}

		SECTION("Scale(float)")
		{
			const Vector2f vectorA = Vector2f(2.f) * Matrix4x4::FromScale(3.f);
			CHECK(vectorA == Vector2f(6.f));

			const Vector2f vectorB = Vector2f(6.f) * Matrix4x4::FromScale(0.5f);
			CHECK(vectorB == Vector2f(3.f));

			const Vector2f vectorC = Vector2f(2.f) * Matrix4x4::FromScale(-3.f);
			CHECK(vectorC == Vector2f(-6.f));
		}

		SECTION("Translate & Scale")
		{
			const Vector2f vector = Vector2f(2.f) * Matrix4x4::FromTransform(Vector3f(1.f), Quaternion::Identity, 2.f);
			CHECK(vector == Vector2f(5.f));
		}

		SECTION("Translate & Rotate")
		{
			const Quaternion quaternion = Quaternion::FromAxisAngle(Vector3f::AxisZ, math::ToRadians(90.f));
			const Vector2f vector = Vector2f(2.f) * Matrix4x4::FromTransform(Vector3f(1.f), quaternion, 1.f);
			CHECK(IsNearly(vector, Vector2f(-1.f, 3.f)));
		}

		SECTION("Rotate & Scale")
		{
			const Quaternion quaternion = Quaternion::FromAxisAngle(Vector3f::AxisZ, math::ToRadians(90.f));
			const Vector2f vector = Vector2f(2.f) * Matrix4x4::FromTransform(Vector3f(0.f), quaternion, 2.f);
			CHECK(IsNearly(vector, Vector2f(-4.f, 4.f)));
		}

		SECTION("Translate, Rotate & Scale")
		{
			const Quaternion quaternion = Quaternion::FromAxisAngle(Vector3f::AxisZ, math::ToRadians(90.f));
			const Vector2f vector = Vector2f(2.f) * Matrix4x4::FromTransform(Vector3f(2.f), quaternion, 2.f);
			CHECK(IsNearly(vector, Vector2f(-2.f, 6.f)));
		}
	}



	SECTION("operator-()")
	{
		CHECK(-Vector2f(1.f) == Vector2f(-1.f));
		CHECK(-Vector2f(-1.f) == Vector2f(1.f));
	}

	SECTION("Length")
	{
		CHECK(Vector2f::Zero.Length() == 0.f);
		CHECK(Vector2f::AxisX.Length() == 1.f);
		CHECK(Vector2f::AxisY.Length() == 1.f);
		CHECK(Vector2f(10.f, 0.f).Length() == 10.f);
		CHECK(Vector2f(0.f, 10.f).Length() == 10.f);
	}

	SECTION("LengthSqr")
	{
		CHECK(Vector2f::Zero.LengthSqr() == 0.f);
		CHECK(Vector2f::AxisX.LengthSqr() == 1.f);
		CHECK(Vector2f::AxisY.LengthSqr() == 1.f);
		CHECK(Vector2f(10.f, 0.f).LengthSqr() == 100.f);
		CHECK(Vector2f(0.f, 10.f).LengthSqr() == 100.f);
	}

	SECTION("Normalize")
	{
		Vector2f vectorA = Vector2f::Zero;
		vectorA.Normalize();
		CHECK(vectorA.Length() == 0.f);
		CHECK(vectorA == Vector2f::Zero);

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
	
	SECTION("NormalizeUnsafe")
	{
		Vector2f vectorB = Vector2f::AxisX;
		vectorB.NormalizeUnsafe();
		CHECK(vectorB.Length() == 1.f);
		CHECK(vectorB == Vector2f::AxisX);

		Vector2f vectorC = Vector2f::AxisY;
		vectorC.NormalizeUnsafe();
		CHECK(vectorC.Length() == 1.f);
		CHECK(vectorC == Vector2f::AxisY);

		Vector2f vectorD = Vector2f::AxisX * 10.f;
		vectorD.NormalizeUnsafe();
		CHECK(vectorD.Length() == 1.f);
		CHECK(vectorD == Vector2f::AxisX);

		Vector2f vectorE = Vector2f::AxisY * 10.f;
		vectorE.NormalizeUnsafe();
		CHECK(vectorE.Length() == 1.f);
		CHECK(vectorE == Vector2f::AxisY);
	}

	SECTION("Normalized")
	{
		const Vector2f vectorA = (Vector2f::Zero).Normalized();
		CHECK(vectorA.Length() == 0.f);
		CHECK(vectorA == Vector2f::Zero);

		const Vector2f vectorB = (Vector2f::AxisX).Normalized();
		CHECK(vectorB.Length() == 1.f);
		CHECK(vectorB == Vector2f::AxisX);

		const Vector2f vectorC = (Vector2f::AxisY).Normalized();
		CHECK(vectorC.Length() == 1.f);
		CHECK(vectorC == Vector2f::AxisY);

		const Vector2f vectorD = (Vector2f::AxisX * 10.f).Normalized();
		CHECK(vectorD.Length() == 1.f);
		CHECK(vectorD == Vector2f::AxisX);

		const Vector2f vectorE = (Vector2f::AxisY * 10.f).Normalized();
		CHECK(vectorE.Length() == 1.f);
		CHECK(vectorE == Vector2f::AxisY);
	}

	SECTION("NormalizedUnsafe")
	{
		const Vector2f vectorB = (Vector2f::AxisX).NormalizedUnsafe();
		CHECK(vectorB.Length() == 1.f);
		CHECK(vectorB == Vector2f::AxisX);

		const Vector2f vectorC = (Vector2f::AxisY).NormalizedUnsafe();
		CHECK(vectorC.Length() == 1.f);
		CHECK(vectorC == Vector2f::AxisY);

		const Vector2f vectorD = (Vector2f::AxisX * 10.f).NormalizedUnsafe();
		CHECK(vectorD.Length() == 1.f);
		CHECK(vectorD == Vector2f::AxisX);

		const Vector2f vectorE = (Vector2f::AxisY * 10.f).NormalizedUnsafe();
		CHECK(vectorE.Length() == 1.f);
		CHECK(vectorE == Vector2f::AxisY);
	}

	SECTION("X0Y")
	{
		CHECK(Vector2f(1.f, 2.f).X0Y() == Vector3f(1.f, 0.f, 2.f));
	}

	SECTION("XY0")
	{
		CHECK(Vector2f(1.f, 2.f).XY0() == Vector3f(1.f, 2.f, 0.f));
	}
}
