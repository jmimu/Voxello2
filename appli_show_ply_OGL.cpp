#include <iostream>
#include <math.h>
#include <vector>
#include <SDL/SDL.h>


#include "vox_scene.h"
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
    
    VoxImg test("data/perso_stand.vox",0,VOX_FILE);
    
    std::vector<std::string> ply_filenames;
    ply_filenames.push_back("data/boudha1.ply");
    ply_filenames.push_back("data/boudha2.ply");
    ply_filenames.push_back("data/boudha3.ply");
    
    test.load_from_ply(ply_filenames,200);
    //test.load_from_ply("data/NuageImProf_LeChantier_Etape_5.ply",200,200,100);
    //test.load_from_ply("data/boudha.ply",100,100,60);
    Vox_Scene scene(test);

    Draw_Vox_Ortho *draw=new Draw_Vox_Ortho(graph,&scene);//maybe create a generic class for otho or perspective
    

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

    bool run=true;
    double t=0.0;
    while (run)
    {
        fps++;
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
        


        

        scene.reinit(test);//have to reinit scene AFTER modifications (add, sub, particle update), and JUST before blit!



        
        /*angleZ += 0.00020 * ellapsed_time;
        angleY += 0.00023 * ellapsed_time;
        angleX += 0.00027 * ellapsed_time;*/
        t+=0.05;


        //drawing
        draw->update_camera(100,100,0,angleX,angleY,angleZ);
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

    //TODO: delete images ?

    return 0;
}

