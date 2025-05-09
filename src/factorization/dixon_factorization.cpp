#include "dixon_factorization.h"

namespace BigPrimeLib {

DixonFactorizer::DixonFactorizer(size_t b_lim) : sieve_(b_lim) {}

DixonFactorizer::DixonFactorizer(const PrimeTester &prime_tester, size_t b_lim)
    : prime_tester_(prime_tester), sieve_(b_lim) {}

PrimalityStatus DixonFactorizer::primality_test(const BigInt &n) {
    return prime_tester_ ? prime_tester_->test(n) : PrimalityStatus::Uncertain;
}

std::optional<std::vector<size_t>> DixonFactorizer::factor_over_base(BigInt x) {
    std::vector<size_t> exps(sieve_.primes.size());
    for (size_t i = 0; i < sieve_.primes.size(); ++i) {
        while (x % sieve_.primes[i] == 0) {
            x /= sieve_.primes[i];
            exps[i]++;
        }
    }
    if (x != 1) {
        return std::nullopt;
    }
    return exps;
}

std::vector<boost::dynamic_bitset<>> DixonFactorizer::nullspace_mod2(std::vector<boost::dynamic_bitset<>> mat) {
    size_t rows = mat.size();
    size_t cols = mat[0].size();

    std::vector<size_t> pivot(rows, cols);
    size_t cur_r = 0;
    for (size_t c = 0; c < cols; ++c) {
        for (size_t r = cur_r; r < rows; ++r) {
            if (mat[r][c]) {
                std::swap(mat[cur_r], mat[r]);
                break;
            }
        }
        if (!mat[cur_r][c]) {
            continue;
        }

        pivot[cur_r] = c;
        for (size_t i = 0; i < rows; ++i) {
            if (i != cur_r && mat[i][c]) {
                mat[i] ^= mat[cur_r];
            }
        }
        ++cur_r;
        if (cur_r == rows) {
            break;
        }
    }

    std::vector<size_t> free_cols;
    free_cols.reserve(cols);
    for (size_t r = 0; r + 1 < rows; ++r) {
        for (size_t c = pivot[r] + 1; c < pivot[r + 1]; ++c) {
            free_cols.push_back(c);
        }
    }

    std::vector<boost::dynamic_bitset<>> basis;
    basis.reserve(free_cols.size());
    for (size_t fc : free_cols) {
        boost::dynamic_bitset<> v(cols);
        v[fc] = true;

        size_t i = rows;
        do {
            --i;
            if (pivot[i] == cols) {
                continue;
            }
            bool s = false;
            for (size_t j = pivot[i] + 1; j < cols; ++j) {
                s ^= (mat[i][j] & v[j]);
            }
            v[pivot[i]] = s;
        } while (i > 0);
        basis.push_back(v);
    }
    return basis;
}

std::optional<BigInt> DixonFactorizer::find_factor(const BigInt &n) {
    for (auto p : sieve_.primes) {
        if (n == p) {
            return std::nullopt;
        }
        if (n % p == 0) {
            return p;
        }
    }

    BigInt root = Math::sqrt(n);
    if (root * root == n) {
        return root;
    }

    size_t np = sieve_.primes.size();
    std::vector<Rels> rels;
    rels.reserve(np + 3);
    for (size_t k = 1; rels.size() < np + 3 && root + k < n; ++k) {
        BigInt x = root + k;
        auto y = x * x % n;
        if (y == 0) {
            return Math::gcd(x, n);
        }
        auto e = factor_over_base(y);
        if (e.has_value()) {
            rels.push_back({x, y, *e});
        }
    }
    if (rels.size() <= np) {
        return std::nullopt;
    }

    size_t rows = rels.size();
    size_t cols = np;

    std::vector<boost::dynamic_bitset<>> mat(cols, boost::dynamic_bitset<>(rows));
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            mat[j][i] = rels[i].exponents[j] % 2;
        }
    }
    std::vector<boost::dynamic_bitset<>> deps = nullspace_mod2(mat);

    for (auto &dep : deps) {
        std::vector<size_t> inds;
        for (size_t i = 0; i < dep.size(); ++i) {
            if (dep[i]) {
                inds.push_back(i);
            }
        }
        if (inds.empty()) {
            continue;
        }
        BigInt a = 1;
        std::vector<size_t> exp_sum(np);
        for (auto i : inds) {
            auto &rel = rels[i];
            a = a * rel.x % n;
            for (size_t j = 0; j < np; ++j) {
                exp_sum[j] += rel.exponents[j];
            }
        }
        BigInt b = 1;
        for (size_t i = 0; i < np; ++i) {
            BigInt p = sieve_.primes[i];
            b = b * Math::powm(p, exp_sum[i] / 2, n) % n;
        }
        auto d = Math::gcd(a - b, n);
        if (d != 1 && d != n) {
            return d;
        }
    }
    return std::nullopt;
}

}
