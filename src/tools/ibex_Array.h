/* ============================================================================
 * I B E X - Array of references
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Apr 22, 2012
 * ---------------------------------------------------------------------------- */

#ifndef __IBEX_ARRAY_H__
#define __IBEX_ARRAY_H__

#include <vector>
#include <cassert>

namespace ibex {

/** \ingroup tools
 *
 * \brief Array of const references.
 *
 */
template<class T>
class Array {
public:
	/**
	 * \brief Create an array of references from an array of pointers.
	 */
	Array(T** array, int n);

	/**
	 * \brief Create an array of references from a vector of pointers.
	 *
	 * \pre The vector must be nonempty.
	 */
	Array(const std::vector<T*>&);

	/**
	 * \brief Create an array with 2 references.
	 */
	Array(T& x1, T& x2);

	/**
	 * \brief Duplicate this instance.
	 *
	 */
	Array(Array&);

	/**
	 * \brief Delete this instance.
	 */
	~Array();

	/**
	 * \brief Return true if empty.
	 */
	inline bool empty() const;

	/**
	 * \brief Return the number of objects.
	 */
	inline int size() const;

	/**
	 * \brief Return the ith reference (as const).
	 */
	inline T& operator[](int i);


	/**
	 * \brief Return the ith reference (as const).
	 */
	inline const T& operator[](int i) const;

protected:
	int _nb;

	/** Array of sub-Ts */
	T** array;
};


/*================================== inline implementations ========================================*/


template<class T>
Array<T>::Array(T** a, int n) : _nb(n), array(new T*[n]) {

	for (int i=0; i<_nb; i++) {
		array[i] = a[i];
	}
}

template<class T>
Array<T>::Array(const std::vector<T*>& vec) : _nb(vec.size()), array(new T*[vec.size()]) {

	assert(vec.size()>0);
	int i=0;
	for (typename std::vector<const T*>::const_iterator it=vec.begin(); it!=vec.end(); it++) {
		array[i++] = *it;
	}
}

template<class T>
Array<T>::Array(T& c1, T& c2) : _nb(2), array(new T*[2]) {
	array[0] = &c1;
	array[1] = &c2;
}

template<class T>
Array<T>::Array(Array<T>& a) : _nb(a.size()), array(new T*[a.size()]) {
	for (int i=0; i<_nb; i++) {
		array[i] = &a[i];
	}
}

template<class T>
Array<T>::~Array() {
	delete[] array;
}

template<class T>
bool Array<T>::empty() const {
	return _nb==0;
}

/**
 * \brief Return the number of objects.
 */
template<class T>
int Array<T>::size() const {
	return _nb;
}

/**
 * \brief Return the ith reference (as const). */
template<class T>
T& Array<T>::operator[](int i) {
	return *array[i];
}

/**
 * \brief Return the ith reference (as const). */
template<class T>
const T& Array<T>::operator[](int i) const {
	return *array[i];
}

} // namespace ibex
#endif // __IBEX_ARRAY_H__