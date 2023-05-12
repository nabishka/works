#include "raylib.h"
#include "math.h"


int main(void)
{
    InitWindow(1900, 1000, ""); //инициализация главного окна
    //для перемещения куба
    float speed = 0.05; //значение скорости перемещения куба
    bool calculate = false; //было ли движение куба до этого
    int point = 0;  //точки до которых дошел куб
    float sum = 0;  //длина вектора
    bool adaptate_speed = false;    //адапитваня скорость
    
    //для перемещения куба
    int a,b,c,d; //для перевода с чар в флоат
    //данные для строк ввода значений координат 
    char input_x[6]="\0";
    char input_y[6]="\0";
    
    char input_z[6]="\0";
    
    bool x_minus = false;
    bool z_minus = false;
    
    int letter_count_x =0;
    int letter_count_y =0;
    int letter_count_z =0;
    
    bool mouse_on_x = false;
    bool mouse_on_y = false;
    bool mouse_on_z = false;
    
    Rectangle cordinating = {1690,5,115,155};
    
    Rectangle text_x = {1700,20,100,25};
    Rectangle text_y = {1700,70,100,25};
    Rectangle text_z = {1700,120,100,25};
    //конец ввода данных для значений     координат
    
    //данные для значений камеры по умолчанию
    Vector3 classic_camera_position = {75, 75, 0};
    Vector3 classic_camera_target = {0, 0, 0}; // также используется в рисовании плоскости так как координаты середины куба плоскости 0,0,0
    Vector3 classic_camera_up = {0.0f, 1.0f, 0.0f};
    
    //структура камеры хранящая ее положение в пространстве
    Camera3D camera;
    camera.position = classic_camera_position;
    camera.target = classic_camera_target;
    camera.up = classic_camera_up;
    camera.fovy = 45.0f;
    //конец ввода данных начального положения камеры
    
    //создание массива векторов, хранящего точки 
    Vector3 mas[45]{};
    int counter = 1; //счетчик для массива
    
    //координаты куба
    Vector3 start_cubePosition = {24.5f, 24.5f, -24.5f};
    Vector3 cubePosition = {24.5f, 24.5f, -24.5f};
    Vector3 for_work_zone = {0, 12.5, 0};   //координаты для создания рабочей зоны
    
    //сохранения координат начальной точки в массив векторов
    mas[0].x = cubePosition.x;
    mas[0].y = cubePosition.y;
    mas[0].z = cubePosition.z;
    
    bool draw = false;  //передает в каком режиме мы работаем, ручном или координатном
    bool movement = false;  //передает была ли смена режима
    
    SetTargetFPS(60);   //задаем кол-во кадров в секунду
    
    while (!WindowShouldClose())    //создаем глав цикл
    {
        
        if (IsKeyPressed(KEY_SPACE))    //смена режима ввода ручной/координатный с помощью пробела
        {
            calculate = false;
            point = 0;
            if (draw)
                draw = false;
            else
                draw = true;
        }
        
        //сохранение текущих координат в массив координат точек если нажали кнопку о
        if (IsKeyPressed(KEY_O))    
        {
            if (counter == 45)
            {
                counter = 1;
                mas[0].x = start_cubePosition.x;
                mas[0].y = start_cubePosition.y;
                mas[0].z = start_cubePosition.z;
            }
            if (!draw)
            {
                mas[counter].x = cubePosition.x;
                mas[counter].y = cubePosition.y;
                mas[counter].z = cubePosition.z;
            }
            else
            {
                mas[counter].x = camera.target.x;
                mas[counter].y = camera.target.y;
                mas[counter].z = camera.target.z;
            }
            counter += 1;
        }
        //конец сохранения координат  в массив
        
        //если режим ввода координатный
        if (!draw)
        {
            if (movement)   //если режим ввода был сменен
            {
                EnableCursor();
                cubePosition = camera.target;
                camera.target = classic_camera_target;
                camera.position = classic_camera_position;
                camera.up = classic_camera_up;
                movement = false;
            }
            //смена характеристик управления камерой
            UpdateCameraPro(&camera,
                            (Vector3){
                                (IsKeyDown(KEY_W)) * 0.5f - (IsKeyDown(KEY_S)) * 0.5f,
                                (IsKeyDown(KEY_D)) * 0.5f - (IsKeyDown(KEY_A)) * 0.5f,
                                (IsKeyDown(KEY_Z)) * 1.0f - (IsKeyDown(KEY_X)) * 1.0f},
                            (Vector3){
                                (IsKeyDown(KEY_RIGHT)) * 0.5f - (IsKeyDown(KEY_LEFT)) * 0.5f,
                                (IsKeyDown(KEY_DOWN)) * 0.5f - (IsKeyDown(KEY_UP)) * 0.5f,
                                (IsKeyDown(KEY_E)) * 0.5f - (IsKeyDown(KEY_Q)) * 0.5f},
                            (GetMouseWheelMove()) * 1.0f);
            
            if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                if (CheckCollisionPointRec(GetMousePosition(), text_x)) mouse_on_x = true;
                else mouse_on_x = false;
                if (CheckCollisionPointRec(GetMousePosition(), text_y)) mouse_on_y = true;
                else mouse_on_y = false;
                if (CheckCollisionPointRec(GetMousePosition(), text_z)) mouse_on_z = true;
                else mouse_on_z = false;
            }
            
            
            //валидаторы и ввод координат в поля
            if (mouse_on_x) //для икс координаты
            {
                //меняем вид курсора
                SetMouseCursor(MOUSE_CURSOR_IBEAM);
                
                // Получить нажатый символ (юникод), вызвать его несколько раз для символов в очереди, возвращает 0, когда очередь пуста
                int key = GetCharPressed();
                
                // x_minus = false;
                
                // Если символы введены
                while (key > 0 && letter_count_x < 5)
                {
                    if(letter_count_x == 0)
                    {
                        if(key==KEY_MINUS) (x_minus = true)&&(letter_count_x=0);
                        if((key >= 48) && (key <= 50)) //первый символ не больше 2
                        {
                            if(key==KEY_ZERO)
                            {
                                input_x[letter_count_x] = (char)key;
                                input_x[letter_count_x+1] = (char)key;
                                input_x[letter_count_x+2] = '\0';
                                letter_count_x +=2;
                            }else
                            {
                                input_x[letter_count_x] = (char)key;
                                input_x[letter_count_x+1] = '\0'; // добавляем нулевое заверршение строки
                                letter_count_x++;
                            }
                        }
                    } else if (letter_count_x == 1)
                    {
                        if((key >= 48) && (key <= 52) || key ==46)
                        {
                            if(key==KEY_PERIOD) 
                            {
                                input_x[letter_count_x] = KEY_ZERO;
                            }
                            else 
                            {
                                input_x[letter_count_x] = (char)key;
                            }
                            input_x[letter_count_x+1] = (char)(KEY_PERIOD); // 3 символ  точка так как число флоат не больше 24.5
                            input_x[letter_count_x+2] = '\0';
                            letter_count_x += 2;
                            
                        }
                    }else if (letter_count_x == 2)
                    {
                        input_x[letter_count_x] = (char)(KEY_PERIOD); 
                        input_x[letter_count_x+1] = '\0';
                        letter_count_x ++;
                        
                        
                    }else if (letter_count_x == 3)
                    {
                        if((key >= 48) && (key <= 57) && !((input_x[0]==(char)(KEY_TWO)) && (input_x[1]==(char)(KEY_FOUR))))
                        {
                            input_x[letter_count_x] = (char)key;
                            input_x[letter_count_x+1] = '\0';
                            letter_count_x ++;
                            
                        } else
                        {
                            input_x[letter_count_x] = (char)(KEY_FIVE);
                            input_x[letter_count_x+1] = (char)(KEY_ZERO); 
                            input_x[letter_count_x+2] = '\0';
                            letter_count_x += 2;
                        }
                    }else if (((key >= 48) && (key <= 57)) && !((input_x[3]!=(char)(KEY_FIVE)) && (input_x[0]!=(char)(KEY_TWO)) && (input_x[1]!=(char)(KEY_FOUR)))) // только от 48 - 57 (ключи цифр)  и не больше 5-ти десятых
                    {
                        input_x[letter_count_x] = (char)key;
                        input_x[letter_count_x+1] = '\0';
                        letter_count_x++;
                    }else 
                    {
                        input_x[letter_count_x] = (char)(KEY_ZERO);
                        input_x[letter_count_x+1] = '\0';
                        letter_count_x++;
                    }
                    
                    key = GetCharPressed();  // переход к следующему символу
                }
                if (IsKeyPressed(KEY_BACKSPACE))    //если был нажат бэкспэйс
                {   //удаляем последний символ
                    letter_count_x--;
                    if(letter_count_x <=0) x_minus=false;
                    if (letter_count_x < 0) letter_count_x= 0;
                    input_x[letter_count_x] = '\0';
                }
            } 
            
            
            if (mouse_on_y) //для игрик координаты
            {
                SetMouseCursor(MOUSE_CURSOR_IBEAM);
                
                int key = GetCharPressed();
                
                
                while (key > 0 && letter_count_y < 5)
                {
                    
                    if(letter_count_y == 0)
                    {
                        
                        if(key==KEY_ZERO)
                        {
                            input_y[letter_count_y] = (char)key;
                            input_y[letter_count_y+1] = (char)key;
                            input_y[letter_count_y+2] = (char)(KEY_PERIOD);
                            input_y[letter_count_y+3] = (char)(KEY_FIVE);
                            input_y[letter_count_y+4] = '\0';
                            letter_count_y +=4;
                        }else if((key >= 48) && (key <= 50))
                        {
                            input_y[letter_count_y] = (char)key;
                            input_y[letter_count_y+1] = '\0'; // добавляем нулевое заверршение строки
                            letter_count_y++;
                        }
                    } else if (letter_count_y == 1)
                    {
                        if((key >= 48) && (key <= 52) || key ==46)
                        {
                            if(key==KEY_PERIOD) 
                            {
                                input_y[letter_count_y] = KEY_ZERO;
                            }else if(input_y[0]==(char)(KEY_ZERO) && key==KEY_ZERO)
                            {
                                input_y[letter_count_y] = KEY_ZERO;
                                input_y[letter_count_y+1] = (char)(KEY_PERIOD); 
                                input_y[letter_count_y+2] = (char)(KEY_FIVE);
                                input_y[letter_count_y+3] = '\0';
                                letter_count_y += 3;
                            }
                            else 
                            {
                                input_y[letter_count_y] = (char)key;
                            }
                            input_y[letter_count_y+1] = (char)(KEY_PERIOD); 
                            input_y[letter_count_y+2] = '\0';
                            letter_count_y += 2;
                            
                        }
                    }else if (letter_count_y == 2)
                    {
                        input_y[letter_count_y] = (char)(KEY_PERIOD); 
                        input_y[letter_count_y+1] = '\0';
                        letter_count_y ++;
                        
                        
                    }else if (letter_count_y == 3)
                    {
                        if((key >= 48) && (key <= 57) && !((input_y[0]==(char)(KEY_TWO)) && (input_y[1]==(char)(KEY_FOUR))))
                        {
                            if((input_y[0]==(char)(KEY_ZERO)) && (input_y[1]==(char)(KEY_ZERO)))
                            { if((key >= 53) && (key <= 57))
                                {
                                    input_y[letter_count_y] = (char)key;
                                    input_y[letter_count_y+1] = '\0';
                                    letter_count_y ++;
                                }
                            }else
                            {
                                input_y[letter_count_y] = (char)key;
                                input_y[letter_count_y+1] = '\0';
                                letter_count_y ++;
                            }
                            
                        } else 
                        {
                            input_y[letter_count_y] = (char)(KEY_FIVE);
                            input_y[letter_count_y+1] = (char)(KEY_ZERO); 
                            input_y[letter_count_y+2] = '\0';
                            letter_count_y += 2;
                        }
                    }else if (((key >= 48) && (key <= 57)) && !((input_y[3]==(char)(KEY_FIVE)) && (input_y[0]==(char)(KEY_TWO)) && (input_y[1]==(char)(KEY_FOUR))))
                    {
                        input_y[letter_count_y] = (char)key;
                        input_y[letter_count_y+1] = '\0';
                        letter_count_y++;
                    }else 
                    {
                        input_y[letter_count_y] = (char)(KEY_ZERO);
                        input_y[letter_count_y+1] = '\0';
                        letter_count_y++;
                    }
                    
                    key = GetCharPressed();  
                }
                if (IsKeyPressed(KEY_BACKSPACE))
                {
                    letter_count_y--;
                    if (letter_count_y < 0) letter_count_y= 0;
                    input_y[letter_count_y] = '\0';
                }
            } 
            
            
            if (mouse_on_z) //для зет координаты
            {
                
                SetMouseCursor(MOUSE_CURSOR_IBEAM);
                
                int key = GetCharPressed();
                
                while (key > 0 && letter_count_z < 5)
                {
                    
                    if(letter_count_z == 0)
                    {
                        if(key==KEY_MINUS) (z_minus = true)&&(letter_count_z=0);
                        if((key >= 48) && (key <= 50)) 
                        {
                            if(key==KEY_ZERO)
                            {
                                input_z[letter_count_z] = (char)key;
                                input_z[letter_count_z+1] = (char)key;
                                input_z[letter_count_z+2] = '\0';
                                letter_count_z +=2;
                            }else
                            {
                                input_z[letter_count_z] = (char)key;
                                input_z[letter_count_z+1] = '\0'; // добавляем нулевое заверршение строки
                                letter_count_z++;
                            }
                        }
                    } else if (letter_count_z == 1)
                    {
                        if((key >= 48) && (key <= 52) || key ==46)
                        {
                            if(key==KEY_PERIOD) 
                            {
                                input_z[letter_count_z] = KEY_ZERO;
                            }
                            else 
                            {
                                input_z[letter_count_z] = (char)key;
                            }
                            input_z[letter_count_z+1] = (char)(KEY_PERIOD);
                            input_z[letter_count_z+2] = '\0';
                            letter_count_z += 2;
                            
                        }
                    }else if (letter_count_z == 2)
                    {
                        input_z[letter_count_z] = (char)(KEY_PERIOD); 
                        input_z[letter_count_z+1] = '\0';
                        letter_count_z ++;
                        
                        
                    }else if (letter_count_z == 3)
                    {
                        if((key >= 48) && (key <= 57)&& !((input_z[0]==(char)(KEY_TWO)) && (input_z[1]==(char)(KEY_FOUR))))
                        {
                            input_z[letter_count_z] = (char)key;
                            input_z[letter_count_z+1] = '\0';
                            letter_count_z ++;
                            
                        } else 
                        {
                            input_z[letter_count_z] = (char)(KEY_FIVE);
                            input_z[letter_count_z+1] = (char)(KEY_ZERO); 
                            input_z[letter_count_z+2] = '\0';
                            letter_count_z += 2;
                        }
                    }else if (((key >= 48) && (key <= 57)) && !((input_z[3]==(char)(KEY_FIVE)) && (input_z[0]==(char)(KEY_TWO)) && (input_z[1]==(char)(KEY_FOUR))))
                    {
                        input_z[letter_count_z] = (char)key;
                        input_z[letter_count_z+1] = '\0';
                        letter_count_z++;
                    }else
                    {
                        input_z[letter_count_z] = (char)(KEY_ZERO);
                        input_z[letter_count_z+1] = '\0';
                        letter_count_z++;
                    }
                    
                    key = GetCharPressed();  
                }
                if (IsKeyPressed(KEY_BACKSPACE))
                {
                    letter_count_z--;
                    if(letter_count_z <=0) z_minus=false;
                    if (letter_count_z < 0) letter_count_z= 0;
                    input_z[letter_count_z] = '\0';
                }
            } 
            //конец валидаторов и ввода координат
            
            if(!mouse_on_x && !mouse_on_y && !mouse_on_z) // если курсор не в поле вода возвращаем ему обычный вид
            {
                SetMouseCursor(MOUSE_CURSOR_DEFAULT);
            }
            
            //ввод координат в массив
            if(IsKeyPressed(KEY_ENTER))
            {
                mas[counter].x = a=b=c=d=0;
                if(letter_count_x > 0) (a = input_x[0]-'0') && (mas[counter].x = a);
                if(letter_count_x > 1) (b = input_x[1] - '0') && (mas[counter].x = mas[counter].x*10+b);
                if(letter_count_x > 3) (c = input_x[3] - '0') && (mas[counter].x = mas[counter].x+(float)c/10);
                if(letter_count_x > 4) (d = input_x[4] - '0') && (mas[counter].x = mas[counter].x+(float)d/100);
                if(x_minus) mas[counter].x = mas[counter].x*-1;
                
                mas[counter].y = a=b=c=d=0;
                if(letter_count_y > 0) (a = input_y[0]-'0') && (mas[counter].y = a);
                if(letter_count_y > 1) (b = input_y[1] - '0') && (mas[counter].y = mas[counter].y*10+b);
                if(letter_count_y > 3) (c = input_y[3] - '0') && (mas[counter].y = mas[counter].y+(float)c/10);
                if(letter_count_y > 4) (d = input_y[4] - '0') && (mas[counter].y = mas[counter].y+(float)d/100);
                if(mas[counter].y == 0)
                {
                    mas[counter].y = 0.5;
                    input_y[0] = (char)(KEY_ZERO);
                    input_y[1] = (char)(KEY_ZERO);
                    input_y[2] = (char)(KEY_PERIOD);
                    input_y[3] = (char)(KEY_FIVE);
                }
                
                mas[counter].z = a=b=c=d=0;
                if(letter_count_z > 0) (a = input_z[0]-'0') && (mas[counter].z = a);
                if(letter_count_z > 1) (b = input_z[1] - '0') && (mas[counter].z = mas[counter].z*10+b);
                if(letter_count_z > 3) (c = input_z[3] - '0') && (mas[counter].z = mas[counter].z+(float)c/10);
                if(letter_count_z > 4) (d = input_z[4] - '0') && (mas[counter].z = mas[counter].z+(float)d/100);
                if(z_minus) mas[counter].z = mas[counter].z*-1;
                
                letter_count_x = 0;
                letter_count_y = 0;
                letter_count_z = 0;
                x_minus = false;
                z_minus = false;
                input_x[letter_count_x] = '\0';
                input_y[letter_count_y] = '\0';
                input_z[letter_count_z] = '\0';
                
                counter +=1;
            }
            // конец ввода координат в массив
            
         if(IsKeyDown(KEY_C))    //движение куба
        {
            if(point < counter-1)
            {
                if(point == 0 && !calculate)
                {
                    cubePosition = start_cubePosition;
                    calculate = true;
                }
                
                sum = 0;
                int next = point+1;
                
                
        
                if((abs(cubePosition.x - mas[next].x)<=0.3) && (abs(cubePosition.y-mas[next].y)<=0.3) && (abs(cubePosition.z-mas[next].z<=0.3)))
                {
                    cubePosition=mas[next];
                    point +=1;
                }else
                {
                    sum = sqrt((mas[next].x - mas[point].x)*(mas[next].x - mas[point].x) + (mas[next].y - mas[point].y)*(mas[next].y - mas[point].y) + (mas[next].z - mas[point].z)*(mas[next].z - mas[point].z));
                    
                    if(adaptate_speed)
                    {
                        if(sum>speed * 100) speed *= 2;
                        else speed = speed / 2;
                    }
                    
                    cubePosition.x += ((mas[next].x - mas[point].x) / sum) * speed;
                    cubePosition.y += ((mas[next].y - mas[point].y) / sum) * speed;
                    cubePosition.z += ((mas[next].z - mas[point].z) / sum) * speed;
                }
                
                
            }else
            {
                point = 0;
                calculate = false;
            }
        }
        if(IsKeyPressed(KEY_V)) // возвращение к началу при движении
        { 
            calculate = false;
            point = 0;
            cubePosition = start_cubePosition;
        }
        
        }
        else    //если режим ввода ручной
        {
            UpdateCamera(&camera, CAMERA_THIRD_PERSON); 
            
            DisableCursor();    //убираем курсор ограничивая его окном
            
            if (!movement)
            {
                camera.target = cubePosition;
                camera.position = (Vector3){cubePosition.x + 10, cubePosition.y + 10, cubePosition.z};
                camera.up = classic_camera_up;
                movement = true;
            }
            
            //передвижение вверх и вниз с помощью кнопок икс и зет
            if (IsKeyDown(KEY_Z))
                camera.target.y += 0.1;
            
            if (IsKeyDown(KEY_X))
                camera.target.y -= 0.1;
            
            //ограничение передвижения за область координат
            if (camera.target.x > 24.5)
                camera.target.x = 24.5;
            if (camera.target.y > 24.5)
                camera.target.y = 24.5;
            if (camera.target.z > 24.5)
                camera.target.z = 24.5;
            
            if (camera.target.x < -24.5)
                camera.target.x = -24.5;
            if (camera.target.y < 0.5)
                camera.target.y = 0.5;
            if (camera.target.z < -24.5)
                camera.target.z = -24.5;
        }
        
        
        
        
        if(IsKeyDown(KEY_N))    //удалеение точек
        {
            counter = 1;
            mas[0].x = start_cubePosition.x;
            mas[0].y = start_cubePosition.y;
            mas[0].z = start_cubePosition.z;
            cubePosition = start_cubePosition;
        }
        

        
        
        BeginDrawing(); //начало отрисовки
        
        ClearBackground(RAYWHITE);  //очищаем прошлый кадр 
        
        // рисуем  блок для ввода точек
        if(!draw)// если режим ввода координатный
        {
            DrawRectangleRec(cordinating,LIGHTGRAY);
            DrawRectangleLines(cordinating.x,cordinating.y,cordinating.width,cordinating.height,DARKGRAY);
            DrawText("INPUT COORDINATES",text_x.x-7, text_x.y-10,10,DARKGRAY);
            
            if (mouse_on_x) DrawRectangleLines((int)text_x.x, (int)text_x.y, (int)text_x.width, (int)text_x.height, RED);
            else DrawRectangleLines((int)text_x.x, (int)text_x.y, (int)text_x.width, (int)text_x.height, DARKGRAY);
            DrawText(input_x, (int)text_x.x + 10, (int)text_x.y + 8, 10, MAROON);
            if(!x_minus)DrawText(TextFormat("INPUT CHARS: %i/%i", letter_count_x, 5),text_x.x, text_x.y+30, 10, DARKGRAY);
            else DrawText(TextFormat("INPUT CHARS: %i/%i", letter_count_x+1, 6),text_x.x, text_x.y+30, 10, DARKGRAY);
            
            if (mouse_on_y) DrawRectangleLines((int)text_y.x, (int)text_y.y, (int)text_y.width, (int)text_y.height, RED);
            else DrawRectangleLines((int)text_y.x, (int)text_y.y, (int)text_y.width, (int)text_y.height, DARKGRAY);
            DrawText(input_y, (int)text_y.x + 10, (int)text_y.y + 8, 10, MAROON);
            DrawText(TextFormat("INPUT CHARS: %i/%i", letter_count_y, 5), text_y.x, text_y.y+30, 10, DARKGRAY);
            
            if (mouse_on_z) DrawRectangleLines((int)text_z.x, (int)text_z.y, (int)text_z.width, (int)text_z.height, RED);
            else DrawRectangleLines((int)text_z.x, (int)text_z.y, (int)text_z.width, (int)text_z.height, DARKGRAY);
            DrawText(input_z, (int)text_z.x + 10, (int)text_z.y + 8, 10, MAROON);
            if(!z_minus)DrawText(TextFormat("INPUT CHARS: %i/%i", letter_count_z, 5), text_z.x, text_z.y+30, 10, DARKGRAY);
            else DrawText(TextFormat("INPUT CHARS: %i/%i", letter_count_z+1, 6), text_z.x, text_z.y+30, 10, DARKGRAY);
            
            if(x_minus) DrawText("-", (int)text_x.x + 5, (int)text_x.y + 8, 10, MAROON);
            if(z_minus)DrawText("-", (int)text_z.x + 5, (int)text_z.y + 8, 10, MAROON);
        }
        //конец рисования блока ввода точек
        
        //начало создания и отрисовки 3д пространства
        BeginMode3D(camera);
        
        DrawCube(classic_camera_target, 50, 0, 50, GRAY);   //рисуем основание рабочей зоны
        
        DrawCubeWires(for_work_zone, 50, 25, 50, GREEN);    //рисуем рабочую зону
        
        
        
        for (int i = 1; i < counter; i++)   //вывод сохраненных точек
        {                
            int last = i-1;
            DrawLine3D({mas[i].x,mas[i].y,mas[i].z}, {mas[last].x,mas[last].y,mas[last].z}, BLACK);
            DrawCube({mas[i].x,mas[i].y,mas[i].z}, 1, 1, 1, GREEN);
            DrawCube({mas[last].x,mas[last].y,mas[last].z}, 1, 1, 1, RED);
        }
        
        
        //рисование куба
        if (!draw )
        {
            DrawCube(cubePosition, 1, 1, 1, BLUE);
        }
        else
        {
            DrawCube(camera.target, 1, 1, 1, BLUE);
        } 
        
        
        
        if (IsKeyDown(KEY_M))   //если нажата м показать сетку основания
            DrawGrid(100, 1.0f);
        
        EndMode3D();
        //конец отрисовки 3д пространства
        
        // вывод координат
        DrawRectangle(5, 5, 230, 30, Fade(SKYBLUE, 0.5f));
        DrawRectangleLines(5, 5, 230, 30, BLUE);
        
        DrawRectangle(5, 50, 230, 940, Fade(SKYBLUE, 0.5f));
        DrawRectangleLines(5, 50, 230, 940, BLUE);
        
        if (!draw)
        {
            DrawText(TextFormat("- Position: (x - %0.2f, y - %0.2f, z - %0.2f)", cubePosition.x, cubePosition.y, cubePosition.z), 10, 10, 10, RED);
        }
        else
        {
            DrawText(TextFormat("- Position: (x - %0.2f, y - %0.2f, z - %0.2f)", camera.target.x, camera.target.y, camera.target.z), 10, 10, 10, RED);
        }
        DrawText(TextFormat("- Counter of points: (%i)", counter), 10, 20, 10, RED);
        
        for (int i = 0; i < counter; i++)
        {
            
            DrawText(TextFormat("  - Point: (x - %0.2f, y - %0.2f, z - %0.2f)", mas[i].x, mas[i].y, mas[i].z), 10, 55+i*20, 10, RED);
            DrawText(TextFormat(" %i", i+1), 5, 55+i*20, 10, RED);
        }  
        //конец вывода координат
        
        //пометка для пользователя
        DrawRectangle(1345, 860, 545, 130, Fade(LIME, 0.5f));
        DrawRectangleLines(1345, 860, 545, 130, GREEN);
        
        DrawText("Remark:", 1350, 865, 12, MAROON);
        DrawText("WSAD - movement", 1365, 875, 12, MAROON);
        DrawText("Arrows - turns", 1365, 885, 12, MAROON);
        DrawText("QE - turns counterclockwise / clockwise", 1365, 895, 12, MAROON);
        DrawText("Space - change input from coordinate to manual", 1365, 905, 12, MAROON);
        DrawText("Enter - adding the entered coordinate", 1365, 915, 12, MAROON);
        DrawText("O - adding cube coordinates at the moment", 1365, 925, 12, MAROON);
        DrawText("N - point removal", 1365, 935, 12, MAROON);
        DrawText("C (hold down) - turn on the movement of the cube by points (use in coordinate mode)", 1365, 945, 12, MAROON);
        DrawText("V - return to the beginning when moving", 1365, 955, 12, MAROON);
        DrawText("M - show base grid", 1365, 965, 12, MAROON);
        DrawText("Z/X - to move up/down", 1500, 875, 12, MAROON);
        //пометка для пользователя
        
        EndDrawing();   //конец отрисовки
        
        
    }
    
    CloseWindow();
    
    return 0;
}
