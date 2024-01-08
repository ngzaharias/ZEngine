#include <Catch2/catch.hpp>

#include <Core/Common.h>
#include <Core/Matrix.h>
#include <Core/Vector.h>
#include <Core/VectorMath.h>

TEST_CASE("math::Vector3f::Constructors")
{
	{
		INFO("Alignment");
		CHECK(alignof(Vector3f) == 4);
	}

	{
		INFO("Vector3f::AxisX");
		CHECK(Vector3f::AxisX == Vector3f(1.f, 0.f, 0.f));
	}

	{
		INFO("Vector3f::AxisY");
		CHECK(Vector3f::AxisY == Vector3f(0.f, 1.f, 0.f));
	}

	{
		INFO("Vector3f::AxisZ");
		CHECK(Vector3f::AxisZ == Vector3f(0.f, 0.f, 1.f));
	}

	{
		INFO("Vector3f::Zero");
		CHECK(Vector3f::Zero == Vector3f(0.f));
	}

	{
		INFO("Vector3f(float)");
		const Vector3f vector(1.f);
		CHECK(vector.x == 1.f);
		CHECK(vector.y == 1.f);
		CHECK(vector.z == 1.f);
	}

	{
		INFO("Vector3f(float, float, float)");
		const Vector3f vector(1.f, 2.f, 3.f);
		CHECK(vector.x == 1.f);
		CHECK(vector.y == 2.f);
		CHECK(vector.z == 3.f);
	}

	{
		INFO("Vector3f(Vector2f, float)");
		const Vector3f vector(Vector2f(1.f, 2.f), 3.f);
		CHECK(vector.x == 1.f);
		CHECK(vector.y == 2.f);
		CHECK(vector.z == 3.f);
	}
}

