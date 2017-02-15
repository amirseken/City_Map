
#include "symbol.h"

//error range used to draw symbol
#define SYMBOL_ERROR_RANGE 0.00005
#define MINIMUM_ERROR_RANGE 0.00005
#define MAXIMUM_ERROR_RANGE 0.0005
#define MAXIMUM_BOUNDARY MINIMUM_ERROR_RANGE*4

// all symbol consists of 16x16 pxicels each one represent maximum_boundary/16
#define ONE_PIXCEL MAXIMUM_BOUNDARY/16 

#define PI 3.14159265

t_bound_box square; 

void create_symbol_box(float x, float y){ //create basic symbol box with different color
    set_draw_mode (DRAW_NORMAL);
    t_point temp;
    temp.x = x-MAXIMUM_BOUNDARY/2;
    temp.y = y-MAXIMUM_BOUNDARY/2;    
    
    square = t_bound_box(temp, MAXIMUM_BOUNDARY, MAXIMUM_BOUNDARY);
    fillrect(square);   
}

void create_bigger_symbol_box(float x, float y){ //create bigger symbol box to highlight other symbol
    set_draw_mode (DRAW_NORMAL);
    setlinewidth(2);
    t_bound_box boundary;
    t_point temp;
    
    temp.x = x-MAXIMUM_BOUNDARY/2 - MAXIMUM_BOUNDARY/16;
    temp.y = y-MAXIMUM_BOUNDARY/2 - MAXIMUM_BOUNDARY/16;   
    
    boundary.bottom_left().x = temp.x;
    boundary.bottom_left().y = temp.y;
    boundary.top_right().x = temp.x + MAXIMUM_BOUNDARY + MAXIMUM_BOUNDARY/8;
    boundary.top_right().y = temp.y + MAXIMUM_BOUNDARY + MAXIMUM_BOUNDARY/8;
    setlinestyle(DASHED);
    setcolor(90,90,90);
    drawrect(boundary);
    
    setcolor(YELLOW);
    square = t_bound_box(temp, MAXIMUM_BOUNDARY + MAXIMUM_BOUNDARY/8, MAXIMUM_BOUNDARY/16);
    fillrect(square);  
 
    square = t_bound_box(temp, MAXIMUM_BOUNDARY/16, MAXIMUM_BOUNDARY + MAXIMUM_BOUNDARY/8);
    fillrect(square);
    
    temp.x = temp.x + MAXIMUM_BOUNDARY + MAXIMUM_BOUNDARY/16;
    square = t_bound_box(temp, MAXIMUM_BOUNDARY/16, MAXIMUM_BOUNDARY + MAXIMUM_BOUNDARY/8);
    fillrect(square);
    
    temp.x = temp.x - MAXIMUM_BOUNDARY - MAXIMUM_BOUNDARY/16;
    temp.y = temp.y + MAXIMUM_BOUNDARY + MAXIMUM_BOUNDARY/16;
    square = t_bound_box(temp, MAXIMUM_BOUNDARY + MAXIMUM_BOUNDARY/8, MAXIMUM_BOUNDARY/16);
    fillrect(square); 
}   

void draw_bottom_box (float x, float y, float width){ //draw bottom box that shows intersection info.
    t_point temp;
    float error_range = width/200;
    temp.x = x - error_range;
    temp.y = y;
    square = t_bound_box(temp, width/(3.5) + error_range*2, width/20 + error_range);
    setcolor(50,50,50);
    fillrect(square);
    temp.x = temp.x + error_range;
    square = t_bound_box(temp, width/(3.5)+ error_range, width/20);
    setcolor(WHITE);
    fillrect(square);   
}

void draw_left_text_box (t_bound_box rect, float width, float length){
    float error_range = width/25;
    
    rect.top_right().x = rect.bottom_left().x + width/2;
    rect.top_right().y = rect.top_right().y - error_range*4;
    rect.bottom_left().y = rect.bottom_left().y + width/(length) - error_range;
    setcolor(WHITE);
    fillrect(rect);
    
    setcolor(40,40,40);
    rect.top_right().x = rect.top_right().x - error_range/3;
    rect.top_right().y = rect.top_right().y - error_range/3;
    rect.bottom_left().y = rect.bottom_left().y + error_range/3;
    fillrect(rect);    
}

