#include <Catch2/catch.hpp>

#include "Math/Common.h"
#include "Math/Matrix.h"

TEST_CASE("Matrix3x3. Alignment.")
{
	CHECK(alignof(Matrix3x3) == 16);
}

TEST_CASE("Matrix3x3. Matrix3x3(float float float float float float float float float).")
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

TEST_CASE("Matrix3x3. Matrix3x3(Vector3f Vector3f Vector3f).")
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

TEST_CASE("Matrix3x3. Matrix3x3(Matrix4x4).")
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

TEST_CASE("Matrix3x3. Matrix3x3(Quaternion).")
{
	Matrix3x3 matrixA = Matrix3x3::FromRotate(Quaternion::FromAxisAngle(Vector3f::AxisZ, math::ToRadians(90.f)));
	Matrix3x3 matrixB = Matrix3x3(
		+0.f, +1.f, 0.f,
		-1.f, -0.f, 0.f,
		+0.f, +0.f, 1.f);

	CHECK(IsNearly(matrixA, matrixB));
}

TEST_CASE("Matrix3x3. Matrix3x3(Rotator).")
{
	CHECK(false);
}

TEST_CASE("Matrix3x3. operator==(Matrix3x3).")
{
	CHECK(Matrix3x3::Zero == Matrix3x3::Zero);
	CHECK_FALSE(Matrix3x3::Zero == Matrix3x3::Identity);
}

TEST_CASE("Matrix3x3. operator!=(Matrix3x3).")
{
	CHECK(Matrix3x3::Zero != Matrix3x3::Identity);
	CHECK_FALSE(Matrix3x3::Zero != Matrix3x3::Zero);
}

TEST_CASE("Matrix3x3. operator+(Matrix3x3).")
{
	Matrix3x3 matrix =
		Matrix3x3(1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f) +
		Matrix3x3(2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f);
	CHECK(matrix == Matrix3x3(
		3.f, 3.f, 3.f,
		3.f, 3.f, 3.f,
		3.f, 3.f, 3.f));
}

TEST_CASE("Matrix3x3. operator-(Matrix3x3).")
{
	Matrix3x3 matrix =
		Matrix3x3(3.f, 3.f, 3.f, 3.f, 3.f, 3.f, 3.f, 3.f, 3.f) -
		Matrix3x3(2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f);
	CHECK(matrix == Matrix3x3(
		1.f, 1.f, 1.f,
		1.f, 1.f, 1.f,
		1.f, 1.f, 1.f));
}

TEST_CASE("Matrix3x3. operator*(Matrix3x3).")
{
	Matrix3x3 matrix =
		Matrix3x3(3.f, 3.f, 3.f, 3.f, 3.f, 3.f, 3.f, 3.f, 3.f) *
		Matrix3x3(2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f);
	CHECK(matrix == Matrix3x3(
		18.f, 18.f, 18.f,
		18.f, 18.f, 18.f,
		18.f, 18.f, 18.f));
}

TEST_CASE("Matrix3x3. operator+=(Matrix3x3).")
{
	Matrix3x3 matrix = Matrix3x3(1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f);
	matrix += Matrix3x3(2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f);
	CHECK(matrix == Matrix3x3(
		3.f, 3.f, 3.f,
		3.f, 3.f, 3.f,
		3.f, 3.f, 3.f));
}

TEST_CASE("Matrix3x3. operator-=(Matrix3x3).")
{
	Matrix3x3 matrix = Matrix3x3(3.f, 3.f, 3.f, 3.f, 3.f, 3.f, 3.f, 3.f, 3.f);
	matrix -= Matrix3x3(2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f);
	CHECK(matrix == Matrix3x3(
		1.f, 1.f, 1.f,
		1.f, 1.f, 1.f,
		1.f, 1.f, 1.f));
}

TEST_CASE("Matrix3x3. operator*=(Matrix3x3).")
{
	Matrix3x3 matrix = Matrix3x3(3.f, 3.f, 3.f, 3.f, 3.f, 3.f, 3.f, 3.f, 3.f);
	matrix *= Matrix3x3(2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f);
	CHECK(matrix == Matrix3x3(
		18.f, 18.f, 18.f,
		18.f, 18.f, 18.f,
		18.f, 18.f, 18.f));
}

TEST_CASE("Matrix3x3. Inverse.")
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

TEST_CASE("Matrix3x3. Transpose.")
{
	Matrix3x3 matrix(1.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f, 8.f, 9.f);
	matrix.Transpose();
	CHECK(matrix == Matrix3x3(
		01.f, 04.f, 07.f,
		02.f, 05.f, 08.f,
		03.f, 06.f, 09.f));
}

TEST_CASE("Matrix3x3. Rotate.")
{
	CHECK(false);
}

TEST_CASE("Matrix3x3. Inversed.")
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

TEST_CASE("Matrix3x3. Transposed.")
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

TEST_CASE("Matrix3x3. SetRotate(Matrix4x4).")
{
	CHECK(false);
}

TEST_CASE("Matrix3x3. SetRotate(Quaternion).")
{
	Quaternion quaternion = Quaternion::FromAxisAngle(Vector3f::AxisY, math::ToRadians(90.f));
	Matrix3x3 matrix = Matrix3x3::Identity;
	matrix.SetRotate(quaternion);
}

TEST_CASE("Matrix3x3. SetRotate(Rotator).")
{
	CHECK(false);
}

TEST_CASE("Matrix3x3. FromRotate(Matrix4x4).")
{
	CHECK(false);
}

TEST_CASE("Matrix3x3. FromRotate(Quaternion).")
{
	Quaternion quaternion = Quaternion::FromAxisAngle(Vector3f::AxisZ, math::ToRadians(90.f));
	Matrix3x3 matrixA = Matrix3x3::FromRotate(quaternion);
	Matrix3x3 matrixB = Matrix3x3(
		+0.f, 1.f, 0.f,
		-1.f, 0.f, 0.f,
		+0.f, 0.f, 1.f);
	CHECK(IsNearly(matrixA, matrixB));
}

TEST_CASE("Matrix3x3. FromRotate(Rotator).")
{
	CHECK(false);
}

TEST_CASE("Matrix3x3. Zero.")
{
	CHECK(Matrix3x3::Zero == Matrix3x3(
		0.f, 0.f, 0.f,
		0.f, 0.f, 0.f,
		0.f, 0.f, 0.f));
}

TEST_CASE("Matrix3x3. Identity.")
{
	CHECK(Matrix3x3::Identity == Matrix3x3(
		1.f, 0.f, 0.f,
		0.f, 1.f, 0.f,
		0.f, 0.f, 1.f));
}