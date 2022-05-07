#pragma once
#include "hfmath.h"
#include "sprite.h"
#include "hftransform.h"
#include "world_component.h"
#include "collision_component.h"
#include "raycast.h"

class Player : public WorldComponent
{
    public:
        Player()
        {
            m_Name = "Player";
        }
        Player(float Pos_X, float Pos_Y)
        {
            m_Transform.SetGlobalPosition(HFMath::Vector2(Pos_X, Pos_Y));
            m_Name = "Player";
        }
        Player(HFMath::Vector2 initialPos)
        { 
            m_Transform.SetGlobalPosition(initialPos);
            m_Name = "Player";
        }

        virtual void Init();
        // virtual void Update(double deltaTime) override;
        virtual void PhysicsUpdate(double deltaTime) override;
        virtual void Ready() override;

        virtual ~Player() {}
        
        Sprite m_Sprite;

        CollisionComponent m_Collider = CollisionComponent();

    private:
        Camera m_Camera;

        bool isOnFloor = false;

        HFMath::Vector2 m_Velocity;

        float m_MaxSpeed = 230.0f;
        float m_Acceleration = 10.0f;
        float m_Deceleration = 12.5f;
        float m_JumpForce = 590.0f;
        float m_GRAVITY = 1800.0f;
        float m_DashForce = 1200.0f;
        float m_FallMultiplier = 1.25f;

        bool m_HasDoubleJump = false;

        void Move(double deltaTime);
        void PlatformerMove(double deltaTime);
        void DirectionalMove(double deltaTime);
        void Jump();
};