void draw_current_location_symbol (float x, float y, float size,  string type){ //draw location symbol to point out intersection
    set_draw_mode (DRAW_NORMAL);
 
    float outter_radius = MINIMUM_ERROR_RANGE * size;
    float error_range = SYMBOL_ERROR_RANGE;
    setcolor(255,241,111);
    fillarc (x,y+outter_radius*2,outter_radius,0,180);
    
    
    set_draw_mode (DRAW_NORMAL);
    setlinestyle (SOLID);
    setlinewidth(0);
    t_point points[3];

    points[0].x = x - outter_radius;
    points[0].y = y + outter_radius*2 + error_range/6;
    points[1].x = x + outter_radius;
    points[1].y = y + outter_radius*2 + error_range/6;
    points[2].x = x;
    points[2].y = y;
    fillpoly(points, 3);
    
    setcolor(50,50,50);
    fillarc (x,y+outter_radius*2,MINIMUM_ERROR_RANGE*((2*size)/5),0, 360);
    
    setlinewidth(0.5);
    setcolor (50,50,50);
    drawarc (x,y+outter_radius*2,outter_radius,0,180);
    drawline(points[0].x, points[0].y, points[2].x, points[2].y);
    drawline(points[1].x, points[1].y, points[2].x, points[2].y);
    
    setfontsize(8);
    setcolor(WHITE);
    
    if (type != "original")
        drawtext(x,y+outter_radius*2, type ,outter_radius,outter_radius);
}

