#include <Catch2/catch.hpp>

#include "Math/Common.h"
#include "Math/Matrix.h"

TEST_CASE("Matrix3x3")
{
	SECTION("Alignment")
	{
		CHECK(alignof(Matrix3x3) == 16);
	}

	SECTION("Matrix3x3::Zero")
	{
		CHECK(Matrix3x3::Zero == Matrix3x3(
			0.f, 0.f, 0.f,
			0.f, 0.f, 0.f,
			0.f, 0.f, 0.f));
	}

	SECTION("Matrix3x3::Identity")
	{
		CHECK(Matrix3x3::Identity == Matrix3x3(
			1.f, 0.f, 0.f,
			0.f, 1.f, 0.f,
			0.f, 0.f, 1.f));
	}

	SECTION("Matrix3x3(float, float, float, float, float, float, float, float, float)")
	{
		Matrix3x3 matrix(
			01.f, 02.f, 03.f,
			05.f, 06.f, 07.f,
			09.f, 10.f, 11.f);
		CHECK(matrix.m_Data[0][0] == 1.f);
		CHECK(matrix.m_Data[0][1] == 2.f);
		CHECK(matrix.m_Data[0][2] == 3.f);

		CHECK(matrix.m_Data[1][0] == 5.f);
		CHECK(matrix.m_Data[1][1] == 6.f);
		CHECK(matrix.m_Data[1][2] == 7.f);

		CHECK(matrix.m_Data[2][0] == 9.f);
		CHECK(matrix.m_Data[2][1] == 10.f);
		CHECK(matrix.m_Data[2][2] == 11.f);
	}

	SECTION("Matrix3x3(Vector3f, Vector3f, Vector3f)")
	{
		Matrix3x3 matrix(
			Vector3f(01.f, 02.f, 03.f),
			Vector3f(05.f, 06.f, 07.f),
			Vector3f(09.f, 10.f, 11.f));

		CHECK(matrix.m_Data[0][0] == 01.f);
		CHECK(matrix.m_Data[0][1] == 02.f);
		CHECK(matrix.m_Data[0][2] == 03.f);

		CHECK(matrix.m_Data[1][0] == 05.f);
		CHECK(matrix.m_Data[1][1] == 06.f);
		CHECK(matrix.m_Data[1][2] == 07.f);

		CHECK(matrix.m_Data[2][0] == 09.f);
		CHECK(matrix.m_Data[2][1] == 10.f);
		CHECK(matrix.m_Data[2][2] == 11.f);
	}

	SECTION("Matrix3x3(Matrix4x4)")
	{
		Matrix4x4 matrixA(
			01.f, 02.f, 03.f, 04.f,
			05.f, 06.f, 07.f, 08.f,
			09.f, 10.f, 11.f, 12.f,
			13.f, 14.f, 15.f, 16.f);
		Matrix3x3 matrixB(matrixA);
		CHECK(matrixB.m_Data[0][0] == matrixA.m_Data[0][0]);
		CHECK(matrixB.m_Data[0][1] == matrixA.m_Data[0][1]);
		CHECK(matrixB.m_Data[0][2] == matrixA.m_Data[0][2]);

		CHECK(matrixB.m_Data[1][0] == matrixA.m_Data[1][0]);
		CHECK(matrixB.m_Data[1][1] == matrixA.m_Data[1][1]);
		CHECK(matrixB.m_Data[1][2] == matrixA.m_Data[1][2]);

		CHECK(matrixB.m_Data[2][0] == matrixA.m_Data[2][0]);
		CHECK(matrixB.m_Data[2][1] == matrixA.m_Data[2][1]);
		CHECK(matrixB.m_Data[2][2] == matrixA.m_Data[2][2]);
	}

	SECTION("Matrix3x3(Quaternion)")
	{
		Matrix3x3 matrixA = Matrix3x3::FromRotate(Quaternion::FromAxisAngle(Vector3f::AxisZ, math::ToRadians(90.f)));
		Matrix3x3 matrixB = Matrix3x3(
			+0.f, +1.f, 0.f,
			-1.f, -0.f, 0.f,
			+0.f, +0.f, 1.f);

		CHECK(IsNearly(matrixA, matrixB));
	}

	SECTION("Matrix3x3(Rotator)")
	{
	}

	SECTION("operator==(Matrix3x3)")
	{
		CHECK(Matrix3x3::Zero == Matrix3x3::Zero);
		CHECK_FALSE(Matrix3x3::Zero == Matrix3x3::Identity);
	}

	SECTION("operator!=(Matrix3x3)")
	{
		CHECK(Matrix3x3::Zero != Matrix3x3::Identity);
		CHECK_FALSE(Matrix3x3::Zero != Matrix3x3::Zero);
	}

	SECTION("operator+(Matrix3x3)")
	{
		Matrix3x3 matrix =
			Matrix3x3(1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f) +
			Matrix3x3(2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f);
		CHECK(matrix == Matrix3x3(
			3.f, 3.f, 3.f,
			3.f, 3.f, 3.f,
			3.f, 3.f, 3.f));
	}

	SECTION("operator-(Matrix3x3)")
	{
		Matrix3x3 matrix =
			Matrix3x3(3.f, 3.f, 3.f, 3.f, 3.f, 3.f, 3.f, 3.f, 3.f) -
			Matrix3x3(2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f);
		CHECK(matrix == Matrix3x3(
			1.f, 1.f, 1.f,
			1.f, 1.f, 1.f,
			1.f, 1.f, 1.f));
	}

	SECTION("operator*(Matrix3x3)")
	{
		Matrix3x3 matrix =
			Matrix3x3(3.f, 3.f, 3.f, 3.f, 3.f, 3.f, 3.f, 3.f, 3.f) *
			Matrix3x3(2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f);
		CHECK(matrix == Matrix3x3(
			18.f, 18.f, 18.f,
			18.f, 18.f, 18.f,
			18.f, 18.f, 18.f));
	}

	SECTION("operator+=(Matrix3x3)")
	{
		Matrix3x3 matrix = Matrix3x3(1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f);
		matrix += Matrix3x3(2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f);
		CHECK(matrix == Matrix3x3(
			3.f, 3.f, 3.f,
			3.f, 3.f, 3.f,
			3.f, 3.f, 3.f));
	}

	SECTION("operator-=(Matrix3x3)")
	{
		Matrix3x3 matrix = Matrix3x3(3.f, 3.f, 3.f, 3.f, 3.f, 3.f, 3.f, 3.f, 3.f);
		matrix -= Matrix3x3(2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f);
		CHECK(matrix == Matrix3x3(
			1.f, 1.f, 1.f,
			1.f, 1.f, 1.f,
			1.f, 1.f, 1.f));
	}

	SECTION("operator*=(Matrix3x3)")
	{
		Matrix3x3 matrix = Matrix3x3(3.f, 3.f, 3.f, 3.f, 3.f, 3.f, 3.f, 3.f, 3.f);
		matrix *= Matrix3x3(2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f);
		CHECK(matrix == Matrix3x3(
			18.f, 18.f, 18.f,
			18.f, 18.f, 18.f,
			18.f, 18.f, 18.f));
	}

	SECTION("Inverse")
	{
		Quaternion quaternion = Quaternion::FromAxisAngle(Vector3f::AxisY, math::ToRadians(90.f));
		Matrix3x3 matrixA = Matrix3x3::FromRotate(quaternion);
		matrixA.Inverse();
		Matrix3x3 matrixB = Matrix3x3(
			+0.f, 0.f, 1.f,
			+0.f, 1.f, 0.f,
			-1.f, 0.f, 0.f);
		CHECK(IsNearly(matrixA, matrixB));
	}

	SECTION("Transpose")
	{
		Matrix3x3 matrix(1.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f, 8.f, 9.f);
		matrix.Transpose();
		CHECK(matrix == Matrix3x3(
			01.f, 04.f, 07.f,
			02.f, 05.f, 08.f,
			03.f, 06.f, 09.f));
	}

	SECTION("Rotate")
	{
	}

	SECTION("Inversed")
	{
		Quaternion quaternion = Quaternion::FromAxisAngle(Vector3f::AxisY, math::ToRadians(90.f));
		Matrix3x3 matrixA = Matrix3x3::FromRotate(quaternion);
		Matrix3x3 matrixB = matrixA.Inversed();
		Matrix3x3 matrixC = Matrix3x3(
			+0.f, 0.f, 1.f,
			+0.f, 1.f, 0.f,
			-1.f, 0.f, 0.f);
		CHECK(IsNearly(matrixB, matrixC));
	}

	SECTION("Transposed")
	{
		Matrix3x3 matrix = Matrix3x3(
			01.f, 02.f, 03.f,
			05.f, 06.f, 07.f,
			09.f, 10.f, 11.f).Transposed();
		CHECK(matrix == Matrix3x3(
			01.f, 05.f, 09.f,
			02.f, 06.f, 10.f,
			03.f, 07.f, 11.f));
	}

	SECTION("SetRotate(Matrix4x4)")
	{
	}

	SECTION("SetRotate(Quaternion)")
	{
		Quaternion quaternion = Quaternion::FromAxisAngle(Vector3f::AxisY, math::ToRadians(90.f));
		Matrix3x3 matrix = Matrix3x3::Identity;
		matrix.SetRotate(quaternion);
	}

	SECTION("SetRotate(Rotator)")
	{
	}

	SECTION("FromRotate(Matrix4x4)")
	{
	}

	SECTION("FromRotate(Quaternion)")
	{
		Quaternion quaternion = Quaternion::FromAxisAngle(Vector3f::AxisZ, math::ToRadians(90.f));
		Matrix3x3 matrixA = Matrix3x3::FromRotate(quaternion);
		Matrix3x3 matrixB = Matrix3x3(
			+0.f, 1.f, 0.f,
			-1.f, 0.f, 0.f,
			+0.f, 0.f, 1.f);
		CHECK(IsNearly(matrixA, matrixB));
	}

	SECTION("FromRotate(Rotator)")
	{
	}
}

