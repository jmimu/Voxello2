#include <SDL/SDL.h>
#include <iostream>
#include <math.h>
#include <vector>


#include "voxspr.h"
#include "vox_scene.h"
#include "particle_manager.h"
#include "graph_OGL.h"
#include "draw_vox_ortho.h"
#include "snowball.h"

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
    std::vector<VoxImg*> walk_anim[4];//for directions E S W N
    std::vector<VoxImg*> throw_anim[4];//for directions E S W N
    VoxImg* pick_img[4];
    for (int i=0;i<4;i++)
    {
        walk_anim[i].push_back(new VoxImg("data/perso_stand.vox",i,VOX_FILE));
        walk_anim[i].push_back(new VoxImg("data/perso_walk1.vox",i,VOX_FILE));
        walk_anim[i].push_back(new VoxImg("data/perso_walk2.vox",i,VOX_FILE));
        walk_anim[i].push_back(new VoxImg("data/perso_walk1.vox",i,VOX_FILE));
        walk_anim[i].push_back(new VoxImg("data/perso_stand.vox",i,VOX_FILE));
        walk_anim[i].push_back(new VoxImg("data/perso_walk3.vox",i,VOX_FILE));
        walk_anim[i].push_back(new VoxImg("data/perso_walk4.vox",i,VOX_FILE));
        walk_anim[i].push_back(new VoxImg("data/perso_walk3.vox",i,VOX_FILE));
        throw_anim[i].push_back(new VoxImg("data/perso_throw1.vox",i,VOX_FILE));
        throw_anim[i].push_back(new VoxImg("data/perso_throw2.vox",i,VOX_FILE));
        throw_anim[i].push_back(new VoxImg("data/perso_throw3.vox",i,VOX_FILE));
        pick_img[i]=new VoxImg("data/perso_take.vox",i,VOX_FILE);
    }
    float anim_index=0;
    int direction=EAST;
    int action=0;//0: do nothing, 1=pick, 2=throw
    bool ball_thrown=true;
    long nbr_snow_blocks=1000;

    VoxImg img_palet("data/palet2.vox",VOX_FILE);
    VoxImg img_subber("data/subber.vox",VOX_FILE);//mask to remove snow when picking
    std::cout<<"Create scene\n";
    Vox_Scene scene(120,100,50,img_palet);
    std::cout<<"Create ground\n";
    Vox_Scene ground(120,100,50,img_palet);
    VoxSpr perso(walk_anim[direction].at(anim_index));
    VoxSpr palet(&img_palet);
    VoxSpr subber(&img_subber);
    ground.init();
    
    Snowball::ground=&ground;
    //randomize Snowball color:
    for (long i=0;i<Snowball::snowballimage.get_xyzsiz();i++)
    {
        if (Snowball::snowballimage.voxels[i]!=0xFF) Snowball::snowballimage.voxels[i]=rand()%(SNOW_END-SNOW_START)+SNOW_START;
    }
    
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
    
    Pt3d pos(0,0,10);
    Pt3d previous_pos(0,0,10);

    Pt3d ball(10,10,1);
    double ball_r=4;

    Pt3d ball_v(1,0.5,1.2);

    draw->update_camera(100,100,0,angleX,angleY,angleZ);//lasy camera updating...
    bool run=true;
    double t=0.0;
    while (run)
    {
        fps++;

        start_time = SDL_GetTicks();
        double move_x=0;
        double move_y=0;
        bool perso_moves=false;
        previous_pos.copy(pos);
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
                        case SDLK_SPACE:
                            if (action==0) action=1;
                            break;
                        case SDLK_RETURN:
                            if (action==0)
                            {
                              action=2;
                              anim_index=0;
                              ball_thrown=false;
                            }
                            break;
                        case SDLK_UP:
                            if (action==0)
                            {
                              move_x=0;
                              move_y=+1;
                              perso_moves=true;
                            }
                            break;
                        case SDLK_DOWN:
                            if (action==0)
                            {
                              move_x=0;
                              move_y=-1;
                              perso_moves=true;
                            }
                            break;
                        case SDLK_RIGHT:
                            if (action==0)
                            {
                              move_x=+1;
                              move_y=0;
                              perso_moves=true;
                            }
                            break;
                        case SDLK_LEFT:
                            if (action==0)
                            {
                              move_x=-1;
                              move_y=0;
                              perso_moves=true;
                            }
                            break;
                        case SDLK_d:
                            if (action==0)
                            {
                              move_x=-pseudo_normalize(draw->vect_vox_x_c.x,graph->ORTHO_ZOOM);
                              move_y=-pseudo_normalize(draw->vect_vox_y_c.x,graph->ORTHO_ZOOM);
                              perso_moves=true;
                            }
                            break;
                        case SDLK_g:
                            if (action==0)
                            {
                              move_x=+pseudo_normalize(draw->vect_vox_x_c.x,graph->ORTHO_ZOOM);
                              move_y=+pseudo_normalize(draw->vect_vox_y_c.x,graph->ORTHO_ZOOM);
                              perso_moves=true;
                            }
                            break;
                        case SDLK_r:
                            if (action==0)
                            {
                              move_x=-pseudo_normalize(draw->vect_vox_x_c.z,graph->ORTHO_ZOOM);
                              move_y=-pseudo_normalize(draw->vect_vox_y_c.z,graph->ORTHO_ZOOM);
                              perso_moves=true;
                            }
                            break;
                        case SDLK_f:
                            if (action==0)
                            {
                              move_x=+pseudo_normalize(draw->vect_vox_x_c.z,graph->ORTHO_ZOOM);
                              move_y=+pseudo_normalize(draw->vect_vox_y_c.z,graph->ORTHO_ZOOM);
                              perso_moves=true;
                            }
                            break;
                        case SDLK_t:
                            if (action==0)
                            {
                              move_x=-pseudo_normalize(draw->vect_vox_x_c.y,graph->ORTHO_ZOOM);
                              move_y=-pseudo_normalize(draw->vect_vox_y_c.y,graph->ORTHO_ZOOM);
                              perso_moves=true;
                            }
                            break;
                        case SDLK_e:
                            if (action==0)
                            {
                              move_x=+pseudo_normalize(draw->vect_vox_x_c.y,graph->ORTHO_ZOOM);
                              move_y=+pseudo_normalize(draw->vect_vox_y_c.y,graph->ORTHO_ZOOM);
                              perso_moves=true;
                            }
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
        
        if (perso_moves)
        {
            anim_index+=1.0;
            pos.x+=move_x;
            pos.y+=move_y;
            if (fabs(move_x)>fabs(move_y))
            {
                if (move_x>0) direction=EAST;
                else direction=WEST;
            }else{
                if (move_y>0) direction=NORTH;
                else direction=SOUTH;
            }
        }
        if (pos.x<perso.get_xsiz()/2) pos.x=perso.get_xsiz()/2;
        if (pos.x>ground.get_xsiz()-perso.get_xsiz()/2) pos.x=ground.get_xsiz()-perso.get_xsiz()/2;
        if (pos.y<perso.get_ysiz()/2) pos.y=perso.get_ysiz()/2;
        if (pos.y>ground.get_ysiz()-perso.get_ysiz()/2) pos.y=ground.get_ysiz()-perso.get_ysiz()/2;
        
        for (int i=0;i<1;i++)
        {
            Pt3d snow(rand()%scene.xsiz,rand()%scene.ysiz,0);
            Pt3d snow_speed(0,0,1);
            //if (int(t*20)%10==0)
#ifdef VOX_24BIT
                Particle_Manager::the_one->add_particle(200+rand()%50,snow,snow_speed,0);
#else
                Particle_Manager::the_one->add_particle(28+rand()%4,snow,snow_speed,0);
#endif
        }

        Snowball::update_snowballs();

        Particle_Manager::the_one->update();
        ground.sub(palet,true,SNOW_START,SNOW_END);
        palet.set_pos(ball);

        perso.set_pos(pos);
        
        //----------------------
        
        if (action==2)
        {
            std::cout<<"Throw: "<<anim_index<<std::endl;
            perso.set_img(throw_anim[direction].at((int)anim_index));
            anim_index+=0.3;
            if ((anim_index>1)&&(!ball_thrown))
            {
                Pt3d v(0,0,-0.5);
                switch (direction)
                {
                    case 0:
                        v.x=3;
                        break;
                    case 1:
                        v.y=-3;
                        break;
                    case 2:
                        v.x=-3;
                        break;
                    case 3:
                        v.y=3;
                        break;
                    default:
                        break;
                }
                Pt3d p(pos);
                p.z-=12;
                if (nbr_snow_blocks>=50)
                {
                    nbr_snow_blocks-=50;
                    new Snowball(p,v);
                }
                ball_thrown=true;
            }
            if (anim_index>=throw_anim[direction].size()) {anim_index=0;action=0;}
        }
        if (action==0)
        {
            if (anim_index>=walk_anim[direction].size()) anim_index=0;
            if (anim_index<0) anim_index=walk_anim[direction].size()-0.01;
            perso.set_img(walk_anim[direction].at((int)anim_index));
        }
        if (action==1)
        {
            subber.set_pos(pos);
            subber.m_pos.z+=1;//lower than player
            nbr_snow_blocks+=ground.sub(subber,false,SNOW_START,SNOW_END);
            std::cout<<nbr_snow_blocks<<" snow blocks\n";
            perso.set_img(pick_img[direction]);
            action=0;
        }

        scene.reinit(ground);//have to reinit scene AFTER modifications (add, sub, particle update), and JUST before blit!
        Particle_Manager::the_one->blit_all();
        Snowball::blit_all(&scene);

        //scene.blit(subber);
        scene.blit(perso);
        scene.blit(palet);
        //scene.blit(&test,&pos);
        
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
        else
            if (ground.collide(perso))
            {
                pos.z-=1;
                //if (ground.collide(perso)) {pos.copy(previous_pos);pos.z-=1;}
            }
            
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