void draw_hospital_symbol (float x, float y){ 
    t_point temp;
    temp.x = x-MAXIMUM_BOUNDARY/2;
    temp.y = y-MAXIMUM_BOUNDARY/2;    
    
    setcolor(109,215,226);
    create_symbol_box(x,y);
    
    temp.x = temp.x + ONE_PIXCEL*4;
    temp.y = temp.y + ONE_PIXCEL*2;

    square = t_bound_box(temp, ONE_PIXCEL*8, (ONE_PIXCEL*(12)));
    setcolor(WHITE);
    fillrect(square);

    setcolor(109,215,226);
    temp.x = temp.x + ONE_PIXCEL*2;
    square = t_bound_box(temp, ONE_PIXCEL*4, (ONE_PIXCEL*5));
    fillrect(square);
    temp.y = temp.y + ONE_PIXCEL*(7);
    square = t_bound_box(temp, ONE_PIXCEL*4, (ONE_PIXCEL*5));
    fillrect(square);   
}
void draw_cafe_symbol (float x, float y){ 
    t_point temp;
    temp.x = x-MAXIMUM_BOUNDARY/2;
    temp.y = y-MAXIMUM_BOUNDARY/2;
    
    setcolor(100,100,100);
    create_symbol_box(x,y);
    
    setcolor(WHITE);
    temp.x = temp.x + ONE_PIXCEL*2;
    temp.y = temp.y + ONE_PIXCEL*2;
    square = t_bound_box(temp, ONE_PIXCEL*10, ONE_PIXCEL);
    fillrect(square);
    
    temp.x = temp.x + ONE_PIXCEL;
    temp.y = temp.y + ONE_PIXCEL*2;
    square = t_bound_box(temp, ONE_PIXCEL*8, ONE_PIXCEL*7);
    fillrect(square);
    
    temp.x = temp.x + ONE_PIXCEL*8;
    temp.y = temp.y + ONE_PIXCEL*3;
    square = t_bound_box(temp, ONE_PIXCEL*3, ONE_PIXCEL*4);
    fillrect(square);
    
    setcolor(100,100,100);
    temp.y = temp.y + ONE_PIXCEL;
    square = t_bound_box(temp, ONE_PIXCEL*2, ONE_PIXCEL*2);
    fillrect(square);
     
}
void draw_restaurant_symbol (float x, float y){ 
    t_point temp;
    temp.x = x-MAXIMUM_BOUNDARY/2;
    temp.y = y-MAXIMUM_BOUNDARY/2;
    setcolor(50,50,50);
    create_symbol_box(x,y);
    
    setcolor(WHITE);
    temp.x = temp.x + ONE_PIXCEL*3;
    temp.y = temp.y + ONE_PIXCEL*9;
    square = t_bound_box(temp, ONE_PIXCEL, ONE_PIXCEL*5);
    fillrect(square);
     
    square = t_bound_box(temp, ONE_PIXCEL*5, ONE_PIXCEL);
    fillrect(square);
    
    temp.x = temp.x + ONE_PIXCEL*4;
    square = t_bound_box(temp, ONE_PIXCEL, ONE_PIXCEL*5);
    fillrect(square);
    
    temp.x = temp.x - ONE_PIXCEL*2;
    temp.y = temp.y - ONE_PIXCEL*7;
    square = t_bound_box(temp, ONE_PIXCEL, ONE_PIXCEL*12);
    fillrect(square);
    
    temp.x = temp.x + ONE_PIXCEL*5;
    square = t_bound_box(temp, ONE_PIXCEL*3, ONE_PIXCEL*12);
    fillrect(square);
    
    setcolor(50,50,50);
    square = t_bound_box(temp, ONE_PIXCEL*2, ONE_PIXCEL*6);
    fillrect(square);
    
}
void draw_subway_symbol (float x, float y){ 
    t_point temp;
    temp.x = x-MAXIMUM_BOUNDARY/2;
    temp.y = y-MAXIMUM_BOUNDARY/2; 
    
    setcolor(220,22,53);
    create_symbol_box(x,y);
    
    setcolor(WHITE);
    temp.x = temp.x + ONE_PIXCEL*2;
    temp.y = temp.y + ONE_PIXCEL;
    square = t_bound_box(temp, ONE_PIXCEL*12, ONE_PIXCEL*13);
    fillrect(square);
    
    setcolor(220,22,53);
    temp.y = temp.y + ONE_PIXCEL*2;
    square = t_bound_box(temp, ONE_PIXCEL, ONE_PIXCEL); 
    fillrect(square);
    temp.y = temp.y + ONE_PIXCEL*10;
    square = t_bound_box(temp, ONE_PIXCEL, ONE_PIXCEL);
    fillrect(square);
    temp.x = temp.x + ONE_PIXCEL*11;
    square = t_bound_box(temp, ONE_PIXCEL, ONE_PIXCEL);
    fillrect(square);    
    temp.y = temp.y - ONE_PIXCEL*10;
    square = t_bound_box(temp, ONE_PIXCEL, ONE_PIXCEL);   
    fillrect(square);
    temp.y = temp.y - ONE_PIXCEL*2;
    temp.x = temp.x - ONE_PIXCEL*3;
    square = t_bound_box(temp, ONE_PIXCEL, ONE_PIXCEL);
    fillrect(square);    
    temp.x = temp.x - ONE_PIXCEL*5;
    square = t_bound_box(temp, ONE_PIXCEL, ONE_PIXCEL);
    fillrect(square);
    
    temp.x = temp.x + ONE_PIXCEL;
    square = t_bound_box(temp, ONE_PIXCEL*4, ONE_PIXCEL*3);
    fillrect(square);
    temp.x = temp.x - ONE_PIXCEL*3;
    temp.y = temp.y + ONE_PIXCEL*5;
    square = t_bound_box(temp, ONE_PIXCEL*2, ONE_PIXCEL*2);
    fillrect(square);
    temp.x = temp.x + ONE_PIXCEL*8;
    square = t_bound_box(temp, ONE_PIXCEL*2, ONE_PIXCEL*2);
    fillrect(square);
    temp.x = temp.x - ONE_PIXCEL*8;
    temp.y = temp.y + ONE_PIXCEL*3;
    square = t_bound_box(temp, ONE_PIXCEL*10, ONE_PIXCEL*3);
    fillrect(square);   
}
void draw_fastfood_symbol (float x, float y){ 
    t_point temp;
    temp.x = x-MAXIMUM_BOUNDARY/2;
    temp.y = y-MAXIMUM_BOUNDARY/2;    
    
    setcolor(20,20,20);
    create_symbol_box(x,y);
    
    temp.x = temp.x + ONE_PIXCEL*3;
    temp.y = temp.y + ONE_PIXCEL*2;
    setcolor(255,191,82);
    square = t_bound_box(temp, ONE_PIXCEL*10, ONE_PIXCEL*2);
    fillrect(square);
    
    temp.x = temp.x - ONE_PIXCEL;
    temp.y = temp.y + ONE_PIXCEL*3;
    setcolor(146,69,64);
    square = t_bound_box(temp, ONE_PIXCEL*12, ONE_PIXCEL*3);
    fillrect(square);
    
    temp.x = temp.x + ONE_PIXCEL;
    temp.y = temp.y + ONE_PIXCEL*4;
    setcolor(255,191,82);
    square = t_bound_box(temp, ONE_PIXCEL*10, ONE_PIXCEL*3);
    fillrect(square);
    
    temp.y = temp.y + ONE_PIXCEL*2;
    setcolor(20,20,20);
    square = t_bound_box(temp, ONE_PIXCEL, ONE_PIXCEL);
    fillrect(square);
    
    temp.x = temp.x + ONE_PIXCEL*9;
    square = t_bound_box(temp, ONE_PIXCEL*2, ONE_PIXCEL);
    fillrect(square);
}
void draw_bank_symbol (float x, float y){
    t_point temp;
    temp.x = x-MAXIMUM_BOUNDARY/2;
    temp.y = y-MAXIMUM_BOUNDARY/2;    
    
    setcolor(77,183,47);
    create_symbol_box(x,y);
    setcolor(WHITE);
    
    temp.x = temp.x + ONE_PIXCEL*3;
    temp.y = temp.y + ONE_PIXCEL*2;
    square = t_bound_box(temp, ONE_PIXCEL*11, ONE_PIXCEL*2);
    fillrect(square);
    
    temp.x = temp.x + ONE_PIXCEL*9;
    square = t_bound_box(temp, ONE_PIXCEL*2, ONE_PIXCEL*7);
    fillrect(square);
    
    temp.x = temp.x - ONE_PIXCEL*9;
    temp.y = temp.y + ONE_PIXCEL*5;
    square = t_bound_box(temp, ONE_PIXCEL*11, ONE_PIXCEL*2);
    fillrect(square);
    
    square = t_bound_box(temp, ONE_PIXCEL*2, ONE_PIXCEL*7);
    fillrect(square);
    
    temp.y = temp.y + ONE_PIXCEL*5;
    square = t_bound_box(temp, ONE_PIXCEL*11, ONE_PIXCEL*2);
    fillrect(square);
    
    temp.x = temp.x + ONE_PIXCEL*4;
    temp.y = temp.y - ONE_PIXCEL*11;
    square = t_bound_box(temp, ONE_PIXCEL, ONE_PIXCEL*14);
    fillrect(square);
    
    temp.x = temp.x + ONE_PIXCEL*3;
    square = t_bound_box(temp, ONE_PIXCEL, ONE_PIXCEL*14);
    fillrect(square);
}

