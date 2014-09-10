//  Copyright Eric Niebler 2014.
//  Copyright Gonzalo Brito Gadeschi 2014.
//
//  Use, modification and distribution is subject to the
//  Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
// For more information, see http://www.boost.org/libs/range/
//
// Implementation based on the code in libc++
//   http://http://libcxx.llvm.org/

//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include <range/v3/core.hpp>
#include <range/v3/numeric/partial_sum.hpp>
#include "../simple_test.hpp"
#include "../test_iterators.hpp"

struct S
{
    int i;
};

template <class InIter, class OutIter, class InSent = InIter> void test()
{
    using ranges::partial_sum;
    using ranges::range;
    { // iterator
        int ir[] = {1, 3, 6, 10, 15};
        const unsigned s = sizeof(ir) / sizeof(ir[0]);
        int ia[] = {1, 2, 3, 4, 5};
        int ib[s] = {0};
        auto r = partial_sum(InIter(ia), InSent(ia + s), OutIter(ib));
        CHECK(base(std::get<0>(r)) == ia + s);
        CHECK(base(std::get<1>(r)) == ib + s);
        for(unsigned i = 0; i < s; ++i)
        {
            CHECK(ib[i] == ir[i]);
        }
    }

    { // range + output iterator
        int ir[] = {1, 3, 6, 10, 15};
        const unsigned s = sizeof(ir) / sizeof(ir[0]);
        int ia[] = {1, 2, 3, 4, 5};
        int ib[s] = {0};
        auto rng = range(InIter(ia), InSent(ia + s));
        auto r = partial_sum(rng, OutIter(ib));
        CHECK(base(std::get<0>(r)) == ia + s);
        CHECK(base(std::get<1>(r)) == ib + s);
        for(unsigned i = 0; i < s; ++i)
        {
            CHECK(ib[i] == ir[i]);
        }
    }

    { // range + output range
        int ir[] = {1, 3, 6, 10, 15};
        const unsigned s = sizeof(ir) / sizeof(ir[0]);
        int ia[] = {1, 2, 3, 4, 5};
        int ib[s] = {0};
        auto rng = range(InIter(ia), InSent(ia + s));
        auto orng = range(OutIter(ib), OutIter(ib + s));
        auto r = partial_sum(rng, orng);
        CHECK(base(std::get<0>(r)) == ia + s);
        CHECK(base(std::get<1>(r)) == ib + s);
        for(unsigned i = 0; i < s; ++i)
        {
            CHECK(ib[i] == ir[i]);
        }
    }

    {
        int ia[] = {1, 2, 3, 4, 5};
        int ir[] = {1, -1, -4, -8, -13};
        const unsigned s = sizeof(ia) / sizeof(ia[0]);
        int ib[s] = {0};
        auto rng = range(InIter(ia), InSent(ia + s));
        auto orng = range(OutIter(ib), OutIter(ib + s));
        auto r = partial_sum(rng, orng, std::minus<int>());
        CHECK(base(std::get<0>(r)) == ia + s);
        CHECK(base(std::get<1>(r)) == ib + s);
        for(unsigned i = 0; i < s; ++i)
        {
            CHECK(ib[i] == ir[i]);
        }
    }
}

int main()
{
    test<input_iterator<const int *>, input_iterator<int *>>();
    test<input_iterator<const int *>, forward_iterator<int *>>();
    test<input_iterator<const int *>, bidirectional_iterator<int *>>();
    test<input_iterator<const int *>, random_access_iterator<int *>>();
    test<input_iterator<const int *>, int *>();

    test<forward_iterator<const int *>, input_iterator<int *>>();
    test<forward_iterator<const int *>, forward_iterator<int *>>();
    test<forward_iterator<const int *>, bidirectional_iterator<int *>>();
    test<forward_iterator<const int *>, random_access_iterator<int *>>();
    test<forward_iterator<const int *>, int *>();

    test<bidirectional_iterator<const int *>, input_iterator<int *>>();
    test<bidirectional_iterator<const int *>, forward_iterator<int *>>();
    test<bidirectional_iterator<const int *>, bidirectional_iterator<int *>>();
    test<bidirectional_iterator<const int *>, random_access_iterator<int *>>();
    test<bidirectional_iterator<const int *>, int *>();

    test<random_access_iterator<const int *>, input_iterator<int *>>();
    test<random_access_iterator<const int *>, forward_iterator<int *>>();
    test<random_access_iterator<const int *>, bidirectional_iterator<int *>>();
    test<random_access_iterator<const int *>, random_access_iterator<int *>>();
    test<random_access_iterator<const int *>, int *>();

    test<const int *, input_iterator<int *>>();
    test<const int *, forward_iterator<int *>>();
    test<const int *, bidirectional_iterator<int *>>();
    test<const int *, random_access_iterator<int *>>();
    test<const int *, int *>();

    using ranges::partial_sum;

    { // Test projections
        S ia[] = {{1}, {2}, {3}, {4}, {5}};
        int ir[] = {1, 3, 6, 10, 15};
        const unsigned s = sizeof(ir) / sizeof(ir[0]);
        int ib[s] = {0};
        auto r = partial_sum(ranges::begin(ia), ranges::begin(ia) + s, ranges::begin(ib),
                             std::plus<int>(), &S::i);
        CHECK(base(std::get<0>(r)) == ia + s);
        CHECK(base(std::get<1>(r)) == ib + s);
        for(unsigned i = 0; i < s; ++i)
        {
            CHECK(ib[i] == ir[i]);
        }
    }

    { // Test BinaryOp
        int ia[] = {1, 2, 3, 4, 5};
        int ir[] = {1, 2, 6, 24, 120};
        const unsigned s = sizeof(ir) / sizeof(ir[0]);
        int ib[s] = {0};
        auto r = partial_sum(ia, ranges::begin(ib), std::multiplies<int>());
        CHECK(base(std::get<0>(r)) == ia + s);
        CHECK(base(std::get<1>(r)) == ib + s);
        for(unsigned i = 0; i < s; ++i)
        {
            CHECK(ib[i] == ir[i]);
        }
    }

    { // Test calling it with an array
        int ia[] = {1, 2, 3, 4, 5};
        int ir[] = {1, 2, 6, 24, 120};
        const unsigned s = sizeof(ir) / sizeof(ir[0]);
        int ib[s] = {0};
        auto r = partial_sum(ia, ib, std::multiplies<int>());
        CHECK(base(std::get<0>(r)) == ia + s);
        CHECK(base(std::get<1>(r)) == ib + s);
        for(unsigned i = 0; i < s; ++i)
        {
            CHECK(ib[i] == ir[i]);
        }
    }

    return ::test_result();
}