#pragma once
#include "TVector.h"

template <typename T>
class TMathVector : public TVector<T> {

public:
	TMathVector() = delete;
	TMathVector(size_t size, T* data) : TVector(data, size) { this->shrink_to_fit(); }
	TMathVector(std::initializer_list<T> init) : TVector<T>(init) { this->shrink_to_fit(); }
	TMathVector(const TMathVector&) noexcept = default;
	TMathVector(TMathVector&&) noexcept = default;
	~TMathVector() noexcept = default;

	TMathVector& operator=(const TMathVector&) noexcept = default;
	TMathVector& operator=(TMathVector&&) noexcept = default;

	TMathVector& operator+=(const TMathVector& other);
	TMathVector& operator-=(const TMathVector& other);
	TMathVector& operator*=(double other);

	TMathVector operator+(const TMathVector& other) const;
	TMathVector operator-(const TMathVector& other) const;
	T operator*(const TMathVector& other) const;
	TMathVector operator*(double other) const;

	bool operator==(const TMathVector& other);
	bool operator!=(const TMathVector& other);
	friend std::ostream& operator<<(std::ostream& os, const TMathVector<T>& v) {
		return os << static_cast<const TVector<T>&>(v);
	}
	friend std::istream& operator>>(std::istream& is, TMathVector<T>& v) {
		is >> dynamic_cast<TVector<T>&>(v);
		v.shrink_to_fit();
		return is;
	}


};

template <typename T>
TMathVector<T>& TMathVector<T>::operator+=(const TMathVector& other) {
	if (other.size() != size()) throw std::invalid_argument("You cannot add vectors of different sizes");
	for (int i = 0; i < size(); ++i) {
		(*this)[i] += other[i];
	}
	return *this;
}

template <typename T>
TMathVector<T>& TMathVector<T>::operator-=(const TMathVector& other) {
	if (other.size() != size()) throw std::invalid_argument("You cannot add vectors of different sizes");
	for (int i = 0; i < size(); ++i) {
		(*this)[i] -= other[i];
	}
	return *this;
}

template <typename T>
TMathVector<T>& TMathVector<T>::operator*=(double mult) {
	for (int i = 0; i < size(); ++i) {
		(*this)[i] *= mult;
	}
	return *this;
}

template <typename T>
T TMathVector<T>::operator*(const TMathVector& other) const {
	if (other.size() != size()) throw std::invalid_argument("You cannot add vectors of different sizes");
	T sum = (*this)[0] * other[0];
	for (int i = 1; i < size(); ++i) {
		sum += (*this)[i] * other[i];
	}
	return sum;
}

template <typename T>
TMathVector<T> TMathVector<T>::operator+(const TMathVector& other) const {
	auto copy = TMathVector(*this);
	copy += other;
	return copy;
}

template <typename T>
TMathVector<T> TMathVector<T>::operator-(const TMathVector& other) const {
	auto copy = TMathVector(*this);
	copy -= other;
	return copy;
}

template <typename T>
TMathVector<T> TMathVector<T>::operator*(double mult) const {
	auto copy = TMathVector(*this);
	copy *= mult;
	return copy;
}

template <typename T>
bool TMathVector<T>::operator==(const TMathVector& other)  {
	if (other.size() != size()) throw std::invalid_argument("You cannot add vectors of different sizes");
	for (int i = 0; i < size(); ++i) {
		if ((*this)[i] != other[i]) return false;
	}
	return true;
}

template <typename T>
bool TMathVector<T>::operator!=(const TMathVector& other) {
	return !(*this == other);
}