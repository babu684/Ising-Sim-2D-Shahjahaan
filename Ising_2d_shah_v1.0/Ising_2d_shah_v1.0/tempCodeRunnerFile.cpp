int main()
{
    std::mt19937 gen(static_cast<unsigned int>(time(nullptr)));
    std::cout << "Enter the Grid Size, Temperature (in epsillon/k), and Total Iterations: ";
    std::cin >> grid_size >> temp >> iterations;
    iterations_per_frame = ceil((float)iterations / 300.0f);
    std::vector<std::vector<int>> grid(grid_size, std::vector<int>(grid_size, 0));
    GRID__init(grid, gen);
    
    float energy = CALC__ENERGY(grid);
    const int window_dim = 800;
    sf::RenderWindow window(sf::VideoMode(window_dim, window_dim), "Ising Model Simulation");
    window.setFramerateLimit(60);

    sf::Texture texture;
    texture.create(grid_size, grid_size);
    
    sf::Sprite sprite(texture);
    float scale = (float)window_dim / (float)grid_size;
    sprite.setScale(scale, scale);

    std::vector<sf::Uint8> pixel_buffer(grid_size * grid_size * 4);

    while (window.isOpen())
    {
        sf::Event event;
        while(window.pollEvent(event))
        {
            if(event.type == sf::Event::Closed)
                window.close();
        }

        if(current_iterations < iterations) 
        {
            for(int i = 0; i < iterations_per_frame; i++) 
            {
                energy += FLIPRAND(grid, gen);
                current_iterations++;
            }
        }

        for (int i = 0; i < grid_size; i++)
        {
            for (int j = 0; j < grid_size; j++)
            {
                int pixel_index = (i * grid_size + j) * 4;
                
                if(grid[i][j] == 1)
                {
                    pixel_buffer[pixel_index]     = 255; // Red
                    pixel_buffer[pixel_index + 1] = 0; // Green
                    pixel_buffer[pixel_index + 2] = 0; // Blue
                    pixel_buffer[pixel_index + 3] = 255;
                }

                else
                {
                    pixel_buffer[pixel_index]     = 0; // Red
                    pixel_buffer[pixel_index + 1] = 0; // Green
                    pixel_buffer[pixel_index + 2] = 255; // Blue
                    pixel_buffer[pixel_index + 3] = 255;
                }
                
            }
        }

        window.clear();
        texture.update(pixel_buffer.data());
        window.draw(sprite);                
        window.display();
    }

    return 0;
}