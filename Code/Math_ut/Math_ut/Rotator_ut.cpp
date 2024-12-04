#include <Catch2/catch.hpp>

#include "Math/Matrix.h"
#include "Math/Rotator.h"
#include "Math/Quaternion.h"

TEST_CASE("Rotator. Rotator(float).")
{
	Rotator rotator = Rotator(45.f);
	CHECK(rotator.m_Pitch == 45.f);
	CHECK(rotator.m_Yaw == 45.f);
	CHECK(rotator.m_Roll == 45.f);
}

TEST_CASE("Rotator. Rotator(float, float, float).")
{
	Rotator rotator = Rotator(10.f, 20.f, 30.f);
	CHECK(rotator.m_Pitch == 10.f);
	CHECK(rotator.m_Yaw == 20.f);
	CHECK(rotator.m_Roll == 30.f);
}

TEST_CASE("Rotator. operator==(Rotator).")
{
	CHECK(Rotator(0.f) == Rotator(0.f));
	CHECK_FALSE(Rotator(0.f) == Rotator(1.f));
}

TEST_CASE("Rotator. operator!=(Rotator).")
{
	CHECK(Rotator(0.f) != Rotator(1.f));
	CHECK_FALSE(Rotator(0.f) != Rotator(0.f));
}

TEST_CASE("Rotator. operator+(Rotator).")
{
	Rotator rotator =
		Rotator(1.f) +
		Rotator(2.f);
	CHECK(rotator == Rotator(3.f));
}

TEST_CASE("Rotator. operator-(Rotator).")
{
	Rotator rotator =
		Rotator(3.f) -
		Rotator(2.f);
	CHECK(rotator == Rotator(1.f));
}

TEST_CASE("Rotator. operator*(float).")
{
	Rotator rotator = Rotator(2.f) * 3.f;
	CHECK(rotator == Rotator(6.f));
}

TEST_CASE("Rotator. operator/(float).")
{
	Rotator rotator = Rotator(6.f) / 3.f;
	CHECK(rotator == Rotator(2.f));
}

TEST_CASE("Rotator. operator+=(Rotator).")
{
	Rotator rotator = Rotator(1.f);
	rotator += Rotator(2.f);
	CHECK(rotator == Rotator(3.f));
}

TEST_CASE("Rotator. operator-=(Rotator).")
{
	Rotator rotator = Rotator(3.f);
	rotator -= Rotator(2.f);
	CHECK(rotator == Rotator(1.f));
}

TEST_CASE("Rotator. operator*=(float).")
{
	Rotator rotator = Rotator(2.f);
	rotator *= 3.f;
	CHECK(rotator == Rotator(6.f));
}

TEST_CASE("Rotator. operator/=(float).")
{
	Rotator rotator = Rotator(6.f);
	rotator /= 3.f;
	CHECK(rotator == Rotator(2.f));
}

TEST_CASE("Rotator. operator-().")
{
	Rotator rotator = -Rotator(1.f);
	CHECK(rotator == Rotator(-1.f));
}

