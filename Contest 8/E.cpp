#include <cmath>
#include <complex>
#include <iostream>
#include <vector>

static const double kEpsilon = 1e-12;

bool compareDoubles(const double val1, const double val2);
void fft(std::vector<std::complex<double>>& coefficients, const bool is_inverse = false);

bool compareDoubles(const double val1, const double val2)
{
    return (val1 - val2) < kEpsilon;
}

void fft(std::vector<std::complex<double>>& coefficients, const bool is_inverse)
{
    size_t N = coefficients.size();
    std::vector<std::complex<double>> precalc(N, 0);
    precalc[0] = 1.0;

    for (size_t i = 1; i < N; i *= 2)
    {
        if (is_inverse)
        {
            precalc[i] = std::polar<double>(1,  2 * M_PI * i / N);
        }
        else
        {
            precalc[i] = std::polar<double>(1, -2 * M_PI * i / N);
        }
    }

    size_t prev = 2;
    for (size_t i = 3; i < N; ++i)
    {
        if (compareDoubles(precalc[i].real(), 0.0) && compareDoubles(precalc[i].imag(), 0.0))
        {
            precalc[i] = precalc[prev] * precalc[i - prev];
        }
        else
        {
            prev = i;
        }
    }

    for (size_t i = N; i > 1; i /= 2)
    {
        std::vector<std::complex<double>> new_coeff(N);
        for (size_t j = 0; j < N; j += i)
        {
            for (size_t k = 0; k < i; ++k)
            {
                new_coeff[j + i / 2 * (k % 2) + k / 2] = coefficients[j + k]; 
            }
        }
        coefficients = new_coeff;
    }

    for (size_t i = 2; i <= N; i *= 2)
    {
        std::vector<std::complex<double> > new_coeff(N);
        size_t mul_coeff = N / i;
        
        for(int j = 0; j < N; j += i) {
            for(int k = 0; k < i / 2; ++k) {
                new_coeff[j + k]         = coefficients[j + k] + precalc[mul_coeff * k] * coefficients[j + i / 2 + k];
                new_coeff[j + i / 2 + k] = coefficients[j + k] - precalc[mul_coeff * k] * coefficients[j + i / 2 + k];
            }
        }
        coefficients = new_coeff;
    }
}

class Polynomial
{
public:

    Polynomial()
      :  coefficients ()
    {}

    explicit Polynomial(const std::vector<double>& _coefficients)
      :  coefficients (_coefficients)
    {}

    friend std::ostream& operator<<(std::ostream& output, const Polynomial& poly)
    {
        output << poly.coefficients.size() - 1 << ' ';

        for (const auto coeff : poly.coefficients)
        {
            output << static_cast<long long>(round(coeff)) << ' ';
        }
        output << '\n';

        return output;
    }

    friend std::istream& operator>>(std::istream& input, Polynomial& poly)
    {
        size_t arr_size = 0;
        input >> arr_size;

        poly.coefficients.resize(arr_size + 1, 0);

        for (size_t i = 0; i <= arr_size; ++i)
        {
            input >> poly.coefficients[i];
        }

        return input;
    }

    friend Polynomial operator*(const Polynomial& lhs, const Polynomial& rhs)
    {
        size_t arr_size = 1;
        size_t res_size = lhs.coefficients.size() + rhs.coefficients.size() - 1;
        while (arr_size < res_size) arr_size *= 2;

        std::vector<std::complex<double>> lhs_compl(arr_size, 0), rhs_compl(arr_size, 0), multiply(arr_size, 0);
        lhs.toComplexVector(lhs_compl);
        rhs.toComplexVector(rhs_compl);
        
        fft(lhs_compl);
        fft(rhs_compl);

        for (size_t i = 0; i < arr_size; ++i)
        {
            multiply[i] = lhs_compl[i] * rhs_compl[i];
        }

        fft(multiply, true);

        std::vector<double> res_vec(res_size);
        for (size_t i = 0; i < res_size; ++i)
        {
            res_vec[i] = multiply[i].real() / arr_size;
        }

        return Polynomial(res_vec);
    }

private:

    void toComplexVector(std::vector<std::complex<double>>& result) const
    {
        size_t N = coefficients.size();
        for (size_t i = 0; i < N; ++i)
        {
            result[i] = coefficients[i];
        }
    }

private:
    std::vector<double> coefficients;
};

int main()
{
    Polynomial pol1, pol2;
    std::cin >> pol1 >> pol2;

    Polynomial mult = pol1 * pol2;
    std::cout << mult;

    return 0;
}