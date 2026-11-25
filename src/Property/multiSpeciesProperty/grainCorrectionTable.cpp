/*------------------------------- phasicFlow ---------------------------------
      O        C enter of
     O O       E ngineering and
    O   O      M ultiscale modeling of
   OOOOOOO     F luid flow
------------------------------------------------------------------------------
  Copyright (C): www.cemf.ir
  email: hamid.r.norouzi AT gmail.com
------------------------------------------------------------------------------
Licence:
  This file is part of phasicFlow code. It is a free software for simulating
  granular and multiphase flows. You can redistribute it and/or modify it under
  the terms of GNU General Public License v3 or any other later versions.

  phasicFlow is distributed to help others in their research in the field of
  granular and multiphase flows, but WITHOUT ANY WARRANTY; without even the
  implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

-----------------------------------------------------------------------------*/

#include "grainCorrectionTable.hpp"
#include <cmath>
#include <vector>
#include <algorithm>
#include <atomic>
#include <thread>

namespace pFlow
{
namespace
{

using vec = std::vector<real>;

/// Finite-volume grid of the spherical particle, uniform in radius.
struct pelletGrid
{
    int32 N;

    /// Cell volumes divided by 4 pi, and their fractions of the particle.
    vec V, w;

    /// Conductance between cell i-1 and cell i (G[0] is unused).
    vec G;

    /// Conductance between the last cell and the surface.
    real Gs;