void draw_arrow(LatLon middle, LatLon to){ //draw arrow in one-way street segment to recognize which way is one-way 
    float arrow = MINIMUM_ERROR_RANGE*2;
    to.lon = to.p_lon(); 
    float degree = get_degree(middle,to);
    setcolor(150,150,150);
    
    if (degree > 350 && degree < 10){
        drawline(middle.lon ,middle.lat, middle.lon-arrow, middle.lat+arrow);
        drawline(middle.lon ,middle.lat, middle.lon-arrow, middle.lat-arrow);
    }
    else if (degree > 10 && degree < 80){
        drawline(middle.lon ,middle.lat, middle.lon-arrow, middle.lat);
        drawline(middle.lon ,middle.lat, middle.lon, middle.lat-arrow);
    }
    else if (degree > 80 && degree < 100){
        drawline(middle.lon ,middle.lat, middle.lon-arrow, middle.lat-arrow);
        drawline(middle.lon ,middle.lat, middle.lon+arrow, middle.lat-arrow);
    }
    else if (degree > 100 && degree < 170){
        drawline(middle.lon ,middle.lat, middle.lon+arrow, middle.lat);
        drawline(middle.lon ,middle.lat, middle.lon, middle.lat-arrow);
    }
    else if (degree > 170 && degree < 190){
        drawline(middle.lon ,middle.lat, middle.lon+arrow, middle.lat+arrow);
        drawline(middle.lon ,middle.lat, middle.lon+arrow, middle.lat-arrow);
    }
    else if (degree > 190 && degree < 260){
         drawline(middle.lon ,middle.lat, middle.lon-arrow, middle.lat);
        drawline(middle.lon ,middle.lat, middle.lon, middle.lat-arrow);
    }
    else if (degree > 260 && degree < 280){
        drawline(middle.lon ,middle.lat, middle.lon+arrow, middle.lat+arrow);
        drawline(middle.lon ,middle.lat, middle.lon-arrow, middle.lat+arrow);
    }
    else if (degree > 280 && degree < 350){
        drawline(middle.lon ,middle.lat, middle.lon-arrow, middle.lat);
        drawline(middle.lon ,middle.lat, middle.lon, middle.lat+arrow);
    }

}

