
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <iostream>

//
#include "man/St_Manager/St_Manager.hpp"

//TIMER
#include "man/utils/Timer.hpp"

template<typename T>
void seetype(T){std::cout<< __PRETTY_FUNCTION__ << "\n"; }

int main(){

	//Control de los FPS del juego (veces que se actualizan los sistemas)
	/*
				  1/fps
	*/

	
	//LOD IA
	uint8_t iatime = 1/20;
	bool iaupdate  = false;

	uint8_t fps    = 1/30;
	teo_time::Timer timer;

	GLFWwindow* window;
	window = startEngine();

	St_Manager state_manager(window, fps, fps);
	state_manager.addState(state_id::PREINTRO, false,window);
	
	while(!glfwWindowShouldClose(window)){
		glfwPollEvents();

		if(timer.getElapsedTime().count() >= fps){
			if(timer.getElapsedTime().count() > iatime)
				iaupdate = true;
			
			timer.setTimer();
			state_manager.ProcessStateChanges();
			state_manager.UpdateEngine(window, iaupdate);
	
			timer.getElapsedTime();
			
		}

		glfwSwapBuffers(window);

	}
	
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
