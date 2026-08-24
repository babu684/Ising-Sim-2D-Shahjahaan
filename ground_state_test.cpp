#include <iostream>
#include <vector>
#include <random>
#include <ctime>
#include <cmath> 
#include <cstdint>   
#include <optional>  
#include <fstream>    

int grid_size; 
float temp;
float target_temp;
float temp_step;
int iterations;
int iterations_per_frame;
int current_iterations = 0;

double energy_sum = 0.0;
double energy_sq_sum = 0.0;
long long sample_count = 0;
bool results_printed = false;

void GRID__init(std::vector<std::vector<int>>& grid, std::mt19937& gen)
{
    std::uniform_int_distribution<int> int_dist(0,1);
    for(int i = 0; i < grid_size; i++)
    {
        for(int j = 0; j < grid_size; j++)
        {
            grid[i][j] = int_dist(gen) ? 1 : -1;
        }
    }
}

float CALC__ENERGY(const std::vector<std::vector<int>>& grid)
{
    float total_energy = 0;
    for(int i = 0; i < grid_size; i++)
    {
        for(int j = 0; j < grid_size; j++)
        {
            int right = (j == grid_size - 1) ? grid[i][0] : grid[i][j + 1];
            int down = (i == grid_size - 1) ? grid[0][j] : grid[i + 1][j];
            total_energy -= grid[i][j] * (right + down); 
        }
    }
    return total_energy;
}

float DELTA_U(int i, int j, const std::vector<std::vector<int>>& grid)
{
    int up, down, left, right;
    if(i == 0)  up = grid[grid_size - 1][j];     else up = grid[i - 1][j];
    if(i == grid_size - 1)  down = grid[0][j];   else down = grid[i + 1][j];
    if(j == 0)  left = grid[i][grid_size - 1];   else left = grid[i][j - 1];
    if(j == grid_size - 1)  right = grid[i][0];  else right = grid[i][j + 1];

    return 2.0f * grid[i][j] * (up + down + left + right);
}

float FLIPRAND(std::vector<std::vector<int>>& grid, std::mt19937& gen)
{
    int i_rand = gen() % grid_size;
    int j_rand = gen() % grid_size;
    
    float delta_U = DELTA_U(i_rand, j_rand, grid);
    float exponent = delta_U / temp;
    
    float random = static_cast<float>(gen()) / static_cast<float>(gen.max());

    if(delta_U <= 0)
    {
        grid[i_rand][j_rand] = -grid[i_rand][j_rand];
        return delta_U;
    }
    else
    {
        if(random < std::exp(-exponent)) 
        {
            grid[i_rand][j_rand] = -grid[i_rand][j_rand];
            return delta_U;
        }  
    }
    return 0;
}

int main()
{
    std::mt19937 gen(static_cast<unsigned int>(time(nullptr)));
    std::cout << "Enter the Grid Size, Temperature (in epsilon/k), and Total sweeps, Target Temperature and Temperature step: ";
    std::cin >> grid_size >> temp >> iterations >> target_temp >> temp_step;
    if(grid_size <= 0 || temp <= 0 || iterations <= 0)
    {
        std::cout << "Bruh input valid values, all 3 inputs cannot be less than or equal to 0.";
        return 1;
    }
    iterations_per_frame = ceil((float)iterations / 300.0f);
    float cool_down_iterations = iterations * grid_size * grid_size * 0.20f;
    std::vector<std::vector<int>> grid(grid_size, std::vector<int>(grid_size, 0));
    GRID__init(grid, gen);
    
    float energy = CALC__ENERGY(grid);
    const int window_dim = 800;

    // 2. Open the CSV file and write the column headers
    std::ofstream csv_file("ising_data.csv");
    csv_file << "Temperature,Avg_Energy,Heat_Capacity\n";
    
    while (temp < target_temp)
    {
        results_printed = false;
        energy_sum = 0;
        energy_sq_sum = 0;
        current_iterations = 0;

        while (!results_printed)
        {
            if(current_iterations < (iterations * grid_size * grid_size)) 
            {
                    energy += FLIPRAND(grid, gen);
                    if(current_iterations > cool_down_iterations)
                    {
                        energy_sum += energy;
                        energy_sq_sum += static_cast<double>(energy) * static_cast<double>(energy);
                    }
                    current_iterations++;
            }
            else if (!results_printed)
            {
                double valid_samples = (iterations * grid_size * grid_size) - cool_down_iterations;

                double avg_E = energy_sum / valid_samples;
                double avg_E_sq = energy_sq_sum / valid_samples;
                double variance_E = avg_E_sq - (avg_E * avg_E);
                double heat_capacity = variance_E / (static_cast<double>(temp) * static_cast<double>(temp));

                std::cout << "\n====================================\n";
                std::cout << "        SIMULATION RESULTS          \n";
                std::cout << "====================================\n";
                std::cout << "Temperature (T):                " << temp << "\n"; // Added T for clarity
                std::cout << "Average Equilibrium Energy <E>: " << avg_E << "\n";   
                std::cout << "Thermal Heat Capacity (Cv):    " << heat_capacity << "\n";
                std::cout << "====================================\n";
                results_printed = true;
                
                csv_file << temp << "," << avg_E << "," << heat_capacity << "\n";
            }
        }
        temp += temp_step;
    }
    
    csv_file.close();
    std::cout << "\nData successfully saved to 'ising_data.csv'!\n";
    
    return 0;
}