# 2D Ising Model Simulation

A real-time 2D Ising Model simulation built with C++ and SFML 3. This project uses the Metropolis Monte Carlo algorithm to simulate simple 2D square grid and compute the system's thermal heat capacity ($C_V$) and average energy.


## How to Run the App (Choose One)

If you just want to see the simulation run without downloading tools or compiling code:
1. Navigate to the **Releases** tab on the right side of this repository page.
2. Download the `Ising_2D_sim_V1.0_shah.zip` archive.
3. Extract the contents completely.
4. Double-click `IsingApp.exe` to launch.

If this doesn't work then you can simply download the whole folder from github and extract the executable(It also has all the .dll files to go along with it).

## Update 1 :

I have added a simple testing tool which plots the Ising sim results over multiple temps and exports it out into a .csv file which can be used anywhere.
It saves the Heat Capacity and Average Energy. It is a headless file (Without GUI) named multi_temp_Ising.cpp. Hope this is nice. <3