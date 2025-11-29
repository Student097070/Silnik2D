class CircleObject : public ShapeObject {
public:
    Circle2Data data;

    CircleObject(float x, float y, float r, ALLEGRO_COLOR color) {
        data.x0 = x;
        data.y0 = y;
        data.R = r;
        data.color = color;
    }

    CircleObject(const Circle2Data& d) {
        data = d;
    }

    void draw() override {
        al_draw_filled_circle(data.x0, data.y0, data.R, data.color);
    }

    void translate(float tx, float ty) override {
        data.x0 += tx;
        data.y0 += ty;
    }

    void rotate(float alpha, float cx, float cy) override {
        rotatePoint(data.x0, data.y0, alpha, cx, cy);
    }

    void scale(float kx, float ky, float cx, float cy) override {
        float k = (kx + ky) / 2.0f;
        scalePoint(data.x0, data.y0, kx, ky, cx, cy);
        data.R *= k;
    }

    void getCenter(float& cx, float& cy) const {
        cx = data.x0;
        cy = data.y0;
    }

private:
    void rotatePoint(float& x, float& y, float alpha, float cx, float cy) {
        float s = sin(alpha);
        float c = cos(alpha);
        x -= cx; y -= cy;
        float nx = x * c - y * s;
        float ny = x * s + y * c;
        x = nx + cx;
        y = ny + cy;
    }

    void scalePoint(float& x, float& y, float kx, float ky, float cx, float cy) {
        x = cx + (x - cx) * kx;
        y = cy + (y - cy) * ky;
    }
};