TEST_CASE("Rotator. FromMatrix(Matrix3x3).")
{
	{
		INFO("+Pitch");
		Matrix3x3 matrix = Matrix3x3::FromRotate(Rotator(45.f, 0.f, 0.f));
		Rotator rotator = Rotator::FromMatrix(matrix);
		CHECK(rotator.m_Pitch == Approx(45.f));
		CHECK(rotator.m_Yaw == 0.f);
		CHECK(rotator.m_Roll == 0.f);
	}

	{
		INFO("-Pitch");
		Matrix3x3 matrix = Matrix3x3::FromRotate(Rotator(-45.f, 0.f, 0.f));
		Rotator rotator = Rotator::FromMatrix(matrix);
		CHECK(rotator.m_Pitch == Approx(-45.f));
		CHECK(rotator.m_Yaw == 0.f);
		CHECK(rotator.m_Roll == 0.f);
	}

	{
		INFO("+Yaw");
		Matrix3x3 matrix = Matrix3x3::FromRotate(Rotator(0.f, 45.f, 0.f));
		Rotator rotator = Rotator::FromMatrix(matrix);
		CHECK(rotator.m_Pitch == 0.f);
		CHECK(rotator.m_Yaw == Approx(45.f));
		CHECK(rotator.m_Roll == 0.f);
	}

	{
		INFO("-Yaw");
		Matrix3x3 matrix = Matrix3x3::FromRotate(Rotator(0.f, -45.f, 0.f));
		Rotator rotator = Rotator::FromMatrix(matrix);
		CHECK(rotator.m_Pitch == 0.f);
		CHECK(rotator.m_Yaw == Approx(-45.f));
		CHECK(rotator.m_Roll == 0.f);
	}

	{
		INFO("+Roll");
		Matrix3x3 matrix = Matrix3x3::FromRotate(Rotator(0.f, 0.f, 45.f));
		Rotator rotator = Rotator::FromMatrix(matrix);
		CHECK(rotator.m_Pitch == 0.f);
		CHECK(rotator.m_Yaw == 0.f);
		CHECK(rotator.m_Roll == Approx(45.f));
	}

	{
		INFO("-Roll");
		Matrix3x3 matrix = Matrix3x3::FromRotate(Rotator(0.f, 0.f, -45.f));
		Rotator rotator = Rotator::FromMatrix(matrix);
		CHECK(rotator.m_Pitch == 0.f);
		CHECK(rotator.m_Yaw == 0.f);
		CHECK(rotator.m_Roll == Approx(-45.f));
	}

	{
		INFO("+Pitch, +Yaw, +Roll");
		Matrix3x3 matrix = Matrix3x3::FromRotate(Rotator(45.f, 45.f, 45.f));
		Rotator rotator = Rotator::FromMatrix(matrix);
		CHECK(rotator.m_Pitch == Approx(45.f));
		CHECK(rotator.m_Yaw == Approx(45.f));
		CHECK(rotator.m_Roll == Approx(45.f));
	}

	{
		INFO("-Pitch, -Yaw, -Roll");
		Matrix3x3 matrix = Matrix3x3::FromRotate(Rotator(-45.f, -45.f, -45.f));
		Rotator rotator = Rotator::FromMatrix(matrix);
		CHECK(rotator.m_Pitch == Approx(-45.f));
		CHECK(rotator.m_Yaw == Approx(-45.f));
		CHECK(rotator.m_Roll == Approx(-45.f));
	}
}

