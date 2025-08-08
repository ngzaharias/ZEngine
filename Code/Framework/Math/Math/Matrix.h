#pragma once

#include "Math/Defines.h"
#include "Math/Types.h"

class Matrix4x4;
class Quaternion;
class Rotator;
class Vector2f;
class Vector3f;
class Vector4f;

/// \brief
class ALIGN_16 Matrix3x3
{
public:
	constexpr Matrix3x3() noexcept;
	constexpr Matrix3x3(
		const float v00, const float v01, const float v02,
		const float v10, const float v11, const float v12,
		const float v20, const float v21, const float v22) noexcept;
	constexpr Matrix3x3(const Vector3f& v0, const Vector3f& v1, const Vector3f& v2) noexcept;
	constexpr Matrix3x3(const Matrix4x4& value) noexcept;

	constexpr bool operator==(const Matrix3x3& rhs) const noexcept;
	constexpr bool operator!=(const Matrix3x3& rhs) const noexcept;

	constexpr float* operator[](const uint32 i) noexcept { return m_Data[i]; }
	constexpr const float* operator[](const uint32 i) const noexcept { return m_Data[i]; }

	constexpr Matrix3x3 operator+(const Matrix3x3& rhs) const noexcept;
	constexpr Matrix3x3 operator-(const Matrix3x3& rhs) const noexcept;

	// #note: Multiplication is Left -> Right (rather than Left <- Right).
	constexpr Matrix3x3 operator*(const Matrix3x3& rhs) const noexcept;

	constexpr Matrix3x3& operator+=(const Matrix3x3& rhs) noexcept { *this = *this + rhs; return *this; }
	constexpr Matrix3x3& operator-=(const Matrix3x3& rhs) noexcept { *this = *this - rhs; return *this; }
	constexpr Matrix3x3& operator*=(const Matrix3x3& rhs) noexcept { *this = *this * rhs; return *this; }

	void Inverse() noexcept;
	void Transpose() noexcept;

	void Rotate(const Quaternion& value) noexcept;
	void Rotate(const Rotator& value) noexcept;

	[[nodiscard]] Matrix3x3 Inversed() const noexcept;
	[[nodiscard]] Matrix3x3 Transposed() const noexcept;

	void SetRotate(const Rotator& value) noexcept;

	constexpr void SetRotate(const Quaternion& value) noexcept;

	static Matrix3x3 FromRotate(const Rotator& value) noexcept;

	static constexpr Matrix3x3 FromRotate(const Quaternion& value) noexcept;

	static const Matrix3x3 Identity;
	static const Matrix3x3 Zero;

public:
	float m_Data[3][3];
};

inline const Matrix3x3 Matrix3x3::Identity(
	1.f, 0.f, 0.f,
	0.f, 1.f, 0.f,
	0.f, 0.f, 1.f);

inline const Matrix3x3 Matrix3x3::Zero(
	0.f, 0.f, 0.f,
	0.f, 0.f, 0.f,
	0.f, 0.f, 0.f);

inline constexpr Vector2f operator*(const Vector2f& lhs, const Matrix3x3& rhs) noexcept;
inline constexpr Vector3f operator*(const Vector3f& lhs, const Matrix3x3& rhs) noexcept;

/// \brief
class ALIGN_16 Matrix4x4
{
public:
	constexpr Matrix4x4() noexcept;
	constexpr Matrix4x4(
		const float v00, const float v01, const float v02, const float v03,
		const float v10, const float v11, const float v12, const float v13,
		const float v20, const float v21, const float v22, const float v23,
		const float v30, const float v31, const float v32, const float v33) noexcept;
	constexpr Matrix4x4(const Vector4f& v0, const Vector4f& v1, const Vector4f& v2, const Vector4f& v3) noexcept;
	constexpr Matrix4x4(const Matrix3x3& value) noexcept;

	constexpr bool operator==(const Matrix4x4& rhs) const noexcept;
	constexpr bool operator!=(const Matrix4x4& rhs) const noexcept;

	constexpr float* operator[](const uint32 i) noexcept { return m_Data[i]; }
	constexpr const float* operator[](const uint32 i) const noexcept { return m_Data[i]; }

