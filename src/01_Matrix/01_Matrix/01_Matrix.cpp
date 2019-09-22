#include "TMatrix.h"
#include <ctime>

template<typename ValueType>
ValueType random(ValueType min = ValueType(0), ValueType max = ValueType(1))
{
	return ValueType(rand() % (int)(max - min + ValueType(1))) + min;
}

int main()
{
	TVector<double> vectors[] =  { TVector<double>(3), TVector<double>(2) };
	TMatrix<double> matrixes[] = { TMatrix<double>(3), TMatrix<double>(3), TMatrix<double>(2),  TMatrix<double>(3) };

	enum Idx
	{
		A, B, C, Result
	};
	
	srand((unsigned)time(0));

	for (size_t i = 0; i < 2; i++)
		vectors[i].fillRandomly(0., 10.);
	for (size_t i = 0; i < 3; i++)
		matrixes[i].fillRandomly(0., 10.);
	matrixes[Idx::Result].fill(0.);

	std::cout << "Vector a: " << vectors[Idx::A] << '\n';
	std::cout << "Vector b: " << vectors[Idx::B] << '\n';

	std::cout << "Matrix A: " << matrixes[Idx::A] << '\n';
	std::cout << "Matrix B: " << matrixes[Idx::B] << '\n';
	std::cout << "Matrix C: " << matrixes[Idx::C] << '\n';

	std::cout << "Assignment B to A:" << std::endl;
	matrixes[Idx::A] = matrixes[Idx::B];

	std::cout << "Equality (A == B): ";
	if (matrixes[Idx::A] == matrixes[Idx::B])
		std::cout << "true" << std::endl;
	else
		std::cout << "false" << std::endl;

	std::cout << "\nInequality (A != B): ";
	if (matrixes[Idx::A] != matrixes[Idx::B])
		std::cout << "true" << std::endl;
	else
		std::cout << "false" << std::endl;

	std::cout << "\nAddition (A + B): " << std::endl;
	try
	{
		std::cout << (matrixes[Idx::Result] = matrixes[Idx::A] + matrixes[Idx::B]);
	}
	catch (MatrixDifferentSizes& e)
	{
		std::cout << e.what();
	}

	std::cout << "\nAddition (A + C): ";
	try
	{
		std::cout << (matrixes[Idx::Result] = matrixes[Idx::A] + matrixes[Idx::C]);
	}
	catch (MatrixDifferentSizes& e)
	{
		std::cout << e.what();
	}

	std::cout << "\nSubstraction (A - B): " << std::endl;
	try
	{
		std::cout << (matrixes[Idx::Result] = matrixes[Idx::A] - matrixes[Idx::B]);
	}
	catch (MatrixDifferentSizes& e)
	{
		std::cout << e.what();
	}

	std::cout << "\nSubstraction (A - C): " << std::endl;
	try
	{
		std::cout << (matrixes[Idx::Result] = matrixes[Idx::A] - matrixes[Idx::C]);
	}
	catch (MatrixDifferentSizes& e)
	{
		std::cout << e.what();
	}

	std::cout << "\nMultiplication (A * B): " << std::endl;
	try
	{
		std::cout << (matrixes[Idx::Result] = matrixes[Idx::A] * matrixes[Idx::B]);
	}
	catch (MatrixDifferentSizes& e)
	{
		std::cout << e.what();
	}

	std::cout << "\nMultiplication (A * C): " << std::endl;
	try
	{
		std::cout << (matrixes[Idx::Result] = matrixes[Idx::A] * matrixes[Idx::C]);
	}
	catch (MatrixDifferentSizes& e)
	{
		std::cout << e.what();
	}

	std::cout << "\nMultiplication (A * C): " << std::endl;
	try
	{
		std::cout << (matrixes[Idx::Result] = matrixes[Idx::A] * matrixes[Idx::C]);
	}
	catch (MatrixDifferentSizes& e)
	{
		std::cout << e.what();
	}

	double operand = random(0., 10.);
	std::cout << "\nAddition (A + " << operand << "): " << (matrixes[Idx::A] + operand);
	std::cout << "\nSubstraction (A - " << operand << "): " << (matrixes[Idx::A] - operand); 
	std::cout << "\nMultiplication (A * " << operand << "): " << (matrixes[Idx::A] * operand);

	std::cout << "\nMultiplication (A * a):" << std::endl;
	try
	{
		std::cout << (matrixes[Idx::A] * vectors[Idx::A]);
	}
	catch (VectorInvalidSize& e)
	{
		std::cout << e.what();
	}
	std::cout << "\nMultiplication (A * b):" << std::endl;
	try
	{
		std::cout << (matrixes[Idx::A] * vectors[Idx::B]);
	}
	catch (VectorInvalidSize& e)
	{
		std::cout << e.what();
	}

	std::cout << "\n Determinant of A: " << matrixes[Idx::A].determinant();
	std::cout << '\n';
	
}