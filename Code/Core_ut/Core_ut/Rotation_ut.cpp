#include <Catch2/catch.hpp>

#include <Core/Matrix.h>
#include <Core/Rotator.h>
#include <Core/Quaternion.h>
#include <Core/Variant.h>
#include <Core/VariantHelpers.h>

namespace
{
	using Option = Variant<Matrix3x3, Matrix4x4, Quaternion, Rotator>;
	const char* ToName(const Option& value)
	{
		return core::VariantMatch(value,
			[](const Matrix3x3&) { return "Matrix3x3"; },
			[](const Matrix4x4&) { return "Matrix4x4"; },
			[](const Quaternion&) { return "Quaternion"; },
			[](const Rotator&) { return "Rotator"; });
	}

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

TEST_CASE("Rotation")
{

	{
		INFO("Rotator -> Matrix3x3 -> Quaternion -> Rotator");

		Rotator input = Rotator(45.f, -45.f, 0.f);

		auto matrix = Matrix3x3::FromRotate(input);
		auto quater = Quaternion::FromMatrix(matrix);
		auto output = Rotator::FromQuaternion(quater);

		CHECK(input.m_Pitch == Approx(output.m_Pitch));
		CHECK(input.m_Yaw == Approx(output.m_Yaw));
		CHECK(input.m_Roll == Approx(output.m_Roll));
	}

	{
		INFO("Rotator -> Quaternion -> Matrix3x3 -> Rotator");

		Rotator input = Rotator(45.f, -45.f, 0.f);

		auto quater = Quaternion::FromRotator(input);
		auto matrix = Matrix3x3::FromRotate(quater);
		auto output = Rotator::FromMatrix(matrix);

		CHECK(input.m_Pitch == Approx(output.m_Pitch));
		CHECK(input.m_Yaw == Approx(output.m_Yaw));
		CHECK(input.m_Roll == Approx(output.m_Roll));
	}

	{
		INFO("Rotator -> Matrix4x4 -> Quaternion -> Rotator");

		Rotator input = Rotator(45.f, -45.f, 0.f);

		auto matrix = Matrix4x4::FromRotate(input);
		auto quater = Quaternion::FromMatrix(matrix);
		auto output = Rotator::FromQuaternion(quater);

		CHECK(input.m_Pitch == Approx(output.m_Pitch));
		CHECK(input.m_Yaw == Approx(output.m_Yaw));
		CHECK(input.m_Roll == Approx(output.m_Roll));
	}

	{
		INFO("Rotator -> Quaternion -> Matrix4x4 -> Rotator");

		Rotator input = Rotator(45.f, -45.f, 0.f);

		auto quater = Quaternion::FromRotator(input);
		auto matrix = Matrix4x4::FromRotate(quater);
		auto output = Rotator::FromMatrix(matrix);

		CHECK(input.m_Pitch == Approx(output.m_Pitch));
		CHECK(input.m_Yaw == Approx(output.m_Yaw));
		CHECK(input.m_Roll == Approx(output.m_Roll));
	}

	{
		const Rotator compare = Rotator(45.f, -45.f, 0.f);
		const Option options[] = { Matrix3x3(), Matrix4x4(), Quaternion(), Rotator() };

		int32 index = 0;
		for (const Option& i : options)
		{
			for (const Option& j : options)
			{
				Option input = Rotator(45.f, -45.f, 0.f);
				Option a = i;
				std::visit([](auto& lhs, auto& rhs) { Convert(lhs, rhs); }, a, input);

				Option b = j;
				std::visit([](auto& lhs, auto& rhs) { Convert(lhs, rhs); }, b, a);

				Option outputA = Rotator();
				Option outputB = Rotator();
				std::visit([](auto& lhs, auto& rhs) { Convert(lhs, rhs); }, outputA, a);
				std::visit([](auto& lhs, auto& rhs) { Convert(lhs, rhs); }, outputB, b);

				const Rotator resultA = std::get<Rotator>(outputA);
				const Rotator resultB = std::get<Rotator>(outputB);

				std::string info;
				info += std::to_string(index);
				info += " : ";
				info += ToName(i);
				info += " -> ";
				info += ToName(j);

				INFO(info.c_str());
				CHECK(resultB.m_Pitch == Approx(compare.m_Pitch));
				CHECK(resultB.m_Yaw == Approx(compare.m_Yaw));
				CHECK(resultB.m_Roll == Approx(compare.m_Roll));
				index++;
			}
		}
	}
}