	constexpr Matrix4x4 operator+(const Matrix4x4& rhs) const noexcept;
	constexpr Matrix4x4 operator-(const Matrix4x4& rhs) const noexcept;

	// #note: Multiplication is Left -> Right (rather than Left <- Right).
	constexpr Matrix4x4 operator*(const Matrix4x4& rhs) const noexcept;

	constexpr Matrix4x4& operator+=(const Matrix4x4& rhs) noexcept { *this = *this + rhs; return *this; }
	constexpr Matrix4x4& operator-=(const Matrix4x4& rhs) noexcept { *this = *this - rhs; return *this; }
	constexpr Matrix4x4& operator*=(const Matrix4x4& rhs) noexcept { *this = *this * rhs; return *this; }

	void Inverse() noexcept;
	void Transpose() noexcept;

	void Translate(const Vector3f& value) noexcept;
	void Rotate(const Quaternion& value) noexcept;
	void Scale(const float value) noexcept;
	void Scale(const Vector3f& value) noexcept;

	bool IsValid() const noexcept;

	[[nodiscard]] Matrix4x4 Inversed() const noexcept;
	[[nodiscard]] Matrix4x4 Transposed() const noexcept;

	/// \brief Sets the translate of the matrix while preserving the rotate and scale.
	void SetTranslate(const Vector3f& value) noexcept;
	/// \brief Sets the rotate of the matrix while preserving the translate and scale.
	void SetRotate(const Rotator& value) noexcept;
	/// \brief Sets the rotate of the matrix while preserving the translate and scale.
	void SetRotate(const Quaternion& value) noexcept;
	/// \brief Sets the scale of the matrix while preserving the translate and rotate.
	void SetScale(const float value) noexcept;
	/// \brief Sets the scale of the matrix while preserving the translate and rotate.
	void SetScale(const Vector3f& value) noexcept;

	static Matrix4x4 FromTransform(const Vector3f& translate, const Rotator& rotate, const float scale) noexcept;
	static Matrix4x4 FromTransform(const Vector3f& translate, const Rotator& rotate, const Vector3f& scale) noexcept;
	static Matrix4x4 FromRotate(const Rotator& value) noexcept;

	static constexpr Matrix4x4 FromTransform(const Vector3f& translate, const float scale) noexcept;
	static constexpr Matrix4x4 FromTransform(const Vector3f& translate, const Vector3f& scale) noexcept;
	static constexpr Matrix4x4 FromTransform(const Vector3f& translate, const Matrix3x3& rotate, const float scale) noexcept;
	static constexpr Matrix4x4 FromTransform(const Vector3f& translate, const Matrix3x3& rotate, const Vector3f& scale) noexcept;
	static constexpr Matrix4x4 FromTransform(const Vector3f& translate, const Quaternion& rotate, const float scale) noexcept;
	static constexpr Matrix4x4 FromTransform(const Vector3f& translate, const Quaternion& rotate, const Vector3f& scale) noexcept;
	static constexpr Matrix4x4 FromTranslate(const Vector3f& value) noexcept;
	static constexpr Matrix4x4 FromRotate(const Quaternion& value) noexcept;
	static constexpr Matrix4x4 FromScale(const float value) noexcept;
	static constexpr Matrix4x4 FromScale(const Vector3f& value) noexcept;

	static const Matrix4x4 Identity;
	static const Matrix4x4 Zero;

public:
	float m_Data[4][4];
};

inline const Matrix4x4 Matrix4x4::Identity(
	1.f, 0.f, 0.f, 0.f,
	0.f, 1.f, 0.f, 0.f,
	0.f, 0.f, 1.f, 0.f,
	0.f, 0.f, 0.f, 1.f);

inline const Matrix4x4 Matrix4x4::Zero(
	0.f, 0.f, 0.f, 0.f,
	0.f, 0.f, 0.f, 0.f,
	0.f, 0.f, 0.f, 0.f,
	0.f, 0.f, 0.f, 0.f);

inline constexpr Vector2f operator*(const Vector2f& lhs, const Matrix4x4& rhs) noexcept;
inline constexpr Vector3f operator*(const Vector3f& lhs, const Matrix4x4& rhs) noexcept;
inline constexpr Vector4f operator*(const Vector4f& lhs, const Matrix4x4& rhs) noexcept;

#include "Math/Matrix.inl"