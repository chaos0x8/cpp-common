#pragma once

namespace Common
{

template <int... Values>
struct IndexList { };

template <int N, int... S>
struct GenerateIndexList : public GenerateIndexList<N-1, N-1, S...> { };

template <int... S>
struct GenerateIndexList<0, S...>
{
    typedef IndexList<S...> type;
};

}
