#pragma once



class WAXGLDLL Point
{
public:
	Point()
	{
		x = 0;
		y = 0;
	}

	Point(const Point& other)
	{
		copy(other);
	}

	Point(int x0, int y0)
	{
		x = x0;
		y = y0;
	}

	void copy( const Point& other)
	{
		x = other.x;
		y = other.y;
	}

	Point& operator + (const Point& other)
	{
		x += other.x;
		y += other.y;
		return *this;
	}

	template <class X>
	void set_x(X val)
	{
		x = (int)val;
	}

	template <class X>
	void set_y(X val)
	{
		y = (int)val;
	}

	int x;
	int y;
};

