#pragma once

#include <iostream>

#define PI 3.14159265358979323846

template<typename Data>
class Vec2
{
public:
	constexpr Vec2() = default;

	constexpr void setX(Data const x) { m_x = x; }
	constexpr void setY(Data const y) { m_y = y; }

	Vec2(Data _x, Data _y) :m_x(_x), m_y(_y) {}
	//Vector/Vector operations
	Vec2 operator+(const Vec2& _other)
	{
		return Vec2(m_x + _other.m_x, m_y + _other.m_y);
	}
	Vec2 operator-(const Vec2& _other) const
	{
		return Vec2(m_x - _other.m_x, m_y - _other.m_y);
	}
	Vec2 operator*(const Vec2& _other) const
	{
		return Vec2(m_x * _other.m_x, m_y * _other.m_y);
	}
	Vec2 operator/(const Vec2& _other)
	{
		return Vec2(m_x / _other.m_x, m_y / _other.m_y);
	}
	//Vector/Escalar operators
	Vec2 operator+(const Data& _value)
	{
		return Vec2{ m_x + _value,m_y + _value };
	}
	Vec2 operator-(const Data& _value)
	{
		return Vec2{ m_x - _value,m_y - _value };
	}
	Vec2 operator*(const Data& _value) const
	{
		return Vec2{ m_x * _value,m_y * _value };
	}

	Vec2 operator/(const Data& _value) const
	{
		return Vec2{ m_x / _value, m_y / _value };
	}
	constexpr Vec2 operator/=(Vec2 const& rhs)
	{
		m_x /= rhs.m_x;
		m_y /= rhs.m_y;
		return *this;
	}
	constexpr Vec2 operator+=(Vec2 const& rhs)
	{
		m_x += rhs.m_x;
		m_y += rhs.m_y;
		return *this;
	}
	constexpr Vec2 operator/=(Data const s)
	{
		m_x /= s;
		m_y /= s;
		return *this;
	}
	const Vec2& operator=(const Vec2& rhs)
	{
		if (this != &rhs) {  // Verificación de autoasignación
			m_x = rhs.m_x;
			m_y = rhs.m_y;
		}
		return *this;
	}

	//Others operations

	Vec2 abs(const Vec2& _other)
	{
		return Vec2(std::abs(_other.m_x), std::abs(_other.m_y));
	}

	Data DotProduct(const Vec2& _other) const
	{
		return { m_x * _other.m_x + m_y * _other.m_y };
	}

	Data LengthSQ() const
	{
		return DotProduct(*this);
	}


	Data Length() const
	{
		if (!length)
		{
			length = std::sqrt(LengthSQ());
		}
		return length;
	}

	Vec2& normalize()
	{
		auto const len = Length();

		if (len >= 1.f)
			*this /= len;
		else
			*this /= (1.f / len);

		return *this;
	}

	constexpr Data angle(const Vec2& _other) const
	{
		return std::acos(DotProduct(_other) / (Length() * _other.Length()));
	}

	Data angleDeg(const Vec2& _other) const
	{
		return static_cast<Data>(angle(_other) * 180 / PI);
	}

	Data distance(const Vec2& _other) const
	{
		auto dx{ _other.m_x - m_x };
		auto dy{ _other.m_y - m_y };
		return std::sqrt(dx * dx + dy * dy);
	}

	Data m_x{}, m_y{};
	mutable Data length{};
};



using vec2f = Vec2<float>;

