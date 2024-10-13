// All my render function
#include "object.cpp"

// A pragma is a compiler directive that allows you to provide additional information to the compiler
#pragma region Background
Object background = Object(WINDOWS_CENTER_X, WINDOWS_CENTER_X);
void renderBackground(){

    background.drawBackgroundRect(0, 0, WINDOWS_WIDTH, WINDOWS_HEIGHT); 	//(0,0) bottom left point
}
#pragma endregion Background

#pragma region Group Flower
Object flower = Object();
void renderFlower(){
    for(int x = 0; x <= WINDOWS_WIDTH; x+=300){
        for(int y = 0; y <= WINDOWS_HEIGHT; y+=300){
            flower.x = x;
            flower.y = y;
            flower.drawFlower(70);
        }
    }
}
#pragma endregion Group Flower

#pragma region Group Watch Body
Object watchBody = Object(WINDOWS_WIDTH/2, WINDOWS_HEIGHT/2);
Object watchStrap = Object(WINDOWS_WIDTH/2, WINDOWS_HEIGHT/2);
void renderWatchBody(){

    watchStrap.drawWatchStrap(150, 200);
    watchBody.drawWatchBody(WATCH_BODY_RADIUS, 5, 0, 0);
}
#pragma endregion Group Watch Body

#pragma region Group Date
Object txtDayDate = Object(WINDOWS_WIDTH/2, WINDOWS_HEIGHT/2);
void renderDate(){
    // Draw day and date text
    txtDayDate.translate(-WATCH_BODY_RADIUS + 90, WATCH_BODY_RADIUS - 55);
    txtDayDate.scale(0.12);
    txtDayDate.drawText("THU 10/24", 2);
}
#pragma endregion Group Date

#pragma region Group Time
Object txtTime = Object(WINDOWS_WIDTH/2, WINDOWS_HEIGHT/2);
void renderTime(){

    // Draw time text
    txtDayDate.translate(-60,-60);
    txtDayDate.scale(0.45);
    txtDayDate.drawText("10:40", 3);
}
#pragma endregion Group Time

#pragma region Group Heart
Object grid = Object();
Object LeftCircle = Object(WINDOWS_WIDTH/2, WINDOWS_HEIGHT/2);
void renderHeart(){
    // grid.drawBackgroundLine(WINDOWS_WIDTH, WINDOWS_HEIGHT, 1);
    LeftCircle.drawHeartContainer((WINDOWS_WIDTH/2)-70, (WINDOWS_HEIGHT/2)-25, 45, 7, 18);
}
#pragma endregion Group Heart

#pragma region Group Battery
Object RightCircle = Object(WINDOWS_WIDTH/2, WINDOWS_HEIGHT/2);
void renderBattery(){
    RightCircle.drawBatteryContainer((WINDOWS_WIDTH/2)+70, (WINDOWS_HEIGHT/2)-25, 45, 7, 10);
}
#pragma endregion Group Battery

#pragma region Group Step
Object step = Object(WINDOWS_WIDTH/2, WINDOWS_HEIGHT/2);
void renderStep(){
    glColor4f(COLOR_BLACK);
    step.translate(-30, -80);
    step.scale(0.12);
    step.drawText("STEPS", 2);

    step.translate(-30, -40);
    step.scale(0.24);
    step.drawText("25686", 3);
}
#pragma endregion Group Step

#pragma region shadow
Object shadow = Object(WINDOWS_WIDTH/2, WINDOWS_HEIGHT/2);
void renderFilter(){
    shadow.drawFilter(WATCH_BODY_RADIUS);
}
#pragma endregion shadow