TEST_CASE("math::Vector3f::Operators")
{
	{
		INFO("operator==(Vector3f)");
		CHECK(Vector3f(0.f) == Vector3f(0.f));
		CHECK_FALSE(Vector3f(0.f) == Vector3f(1.f));
	}

	{
		INFO("operator!=(Vector3f)");
		CHECK(Vector3f(0.f) != Vector3f(1.f));
		CHECK_FALSE(Vector3f(0.f) != Vector3f(0.f));
	}

	{
		INFO("operator+(Vector3f)");
		Vector3f vector = Vector3f(1.f) + Vector3f(2.f);
		CHECK(vector == Vector3f(3.f));
	}

	{
		INFO("operator-(Vector3f)");
		Vector3f vector = Vector3f(3.f) - Vector3f(2.f);
		CHECK(vector == Vector3f(1.f));
	}

	{
		INFO("operator+=(Vector3f)");
		Vector3f vector = Vector3f(1.f);
		vector += Vector3f(2.f);
		CHECK(vector == Vector3f(3.f));
	}

	{
		INFO("operator-=(Vector3f)");
		Vector3f vector = Vector3f(3.f);
		vector -= Vector3f(2.f);
		CHECK(vector == Vector3f(1.f));
	}

	{
		INFO("operator*(float)");
		Vector3f vector = Vector3f(2.f) * 3.f;
		CHECK(vector == Vector3f(6.f));
	}

	{
		INFO("operator/(float)");
		Vector3f vector = Vector3f(6.f) / 3.f;
		CHECK(vector == Vector3f(2.f));
	}

	{
		INFO("operator*=(float)");
		Vector3f vector = Vector3f(2.f);
		vector *= 3.f;
		CHECK(vector == Vector3f(6.f));
	}

	{
		INFO("operator/=(float)");
		Vector3f vector = Vector3f(6.f);
		vector /= 3.f;
		CHECK(vector == Vector3f(2.f));
	}

	{
		INFO("operator*(Matrix4x4)");
		{
			INFO("Translate");
			const Vector3f vectorA = Vector3f(2.f) * Matrix4x4::FromTranslate(Vector3f(4.f));
			CHECK(vectorA == Vector3f(6.f));

			const Vector3f vectorB = Vector3f(2.f) * Matrix4x4::FromTranslate(Vector3f(-4.f));
			CHECK(vectorB == Vector3f(-2.f));
		}

		{
			INFO("Rotate");
			{
				INFO("X-Axis");
				const Quaternion quaternion = Quaternion::FromAxisAngle(Vector3f::AxisX, math::ToRadians(90.f));

				const Vector3f vectorA = Vector3f(1.f, 0.f, 0.f) * Matrix4x4::FromRotate(quaternion);
				CHECK(IsNearly(vectorA, Vector3f(1.f, 0.f, 0.f)));

				const Vector3f vectorB = Vector3f(0.f, 1.f, 0.f) * Matrix4x4::FromRotate(quaternion);
				CHECK(IsNearly(vectorB, Vector3f(0.f, 0.f, 1.f)));

				const Vector3f vectorC = Vector3f(0.f, 0.f, 1.f) * Matrix4x4::FromRotate(quaternion);
				CHECK(IsNearly(vectorC, Vector3f(0.f, -1.f, 0.f)));
			}

			{
				INFO("Y-Axis");
				const Quaternion quaternion = Quaternion::FromAxisAngle(Vector3f::AxisY, math::ToRadians(90.f));

				const Vector3f vectorA = Vector3f(1.f, 0.f, 0.f) * Matrix4x4::FromRotate(quaternion);
				CHECK(IsNearly(vectorA, Vector3f(0.f, 0.f, -1.f)));

				const Vector3f vectorB = Vector3f(0.f, 1.f, 0.f) * Matrix4x4::FromRotate(quaternion);
				CHECK(IsNearly(vectorB, Vector3f(0.f, 1.f, 0.f)));

				const Vector3f vectorC = Vector3f(0.f, 0.f, 1.f) * Matrix4x4::FromRotate(quaternion);
				CHECK(IsNearly(vectorC, Vector3f(1.f, 0.f, 0.f)));
			}

			{
				INFO("Z-Axis");
				const Quaternion quaternion = Quaternion::FromAxisAngle(Vector3f::AxisZ, math::ToRadians(90.f));

				const Vector3f vectorA = Vector3f(1.f, 0.f, 0.f) * Matrix4x4::FromRotate(quaternion);
				CHECK(IsNearly(vectorA, Vector3f(0.f, 1.f, 0.f)));

				const Vector3f vectorB = Vector3f(0.f, 1.f, 0.f) * Matrix4x4::FromRotate(quaternion);
				CHECK(IsNearly(vectorB, Vector3f(-1.f, 0.f, 0.f)));

				const Vector3f vectorC = Vector3f(0.f, 0.f, 1.f) * Matrix4x4::FromRotate(quaternion);
				CHECK(IsNearly(vectorC, Vector3f(0.f, 0.f, 1.f)));
			}

			{
				INFO("XYZ-Axis");
				const Quaternion quaternion = Quaternion::FromRotator({ 90.f });
				const Vector3f vector = Vector3f(1.f) * Matrix4x4::FromRotate(quaternion);
				CHECK(IsNearly(vector, Vector3f(1.f, 1.f, -1.f)));
			}
		}

		{
			INFO("Scale(Vector3f)");
			const Vector3f vectorA = Vector3f(2.f) * Matrix4x4::FromScale({ 3.f });
			CHECK(vectorA == Vector3f(6.f));

			const Vector3f vectorB = Vector3f(6.f) * Matrix4x4::FromScale({ 0.5f });
			CHECK(vectorB == Vector3f(3.f));

			const Vector3f vectorC = Vector3f(2.f) * Matrix4x4::FromScale({ -3.f });
			CHECK(vectorC == Vector3f(-6.f));
		}

		{
			INFO("Scale(float)");
			const Vector3f vectorA = Vector3f(2.f) * Matrix4x4::FromScale(3.f);
			CHECK(vectorA == Vector3f(6.f));

			const Vector3f vectorB = Vector3f(6.f) * Matrix4x4::FromScale(0.5f);
			CHECK(vectorB == Vector3f(3.f));

			const Vector3f vectorC = Vector3f(2.f) * Matrix4x4::FromScale(-3.f);
			CHECK(vectorC == Vector3f(-6.f));
		}

		{
			INFO("Translate & Rotate");
			const Quaternion quaternion = Quaternion::FromAxisAngle(Vector3f::AxisZ, math::ToRadians(90.f));
			const Vector3f vector = Vector3f(2.f) * Matrix4x4::FromTransform(Vector3f(1.f), quaternion, 1.f);
			CHECK(IsNearly(vector, Vector3f(-1.f, 3.f, 3.f)));
		}

		{
			INFO("Translate & Scale");
			const Vector3f vector = Vector3f(2.f) * Matrix4x4::FromTransform(Vector3f(1.f), Quaternion::Identity, 2.f);
			CHECK(vector == Vector3f(5.f));
		}

		{
			INFO("Rotate & Scale");
			const Quaternion quaternion = Quaternion::FromAxisAngle(Vector3f::AxisZ, math::ToRadians(90.f));
			const Vector3f vector = Vector3f(2.f) * Matrix4x4::FromTransform(Vector3f(0.f), quaternion, 2.f);
			CHECK(IsNearly(vector, Vector3f(-4.f, 4.f, 4.f)));
		}

		{
			INFO("Translate, Rotate & Scale");
			const Quaternion quaternion = Quaternion::FromAxisAngle(Vector3f::AxisZ, math::ToRadians(90.f));
			const Vector3f vector = Vector3f(2.f) * Matrix4x4::FromTransform(Vector3f(2.f), quaternion, 2.f);
			CHECK(IsNearly(vector, Vector3f(-2.f, 6.f, 6.f)));
		}
	}

	{
		INFO("operator-()");
		CHECK(-Vector3f(1.f) == Vector3f(-1.f));
		CHECK(-Vector3f(-1.f) == Vector3f(1.f));
	}
}

