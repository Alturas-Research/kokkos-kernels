/*
//@HEADER
// ************************************************************************
//
//                        Kokkos v. 3.0
//       Copyright (2020) National Technology & Engineering
//               Solutions of Sandia, LLC (NTESS).
//
// Under the terms of Contract DE-NA0003525 with NTESS,
// the U.S. Government retains certain rights in this software.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
// 1. Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright
// notice, this list of conditions and the following disclaimer in the
// documentation and/or other materials provided with the distribution.
//
// 3. Neither the name of the Corporation nor the names of the
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY NTESS "AS IS" AND ANY
// EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
// PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL NTESS OR THE
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
// LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// Questions? Contact Siva Rajamanickam (srajama@sandia.gov)
//
// ************************************************************************
//@HEADER
*/
#ifndef KOKKOSBLAS1_ROTM_SPEC_HPP_
#define KOKKOSBLAS1_ROTM_SPEC_HPP_

#include <KokkosKernels_config.h>
#include <Kokkos_Core.hpp>

// Include the actual functors
#if !defined(KOKKOSKERNELS_ETI_ONLY) || KOKKOSKERNELS_IMPL_COMPILE_LIBRARY
#include <KokkosBlas1_rotm_impl.hpp>
#endif

namespace KokkosBlas {
namespace Impl {
// Specialization struct which defines whether a specialization exists
template <class execution_space, class VectorView, class ParamView>
struct rotm_eti_spec_avail {
  enum : bool { value = false };
};
}  // namespace Impl
}  // namespace KokkosBlas

//
// Macro for declaration of full specialization availability
// KokkosBlas::Impl::Rotm.  This is NOT for users!!!  All
// the declarations of full specializations go in this header file.
// We may spread out definitions (see _INST macro below) across one or
// more .cpp files.
//
#define KOKKOSBLAS1_ROTM_ETI_SPEC_AVAIL(SCALAR, LAYOUT, EXEC_SPACE, MEM_SPACE) \
  template <>                                                                  \
  struct rotm_eti_spec_avail<                                                  \
      EXEC_SPACE,                                                              \
      Kokkos::View<SCALAR*, LAYOUT, Kokkos::Device<EXEC_SPACE, MEM_SPACE>,     \
                   Kokkos::MemoryTraits<Kokkos::Unmanaged>>,                   \
      Kokkos::View<const SCALAR[5], LAYOUT,                                    \
                   Kokkos::Device<EXEC_SPACE, MEM_SPACE>,                      \
                   Kokkos::MemoryTraits<Kokkos::Unmanaged>>> {                 \
    enum : bool { value = true };                                              \
  };

// Include the actual specialization declarations
#include <KokkosBlas1_rotm_tpl_spec_avail.hpp>
#include <generated_specializations_hpp/KokkosBlas1_rotm_eti_spec_avail.hpp>

namespace KokkosBlas {
namespace Impl {

// Unification layer
template <
    class execution_space, class VectorView, class ParamView,
    bool tpl_spec_avail =
        rotm_tpl_spec_avail<execution_space, VectorView, ParamView>::value,
    bool eti_spec_avail =
        rotm_eti_spec_avail<execution_space, VectorView, ParamView>::value>
struct Rotm {
  static void rotm(execution_space const& space, VectorView const& X,
                   VectorView const& Y, ParamView const& param);
};

#if !defined(KOKKOSKERNELS_ETI_ONLY) || KOKKOSKERNELS_IMPL_COMPILE_LIBRARY
//! Full specialization of Rotm.
template <class execution_space, class VectorView, class ParamView>
struct Rotm<execution_space, VectorView, ParamView, false,
            KOKKOSKERNELS_IMPL_COMPILE_LIBRARY> {
  static void rotm(execution_space const& space, VectorView const& X,
                   VectorView const& Y, ParamView const& param) {
    Kokkos::Profiling::pushRegion(KOKKOSKERNELS_IMPL_COMPILE_LIBRARY
                                      ? "KokkosBlas::rotm[ETI]"
                                      : "KokkosBlas::rotm[noETI]");
#ifdef KOKKOSKERNELS_ENABLE_CHECK_SPECIALIZATION
    if (KOKKOSKERNELS_IMPL_COMPILE_LIBRARY)
      printf("KokkosBlas1::rotm<> ETI specialization for < %s, %s >\n",
             typeid(VectorView).name(), typeid(ParamView).name());
    else {
      printf("KokkosBlas1::rotm<> non-ETI specialization for < %s, %s >\n",
             typeid(VectorView).name(), typeid(ParamView).name());
    }
#endif
    Rotm_Invoke<execution_space, VectorView, ParamView>(space, X, Y, param);
    Kokkos::Profiling::popRegion();
  }
};
#endif

}  // namespace Impl
}  // namespace KokkosBlas

//
// Macro for declaration of full specialization of
// KokkosBlas::Impl::Rotm.  This is NOT for users!!!  All
// the declarations of full specializations go in this header file.
// We may spread out definitions (see _DEF macro below) across one or
// more .cpp files.
//
#define KOKKOSBLAS1_ROTM_ETI_SPEC_DECL(SCALAR, LAYOUT, EXEC_SPACE, MEM_SPACE) \
  extern template struct Rotm<                                                \
      EXEC_SPACE,                                                             \
      Kokkos::View<SCALAR*, LAYOUT, Kokkos::Device<EXEC_SPACE, MEM_SPACE>,    \
                   Kokkos::MemoryTraits<Kokkos::Unmanaged>>,                  \
      Kokkos::View<const SCALAR[5], LAYOUT,                                   \
                   Kokkos::Device<EXEC_SPACE, MEM_SPACE>,                     \
                   Kokkos::MemoryTraits<Kokkos::Unmanaged>>,                  \
      false, true>;

//
// Macro for definition of full specialization of
// KokkosBlas::Impl::Rotm.  This is NOT for users!!!  We
// use this macro in one or more .cpp files in this directory.
//
#define KOKKOSBLAS1_ROTM_ETI_SPEC_INST(SCALAR, LAYOUT, EXEC_SPACE, MEM_SPACE) \
  template struct Rotm<                                                       \
      EXEC_SPACE,                                                             \
      Kokkos::View<SCALAR*, LAYOUT, Kokkos::Device<EXEC_SPACE, MEM_SPACE>,    \
                   Kokkos::MemoryTraits<Kokkos::Unmanaged>>,                  \
      Kokkos::View<const SCALAR[5], LAYOUT,                                   \
                   Kokkos::Device<EXEC_SPACE, MEM_SPACE>,                     \
                   Kokkos::MemoryTraits<Kokkos::Unmanaged>>,                  \
      false, true>;

#include <KokkosBlas1_rotm_tpl_spec_decl.hpp>
#include <generated_specializations_hpp/KokkosBlas1_rotm_eti_spec_decl.hpp>

#endif  // KOKKOSBLAS1_ROTM_SPEC_HPP_
