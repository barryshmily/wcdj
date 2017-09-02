#include <iostream>

class Point {
public:
	Point(int x, int y) {
		m_x = x;
		m_y = y;
	}
	void setX(int newVal) {
		m_x = newVal;
	}
	void setY(int newVal) {
		m_y = newVal;
	}

	void show() const {
		std::cout << m_x << "," << m_y << std::endl;
	}

private:
	int m_x;
	int m_y;
};

class PointMgr {
public:
	PointMgr() : m_point(1, 1) {
	}
	
	// error: binding of reference to type 'Point' to a value of type 'const Point' drops qualifiers
	//Point& getPoint() const {

#if 0
	// ok, but not suggested
	Point& getPoint() {
		return m_point;
	}
#endif

	// ok, suggested
	const Point& getPoint() const {
		return m_point;
	}


	void showPoint() const {
		m_point.show();
	}

private:
	Point m_point;
};

int main()
{
	PointMgr point_mgr;
	point_mgr.showPoint();

	// error
	//point_mgr.getPoint().setX(2);
	//point_mgr.getPoint().setY(2);
	
	point_mgr.getPoint().show();

}
/*
1,1
1,1 
 */
