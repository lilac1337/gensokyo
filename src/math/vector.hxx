#pragma once

template<class T>
class Vector2 {
public:
	T x, y;

    [[nodiscard]] float twoDLength() {
        return sqrt(this->x * this->x + this->y * this->y);
    }

    [[nodiscard]] Vector2 operator+(const T base)
    {
        return { x + base, y + base };
    }

    [[nodiscard]] Vector2 operator-(const T base)
    {
        return { x - base, y - base };
    }

    [[nodiscard]] Vector2 operator*(const T base)
    {
        return { x * base, y * base };
    }

    [[nodiscard]] Vector2 operator/(const T base)
    {
        return { x / base, y / base };
    }
};

template<class T>
class Vector3 {
public:
	T x, y, z;

    [[nodiscard]] constexpr Vector3 operator+(const Vector3 base) {
        return { x + base.x, y + base.y, z + base.z };
    }
};

template<class T>
class Vector4 {
public:
    T w, x, y, z;
};



// DELETE ME
class Vector {
public:
	float x, y, z;
};