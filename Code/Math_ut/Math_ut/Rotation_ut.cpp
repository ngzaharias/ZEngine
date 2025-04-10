#include "MathPCH.h"

#include "Math/Matrix.h"
#include "Math/Rotator.h"
#include "Math/Quaternion.h"

namespace
{
	void Convert(Matrix3x3& lhs, const Matrix3x3& rhs)
	{
		lhs = rhs;
	}
	void Convert(Matrix3x3& lhs, const Matrix4x4& rhs)
	{
		lhs = Matrix3x3(rhs);
	}
	void Convert(Matrix3x3& lhs, const Rotator& rhs)
	{
		lhs = Matrix3x3::FromRotate(rhs);
	}
	void Convert(Matrix3x3& lhs, const Quaternion& rhs)
	{
		lhs = Matrix3x3::FromRotate(rhs);
	}

	void Convert(Matrix4x4& lhs, const Matrix3x3& rhs)
	{
		lhs = Matrix4x4(rhs);
	}
	void Convert(Matrix4x4& lhs, const Matrix4x4& rhs)
	{
		lhs = rhs;
	}
	void Convert(Matrix4x4& lhs, const Rotator& rhs)
	{
		lhs = Matrix4x4::FromRotate(rhs);
	}
	void Convert(Matrix4x4& lhs, const Quaternion& rhs)
	{
		lhs = Matrix4x4::FromRotate(rhs);
	}

	void Convert(Quaternion& lhs, const Matrix3x3& rhs)
	{
		lhs = Quaternion::FromMatrix(rhs);
	}
	void Convert(Quaternion& lhs, const Matrix4x4& rhs)
	{
		lhs = Quaternion::FromMatrix(rhs);
	}
	void Convert(Quaternion& lhs, const Quaternion& rhs)
	{
		lhs = rhs;
	}
	void Convert(Quaternion& lhs, const Rotator& rhs)
	{
		lhs = Quaternion::FromRotator(rhs);
	}

	void Convert(Rotator& lhs, const Matrix3x3& rhs)
	{
		lhs = Rotator::FromMatrix(rhs);
	}
	void Convert(Rotator& lhs, const Matrix4x4& rhs)
	{
		lhs = Rotator::FromMatrix(rhs);
	}
	void Convert(Rotator& lhs, const Quaternion& rhs)
	{
		lhs = Rotator::FromQuaternion(rhs);
	}
	void Convert(Rotator& lhs, const Rotator& rhs)
	{
		lhs = rhs;
	}
}

TEST_CASE("Rotation. Rotator -> Matrix3x3 -> Quaternion -> Rotator.")
{
	Rotator input = Rotator(45.f, -45.f, 0.f);

	auto matrix = Matrix3x3::FromRotate(input);
	auto quater = Quaternion::FromMatrix(matrix);
	auto output = Rotator::FromQuaternion(quater);

	CHECK(input.m_Pitch == Approx(output.m_Pitch));
	CHECK(input.m_Yaw == Approx(output.m_Yaw));
	CHECK(input.m_Roll == Approx(output.m_Roll));
}

TEST_CASE("Rotation. Rotator -> Quaternion -> Matrix3x3 -> Rotator.")
{
	Rotator input = Rotator(45.f, -45.f, 0.f);

	auto quater = Quaternion::FromRotator(input);
	auto matrix = Matrix3x3::FromRotate(quater);
	auto output = Rotator::FromMatrix(matrix);

	CHECK(input.m_Pitch == Approx(output.m_Pitch));
	CHECK(input.m_Yaw == Approx(output.m_Yaw));
	CHECK(input.m_Roll == Approx(output.m_Roll));
}

TEST_CASE("Rotation. Rotator -> Matrix4x4 -> Quaternion -> Rotator.")
{
	Rotator input = Rotator(45.f, -45.f, 0.f);

	auto matrix = Matrix4x4::FromRotate(input);
	auto quater = Quaternion::FromMatrix(matrix);
	auto output = Rotator::FromQuaternion(quater);

	CHECK(input.m_Pitch == Approx(output.m_Pitch));
	CHECK(input.m_Yaw == Approx(output.m_Yaw));
	CHECK(input.m_Roll == Approx(output.m_Roll));
}

TEST_CASE("Rotation. Rotator -> Quaternion -> Matrix4x4 -> Rotator.")
{
	Rotator input = Rotator(45.f, -45.f, 0.f);

	auto quater = Quaternion::FromRotator(input);
	auto matrix = Matrix4x4::FromRotate(quater);
	auto output = Rotator::FromMatrix(matrix);

	CHECK(input.m_Pitch == Approx(output.m_Pitch));
	CHECK(input.m_Yaw == Approx(output.m_Yaw));
	CHECK(input.m_Roll == Approx(output.m_Roll));
}