TEST_CASE("Matrix4x4")
{
	SECTION("Alignment")
	{
		CHECK(alignof(Matrix4x4) == 16);
	}

	SECTION("Matrix4x4::Zero")
	{
		CHECK(Matrix4x4::Zero == Matrix4x4(
			0.f, 0.f, 0.f, 0.f, 
			0.f, 0.f, 0.f, 0.f, 
			0.f, 0.f, 0.f, 0.f, 
			0.f, 0.f, 0.f, 0.f));
	}

	SECTION("Matrix4x4::Identity")
	{
		CHECK(Matrix4x4::Identity == Matrix4x4(
			1.f, 0.f, 0.f, 0.f,
			0.f, 1.f, 0.f, 0.f,
			0.f, 0.f, 1.f, 0.f,
			0.f, 0.f, 0.f, 1.f));
	}

	SECTION("Matrix4x4(float, float, float, float, float, float, float, float, float, float, float, float, float, float, float, float)")
	{
		Matrix4x4 matrix(
			01.f, 02.f, 03.f, 04.f,
			05.f, 06.f, 07.f, 08.f,
			09.f, 10.f, 11.f, 12.f,
			13.f, 14.f, 15.f, 16.f);
		CHECK(matrix.m_Data[0][0] == 1.f);
		CHECK(matrix.m_Data[0][1] == 2.f);
		CHECK(matrix.m_Data[0][2] == 3.f);
		CHECK(matrix.m_Data[0][3] == 4.f);

		CHECK(matrix.m_Data[1][0] == 5.f);
		CHECK(matrix.m_Data[1][1] == 6.f);
		CHECK(matrix.m_Data[1][2] == 7.f);
		CHECK(matrix.m_Data[1][3] == 8.f);

		CHECK(matrix.m_Data[2][0] == 9.f);
		CHECK(matrix.m_Data[2][1] == 10.f);
		CHECK(matrix.m_Data[2][2] == 11.f);
		CHECK(matrix.m_Data[2][3] == 12.f);

		CHECK(matrix.m_Data[3][0] == 13.f);
		CHECK(matrix.m_Data[3][1] == 14.f);
		CHECK(matrix.m_Data[3][2] == 15.f);
		CHECK(matrix.m_Data[3][3] == 16.f);
	}

	SECTION("Matrix4x4(Vector4f, Vector4f, Vector4f, Vector4f)")
	{
		Matrix4x4 matrix(
			Vector4f(01.f, 02.f, 03.f, 04.f),
			Vector4f(05.f, 06.f, 07.f, 08.f),
			Vector4f(09.f, 10.f, 11.f, 12.f),
			Vector4f(13.f, 14.f, 15.f, 16.f));

		CHECK(matrix.m_Data[0][0] == 01.f);
		CHECK(matrix.m_Data[0][1] == 02.f);
		CHECK(matrix.m_Data[0][2] == 03.f);
		CHECK(matrix.m_Data[0][3] == 04.f);

		CHECK(matrix.m_Data[1][0] == 05.f);
		CHECK(matrix.m_Data[1][1] == 06.f);
		CHECK(matrix.m_Data[1][2] == 07.f);
		CHECK(matrix.m_Data[1][3] == 08.f);

		CHECK(matrix.m_Data[2][0] == 09.f);
		CHECK(matrix.m_Data[2][1] == 10.f);
		CHECK(matrix.m_Data[2][2] == 11.f);
		CHECK(matrix.m_Data[2][3] == 12.f);

		CHECK(matrix.m_Data[3][0] == 13.f);
		CHECK(matrix.m_Data[3][1] == 14.f);
		CHECK(matrix.m_Data[3][2] == 15.f);
		CHECK(matrix.m_Data[3][3] == 16.f);
	}

	SECTION("Matrix3x3(Matrix4x4)")
	{
		Matrix3x3 matrixA(
			01.f, 02.f, 03.f,
			05.f, 06.f, 07.f,
			09.f, 10.f, 11.f);
		Matrix4x4 matrixB(matrixA);
		CHECK(matrixB.m_Data[0][0] == matrixA.m_Data[0][0]);
		CHECK(matrixB.m_Data[0][1] == matrixA.m_Data[0][1]);
		CHECK(matrixB.m_Data[0][2] == matrixA.m_Data[0][2]);

		CHECK(matrixB.m_Data[1][0] == matrixA.m_Data[1][0]);
		CHECK(matrixB.m_Data[1][1] == matrixA.m_Data[1][1]);
		CHECK(matrixB.m_Data[1][2] == matrixA.m_Data[1][2]);

		CHECK(matrixB.m_Data[2][0] == matrixA.m_Data[2][0]);
		CHECK(matrixB.m_Data[2][1] == matrixA.m_Data[2][1]);
		CHECK(matrixB.m_Data[2][2] == matrixA.m_Data[2][2]);
	}

	SECTION("operator==(Matrix4x4)")
	{
		CHECK(Matrix4x4::Zero == Matrix4x4::Zero);
		CHECK_FALSE(Matrix4x4::Zero == Matrix4x4::Identity);
	}

	SECTION("operator!=(Matrix4x4)")
	{
		CHECK(Matrix4x4::Zero != Matrix4x4::Identity);
		CHECK_FALSE(Matrix4x4::Zero != Matrix4x4::Zero);
	}

	SECTION("operator+(Matrix4x4)")
	{
		Matrix4x4 matrix =
			Matrix4x4(1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f) +
			Matrix4x4(2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f);
		CHECK(matrix == Matrix4x4(
			3.f, 3.f, 3.f, 3.f,
			3.f, 3.f, 3.f, 3.f,
			3.f, 3.f, 3.f, 3.f,
			3.f, 3.f, 3.f, 3.f));
	}

	SECTION("operator-(Matrix4x4)")
	{
		Matrix4x4 matrix =
			Matrix4x4(3.f, 3.f, 3.f, 3.f, 3.f, 3.f, 3.f, 3.f, 3.f, 3.f, 3.f, 3.f, 3.f, 3.f, 3.f, 3.f) -
			Matrix4x4(2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f);
		CHECK(matrix == Matrix4x4(
			1.f, 1.f, 1.f, 1.f,
			1.f, 1.f, 1.f, 1.f,
			1.f, 1.f, 1.f, 1.f,
			1.f, 1.f, 1.f, 1.f));
	}

	SECTION("operator*(Matrix4x4)")
	{
		Matrix4x4 matrix =
			Matrix4x4(3.f, 3.f, 3.f, 3.f, 3.f, 3.f, 3.f, 3.f, 3.f, 3.f, 3.f, 3.f, 3.f, 3.f, 3.f, 3.f) *
			Matrix4x4(2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f);
		CHECK(matrix == Matrix4x4(
			24.f, 24.f, 24.f, 24.f,
			24.f, 24.f, 24.f, 24.f,
			24.f, 24.f, 24.f, 24.f,
			24.f, 24.f, 24.f, 24.f));
	}

	SECTION("operator+=(Matrix4x4)")
	{
		Matrix4x4 matrix = Matrix4x4(1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f);
		matrix += Matrix4x4(2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f);
		CHECK(matrix == Matrix4x4(
			3.f, 3.f, 3.f, 3.f,
			3.f, 3.f, 3.f, 3.f,
			3.f, 3.f, 3.f, 3.f,
			3.f, 3.f, 3.f, 3.f));
	}

	SECTION("operator-=(Matrix4x4)")
	{
		Matrix4x4 matrix = Matrix4x4(3.f, 3.f, 3.f, 3.f, 3.f, 3.f, 3.f, 3.f, 3.f, 3.f, 3.f, 3.f, 3.f, 3.f, 3.f, 3.f);
		matrix -= Matrix4x4(2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f);
		CHECK(matrix == Matrix4x4(
			1.f, 1.f, 1.f, 1.f,
			1.f, 1.f, 1.f, 1.f,
			1.f, 1.f, 1.f, 1.f,
			1.f, 1.f, 1.f, 1.f));
	}

	SECTION("operator*=(Matrix4x4)")
	{
		Matrix4x4 matrix = Matrix4x4(3.f, 3.f, 3.f, 3.f, 3.f, 3.f, 3.f, 3.f, 3.f, 3.f, 3.f, 3.f, 3.f, 3.f, 3.f, 3.f);
		matrix *= Matrix4x4(2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f);
		CHECK(matrix == Matrix4x4(
			24.f, 24.f, 24.f, 24.f,
			24.f, 24.f, 24.f, 24.f,
			24.f, 24.f, 24.f, 24.f,
			24.f, 24.f, 24.f, 24.f));
	}

	SECTION("Inverse")
	{
		Matrix4x4 matrix = Matrix4x4::FromTranslate(Vector3f(-9.f));
		matrix.Inverse();
		CHECK(matrix == Matrix4x4(
			1.f, 0.f, 0.f, 0.f,
			0.f, 1.f, 0.f, 0.f,
			0.f, 0.f, 1.f, 0.f,
			9.f, 9.f, 9.f, 1.f));
	}

	SECTION("Transpose")
	{
		Matrix4x4 matrix(1.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f, 8.f, 9.f, 10.f, 11.f, 12.f, 13.f, 14.f, 15.f, 16.f);
		matrix.Transpose();
		CHECK(matrix == Matrix4x4(
			01.f, 05.f, 09.f, 13.f,
			02.f, 06.f, 10.f, 14.f,
			03.f, 07.f, 11.f, 15.f,
			04.f, 08.f, 12.f, 16.f));
	}

	SECTION("Translate")
	{
	}

	SECTION("Rotate")
	{
	}

	SECTION("Scale")
	{
	}

	SECTION("Inversed")
	{
		Matrix4x4 matrix = Matrix4x4::FromTranslate(Vector3f(-9.f)).Inversed();
		CHECK(matrix == Matrix4x4(
			1.f, 0.f, 0.f, 0.f,
			0.f, 1.f, 0.f, 0.f,
			0.f, 0.f, 1.f, 0.f,
			9.f, 9.f, 9.f, 1.f));
	}

	SECTION("Transposed")
	{
		Matrix4x4 matrix = Matrix4x4(
			01.f, 02.f, 03.f, 04.f,
			05.f, 06.f, 07.f, 08.f,
			09.f, 10.f, 11.f, 12.f,
			13.f, 14.f, 15.f, 16.f).Transposed();
		CHECK(matrix == Matrix4x4(
			01.f, 05.f, 09.f, 13.f,
			02.f, 06.f, 10.f, 14.f,
			03.f, 07.f, 11.f, 15.f,
			04.f, 08.f, 12.f, 16.f));
	}

	SECTION("SetTranslate(Vector3f)")
	{
		Matrix4x4 matrix = Matrix4x4::Identity;
		matrix.SetTranslate(Vector3f(1.f, 2.f, 3.f));
		CHECK(matrix == Matrix4x4(
			1.f, 0.f, 0.f, 0.f,
			0.f, 1.f, 0.f, 0.f,
			0.f, 0.f, 1.f, 0.f,
			1.f, 2.f, 3.f, 1.f));
	}

	SECTION("SetRotate(Matrix3x3)")
	{
	}

	SECTION("SetRotate(Quaternion)")
	{
		Quaternion quaternion = Quaternion::FromAxisAngle(Vector3f::AxisY, math::ToRadians(90.f));
		Matrix4x4 matrix = Matrix4x4::Identity;
		matrix.SetTranslate(Vector3f(9.f, 9.f, 9.f));
		matrix.SetScale(9.f);
		matrix.SetRotate(quaternion);
	}

	SECTION("SetRotate(Rotator)")
	{
	}

	SECTION("SetScale(float)")
	{
		Matrix4x4 matrix = Matrix4x4::Identity;
		matrix.SetScale(9.f);
		CHECK(matrix == Matrix4x4(
			9.f, 0.f, 0.f, 0.f,
			0.f, 9.f, 0.f, 0.f,
			0.f, 0.f, 9.f, 0.f,
			0.f, 0.f, 0.f, 1.f));
	}

	SECTION("SetScale(Vector3f)")
	{
		Matrix4x4 matrix = Matrix4x4::Identity;
		matrix.SetScale(Vector3f(1.f, 2.f, 3.f));
		CHECK(matrix == Matrix4x4(
			1.f, 0.f, 0.f, 0.f,
			0.f, 2.f, 0.f, 0.f,
			0.f, 0.f, 3.f, 0.f,
			0.f, 0.f, 0.f, 1.f));
	}

	SECTION("FromTranslate(Vector3f)")
	{
		Matrix4x4 matrix = Matrix4x4::FromTranslate(Vector3f(1.f, 2.f, 3.f));
		CHECK(matrix == Matrix4x4(
			1.f, 0.f, 0.f, 0.f,
			0.f, 1.f, 0.f, 0.f,
			0.f, 0.f, 1.f, 0.f,
			1.f, 2.f, 3.f, 1.f));
	}

	SECTION("FromRotate(Matrix3x3)")
	{
	}

	SECTION("FromRotate(Quaternion)")
	{
		Quaternion quaternion = Quaternion::FromAxisAngle(Vector3f::AxisZ, math::ToRadians(90.f));
		Matrix4x4 matrixA = Matrix4x4::FromRotate(quaternion);
		Matrix4x4 matrixB = Matrix4x4(
			0.f, 1.f, 0.f, 0.f,
			-1.f, 0.f, 0.f, 0.f,
			0.f, 0.f, 1.f, 0.f,
			0.f, 0.f, 0.f, 1.f);
		CHECK(IsNearly(matrixA, matrixB));
	}

	SECTION("FromRotate(Rotator)")
	{
	}

	SECTION("FromScale(float)")
	{
		Matrix4x4 matrix = Matrix4x4::FromScale(3.f);
		CHECK(matrix == Matrix4x4(
			3.f, 0.f, 0.f, 0.f,
			0.f, 3.f, 0.f, 0.f,
			0.f, 0.f, 3.f, 0.f,
			0.f, 0.f, 0.f, 1.f));
	}

	SECTION("FromScale(Vector3f)")
	{
		Matrix4x4 matrix = Matrix4x4::FromScale(Vector3f(4.f, 5.f, 6.f));
		CHECK(matrix == Matrix4x4(
			4.f, 0.f, 0.f, 0.f,
			0.f, 5.f, 0.f, 0.f,
			0.f, 0.f, 6.f, 0.f,
			0.f, 0.f, 0.f, 1.f));
	}
}