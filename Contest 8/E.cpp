#include <cmath>
#include <complex>
#include <iostream>
#include <vector>

void fft(const std::vector<std::complex<double>>& coefficients, std::vector<std::complex<double>>& result, bool is_inverse = false);

void fft(const std::vector<std::complex<double>>& coefficients, std::vector<std::complex<double>>& result, bool is_inverse)
{
    size_t N = coefficients.size();

    result.resize(N);

    if (N == 0) return;
    if (N == 1)
    {
        result[0] = coefficients[0];
        return;
    }

    std::vector<std::complex<double>>     even,     odd;
    std::vector<std::complex<double>> res_even, res_odd;

    for (size_t i = 0; i < N; ++i)
    {
        if (i % 2 == 1) odd .push_back(coefficients[i]);
        else            even.push_back(coefficients[i]);
    }

    fft(even, res_even, is_inverse);
    fft(odd , res_odd , is_inverse);

    std::complex<double> delta = std::exp(std::complex<double>(0, 2 * M_PI / N));
    if (is_inverse)      delta = std::complex<double>(1, 0) / delta;

    std::complex<double> temp(1);
    for (size_t i = 0; i < N / 2; ++i)
    {
        result[i]         = even[i] + temp * odd[i];
        result[i + N / 2] = even[i] - temp * odd[i];

        if (is_inverse)
        {
            result[i]         /= 2;
            result[i + N / 2] /= 2;
        }

        temp *= delta;
    }
}

class Polynomial
{
public:

    explicit Polynomial()
      :  coefficients ()
    {}

    explicit Polynomial(std::vector<double>& _coefficients)
      :  coefficients (_coefficients)
    {}

    friend std::ostream& operator<<(std::ostream& output, const Polynomial& poly)
    {
        output << poly.coefficients.size() - 1 << ' ';

        for (const auto coeff : poly.coefficients)
        {
            output << coeff << ' ';
        }
        output << '\n';

        return output;
    }

    friend std::istream& operator>>(std::istream& input, Polynomial& poly)
    {
        size_t arr_size = 0;
        input >> arr_size;

        poly.coefficients.resize(arr_size, 0);

        for (size_t i = 0; i < arr_size; ++i)
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

        std::vector<std::complex<double>> lhs_compl    (arr_size, 0), rhs_compl    (arr_size, 0), multiply    (arr_size, 0);
        std::vector<std::complex<double>> lhs_compl_fft(arr_size, 0), rhs_compl_fft(arr_size, 0), multiply_fft(arr_size, 0);
        lhs.toComplexVector(lhs_compl);
        rhs.toComplexVector(rhs_compl);
        
        fft(lhs_compl, lhs_compl_fft);
        fft(rhs_compl, rhs_compl_fft);

        for (size_t i = 0; i < arr_size; ++i)
        {
            multiply[i] = lhs_compl_fft[i] * rhs_compl_fft[i];
        }

        fft(multiply, multiply_fft, true);

        std::vector<double> res_vec(res_size);
        for (size_t i = 0; i < res_size; ++i)
        {
            res_vec[i] = multiply_fft[i].real() / arr_size;
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