void draw_help_text(t_bound_box rect){ // draw text for help button
    
    float width = (rect.top_right().x - rect.bottom_left().x)/2;
    float error_range = (rect.top_right().y - rect.bottom_left().y)/50;
    
    draw_left_text_box(rect, width, 5);
    
    rect.bottom_left().x = rect.bottom_left().x + width/4;
    rect.top_right().y = rect.top_right().y - width/6;
    
    setfontsize(11);
    setcolor(PINK);
    drawtext(rect.bottom_left().x, rect.top_right().y - error_range ,"Change map:", width, width);
    setfontsize(9);
    setcolor(WHITE);
    drawtext(rect.bottom_left().x, rect.top_right().y - error_range*2 ,"1.Type number 1-6 in command line", width, width);
    
    setfontsize(11);
    setcolor(PINK);
    drawtext(rect.bottom_left().x, rect.top_right().y - error_range*4 ,"Find POI(Point of Interest):", width, width);
    setfontsize(9);
    setcolor(WHITE);
    drawtext(rect.bottom_left().x, rect.top_right().y - error_range*5 ,"1. Left-click on intersection", width, width);
    drawtext(rect.bottom_left().x, rect.top_right().y - error_range*6 ,"2. Click 'Find Interest' button", width, width);
    drawtext(rect.bottom_left().x, rect.top_right().y - error_range*7 ,"3. Type distance you want to see point of interest", width, width);
    drawtext(rect.bottom_left().x, rect.top_right().y - error_range*8 ,"4. Map will show point of interests within distance", width, width);
    drawtext(rect.bottom_left().x, rect.top_right().y - error_range*9 ,"5. Click symbol for additional information", width, width);
    
    setfontsize(11);
    setcolor(PINK);
    drawtext(rect.bottom_left().x, rect.top_right().y - error_range*11 ,"Disable All:", width, width);
    setfontsize(9);
    setcolor(WHITE);
    drawtext(rect.bottom_left().x, rect.top_right().y - error_range*12 ,"1. Click the button to disable all of extra graphics", width, width);   
    
    setfontsize(11);
    setcolor(PINK);
    drawtext(rect.bottom_left().x, rect.top_right().y - error_range*14 ,"Fastest Path(Click):", width, width);
    setfontsize(9);
    setcolor(WHITE);
    drawtext(rect.bottom_left().x, rect.top_right().y - error_range*15 ,"1. Left-click on intersection", width, width);
    drawtext(rect.bottom_left().x, rect.top_right().y - error_range*16 ,"2. Click 'Fastest Path(click)' button", width, width);
    drawtext(rect.bottom_left().x, rect.top_right().y - error_range*17 ,"3. Left-click another intersection", width, width);
    drawtext(rect.bottom_left().x, rect.top_right().y - error_range*18 ,"4. Fastest Path will be shown", width, width);
    
    setfontsize(11);
    setcolor(PINK);
    drawtext(rect.bottom_left().x, rect.top_right().y - error_range*20 ,"Fastest Path(Type):", width, width);
    setfontsize(9);
    setcolor(WHITE);
    drawtext(rect.bottom_left().x, rect.top_right().y - error_range*21 ,"1. Left-click on intersection", width, width);
    drawtext(rect.bottom_left().x, rect.top_right().y - error_range*22 ,"2. Click 'Fastest Path(Type)' button", width, width);
    drawtext(rect.bottom_left().x, rect.top_right().y - error_range*23 ,"3. Follow instructions in search box and type", width, width);
    drawtext(rect.bottom_left().x, rect.top_right().y - error_range*24 ,"4. With invalid name, it will find most similar name", width, width);
    drawtext(rect.bottom_left().x, rect.top_right().y - error_range*25 ,"5. Fastest Path will be shown", width, width);
    
    setfontsize(11);
    setcolor(PINK);
    drawtext(rect.bottom_left().x, rect.top_right().y - error_range*27 ,"Find Street:", width, width);
    setfontsize(9);
    setcolor(WHITE);
    drawtext(rect.bottom_left().x, rect.top_right().y - error_range*28 ,"1. Type street name in command line", width, width);
    
    setfontsize(11);
    setcolor(PINK);
    drawtext(rect.bottom_left().x, rect.top_right().y - error_range*30 ,"Find Intersection:", width, width);
    setfontsize(9);
    setcolor(WHITE);
    drawtext(rect.bottom_left().x, rect.top_right().y - error_range*31 ,"1. Type two street names in command line", width, width);
    setcolor(WHITE);
    drawtext(rect.bottom_left().x, rect.top_right().y - error_range*33 ,"For Search box, you can use auto-fill feature", width, width);
    drawtext(rect.bottom_left().x, rect.top_right().y - error_range*34 ,"Press 'TAB' key to fill street or intersection name", width, width);
    drawtext(rect.bottom_left().x, rect.top_right().y - error_range*35 ,"Ex) Dun -> ('TAB' key) Dundas Street", width, width);
}

