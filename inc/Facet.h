/*
Copyright 2016 Martin Buck
This file is part of lib_3d.
lib_3d is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.
lib_3d is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.
You should have received a copy of the GNU Lesser General Public License
along with lib_3d.  If not, see <http://www.gnu.org/licenses/>.
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
};

} // namespace lib_3d


#endif // FACET_H_INCLUDED
