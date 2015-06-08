/**
 * \file    Facet.h
 * \author  Martin Buck
 * \date    June 2015
 * \version pre-release
 * \brief   TODO
 */

#ifndef FACET_H_INCLUDED
#define FACET_H_INCLUDED

namespace lib_3d {

class Facet {
public:
    size_t
        a, b, c;

    Facet(size_t a, size_t b, size_t c) :
        a(a),
        b(b),
        c(c)
    {

    }

    ~Facet() {

    }
};

} // namespace lib_3d


#endif // FACET_H_INCLUDED
