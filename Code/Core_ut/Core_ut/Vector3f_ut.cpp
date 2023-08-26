#include <Catch2/catch.hpp>

#include <Core/Matrix.h>
#include <Core/Vector.h>
#include <Core/VectorHelpers.h>

TEST_CASE("Vector3f")
{
	SECTION("Alignment")
	{
		CHECK(alignof(Vector3f) == 4);
	}

	SECTION("Vector3f::AxisX")
	{
		CHECK(Vector3f::AxisX == Vector3f(1.f, 0.f, 0.f));
	}

	SECTION("Vector3f::AxisY")
	{
		CHECK(Vector3f::AxisY == Vector3f(0.f, 1.f, 0.f));
	}

	SECTION("Vector3f::AxisZ")
	{
		CHECK(Vector3f::AxisZ == Vector3f(0.f, 0.f, 1.f));
	}

	SECTION("Vector3f::Zero")
	{
		CHECK(Vector3f::Zero == Vector3f(0.f));
	}

	SECTION("Vector3f(float)")
	{
		const Vector3f vector(1.f);
		CHECK(vector.x == 1.f);
		CHECK(vector.y == 1.f);
		CHECK(vector.z == 1.f);
	}

	SECTION("Vector3f(float, float, float)")
	{
		const Vector3f vector(1.f, 2.f, 3.f);
		CHECK(vector.x == 1.f);
		CHECK(vector.y == 2.f);
		CHECK(vector.z == 3.f);
	}

	SECTION("Vector3f(Vector2f, float)")
	{
		const Vector3f vector(Vector2f(1.f, 2.f), 3.f);
		CHECK(vector.x == 1.f);
		CHECK(vector.y == 2.f);
		CHECK(vector.z == 3.f);
	}

	SECTION("operator==(Vector3f)")
	{
		CHECK(Vector3f(0.f) == Vector3f(0.f));
		CHECK_FALSE(Vector3f(0.f) == Vector3f(1.f));
	}

	SECTION("operator!=(Vector3f)")
	{
		CHECK(Vector3f(0.f) != Vector3f(1.f));
		CHECK_FALSE(Vector3f(0.f) != Vector3f(0.f));
	}

	SECTION("operator+(Vector3f)")
	{
		Vector3f vector = Vector3f(1.f) + Vector3f(2.f);
		CHECK(vector == Vector3f(3.f));
	}

	SECTION("operator-(Vector3f)")
	{
		Vector3f vector = Vector3f(3.f) - Vector3f(2.f);
		CHECK(vector == Vector3f(1.f));
	}

	SECTION("operator+=(Vector3f)")
	{
		Vector3f vector = Vector3f(1.f);
		vector += Vector3f(2.f);
		CHECK(vector == Vector3f(3.f));
	}

	SECTION("operator-=(Vector3f)")
	{
		Vector3f vector = Vector3f(3.f);
		vector -= Vector3f(2.f);
		CHECK(vector == Vector3f(1.f));
	}

	SECTION("operator*(float)")
	{
		Vector3f vector = Vector3f(2.f) * 3.f;
		CHECK(vector == Vector3f(6.f));
	}

	SECTION("operator/(float)")
	{
		Vector3f vector = Vector3f(6.f) / 3.f;
		CHECK(vector == Vector3f(2.f));
	}

	SECTION("operator*=(float)")
	{
		Vector3f vector = Vector3f(2.f);
		vector *= 3.f;
		CHECK(vector == Vector3f(6.f));
	}

	SECTION("operator/=(float)")
	{
		Vector3f vector = Vector3f(6.f);
		vector /= 3.f;
		CHECK(vector == Vector3f(2.f));
	}

	SECTION("operator*(Matrix4x4)")
	{
		SECTION("Translate")
		{
			const Vector3f vectorA = Vector3f(2.f) * Matrix4x4::FromTranslate(Vector3f(4.f));
			CHECK(vectorA == Vector3f(6.f));

			const Vector3f vectorB = Vector3f(2.f) * Matrix4x4::FromTranslate(Vector3f(-4.f));
			CHECK(vectorB == Vector3f(-2.f));
		}

		SECTION("Rotate")
		{
			SECTION("X-Axis")
			{
				const Quaternion quaternion = Quaternion::FromAxisAngle(Vector3f::AxisX, math::ToRadians(90.f));

				const Vector3f vectorA = Vector3f(1.f, 0.f, 0.f) * Matrix4x4::FromRotate(quaternion);
				CHECK(IsNearly(vectorA, Vector3f(1.f, 0.f, 0.f)));

				const Vector3f vectorB = Vector3f(0.f, 1.f, 0.f) * Matrix4x4::FromRotate(quaternion);
				CHECK(IsNearly(vectorB, Vector3f(0.f, 0.f, 1.f)));

				const Vector3f vectorC = Vector3f(0.f, 0.f, 1.f) * Matrix4x4::FromRotate(quaternion);
				CHECK(IsNearly(vectorC, Vector3f(0.f, -1.f, 0.f)));
			}

			SECTION("Y-Axis")
			{
				const Quaternion quaternion = Quaternion::FromAxisAngle(Vector3f::AxisY, math::ToRadians(90.f));

				const Vector3f vectorA = Vector3f(1.f, 0.f, 0.f) * Matrix4x4::FromRotate(quaternion);
				CHECK(IsNearly(vectorA, Vector3f(0.f, 0.f, -1.f)));

				const Vector3f vectorB = Vector3f(0.f, 1.f, 0.f) * Matrix4x4::FromRotate(quaternion);
				CHECK(IsNearly(vectorB, Vector3f(0.f, 1.f, 0.f)));

				const Vector3f vectorC = Vector3f(0.f, 0.f, 1.f) * Matrix4x4::FromRotate(quaternion);
				CHECK(IsNearly(vectorC, Vector3f(1.f, 0.f, 0.f)));
			}

			SECTION("Z-Axis")
			{
				const Quaternion quaternion = Quaternion::FromAxisAngle(Vector3f::AxisZ, math::ToRadians(90.f));

				const Vector3f vectorA = Vector3f(1.f, 0.f, 0.f) * Matrix4x4::FromRotate(quaternion);
				CHECK(IsNearly(vectorA, Vector3f(0.f, 1.f, 0.f)));

				const Vector3f vectorB = Vector3f(0.f, 1.f, 0.f) * Matrix4x4::FromRotate(quaternion);
				CHECK(IsNearly(vectorB, Vector3f(-1.f, 0.f, 0.f)));

				const Vector3f vectorC = Vector3f(0.f, 0.f, 1.f) * Matrix4x4::FromRotate(quaternion);
				CHECK(IsNearly(vectorC, Vector3f(0.f, 0.f, 1.f)));
			}

			SECTION("XYZ-Axis")
			{
				const Quaternion quaternion = Quaternion::FromRotator({ 90.f});
				const Vector3f vector = Vector3f(1.f) * Matrix4x4::FromRotate(quaternion);
				CHECK(IsNearly(vector, Vector3f(1.f, 1.f, -1.f)));
			}
		}

		SECTION("Scale(Vector3f)")
		{
			const Vector3f vectorA = Vector3f(2.f) * Matrix4x4::FromScale({ 3.f });
			CHECK(vectorA == Vector3f(6.f));

			const Vector3f vectorB = Vector3f(6.f) * Matrix4x4::FromScale({ 0.5f });
			CHECK(vectorB == Vector3f(3.f));

			const Vector3f vectorC = Vector3f(2.f) * Matrix4x4::FromScale({ -3.f });
			CHECK(vectorC == Vector3f(-6.f));
		}

		SECTION("Scale(float)")
		{
			const Vector3f vectorA = Vector3f(2.f) * Matrix4x4::FromScale(3.f);
			CHECK(vectorA == Vector3f(6.f));

			const Vector3f vectorB = Vector3f(6.f) * Matrix4x4::FromScale(0.5f);
			CHECK(vectorB == Vector3f(3.f));

			const Vector3f vectorC = Vector3f(2.f) * Matrix4x4::FromScale(-3.f);
			CHECK(vectorC == Vector3f(-6.f));
		}

		SECTION("Translate & Rotate")
		{
			const Quaternion quaternion = Quaternion::FromAxisAngle(Vector3f::AxisZ, math::ToRadians(90.f));
			const Vector3f vector = Vector3f(2.f) * Matrix4x4::FromTransform(Vector3f(1.f), quaternion, 1.f);
			CHECK(IsNearly(vector, Vector3f(-1.f, 3.f, 3.f)));
		}

		SECTION("Translate & Scale")
		{
			const Vector3f vector = Vector3f(2.f) * Matrix4x4::FromTransform(Vector3f(1.f), Quaternion::Identity, 2.f);
			CHECK(vector == Vector3f(5.f));
		}

		SECTION("Rotate & Scale")
		{
			const Quaternion quaternion = Quaternion::FromAxisAngle(Vector3f::AxisZ, math::ToRadians(90.f));
			const Vector3f vector = Vector3f(2.f) * Matrix4x4::FromTransform(Vector3f(0.f), quaternion, 2.f);
			CHECK(IsNearly(vector, Vector3f(-4.f, 4.f, 4.f)));
		}

		SECTION("Translate, Rotate & Scale")
		{
			const Quaternion quaternion = Quaternion::FromAxisAngle(Vector3f::AxisZ, math::ToRadians(90.f));
			const Vector3f vector = Vector3f(2.f) * Matrix4x4::FromTransform(Vector3f(2.f), quaternion, 2.f);
			CHECK(IsNearly(vector, Vector3f(-2.f, 6.f, 6.f)));
		}
	}

	SECTION("operator-()")
	{
		CHECK(-Vector3f(1.f) == Vector3f(-1.f));
		CHECK(-Vector3f(-1.f) == Vector3f(1.f));
	}

	SECTION("Cross")
	{
		CHECK(Vector3f::Cross(Vector3f::AxisX, Vector3f::AxisY) == Vector3f::AxisZ);
		CHECK(Vector3f::Cross(Vector3f::AxisY, Vector3f::AxisZ) == Vector3f::AxisX);
		CHECK(Vector3f::Cross(Vector3f::AxisZ, Vector3f::AxisX) == Vector3f::AxisY);
	}

	SECTION("Dot")
	{
		CHECK(Vector3f::Dot(Vector3f::AxisX, Vector3f::AxisX) == 1.f);
		CHECK(Vector3f::Dot(Vector3f::AxisX, Vector3f::AxisY) == 0.f);
		CHECK(Vector3f::Dot(Vector3f::AxisX, Vector3f::AxisZ) == 0.f);
		CHECK(Vector3f::Dot(Vector3f::AxisX, -Vector3f::AxisX) == -1.f);
	}

	SECTION("Length")
	{
		CHECK(Vector3f::Zero.Length() == 0.f);
		CHECK(Vector3f::AxisX.Length() == 1.f);
		CHECK(Vector3f::AxisY.Length() == 1.f);
		CHECK(Vector3f::AxisZ.Length() == 1.f);
		CHECK(Vector3f(10.f, 0.f, 0.f).Length() == 10.f);
		CHECK(Vector3f(0.f, 10.f, 0.f).Length() == 10.f);
		CHECK(Vector3f(0.f, 0.f, 10.f).Length() == 10.f);
	}

	SECTION("LengthSqr")
	{
		CHECK(Vector3f::Zero.LengthSqr() == 0.f);
		CHECK(Vector3f::AxisX.LengthSqr() == 1.f);
		CHECK(Vector3f::AxisY.LengthSqr() == 1.f);
		CHECK(Vector3f::AxisZ.LengthSqr() == 1.f);
		CHECK(Vector3f(10.f, 0.f, 0.f).LengthSqr() == 100.f);
		CHECK(Vector3f(0.f, 10.f, 0.f).LengthSqr() == 100.f);
		CHECK(Vector3f(0.f, 0.f, 10.f).LengthSqr() == 100.f);
	}

	SECTION("Normalize")
	{
		Vector3f vectorA = Vector3f::Zero;
		vectorA.Normalize();
		CHECK(vectorA.Length() == 0.f);
		CHECK(vectorA == Vector3f::Zero);

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

	SECTION("NormalizeUnsafe")
	{
		Vector3f vectorB = Vector3f::AxisX;
		vectorB.NormalizeUnsafe();
		CHECK(vectorB.Length() == 1.f);
		CHECK(vectorB == Vector3f::AxisX);

		Vector3f vectorC = Vector3f::AxisY;
		vectorC.NormalizeUnsafe();
		CHECK(vectorC.Length() == 1.f);
		CHECK(vectorC == Vector3f::AxisY);

		Vector3f vectorD = Vector3f::AxisZ;
		vectorD.NormalizeUnsafe();
		CHECK(vectorD.Length() == 1.f);
		CHECK(vectorD == Vector3f::AxisZ);

		Vector3f vectorF = Vector3f::AxisX * 10.f;
		vectorF.NormalizeUnsafe();
		CHECK(vectorF.Length() == 1.f);
		CHECK(vectorF == Vector3f::AxisX);

		Vector3f vectorG = Vector3f::AxisY * 10.f;
		vectorG.NormalizeUnsafe();
		CHECK(vectorG.Length() == 1.f);
		CHECK(vectorG == Vector3f::AxisY);

		Vector3f vectorH = Vector3f::AxisZ * 10.f;
		vectorH.NormalizeUnsafe();
		CHECK(vectorH.Length() == 1.f);
		CHECK(vectorH == Vector3f::AxisZ);
	}

	SECTION("Normalized")
	{
		const Vector3f vectorA = (Vector3f::Zero).Normalized();
		CHECK(vectorA.Length() == 0.f);
		CHECK(vectorA == Vector3f::Zero);

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

	SECTION("NormalizedUnsafe")
	{
		const Vector3f vectorB = (Vector3f::AxisX).NormalizedUnsafe();
		CHECK(vectorB.Length() == 1.f);
		CHECK(vectorB == Vector3f::AxisX);

		const Vector3f vectorC = (Vector3f::AxisY).NormalizedUnsafe();
		CHECK(vectorC.Length() == 1.f);
		CHECK(vectorC == Vector3f::AxisY);

		const Vector3f vectorD = (Vector3f::AxisZ).NormalizedUnsafe();
		CHECK(vectorD.Length() == 1.f);
		CHECK(vectorD == Vector3f::AxisZ);

		const Vector3f vectorF = (Vector3f::AxisX * 10.f).NormalizedUnsafe();
		CHECK(vectorF.Length() == 1.f);
		CHECK(vectorF == Vector3f::AxisX);

		const Vector3f vectorG = (Vector3f::AxisY * 10.f).NormalizedUnsafe();
		CHECK(vectorG.Length() == 1.f);
		CHECK(vectorG == Vector3f::AxisY);

		const Vector3f vectorH = (Vector3f::AxisZ * 10.f).NormalizedUnsafe();
		CHECK(vectorH.Length() == 1.f);
		CHECK(vectorH == Vector3f::AxisZ);
	}

	SECTION("XY")
	{
		CHECK(Vector3f(1.f, 2.f, 3.f).XY() == Vector2f(1.f, 2.f));
	}

	SECTION("XZ")
	{
		CHECK(Vector3f(1.f, 2.f, 3.f).XZ() == Vector2f(1.f, 3.f));
	}

	SECTION("X0Z")
	{
		CHECK(Vector3f(1.f, 2.f, 3.f).X0Z() == Vector3f(1.f, 0.f, 3.f));
	}
}
