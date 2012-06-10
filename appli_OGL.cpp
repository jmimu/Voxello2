#include <SDL/SDL.h>

#include <iostream>
#include <math.h>

#include "voxspr.h"
#include "vox_scene.h"
#include "particle_manager.h"
#include "graph_OGL.h"
#include "draw_vox_ortho.h"

int main(int argc, char *argv[])
{
    (void) argc;
    (void) argv;
    
    //SDL is used for inputs and time
    SDL_Init(SDL_INIT_VIDEO);
    atexit(SDL_Quit);
    SDL_EnableKeyRepeat(10, 10);
    
    
    Graph *graph=new Graph_OGL();
    graph->init();
    //create vox obj
    std::vector<VoxImg*> walk_anim;
    walk_anim.push_back(new VoxImg("data/perso_stand.vox",VOX_FILE));
    walk_anim.push_back(new VoxImg("data/perso_walk1.vox",VOX_FILE));
    walk_anim.push_back(new VoxImg("data/perso_walk2.vox",VOX_FILE));
    walk_anim.push_back(new VoxImg("data/perso_walk1.vox",VOX_FILE));
    walk_anim.push_back(new VoxImg("data/perso_stand.vox",VOX_FILE));
    walk_anim.push_back(new VoxImg("data/perso_walk3.vox",VOX_FILE));
    walk_anim.push_back(new VoxImg("data/perso_walk4.vox",VOX_FILE));
    walk_anim.push_back(new VoxImg("data/perso_walk3.vox",VOX_FILE));
    float anim_index=0;

    VoxImg img_palet("data/palet2.vox",VOX_FILE);
    Vox_Scene scene(160,120,70,img_palet);
    Vox_Scene ground(160,120,70,img_palet);
    VoxSpr perso(walk_anim.at(anim_index));
    //Vox_Scene scene(img_palet);
    VoxSpr palet(&img_palet);
    ground.init();
    
    Draw_Vox_Ortho *draw=new Draw_Vox_Ortho(graph,&scene);//maybe create a generic class for otho or perspective
    new Particle_Manager(&scene,&ground);

    //----- events -----
    SDL_Event event;

    //----- timing -----
    Uint32 last_time = SDL_GetTicks();
    Uint32 current_time,ellapsed_time,start_time;
    Uint32 previous_fps_time=SDL_GetTicks()/1000;
    int fps=0;


    double angleZ = 0;
    double angleY = 0;
    double angleX = 0;
    
    Pt3d pos(10,10,10);

    Pt3d ball(10,10,1);
    double ball_r=4;

    Pt3d ball_v(1,1,1);

    draw->update_camera(100,100,0,angleX,angleY,angleZ);//lasy camera updating...
    bool run=true;
    double t=0.0;
    while (run)
    {
        fps++;

        start_time = SDL_GetTicks();
        while (SDL_PollEvent(&event))
        {

            switch(event.type)
            {
                case SDL_QUIT:
                    exit(0);
                break;

                case SDL_KEYDOWN:
                    switch(event.key.keysym.sym)
                    {
                        case SDLK_ESCAPE:
                            run=false;
                            break;
                        case SDLK_UP:
                            pos.x-=pseudo_normalize(draw->vect_vox_x_c.z,graph->ORTHO_ZOOM);
                            pos.y-=pseudo_normalize(draw->vect_vox_y_c.z,graph->ORTHO_ZOOM);
                            anim_index+=0.5;
                            break;
                        case SDLK_DOWN:
                            pos.x+=pseudo_normalize(draw->vect_vox_x_c.z,graph->ORTHO_ZOOM);
                            pos.y+=pseudo_normalize(draw->vect_vox_y_c.z,graph->ORTHO_ZOOM);
                            anim_index+=0.5;
                            break;
                        case SDLK_RIGHT:
                            pos.x+=pseudo_normalize(draw->vect_vox_x_c.x,graph->ORTHO_ZOOM);
                            pos.y+=pseudo_normalize(draw->vect_vox_y_c.x,graph->ORTHO_ZOOM);
                            anim_index+=0.5;
                            break;
                        case SDLK_LEFT:
                            pos.x-=pseudo_normalize(draw->vect_vox_x_c.x,graph->ORTHO_ZOOM);
                            pos.y-=pseudo_normalize(draw->vect_vox_y_c.x,graph->ORTHO_ZOOM);
                            anim_index+=0.5;
                            break;
                        case SDLK_q:
                            pos.x-=pseudo_normalize(draw->vect_vox_x_c.y,graph->ORTHO_ZOOM);
                            pos.y-=pseudo_normalize(draw->vect_vox_y_c.y,graph->ORTHO_ZOOM);
                            anim_index+=0.5;
                            break;
                        case SDLK_w:
                            pos.x+=pseudo_normalize(draw->vect_vox_x_c.y,graph->ORTHO_ZOOM);
                            pos.y+=pseudo_normalize(draw->vect_vox_y_c.y,graph->ORTHO_ZOOM);
                            anim_index+=0.5;
                            break;
                        default:
                            break;
                    }
                    break;
                
                case SDL_MOUSEMOTION:
                    angleZ -= event.motion.xrel*0.01;
                    angleX += event.motion.yrel*0.01;
                    draw->update_camera(100,100,0,angleX,angleY,angleZ);
                break;

                case SDL_MOUSEBUTTONDOWN:
                    if ((event.button.button == SDL_BUTTON_WHEELUP)&&(event.button.type == SDL_MOUSEBUTTONDOWN))
                    {
                        graph->zoom_change(+0.5);
                        draw->update_camera(100,100,0,angleX,angleY,angleZ);
                    }
                    if ((event.button.button == SDL_BUTTON_WHEELDOWN)&&(event.button.type == SDL_MOUSEBUTTONDOWN))
                    {
                        graph->zoom_change(-0.5);
                        draw->update_camera(100,100,0,angleX,angleY,angleZ);
                    }
                    break;

            }
        }
        
        for (int i=0;i<10;i++)
        {
            Pt3d snow(rand()%scene.xsiz,rand()%scene.ysiz,0);
            Pt3d snow_speed(0,0,1);
            /*if (int(t*20)%10==0)*/ Particle_Manager::the_one->add_particle(31,snow,snow_speed,0);
        }
        Particle_Manager::the_one->update();
        ground.sub(palet);
        if (anim_index>=walk_anim.size()) anim_index=0;
        if (anim_index<0) anim_index=walk_anim.size()-0.01;
        perso.set_img(walk_anim.at((int)anim_index));
        perso.set_pos(pos);
        palet.set_pos(ball);
        scene.reinit(ground);//have to reinit scene AFTER modifications (add, sub, particle update), and JUST before blit!
        Particle_Manager::the_one->blit_all();
        scene.blit(perso);
        scene.blit(palet);
        
        //scene.reinit(img_palet);
        
        //angleZ += 0.00020 * ellapsed_time;
        //angleY += 0.00023 * ellapsed_time;
        //angleX += 0.00027 * ellapsed_time;
        t+=0.05;

        ball.add(ball_v,&ball);
        if (ball.x>=scene.xsiz-ball_r-1) ball_v.x*=-1;
        if (ball.y>=scene.ysiz-ball_r-1) ball_v.y*=-1;
        if (ball.z>=scene.zsiz-1) ball_v.z*=-1;
        if (ball.x<=ball_r) ball_v.x*=-1;
        if (ball.y<=ball_r) ball_v.y*=-1;
        if (ball.z<=0) ball_v.z*=-1;
        if (ground.collide(palet) && (ball_v.z>0)) ball_v.z*=-1;

        if (!ground.touch_bottom(perso)) pos.z+=1;
        else if (ground.collide(perso)) pos.z-=1;
        /*else
        {
            //if (obj1.pos.z<scene.zsiz)
            {
                obj1.pos.z+=1;
                if (!ground.collide(perso)) pos.z+=1;
                obj1.pos.z-=1;
            }
        }*/

        //drawing
        //draw->update_camera(100,100,0,angleX,angleY,angleZ);
        draw->render();


        //----- timing -----
        if (previous_fps_time!=current_time/1000)
        {
            std::cout<<"FPS: "<<fps<<std::endl;
            previous_fps_time=current_time/1000;
            fps=0;
        }
        current_time = SDL_GetTicks();
        ellapsed_time = current_time - last_time;
        last_time = current_time;
        ellapsed_time = SDL_GetTicks() - start_time;
        //if (ellapsed_time < 20)
        //    SDL_Delay(20 - ellapsed_time);
        //----- end timing -----
    }
    
    
    return 0;
}