void print_help_fastest_path(t_bound_box rect){
    float width = (rect.top_right().x - rect.bottom_left().x)/2;
    draw_left_text_box(rect, width, 4);
}

void invalid_input_box(float x, float y, float width){
    t_point temp;
    float error_range = width/200;
    temp.x = x - (width/(5) + error_range*2)/2;
    temp.y = y - (width/20 + error_range)/2;
    square = t_bound_box(temp, width/(5) + error_range*2, width/20 + error_range);
    setcolor(PINK);
    fillrect(square);
    temp.x = temp.x + error_range;
    temp.y = temp.y + error_range;
    square = t_bound_box(temp, width/(5), width/20 - error_range);
    setcolor(WHITE);
    fillrect(square); 
    
    temp.x = temp.x + (width/(5) + error_range)/2;
    temp.y = temp.y + (width/40) + error_range*2;
    setfontsize(9);
    setcolor(BLACK);
    drawtext(temp.x, temp.y, "Invalid Input is typed", width, width);
    drawtext(temp.x, temp.y-error_range*2.5, "Click 'Disable Button' and try again", width, width);
    
    temp.x = temp.x - error_range*2;
    temp.y = temp.y - error_range*6;
    setcolor(PINK);
    square = t_bound_box(temp, width/40, width/80);
    fillrect(square);
    setcolor(BLACK);
    setlinewidth(1);
    drawrect(square);
    
    setfontsize(10);
    drawtext(temp.x+width/80, temp.y+width/80 - error_range, "OK", width, width);
}

float get_degree(LatLon from, LatLon to){ //get degree from two position
    float degree;
    float x = to.lon - from.lon;
    float y = to.lat - from.lat;
    degree = fabs((atan(y/x)* 180 / PI));
    
    if (x < 0 && y > 0)
        degree = (90-degree) + 90;
    else if (x < 0 && y < 0)
        degree = degree + 180;
    else if (x > 0 && y < 0)
        degree = (90-degree) + 270;
    else
        degree = degree;
    
    return degree; 
}