TEST_CASE("Rotator. FromMatrix(Matrix4x4).")
{
	{
		INFO("+Pitch");
		Matrix4x4 matrix = Matrix4x4::FromRotate(Rotator(45.f, 0.f, 0.f));
		Rotator rotator = Rotator::FromMatrix(matrix);
		CHECK(rotator.m_Pitch == Approx(45.f));
		CHECK(rotator.m_Yaw == 0.f);
		CHECK(rotator.m_Roll == 0.f);
	}

	{
		INFO("-Pitch");
		Matrix4x4 matrix = Matrix4x4::FromRotate(Rotator(-45.f, 0.f, 0.f));
		Rotator rotator = Rotator::FromMatrix(matrix);
		CHECK(rotator.m_Pitch == Approx(-45.f));
		CHECK(rotator.m_Yaw == 0.f);
		CHECK(rotator.m_Roll == 0.f);
	}

	{
		INFO("+Yaw");
		Matrix4x4 matrix = Matrix4x4::FromRotate(Rotator(0.f, 45.f, 0.f));
		Rotator rotator = Rotator::FromMatrix(matrix);
		CHECK(rotator.m_Pitch == 0.f);
		CHECK(rotator.m_Yaw == Approx(45.f));
		CHECK(rotator.m_Roll == 0.f);
	}

	{
		INFO("-Yaw");
		Matrix4x4 matrix = Matrix4x4::FromRotate(Rotator(0.f, -45.f, 0.f));
		Rotator rotator = Rotator::FromMatrix(matrix);
		CHECK(rotator.m_Pitch == 0.f);
		CHECK(rotator.m_Yaw == Approx(-45.f));
		CHECK(rotator.m_Roll == 0.f);
	}

	{
		INFO("+Roll");
		Matrix4x4 matrix = Matrix4x4::FromRotate(Rotator(0.f, 0.f, 45.f));
		Rotator rotator = Rotator::FromMatrix(matrix);
		CHECK(rotator.m_Pitch == 0.f);
		CHECK(rotator.m_Yaw == 0.f);
		CHECK(rotator.m_Roll == Approx(45.f));
	}

	{
		INFO("-Roll");
		Matrix4x4 matrix = Matrix4x4::FromRotate(Rotator(0.f, 0.f, -45.f));
		Rotator rotator = Rotator::FromMatrix(matrix);
		CHECK(rotator.m_Pitch == 0.f);
		CHECK(rotator.m_Yaw == 0.f);
		CHECK(rotator.m_Roll == Approx(-45.f));
	}

	{
		INFO("+Pitch, +Yaw, +Roll");
		Matrix4x4 matrix = Matrix4x4::FromRotate(Rotator(45.f, 45.f, 45.f));
		Rotator rotator = Rotator::FromMatrix(matrix);
		CHECK(rotator.m_Pitch == Approx(45.f));
		CHECK(rotator.m_Yaw == Approx(45.f));
		CHECK(rotator.m_Roll == Approx(45.f));
	}

	{
		INFO("-Pitch, -Yaw, -Roll");
		Matrix4x4 matrix = Matrix4x4::FromRotate(Rotator(-45.f, -45.f, -45.f));
		Rotator rotator = Rotator::FromMatrix(matrix);
		CHECK(rotator.m_Pitch == Approx(-45.f));
		CHECK(rotator.m_Yaw == Approx(-45.f));
		CHECK(rotator.m_Roll == Approx(-45.f));
	}
}

TEST_CASE("Rotator. FromQuaternion.")
{
	{
		INFO("Pitch");
		Quaternion quaternion = Quaternion::FromRotator(Rotator(45.f, 0.f, 0.f));
		Rotator rotator = Rotator::FromQuaternion(quaternion);
		CHECK(rotator.m_Pitch == Approx(45.f));
		CHECK(rotator.m_Yaw == 0.f);
		CHECK(rotator.m_Roll == 0.f);
	}

	{
		INFO("Yaw");
		Quaternion quaternion = Quaternion::FromRotator(Rotator(0.f, 45.f, 0.f));
		Rotator rotator = Rotator::FromQuaternion(quaternion);
		CHECK(rotator.m_Pitch == 0.f);
		CHECK(rotator.m_Yaw == Approx(45.f));
		CHECK(rotator.m_Roll == 0.f);
	}

	{
		INFO("Roll");
		Quaternion quaternion = Quaternion::FromRotator(Rotator(0.f, 0.f, 45.f));
		Rotator rotator = Rotator::FromQuaternion(quaternion);
		CHECK(rotator.m_Pitch == 0.f);
		CHECK(rotator.m_Yaw == 0.f);
		CHECK(rotator.m_Roll == Approx(45.f));
	}

	{
		INFO("Pitch, Yaw, Roll");
		Quaternion quaternion = Quaternion::FromRotator(Rotator(45.f, 45.f, 45.f));
		Rotator rotator = Rotator::FromQuaternion(quaternion);
		CHECK(rotator.m_Pitch == Approx(45.f));
		CHECK(rotator.m_Yaw == Approx(45.f));
		CHECK(rotator.m_Roll == Approx(45.f));

		quaternion = Quaternion::FromRotator(Rotator(45.f, -45.f, 0.f));
		rotator = Rotator::FromQuaternion(quaternion);
		CHECK(rotator.m_Pitch == Approx(45.f));
		CHECK(rotator.m_Yaw == Approx(-45.f));
		CHECK(rotator.m_Roll == Approx(0.f));
	}
}
