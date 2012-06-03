#include <iostream>
#include <math.h>
#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>


#include "vox_scene.h"
#include "graph_SFML.h"
#include "draw_vox_ortho.h"


int main(int argc, char *argv[])
{
    (void) argc;
    (void) argv;
    
    //SFML is used for inputs and time
    
    
    Graph *graph=new Graph_SFML();
    graph->init();
    
    VoxImg test("data/perso_stand.vox",0,VOX_FILE);
    test.load_from_ply("data/NuageImProf_LeChantier_Etape_5.ply",150,110,40);
    Vox_Scene scene(150,110,40,test);

    Draw_Vox_Ortho *draw=new Draw_Vox_Ortho(graph,&scene);//maybe create a generic class for otho or perspective
    

    //----- events -----
    sf::Event event;

    //----- timing -----
    sf::Clock clock;
    unsigned long last_time = clock.getElapsedTime().asMilliseconds();
    unsigned long current_time,ellapsed_time,start_time;
    unsigned long previous_fps_time=clock.getElapsedTime().asMilliseconds()/1000;
    int fps=0;


    double angleZ = 0;
    double angleY = 0;
    double angleX = 0;
    





    bool run=true;
    double t=0.0;
    while (((Graph_SFML*)graph)->getWindow()->isOpen() && run)
    {
        fps++;
        while (((Graph_SFML*)graph)->getWindow()->pollEvent(event))
        {
            // Window closed or escape key pressed : exit
            if ((event.type == sf::Event::Closed) || 
               ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape)))
            {
                ((Graph_SFML*)graph)->getWindow()->close();
                break;
            }
            if (event.type == sf::Event::MouseMoved)
            {
                angleZ = event.mouseMove.x*0.01;
                angleX = 3.14+event.mouseMove.y*0.01;
            }
            if (event.type == sf::Event::MouseWheelMoved)
            {
                graph->ORTHO_ZOOM+=event.mouseWheel.delta/10.0;
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
        current_time = clock.getElapsedTime().asMilliseconds();
        ellapsed_time = current_time - last_time;
        last_time = current_time;
        ellapsed_time =clock.getElapsedTime().asMilliseconds() - start_time;
        //----- end timing -----
        
    }

    //TODO: delete images ?

    return 0;
}

