#include <raylib.h>
#include <iostream>

//TODO: make the resetPos based on the winner instead of fully RNG.

const float window_width = 960;
const float window_heigth = 540;

class Ball{

    private:
        float circle_position_x = 480;
        float circle_position_y = 270;
        float circle_radius = 10;
        float x_speed =  60 * 7;
        float y_speed =  60 * 7;
        int score1 = 0;
        int score2 = 0; 
        Color color = WHITE;

    public: 

        void Draw(float dt)
        {
            DrawCircle(circle_position_x, circle_position_y, circle_radius, color);

            circle_position_x += x_speed * dt;
            circle_position_y += y_speed * dt;

            std::cout << circle_position_x << "\n";

            //Borders checks
            if(circle_position_y + circle_radius > window_heigth)
            {
                y_speed *= -1;
                circle_position_y = window_heigth - circle_radius;
            }
            if(circle_position_y - circle_radius < 0)
            {
                y_speed *= -1;
                circle_position_y = circle_radius;
            }
            if(circle_position_x + circle_radius > window_width)
            {
                //x_speed *= -1;
                //circle_position_x = window_width - circle_radius;
                resetPos();
                score1++;
                
            }
            if(circle_position_x - circle_radius < 0)
            {
                //x_speed *= -1;
                //circle_position_x = circle_radius;
                resetPos();
                score2++;
            }
        }

        void setColor(Color color)
        {
            this->color = color;
        }

        int getScore1()
        {
            return score1;
        }

        int getScore2()
        {
            return score2;
        }

        float getX()
        {
            return circle_position_x; 
        }

        float getY()
        {
            return circle_position_y;
        }

        void setX(float x)
        {
            this->circle_position_x = x;
        }

        void setY(float y)
        {
            this->circle_position_y = y;
        }

        float getSpeedX()
        {
            return x_speed;
        }

        float getSpeedY()
        {
            return y_speed;
        }

        void setSpeedX(float speed_x)
        {
            this->x_speed = speed_x;
        }

        void setSpeedY(float speed_y)
        {
            this->y_speed = speed_y;
        }

        float getRadius()
        {
            return circle_radius;
        }

        void resetPos()
        {

            circle_position_x = 480;
            circle_position_y = 270;
            x_speed =  60 * 7;
            y_speed =  60 * 7;

            int x = GetRandomValue(0,3);
            if (x == 0)
            {
                x_speed *= -1;
                y_speed *= -1;
            }
            else if (x == 1)
            {
                x_speed *= 1;
                y_speed *= -1;
            }
            else if (x == 2)
            {
                x_speed *= -1;
                y_speed *= 1;
            }
            else if (x == 3)
            {
                x_speed *= 1;
                y_speed *= 1;
            }
            
        }
        
    };

class Player{
    
    private:

    Rectangle rec;

    public:

    void Draw(float width, float heigth)
    {
        rec.width = width;
        rec.height = heigth;

        //DrawRectangleRec(rec, WHITE);
        DrawRectangleRounded(rec, 10, 5, WHITE);
    }

    Rectangle getRect()
    {
        return {rec.x, rec.y, rec.width, rec.height};
    }

    float getRectX()
    {
        return rec.x;
    }

    float getRectY()
    {
        return rec.y;
    }

    float getRectHeigth()
    {
        return rec.height;
    }

    float getRectWidth()
    {
        return rec.width;
    }
    void controlsCheck(float dt)
    {
        //Key checks
        if(IsKeyDown(KEY_UP) || IsKeyDown(KEY_W))
        {
            rec.y -= 60*8 * dt;
        }
        if(IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S))
        {
            rec.y += 60*8 * dt;
        }

        //Borders checks
        if(rec.y - 10 < 0)
        {
            rec.y = 10;
        }
        if(rec.y + rec.height + 10 > window_heigth)
        {
            rec.y = window_heigth - rec.height - 10;
        }
    }

    void controlsCheckCPU(float dt, float ball_y)
    {
        if(rec.y + rec.height/2 > ball_y)
        {
            rec.y -= 40*8 * dt;
        }

        if(rec.y + rec.height/2 < ball_y)
        {
            rec.y += 40*8 * dt;
        }

        if(rec.y - 10 < 0)
        {
            rec.y = 10;
        }
        if(rec.y + rec.height + 10 > window_heigth)
        {
            rec.y = window_heigth - rec.height - 10;
        }
    }

    void setRectX(float x)
    {
        rec.x = x;
    }

    void setRectY(float y)
    {
        rec.y = y;
    }
};


int main()
{

    Ball ball;

    Player player1;

    player1.setRectX(10);
    player1.setRectY(window_heigth/2 - 50);

    Player player2;

    player2.setRectX(window_width - 30);
    player2.setRectY(window_heigth/2 - 50);

    
    InitWindow(window_width, window_heigth, "Test");

    //SetWindowState(FLAG_VSYNC_HINT);
    SetTargetFPS(60);

    while(!WindowShouldClose())
    {
        float dt = GetFrameTime();
        //Screen drawing
        BeginDrawing();

        //Update
 
        player1.controlsCheck(dt);
        player2.controlsCheckCPU(dt, ball.getY());

        //Checking if the ball collides with the Player 1.
        if(CheckCollisionCircleRec(Vector2(ball.getX(), ball.getY()), ball.getRadius(), player1.getRect()))
        {
            //checking if the ball is going to the left and if its not going past the paddle.
            if(ball.getSpeedX() < 0 && (ball.getX() - ball.getRadius()) >= player1.getRectX())
            {
                ball.setSpeedX((ball.getSpeedX() * -1.1));
            }
            
        }

        //Checking if the ball collides with the Player 2.
        if(CheckCollisionCircleRec(Vector2(ball.getX(), ball.getY()), ball.getRadius(), player2.getRect()))
        {
            //checking if the ball is going to the right and if its not going past the paddle.
            if(ball.getSpeedX() > 0 && (ball.getX() + ball.getRadius()) <= (player2.getRectX() + player2.getRectWidth()/2))
            {
                ball.setSpeedX((ball.getSpeedX() * -1.1));
            }
        }
        //Draw

        ClearBackground(BLACK);

        DrawFPS(10, 10);

        DrawLine(window_width/2, window_heigth, window_width/2, 0, WHITE);

        DrawCircleLines(window_width/2, window_heigth/2, 200, WHITE);
        
        DrawText(TextFormat("%i", ball.getScore1()), window_width / 5, window_heigth / 15, 50, WHITE);

        DrawText(TextFormat("%i", ball.getScore2()), (window_width / 3) * 2.4, window_heigth / 15, 50, WHITE);

        ball.Draw(dt);

        player1.Draw(20, 100);
        
        player2.Draw(20, 100);


        //End screen drawing
        EndDrawing();
    }

    return 0;
}