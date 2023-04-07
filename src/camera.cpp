#include "camera.h"
#include "coordinate.h"
Ray sample_ray(const Camera &camera, const Vector2f &screen_pos)
{
    static float scale = tan(deg2rad(camera.fov * 0.5));
    static float imageAspectRatio = camera.width / (float)camera.height;

    static Coordinate camera_coord = Coordinate(camera.right, camera.up, camera.lookat);
    Vector2f offset;
    float x = (2 * (screen_pos.x) / (float)camera.width - 1) *
              imageAspectRatio * scale;
    float y = (1 - 2 * (screen_pos.y) / (float)camera.height) * scale;
    Vector3f dir(x, y, 1);
    dir = to_world(camera_coord, dir.normalized()).normalized();
    return Ray{camera.position, dir};
}
