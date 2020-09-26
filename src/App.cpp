#include "App.hpp"

void App::start(const std::string& dir, int sz_min, int sz_max, 
                                  		int dl_min, int dl_max)
{
    if (!glfwInit())
    {
        fprintf(stderr, "Failed to initialize GLFW.\n");
        getchar();
        return;
    }

    if (!Window::create(1000, 1000, "Terrain"))
    {
    	fprintf(stderr, "Failed to create window.\n");
    	getchar();
    	glfwTerminate();
    	return;
    }

    glewExperimental = true;
    if (glewInit() != GLEW_OK)
    {
    	fprintf(stderr, "Failed to initialize GLEW.\n");
    	getchar();
    	glfwTerminate();
    	return;
    }

    glfwSetInputMode(Window::get(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_CULL_FACE);

    view.init(dir, sz_min, sz_max, dl_min, dl_max);
    run();
}

void App::run()
{
	printHelp();
	fps.start();
	
    while (!Window::shouldClose())
    {
    	handleInput();
        draw();
        printStats();

        if (auto_lod && updated && fps.getFrameRate() < 10)
        {
        	updated = false;
       		view.changeLOD(view.getLOD() + 1);
        }

        glfwPollEvents();
    }

    view.terminate();
    glfwTerminate();
}

void App::draw()
{
	Window::clear();
    view.draw();
	Window::display();
}

void App::handleInput()
{
	bool tmp = auto_lod;
	
	static double last_time = glfwGetTime();
	double now = glfwGetTime();
	float delta_time = now - last_time;

	if (Window::pressed(GLFW_KEY_G))
		view.switchToMode(View::Mode::GLOB);
	if (Window::pressed(GLFW_KEY_M))
		view.switchToMode(View::Mode::MAP);

	if (Window::pressed(GLFW_KEY_1))
	{
		auto_lod = false;

		if (view.getLOD() >= 9)
		{
			if (delta_time >= 0.3f)	
			{
				view.changeLOD(view.getLOD() + 1);
				last_time = now;
			}
		}
		else	
		{
			view.changeLOD(9);
			last_time = now;
		}
	}
	if (Window::pressed(GLFW_KEY_2))
	{
		auto_lod = false;
		view.changeLOD(8);
	}
	if (Window::pressed(GLFW_KEY_3))
	{
		auto_lod = false;
		view.changeLOD(7);
	}
	if (Window::pressed(GLFW_KEY_4))
	{
		auto_lod = false;
		view.changeLOD(6);
	}
	if (Window::pressed(GLFW_KEY_5))
	{
		auto_lod = false;
		view.changeLOD(5);
	}
	if (Window::pressed(GLFW_KEY_6))
	{
		auto_lod = false;
		view.changeLOD(4);
	}
	if (Window::pressed(GLFW_KEY_7))
	{
		auto_lod = false;
		view.changeLOD(3);
	}
	if (Window::pressed(GLFW_KEY_8))
	{
		auto_lod = false;
		view.changeLOD(2);
	}
	if (Window::pressed(GLFW_KEY_9))
	{
		auto_lod = false;
		view.changeLOD(1);
	}
	if (Window::pressed(GLFW_KEY_0))
		auto_lod = true;

	if (auto_lod && !tmp)
		std::cout << "\nAutomatic level of detail enabled\n\n";
	else if (!auto_lod && tmp)
		std::cout << "\nAutomatic level of detail disabled\n\n";

	view.handleControls();
}

void App::printStats()
{
	if (fps.update())
	{
		size_t triangles = view.getTriangleCount();
		size_t vertices = view.getVertexCount();
		int frame_rate = fps.getFrameRate();
		std::cout << triangles << " triangles drawn, " << vertices << " vertices drawn, " 
				  << "FPS: " << frame_rate << "\n";
		updated = true;
	}
}

void App::printHelp()
{
	std::cout << "\nNAVIGATION\n\n";
	std::cout << "1, 2, ... , 9 - choose Level of Detail (the lower the less detailed the terrain is)\n";
	std::cout << "If Level of Detail is less or equal to 1, pressing 1 button\n"
			   "will cause further decrease in Level of Detail (till the minimal available level)\n";
	std::cout << "0 - automatic Level of Detail mode\n";
	std::cout << "M - map mode\n";
	std::cout << "G - globe/3D mode\n\n";
	std::cout << "Map mode navigation:\n";
	std::cout << "WASD/arrows - moving the camera\n";
	std::cout << "Z - zoom in\n";
	std::cout << "X - zoom out\n\n";
	std::cout << "Globe/3D mode:\n\n";
	std::cout << "WASD/arrows - move horizontally\n";
	std::cout << "Space - move up\n";
	std::cout << "Left Alt - move down\n";
	std::cout << "U - increase vertical velocity\n";
	std::cout << "J - decrease vertical velocity\n";
	std::cout << "Mouse - look around\n";
	std::cout << "Mouse scroll - increase/decrease horizontal velocity\n\n";
	// std::cout << "\nINSTRUKCJA\n\n";
	// std::cout << "1, 2,..., 9 - wybor Level of Detail (im nizszy tym dokladniejeszy teren)\n";
	// std::cout << "Jesli Level of Detail jest wiekszy badz rowny od 9, nacisniecie przyckisku 9,\n"
				 // "spowoduje dalsze zwiekszenie poziomu Level of Detail (az do maksymalnego dostepnego poziomu)\n";
	// std::cout << "0 - tryb automatyczny wyboru Level of Detail\n";
	// std::cout << "M - tryb mapy\n";
	// std::cout << "G - tryb 3D\n\n";
	// std::cout << "Tryb mapy:\n";
	// std::cout << "WASD / strzalki - poruszanie sie\n";
	// std::cout << "Z - przybliz widok\n";
	// std::cout << "X - oddal widok\n\n";
	// std::cout << "Tryb 3D:\n";
	// std::cout << "WASD / strzalki - poruszanie sie w poziomie\n";
	// std::cout << "Spacja - zwiekszenie wysokosci lotu\n";
	// std::cout << "Lewy Alt - zmniejszenie wysokosci lotu\n";
	// std::cout << "U - zwiekszenie predkosci w pionie\n";
	// std::cout << "J - zmniejszenie predkosci w pionie\n";
	// std::cout << "Obracanie kamery za pomoca myszki\n";
	// std::cout << "Uzyj scroll'a zeby zmniejszyc/zwiekszyc predkosc poruszania\n\n";
}
