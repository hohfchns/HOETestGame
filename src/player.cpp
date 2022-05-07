#include "player.h"
#include "game.h"
#include <iostream>
#include "hf_input.h"
#include <math.h>

void Player::Init()
{
    Game& game = Game::GetInstance();

    game.AddComponentCallback(this, READYUPDATE);
    game.AddComponentCallback(&m_Sprite, READYUPDATE);
    game.AddRenderComponent(&m_Sprite);
    game.AddCollisionComponent(&m_Collider);

    m_Sprite.load(Game::GetInstance().m_CurRenderer, "resources/player.png");
    m_Transform.AddChild(&m_Sprite.m_Transform);

    m_Collider.m_Transform.SetGlobalPosition(HFMath::Vector2(0.0f, 0.0f));
    m_Collider.m_Size.SetX(32.0f);
    m_Collider.m_Size.SetY(32.0f);
    m_Transform.AddChild(&m_Collider.m_Transform);
    m_Transform.AddCollider(&m_Collider);

    HFMath::Vector2 startPos;
    startPos.SetX(400);
    startPos.SetY(300);
    m_Transform.SetGlobalPosition(startPos);
}

void Player::Ready()
{
   
}

// void Player::Update(double deltaTime)
// {
//     Move(deltaTime);
// }

void Player::PhysicsUpdate(double deltaTime)
{
    Move(deltaTime);
}


void Player::Move(double deltaTime)
{
    // DirectionalMove(deltaTime);
    PlatformerMove(deltaTime);}

void Player::PlatformerMove(double deltaTime)
{
    // m_Camera.m_Transform.SetGlobalPosition(m_Transform.GetGlobalPosition());
    // m_Camera.SetCenter(m_Transform.GetGlobalPosition());
    // Game::GetInstance().m_MainCamera = &m_Camera;

    float dirX = 0.0f;
    float dirY = 0.0f;

    HFInput& inputSystem = HFInput::GetInstance();

    if (inputSystem.IsKeyDown(SDLK_d))
    {
        dirX += 1.0f;
    }
    if (inputSystem.IsKeyDown(SDLK_a))
    {
        dirX -= 1.0f;
    }
    

    HFMath::Vector2 dirVec = HFMath::Vector2(dirX, dirY);
    dirVec = dirVec.Normalized();
    
    if (!dirX || abs(m_Velocity.GetX()) > m_MaxSpeed)
    {
        m_Velocity.SetX(HFMath::lerp(m_Velocity.GetX(), 0.0f, m_Deceleration * deltaTime));
    }
    else if (dirX)
    {
        m_Velocity.SetX(HFMath::lerp(m_Velocity.GetX(), dirX * m_MaxSpeed, m_Acceleration * deltaTime));
    }

    float gravityMultiplier = 1.0f;
    if (m_Velocity.GetY() > 0)
    {
        gravityMultiplier = m_FallMultiplier;
    }
 
    m_Velocity.SetY(m_Velocity.GetY() + m_GRAVITY * gravityMultiplier * deltaTime);

    HFMath::Vector2 xOffset = m_Transform.GetGlobalPosition() + HFMath::Vector2(m_Velocity.GetX() * deltaTime, 0.0f);
    m_Transform.MoveAndCollide(xOffset);
    HFMath::Vector2 yOffset = m_Transform.GetGlobalPosition() + HFMath::Vector2(0.0f, m_Velocity.GetY() * deltaTime);
    isOnFloor = m_Transform.MoveAndCollide(yOffset);

    if (isOnFloor)
    {
        m_Velocity.SetY(0.0f);
        m_HasDoubleJump = true;
    }

    if(inputSystem.IsKeyJustDown(SDLK_SPACE))
    {
        // printf("pressed jump\n");
        if (isOnFloor)
        {
            Jump();
            // printf("jumped\n");
        }
        else if (m_HasDoubleJump)
        {
            Jump();
            m_HasDoubleJump = false;
            // printf("used double jump\n");
        }
    }

    if (inputSystem.IsKeyJustDown(SDLK_LSHIFT))
    {
        // printf("shift\n");
        m_Velocity.SetX(dirX * m_DashForce);
        m_Velocity.SetY(-25.0f);
    }

    // printf("has double jump: %d\n", (int)m_HasDoubleJump);
}


void Player::Jump()
{
    m_Velocity.SetY(-m_JumpForce);
}


void Player::DirectionalMove(double deltaTime)
{
    float dirX = 0.0f;
    float dirY = 0.0f;

    HFInput& inputSystem = HFInput::GetInstance();

    if (inputSystem.IsKeyDown(SDLK_d))
    {
        dirX += 1.0f;
    }
    if (inputSystem.IsKeyDown(SDLK_a))
    {
        dirX -= 1.0f;
    }
    if(inputSystem.IsKeyDown(SDLK_w))
    {
        dirY -= 1.0f;
    }
    if(inputSystem.IsKeyDown(SDLK_s))
    {
        dirY += 1.0f;
    }

    HFMath::Vector2 dirVec = HFMath::Vector2(dirX, dirY);
    dirVec = dirVec.Normalized();
    
    m_Velocity = dirVec * m_MaxSpeed * deltaTime;

    HFMath::Vector2 xOffset = m_Transform.GetGlobalPosition() + HFMath::Vector2(m_Velocity.GetX(), 0.0f);
    m_Transform.MoveAndCollide(xOffset);
    HFMath::Vector2 yOffset = m_Transform.GetGlobalPosition() + HFMath::Vector2(0.0f, m_Velocity.GetY());
    m_Transform.MoveAndCollide(yOffset);
}
