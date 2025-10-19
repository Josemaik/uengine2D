#pragma once

template<typename Data>
struct Vec4
{
	constexpr Vec4() = default;
	Vec4(Data _r, Data _g,Data _b,Data _a) :m_r(_r), m_g(_g),m_b(_b),m_a(_a) {}

	Data m_r{}, m_g{}, m_b{}, m_a{};
};

using vec4f = Vec4<float>;