#pragma once

#ifndef __ESP_H__
#define __ESP_H__

#include "wanbai.hpp"
#include "imgui.h"

struct Vec2 {
    float x;
    float y;
    Vec2()
    {
        this->x = 0;
        this->y = 0;
    }
    Vec2(float x, float y)
    {
        this->x = x;
        this->y = y;
    }
    Vec2 operator+(float v) const
    {
        return Vec2(x + v, y + v);
    }
    Vec2 operator-(float v) const
    {
        return Vec2(x - v, y - v);
    }
    Vec2 operator*(float v) const
    {
        return Vec2(x * v, y * v);
    }
    Vec2 operator/(float v) const
    {
        return Vec2(x / v, y / v);
    }
    Vec2& operator+=(float v)
    {
        x += v;
        y += v;
        return *this;
    }
    Vec2& operator-=(float v)
    {
        x -= v;
        y -= v;
        return *this;
    }
    Vec2& operator*=(float v)
    {
        x *= v;
        y *= v;
        return *this;
    }
    Vec2& operator/=(float v)
    {
        x /= v;
        y /= v;
        return *this;
    }
    Vec2 operator+(const Vec2& v) const
    {
        return Vec2(x + v.x, y + v.y);
    }
    Vec2 operator-(const Vec2& v) const
    {
        return Vec2(x - v.x, y - v.y);
    }
    Vec2 operator*(const Vec2& v) const
    {
        return Vec2(x * v.x, y * v.y);
    }
    Vec2 operator/(const Vec2& v) const
    {
        return Vec2(x / v.x, y / v.y);
    }
    Vec2& operator+=(const Vec2& v)
    {
        x += v.x;
        y += v.y;
        return *this;
    }
    Vec2& operator-=(const Vec2& v)
    {
        x -= v.x;
        y -= v.y;
        return *this;
    }
    Vec2& operator*=(const Vec2& v)
    {
        x *= v.x;
        y *= v.y;
        return *this;
    }
    Vec2& operator/=(const Vec2& v)
    {
        x /= v.x;
        y /= v.y;
        return *this;
    }
    operator ImVec2() const
    {
        return ImVec2(this->x, this->y);
    }
};

struct Vec3
{
    float x;
    float y;
    float z;
    Vec3()
    {
        this->x = 0;
        this->y = 0;
        this->z = 0;
    }
    Vec3(float x, float y, float z)
    {
        this->x = x;
        this->y = y;
        this->z = z;
    }
    Vec3 operator+(float v) const {
        return Vec3(x + v, y + v, z + v);
    }
    Vec3 operator-(float v) const {
        return Vec3(x - v, y - v, z - v);
    }
    Vec3 operator*(float v) const {
        return Vec3(x * v, y * v, z * v);
    }
    Vec3 operator/(float v) const {
        return Vec3(x / v, y / v, z / v);
    }
    Vec3& operator+=(float v)
    {
        x += v;
        y += v;
        z += v;
        return *this;
    }
    Vec3& operator-=(float v)
    {
        x -= v;
        y -= v;
        z -= v;
        return *this;
    }
    Vec3& operator*=(float v)
    {
        x *= v;
        y *= v;
        z *= v;
        return *this;
    }
    Vec3& operator/=(float v)
    {
        x /= v;
        y /= v;
        z /= v;
        return *this;
    }
    Vec3 operator+(const Vec3& v) const {
        return Vec3(x + v.x, y + v.y, z + v.z);
    }
    Vec3 operator-(const Vec3& v) const {
        return Vec3(x - v.x, y - v.y, z - v.z);
    }
    Vec3 operator*(const Vec3& v) const {
        return Vec3(x * v.x, y * v.y, z * v.z);
    }
    Vec3 operator/(const Vec3& v) const {
        return Vec3(x / v.x, y / v.y, z / v.z);
    }
    Vec3& operator+=(const Vec3& v)
    {
        x += v.x;
        y += v.y;
        z += v.z;
        return *this;
    }
    Vec3& operator-=(const Vec3& v)
    {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        return *this;
    }
    Vec3& operator*=(const Vec3& v)
    {
        x *= v.x;
        y *= v.y;
        z *= v.z;
        return *this;
    }
    Vec3& operator/=(const Vec3& v)
    {
        x /= v.x;
        y /= v.y;
        z /= v.z;
        return *this;
    }
};

struct Player
{
    float Health {0};
    struct {
        struct Vec3 Position {0, 0, 0};
        struct Vec2 Screen_Position {0, 0};
    } Position,
        Head,
        Chest,
        Pelvis,
        lSh,
        rSh,
        lElbow,
        rElbow,
        lWrist,
        rWrist,
        lLeg,
        rLeg,
        lKnee,
        rKnee,
        lAnkle,
        rAnkle;
    int TeamID {0};
    bool Bot {false};
    float Distance {0};
    float Camera {0};
    float Width {0};
};

struct FMatrix
{
    float M[4][4];
};

struct Quat
{
    float x;
    float y;
    float z;
    float w;
};

struct FTransform
{
    Quat Rotation;
    Vec3 Translation;
    float chunk;
    Vec3 Scale3D;
};

extern bool _shutdown;

class ESP: private c_driver
{
    public:
        ESP(const char *driver_path, const char *name);
        ~ESP(void);
        pid_t get_pid(const char *package);
        const char *exec(const char *command);
        inline uintptr_t getUE4(void) {return libUE4; };
        void start(void);
        uintptr_t libUE4 {0};
        uintptr_t UWorld {0},
            ULevel {0},
            Array {0},
            Matrix {0},
            Self {0},
            Mesh {0},
            Bone {0},
            Object {0};
        std::thread *thread {nullptr};
        int Count {0}, my_team_id {0};
        struct Vec3 My_pos;
        float matrix_content[16] {0.0f}, Fov {0.0f}, Camera {0.0f};
    private:
};

#endif // ESP_H
