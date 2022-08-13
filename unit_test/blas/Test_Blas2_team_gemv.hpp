#include <KokkosBlas_util.hpp>
#include <KokkosKernels_TestUtils.hpp>  // for test/inst guards
// Note: include serial gemv before util so it knows if CompactMKL is available
#include <Test_Blas2_gemv_util.hpp>
#include <KokkosBlas2_team_gemv.hpp>

namespace Test {

template <class AType, class XType, class YType, class ScalarType,
          class AlgoTag>
KK_DEFINE_BLAS2_GEMV_TEST_OP_CLASS(TeamGEMVOp)
template <typename TeamMember>
KOKKOS_INLINE_FUNCTION void operator()(const TeamMember& member) const {
  KokkosBlas::Experimental::Gemv<KokkosBlas::Mode::Team, AlgoTag>::invoke(
      member, params::trans, params::alpha, params::A, params::x, params::beta,
      params::y);
}
KK_END_BLAS2_GEMV_TEST_OP_CLASS

struct TeamGemvFactory {
  template <class AlgoTag, class ViewTypeA, class ViewTypeX, class ViewTypeY,
            class Device, class ScalarType>
  using functor_type =
      TeamGEMVOp<ViewTypeA, ViewTypeX, ViewTypeY, ScalarType, AlgoTag>;

  using algorithms = std::tuple<KokkosBlas::Algo::Gemv::Unblocked,
                                KokkosBlas::Algo::Gemv::Blocked>;

  template <class... Params>
  static constexpr bool allow_algorithm = true;

  template <class... Params>
  static bool allow_mode(char trans) {
    return true;
  }
};

}  // namespace Test

#define TEST_TEAM_CASE4(N, A, X, Y, SC) \
  TEST_CASE4(team, TeamGemvFactory, N, A, X, Y, SC)
#define TEST_TEAM_CASE2(N, S, SC) TEST_CASE2(team, TeamGemvFactory, N, S, SC)
#define TEST_TEAM_CASE(N, S) TEST_CASE(team, TeamGemvFactory, N, S)

#ifdef KOKKOSKERNELS_TEST_FLOAT
TEST_TEAM_CASE(float, float)
#endif

#ifdef KOKKOSKERNELS_TEST_DOUBLE
TEST_TEAM_CASE(double, double)
#endif

#ifdef KOKKOSKERNELS_TEST_COMPLEX_DOUBLE
TEST_TEAM_CASE(complex_double, Kokkos::complex<double>)
#endif

#ifdef KOKKOSKERNELS_TEST_COMPLEX_FLOAT
TEST_TEAM_CASE(complex_float, Kokkos::complex<float>)
#endif

#ifdef KOKKOSKERNELS_TEST_INT
TEST_TEAM_CASE(int, int)
#endif

#ifdef KOKKOSKERNELS_TEST_ALL_TYPES
// test mixed scalar types (void -> default alpha/beta)
TEST_TEAM_CASE4(mixed, double, int, float, void)

// test arbitrary double alpha/beta with complex<double> values
TEST_TEAM_CASE2(alphabeta, Kokkos::complex<double>, double)
#endif

#undef TEST_TEAM_CASE4
#undef TEST_TEAM_CASE2
#undef TEST_TEAM_CASE
