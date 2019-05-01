#pragma once
#include "Complex.h"
#include "../CommonExport.h"

class COMMON_DLL_EXPORT CFFT
{
public:
	//   FORWARD FOURIER TRANSFORM
	//     Input  - input data
	//     Output - transform result
	//     N      - length of both input data and result
	static bool Forward(const CComplex *const Input, CComplex *const Output, const unsigned int N);

	//   FORWARD FOURIER TRANSFORM, INPLACE VERSION
	//     Data - both input data and output
	//     N    - length of input data
	static bool Forward(CComplex *const Data, const unsigned int N);

	//   INVERSE FOURIER TRANSFORM
	//     Input  - input data
	//     Output - transform result
	//     N      - length of both input data and result
	//     Scale  - if to scale result
	static bool Inverse(const CComplex *const Input, CComplex *const Output, const unsigned int N, const bool Scale = true);

	//   INVERSE FOURIER TRANSFORM, INPLACE VERSION
	//     Data  - both input data and output
	//     N     - length of both input data and result
	//     Scale - if to scale result
	static bool Inverse(CComplex *const Data, const unsigned int N, const bool Scale = true);

protected:
	//   Rearrange function and its inplace version
	static void Rearrange(const CComplex *const Input, CComplex *const Output, const unsigned int N);
	static void Rearrange(CComplex *const Data, const unsigned int N);

	//   FFT implementation
	static void Perform(CComplex *const Data, const unsigned int N, const bool Inverse = false);

	//   Scaling of inverse FFT result
	static void Scale(CComplex *const Data, const unsigned int N);
};