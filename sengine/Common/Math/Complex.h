#pragma once
#include "../CommonExport.h"

class COMMON_DLL_EXPORT CComplex
{
protected:
	//   Internal presentation - real and imaginary parts
	double m_re;
	double m_im;

public:
	//   Imaginary unity
	static const CComplex i;
	static const CComplex j;

	//   Constructors
	CComplex() : m_re(0.), m_im(0.) {}
	CComplex(double re, double im) : m_re(re), m_im(im) {}
	CComplex(double val) : m_re(val), m_im(0.) {}

	//   Assignment
	CComplex& operator= (const double val)
	{
		m_re = val;
		m_im = 0.;
		return *this;
	}

	//   Basic operations - taking parts
	double re() const { return m_re; }
	double im() const { return m_im; }

	//   Conjugate number
	CComplex conjugate() const
	{
		return CComplex(m_re, -m_im);
	}

	//   Norm   
	double norm() const
	{
		return m_re * m_re + m_im * m_im;
	}

	//   Arithmetic operations
	CComplex operator+ (const CComplex& other) const
	{
		return CComplex(m_re + other.m_re, m_im + other.m_im);
	}

	CComplex operator- (const CComplex& other) const
	{
		return CComplex(m_re - other.m_re, m_im - other.m_im);
	}

	CComplex operator* (const CComplex& other) const
	{
		return CComplex(m_re * other.m_re - m_im * other.m_im,
			m_re * other.m_im + m_im * other.m_re);
	}

	CComplex operator/ (const CComplex& other) const
	{
		const double denominator = other.m_re * other.m_re + other.m_im * other.m_im;
		return CComplex((m_re * other.m_re + m_im * other.m_im) / denominator,
			(m_im * other.m_re - m_re * other.m_im) / denominator);
	}

	CComplex& operator+= (const CComplex& other)
	{
		m_re += other.m_re;
		m_im += other.m_im;
		return *this;
	}

	CComplex& operator-= (const CComplex& other)
	{
		m_re -= other.m_re;
		m_im -= other.m_im;
		return *this;
	}

	CComplex& operator*= (const CComplex& other)
	{
		const double temp = m_re;
		m_re = m_re * other.m_re - m_im * other.m_im;
		m_im = m_im * other.m_re + temp * other.m_im;
		return *this;
	}

	CComplex& operator/= (const CComplex& other)
	{
		const double denominator = other.m_re * other.m_re + other.m_im * other.m_im;
		const double temp = m_re;
		m_re = (m_re * other.m_re + m_im * other.m_im) / denominator;
		m_im = (m_im * other.m_re - temp * other.m_im) / denominator;
		return *this;
	}

	CComplex& operator++ ()
	{
		++m_re;
		return *this;
	}

	CComplex operator++ (int)
	{
		CComplex temp(*this);
		++m_re;
		return temp;
	}

	CComplex& operator-- ()
	{
		--m_re;
		return *this;
	}

	CComplex operator-- (int)
	{
		CComplex temp(*this);
		--m_re;
		return temp;
	}

	CComplex operator+ (const double val) const
	{
		return CComplex(m_re + val, m_im);
	}

	CComplex operator- (const double val) const
	{
		return CComplex(m_re - val, m_im);
	}

	CComplex operator* (const double val) const
	{
		return CComplex(m_re * val, m_im * val);
	}

	CComplex operator/ (const double val) const
	{
		return CComplex(m_re / val, m_im / val);
	}

	CComplex& operator+= (const double val)
	{
		m_re += val;
		return *this;
	}

	CComplex& operator-= (const double val)
	{
		m_re -= val;
		return *this;
	}

	CComplex& operator*= (const double val)
	{
		m_re *= val;
		m_im *= val;
		return *this;
	}

	CComplex& operator/= (const double val)
	{
		m_re /= val;
		m_im /= val;
		return *this;
	}

	friend CComplex operator+ (const double left, const CComplex& right)
	{
		return CComplex(left + right.m_re, right.m_im);
	}

	friend CComplex operator- (const double left, const CComplex& right)
	{
		return CComplex(left - right.m_re, -right.m_im);
	}

	friend CComplex operator* (const double left, const CComplex& right)
	{
		return CComplex(left * right.m_re, left * right.m_im);
	}

	friend CComplex operator/ (const double left, const CComplex& right)
	{
		const double denominator = right.m_re * right.m_re + right.m_im * right.m_im;
		return CComplex(left * right.m_re / denominator,
			-left * right.m_im / denominator);
	}

	//   Boolean operators
	bool operator== (const CComplex &other) const
	{
		return m_re == other.m_re && m_im == other.m_im;
	}

	bool operator!= (const CComplex &other) const
	{
		return m_re != other.m_re || m_im != other.m_im;
	}

	bool operator== (const double val) const
	{
		return m_re == val && m_im == 0.;
	}

	bool operator!= (const double val) const
	{
		return m_re != val || m_im != 0.;
	}

	friend bool operator== (const double left, const CComplex& right)
	{
		return left == right.m_re && right.m_im == 0.;
	}

	friend bool operator!= (const double left, const CComplex& right)
	{
		return left != right.m_re || right.m_im != 0.;
	}
};