    explicit pelletGrid(int32 n)
    :
        N(n), V(n), w(n), G(n, real(0))
    {
        const real h = real(1) / n;

        for (int32 i = 0; i < n; ++i)
        {
            const real f0 = i * h;
            const real f1 = (i + 1) * h;

            V[i] = (f1 * f1 * f1 - f0 * f0 * f0) / real(3);
            w[i] = real(3) * V[i];

            if (i > 0) G[i] = f0 * f0 / h;
        }

        Gs = real(1) / (real(0.5) * h);
    }
};

/// Solves the tridiagonal system with diagonal d, equal sub- and
/// super-diagonal G[1..N-1] and right-hand side b; the solution is in b.
void solveTridiagonal(const pelletGrid& P, vec& d, vec& b)
{
    const int32 N = P.N;

    for (int32 i = 1; i < N; ++i)
    {
        const real m = P.G[i] / d[i - 1];

        d[i] -= m * P.G[i];
        b[i] -= m * b[i - 1];
    }

    b[N - 1] /= d[N - 1];

    for (int32 i = N - 2; i >= 0; --i)
        b[i] = (b[i] - P.G[i + 1] * b[i + 1]) / d[i];
}

/// Rate of a grain, psi_s^n, for the gas concentration psi around it.
/// psi_s is the concentration at the front of the grain, behind the product
/// layer of resistance q = 6 g xi (1 - xi):
///
///   q psi_s^n + psi_s = psi
///
/// dRate is the derivative of the rate with respect to psi. x holds a first
/// guess of psi_s on entry and psi_s on return.
real grainRate(
    const real psi,
    const real q,
    const real n,
    real&      dRate,
    real&      x)
{
    if (n == real(1))
    {
        dRate = real(1) / (real(1) + q);
        x     = psi * dRate;
        return x;
    }

    if (psi <= real(0))
    {
        dRate = real(0);
        x     = real(0);
        return real(0);
    }

    if (q <= real(0))
    {
        x = psi;
    }
    else
    {
        if (!(x > real(0) && x < psi)) x = psi;

        // Newton iteration kept inside the bracket [lo, hi]
        real lo = real(0);
        real hi = psi;

        for (int32 it = 0; it < 100; ++it)
        {
            const real xn = std::pow(x, n);
            const real F  = q * xn + x - psi;

            if (std::abs(F) <= real(1e-14) * psi) break;

            if (F > real(0)) hi = x;
            else             lo = x;

            real xNew = x - F / (q * n * xn / x + real(1));

            if (!(xNew > lo && xNew < hi)) xNew = real(0.5) * (lo + hi);

            x = xNew;
        }
    }

    const real xn = std::pow(x, n);
    const real dx = n * xn / x;

    dRate = dx / (real(1) + q * dx);

    return xn;
}

/// Quasi-steady gas profile psi for the local coefficients a and q and the
/// order n. psi and psiS hold the previous profiles of the gas and of the
/// grain fronts on entry (start of the Newton iterations).
void solveGas(
    const pelletGrid& P,
    const vec&        a,
    const vec&        q,
    const real        n,
    vec&              psi,
    vec&              psiS,
    vec&              d,
    vec&              b)
{
    const int32 N = P.N;

    // Diagonal of the diffusion operator
    auto diffusionDiag = [&](int32 i)
    {
        return i == N - 1
            ? -P.G[i] - P.Gs
            : -(P.G[i] + P.G[i + 1]);
    };

    if (n == real(1))
    {
        for (int32 i = 0; i < N; ++i)
        {
            d[i] = diffusionDiag(i) - a[i] * P.V[i] / (real(1) + q[i]);
            b[i] = real(0);
        }

        b[N - 1] = -P.Gs;

        solveTridiagonal(P, d, b);
        psi = b;
        return;
    }

    for (int32 i = 0; i < N; ++i)
        psi[i] = std::min(std::max(psi[i], real(1e-12)), real(1));

    for (int32 it = 0; it < 60; ++it)
    {
        for (int32 i = 0; i < N; ++i)
        {
            real       dRate;
            const real rate = grainRate(psi[i], q[i], n, dRate, psiS[i]);
            const real aV   = a[i] * P.V[i];
            real       F    = diffusionDiag(i) * psi[i] - aV * rate;

            if (i > 0)     F += P.G[i] * psi[i - 1];
            if (i < N - 1) F += P.G[i + 1] * psi[i + 1];
            if (i == N - 1) F += P.Gs;

            d[i] = diffusionDiag(i) - aV * dRate;
            b[i] = -F;
        }

        solveTridiagonal(P, d, b);

        // Damping keeps the profile positive
        real lam = real(1);

        auto negative = [&]()
        {
            for (int32 i = 0; i < N; ++i)
                if (psi[i] + lam * b[i] < real(0)) return true;
            return false;
        };

        while (negative() && lam > real(1e-3)) lam *= real(0.5);

        real change = real(0);

        for (int32 i = 0; i < N; ++i)
        {
            change = std::max(change, std::abs(b[i]));
            psi[i] = std::min(
                std::max(psi[i] + lam * b[i], real(1e-14)), real(1));
        }

        if (change < real(1e-11)) break;
    }
}

/// Effectiveness factor of a sphere.
real effectiveness(real phi)
{
    if (phi < real(1e-3)) return real(1) - phi * phi / real(15);

    return real(3) * (phi / std::tanh(phi) - real(1)) / (phi * phi);
}

/// Integrates the grain model for one (sigma2, g) and returns the
/// conversion X and the rate dX/dt at every step.
void integrate(
    const pelletGrid& P,
    const real        sigma2,
    const real        g,
    const real        n,
    vec&              Xs,
    vec&              rs)
{
    constexpr int32 nSteps = 10000;
    constexpr real  Xstop  = real(0.96);

    const int32 N    = P.N;
    const real  beta = real(6) * g;
    const real  tEnd = real(1) + g + sigma2;
    const real  dt   = tEnd / nSteps;

    vec fr(N, real(1)), f1(N), psi(N, real(1)), psiS(N, real(1));
    vec a(N), q(N), k1(N), k2(N), d(N), b(N);

    // Front velocity of the grains for the fronts f
    auto rates = [&](const vec& f, vec& k)
    {
        for (int32 i = 0; i < N; ++i)
        {
            a[i] = f[i] > real(1e-12)
                ? real(18) * sigma2 * f[i] * f[i]
                : real(0);
            q[i] = beta * f[i] * (real(1) - f[i]);
        }

        solveGas(P, a, q, n, psi, psiS, d, b);

        for (int32 i = 0; i < N; ++i)
        {
            real dRate;

            k[i] = f[i] > real(1e-12)
                ? -grainRate(psi[i], q[i], n, dRate, psiS[i])
                : real(0);
        }
    };

    auto record = [&](const vec& k)
    {
        real X = real(1);
        real r = real(0);

        for (int32 i = 0; i < N; ++i)
        {
            X -= P.w[i] * fr[i] * fr[i] * fr[i];
            r -= P.w[i] * real(3) * fr[i] * fr[i] * k[i];
        }

        Xs.push_back(X);
        rs.push_back(r);
    };

    Xs.clear();
    rs.clear();

    rates(fr, k1);
    record(k1);

    real t = real(0);

    // Heun steps of the fronts
    while (Xs.back() < Xstop && t < real(50) * tEnd)
    {
        for (int32 i = 0; i < N; ++i)
            f1[i] = std::max(fr[i] + dt * k1[i], real(0));

        rates(f1, k2);

        for (int32 i = 0; i < N; ++i)
        {
            fr[i] = std::max(
                fr[i] + real(0.5) * dt * (k1[i] + k2[i]), real(0));
        }

        t += dt;

        rates(fr, k1);
        record(k1);
    }
}

} // namespace

void buildGrainCorrectionTable(
    real        order,
    realVector& sigma2,
    realVector& g,
    realVector& X,
    realVector& lnc)
{
    constexpr int32 nCells = 300;

    sigma2.clear();
    g.clear();
    X.clear();
    lnc.clear();

    // sigma2 from 0.01 to 327.68 with the ratio sqrt(2)
    for (int32 i = 0; i <= 30; ++i)
        sigma2.push_back(real(0.01) * std::pow(real(2), real(0.5) * i));

    for (real v : {0.0, 0.25, 0.5, 1.0, 2.0, 4.0}) g.push_back(v);

    // X from 0.001 to 0.1 in steps of 0.001, then to 0.95 in steps of 0.01
    for (int32 k = 1; k <= 100; ++k) X.push_back(real(0.001) * k);
    for (int32 k = 11; k <= 95; ++k) X.push_back(real(0.01) * k);

    const pelletGrid P(nCells);

    const size_t nS = sigma2.size();
    const size_t nG = g.size();
    const size_t nX = X.size();

    lnc.resize(nS * nG * nX, real(0));

    // One integration per (sigma2, g), shared among the hardware threads
    std::atomic<size_t> next(0);

    auto work = [&]()
    {
        vec Xs, rs;

        for (size_t c = next++; c < nS * nG; c = next++)
        {
            const size_t i = c / nG;
            const size_t j = c % nG;

            integrate(P, sigma2[i], g[j], order, Xs, rs);

            size_t p = 0;

            for (size_t k = 0; k < X.size(); ++k)
            {
                // Rate of the grain model at X[k], linear between steps
                while (p + 2 < Xs.size() && Xs[p + 1] < X[k]) ++p;

                const real wgt = (X[k] - Xs[p]) / (Xs[p + 1] - Xs[p]);
                const real r   = (real(1) - wgt) * rs[p] + wgt * rs[p + 1];

                const real s     = std::cbrt(real(1) - X[k]);
                const real pore  = real(6) * sigma2[i] * s * (real(1) - s);
                const real start =
                    real(1)
                    / effectiveness(std::sqrt(real(18) * sigma2[i]) * s)
                    - real(1);

                const real closedForm =
                    real(1) + real(6) * g[j] * s * (real(1) - s)
                    + std::max(pore, start);

                lnc[(i * nG + j) * nX + k] =
                    std::log(real(3) * s * s / (r * closedForm));
            }
        }
    };

    const size_t nThreads =
        std::max<size_t>(1, std::thread::hardware_concurrency());

    std::vector<std::thread> threads;

    for (size_t t = 1; t < nThreads; ++t) threads.emplace_back(work);

    work();

    for (auto& t : threads) t.join();
}

//+ + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + +

} // pFlow
