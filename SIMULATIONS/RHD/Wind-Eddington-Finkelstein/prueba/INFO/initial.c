/*
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

//Do not erase any of these libraries//
#include"main.h"

void Initial()
{
   int n, i, j, k, cell;

   //Initialize grid.time
   grid.time = 0.0;

   //Initialize dt
   dt = 0.0;

   gauge_ local_grid;
   velocity_0 = vinf;

#if DIM == 2

   ////////////////////////////
   //----------Wind----------//
   ////////////////////////////

   for(i = 0; i <= Nx1; i++)
   {
      for(j = 0; j <= Nx2; j++)
      {
         local_grid.x[1] = grid.X1[i];
         local_grid.x[2] = grid.X2[j];

         Get_Metric_Components(&local_grid);

         U(0,i,j) =  density_0;
         U(1,i,j) =  (K - 1.0)*U(0,i,j)*pow(velocity_0/Mach,2.0)/(K*(K - 1.0) - K*pow(velocity_0/Mach,2.0));
         U(2,i,j) =  velocity_0*cos(grid.X2[j])/sqrt(local_grid.gamma_con[0][0]);
         U(3,i,j) = -velocity_0*sin(grid.X2[j])/sqrt(local_grid.gamma_con[1][1]);
      }
   }

#elif DIM == 4

   ////////////////////////////
   //----------Wind----------//
   ////////////////////////////

   for(i = 0; i <= Nx1; i++)
   {
      for(j = 0; j <= Nx2; j++)
      {
         local_grid.x[1] = grid.X1[i];
         local_grid.x[2] = grid.X2[j];

         Get_Metric_Components(&local_grid);

         U(0,i,j) =  density_0;
         U(1,i,j) =  (K - 1.0)*U(0,i,j)*pow(velocity_0/Mach,2.0)/(K*(K - 1.0) - K*pow(velocity_0/Mach,2.0));
         U(2,i,j) =  velocity_0*cos(grid.X2[j])/sqrt(local_grid.gamma_con[0][0]);
         U(3,i,j) = -velocity_0*sin(grid.X2[j])/sqrt(local_grid.gamma_con[1][1]);
         U(4,i,j) = 0.0;
      }
   }

#endif
}
