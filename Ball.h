#pragma once
class Ball
{
private:
	float x;
	float y;
	float dx;
	float dy;
	int lastPlayer;
public:
	Ball();
	~Ball();
	float getX() { return x; }
	float getY() { return y; }
	float getDx() { return dx; }
	float getDy() { return dy; }
	int getLastPlayer(){ return lastPlayer; }
	void setLastPlayer(int i) { lastPlayer = i; }
	void updateX() { x += dx; }
	void updateY() { y += dy; }
	void update() { updateX(); updateY(); }
	void setDx(float x) { dx = x; }
	void setDy(float y) { dy = y; }
};

