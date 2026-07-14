# include<math.h>
class Vec2 {

 public:

	 float x=0.f;
	 float y=0.f;

	 Vec2(float xin,float yin) :x(xin), y(yin) {

	 }

	 Vec2 operator +(const Vec2& rhs) const;
	 void operator +=(const Vec2& rhs);
	 Vec2 operator *(float value) const;
	 
	 Vec2& add(const Vec2& v);
	 Vec2& rotate(float deg);

     Vec2& scale(float s);
	 Vec2 operator -(const Vec2& rhs) const;

     float dist(const Vec2& v) const;

	 float length() const;
	 Vec2 normalized() const;
};