TEST_CASE("math::Vector3f::Length")
{
	CHECK(Vector3f::Zero.Length() == 0.f);
	CHECK(Vector3f::AxisX.Length() == 1.f);
	CHECK(Vector3f::AxisY.Length() == 1.f);
	CHECK(Vector3f::AxisZ.Length() == 1.f);
	CHECK(Vector3f(10.f, 0.f, 0.f).Length() == 10.f);
	CHECK(Vector3f(0.f, 10.f, 0.f).Length() == 10.f);
	CHECK(Vector3f(0.f, 0.f, 10.f).Length() == 10.f);
}

TEST_CASE("math::Vector3f::LengthXY")
{
}

TEST_CASE("math::Vector3f::LengthXZ")
{
}

TEST_CASE("math::Vector3f::LengthSqr")
{
	CHECK(Vector3f::Zero.LengthSqr() == 0.f);
	CHECK(Vector3f::AxisX.LengthSqr() == 1.f);
	CHECK(Vector3f::AxisY.LengthSqr() == 1.f);
	CHECK(Vector3f::AxisZ.LengthSqr() == 1.f);
	CHECK(Vector3f(10.f, 0.f, 0.f).LengthSqr() == 100.f);
	CHECK(Vector3f(0.f, 10.f, 0.f).LengthSqr() == 100.f);
	CHECK(Vector3f(0.f, 0.f, 10.f).LengthSqr() == 100.f);
}

TEST_CASE("math::Vector3f::LengthXYSqr")
{
}

TEST_CASE("math::Vector3f::LengthXZSqr")
{
}

TEST_CASE("math::Vector3f::Normalize")
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

TEST_CASE("math::Vector3f::Normalized")
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

TEST_CASE("math::Vector3f::XY")
{
	CHECK(Vector3f(1.f, 2.f, 3.f).XY() == Vector2f(1.f, 2.f));
}

TEST_CASE("math::Vector3f::XZ")
{
	CHECK(Vector3f(1.f, 2.f, 3.f).XZ() == Vector2f(1.f, 3.f));
}

TEST_CASE("math::Vector3f::XY0")
{
}

TEST_CASE("math::Vector3f::X0Z")
{
	CHECK(Vector3f(1.f, 2.f, 3.f).X0Z() == Vector3f(1.f, 0.f, 3.f));
}
