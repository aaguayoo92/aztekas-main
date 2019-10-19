#include"main.h"

#if DIM == 1

void Runge_Kutta(int order)
{
   int I[3];
   double Dt = dt;
   double Dx1 = dx1;
   double Q[eq+1], F[eq+1], L[eq+1];
   gauge_ local_grid;
   lim_ limiter;
   vec_ vectors;


      for(int i = gc; i <= Nx1-gc; i++)
      {
         local_grid.x[0] = grid.time;
         local_grid.x[1] = grid.X1[i];
         local_grid.x[2] = 0.0;
         local_grid.x[3] = 0.0;
         #if COORDINATES == SPHERICAL
         local_grid.x[2] = M_PI_2;
         #endif

         I[0] = i;
    
         Dx1 = grid.X1p[i] - grid.X1m[i];

         Reconst1D(aztekas_Vec.U,&limiter,I);

         Prim2Cons(vectors.Q,limiter.ux,local_grid);
         Flux1D(&vectors,&limiter,I);
         Sources(limiter.ux,&vectors,I);
    
         for(int n = 0; n < eq; n++)
         {
            Q[n] = vectors.Q[n];
            F[n] = (S1p(i)*vectors.Fp[n] - S1m(i)*vectors.Fm[n])/(Dx1) - \
            vectors.S[n];
         }

#if INTEGRATION == PVRS //PVRS
         for(int n = 0; n < eq; n++)
         {
            L[n] = F[n];
         }

         MxV(v.A,L,F);
#endif

         switch(order)
         {
            case 1:
               for(int n = 0; n < eq; n++)
               {
                  Q[n] = Q[n] - (Dt)*(F[n]);
                  Q1(n,i)  = Q[n];
               }
            break;

            case 2:
               for(int n = 0; n < eq; n++)
               {
                  Q[n] = 0.5*(Q[n] + Q1(n,i) - (Dt)*F[n]);
                  Q2(n,i)  = Q[n];
               }
            break;
         }

         Cons2Prim(vectors.U,Q,local_grid);
         for(int n = 0; n < eq; n++)
         {
            U(n,i) = vectors.U[n];   
         }
      }
}

#endif

//#if DIM == 1
//
//int RK1D(double *u, double *q, double *q1, double *q2, int order)
//{
//   char r;
//   int I[3];
//   double Dx1 = dx1;
//   double Dt  = dt;
//   double L[eq+1], F[eq+1], Prim[eq+1];
//   double g[6], g1p[6], g1m[6];
//   gauge_ local_grid;
//   vec_ v;
//   lim_ l;
//
//   #pragma omp parallel shared(U,grid,Dt) if (OMP_NUM > 1)
//   {
//      #pragma omp for private(I,l,v,F,L,Prim,Dx1,local_grid) 
//      for(int i = gc; i <= Nx1-gc; i++)
//      {
//         local_grid.x[0] = grid.time;
//         local_grid.x[1] = grid.X1[i];
//         local_grid.x[2] = 0.0;
//         local_grid.x[3] = 0.0;
//         #if COORDINATES == SPHERICAL
//         local_grid.x[2] = M_PI_2;
//         #endif
//
//         I[0] = i;
//    
//         Dx1 = grid.X1p[i] - grid.X1m[i];
//
//         Reconst1D(u,&l,I);
//         Flux1D(&v,&l,I);
//         Sources(l.ux,&v,I);
//    
//         for(int n = 0; n < eq; n++)
//         {
//            F[n] = (S1p(i)*v.Fp[n] - S1m(i)*v.Fm[n])/(Dx1) - \
//            v.S[n];
//         }
//
//#if INTEGRATION == PVRS //PVRS
//         for(int n = 0; n < eq; n++)
//         {
//            L[n] = F[n];
//         }
//
//         MxV(v.A,L,F);
//#endif
//
//         switch(order)
//         {
//            case 1:
//               for(int n = 0; n < eq; n++)
//               {
//                  q1(n,i) = q(n,i) - (Dt)*(F[n]);
//               }
//            break;
//
//            case 2:
//               for(int n = 0; n < eq; n++)
//               {
//                  q2(n,i) = 0.5*(q1(n,i) + q(n,i) - (Dt)*F[n]);
//               }
//            break;
//         }
//      }
//   }
//
//   return 0;
//}
//
//#elif DIM == 2 || DIM == 4
//
//int RK2D(double *u, double *q, double *q1, double *q2, int order)
//{
//   char r;
//   int I[3];
//   double Dx1 = dx1;
//   double Dx2 = dx2;
//   double Dt  = dt;
//   double L[eq+1], F[eq+1];
//   double g[6], g1p[6], g1m[6];
//   double g2p[6], g2m[6];
//   vec_ v;
//   lim_ l;
//
//   #pragma omp parallel shared(U,grid,Dt) if (OMP_NUM > 1)
//   {
//      #pragma omp for private(I,l,v,F,L,Dx1,Dx2) collapse(2)
//      for(int j = gc; j <= Nx2-gc; j++)
//      {
//         for(int i = gc; i <= Nx1-gc; i++)
//         {
//            I[0] = i;
//            I[1] = j;
//
//            Dx1 = grid.X1p[i] - grid.X1m[i];
//            Dx2 = grid.X2p[j] - grid.X2m[j];
//
//            Reconst2D(u,&l,I);
//            Sources(l.ux,&v,I);
//            Flux2D(&v,&l,I);
//
//            for(int n = 0; n < eq; n++)
//            {
//               F[n] = (S1p(i,j)*v.Fp[n] - S1m(i,j)*v.Fm[n])/(Dx1) + \
//                      (S2p(i,j)*v.Gp[n] - S2m(i,j)*v.Gm[n])/(Dx2) - \
//                      v.S[n];
//            }
//
//#if INTEGRATION == PVRS //PVRS
//            for(int n = 0; n < eq; n++)
//            {
//               L[n] = F[n];
//            }
//
//            MxV(v.A,L,F);
//#endif
//
//            switch(order)
//            {
//               case 1:
//                  for(int n = 0; n < eq; n++)
//                  {
//                     q1(n,i,j) = q(n,i,j) - (Dt)*(F[n]);
//                  }
//               break;
//
//               case 2:
//                  for(int n = 0; n < eq; n++)
//                  {
//                     q2(n,i,j) = 0.5*(q1(n,i,j) + q(n,i,j) - (Dt)*F[n]);
//                  }
//               break;
//            }
//         }
//      }
//   }
//
//   return 0;
//}
//
//#elif DIM == 3
//
//int RK3D(double *u, double *q, double *q1, double *q2, int order)
//{
//   char r;
//   int n, i, j, k;
//   int I[3];
//   double Dx1 = dx1;
//   double Dx2 = dx2;
//   double Dx3 = dx3;
//   double Dt  = dt;
//   double g[6], g1p[6], g1m[6];
//   double g2p[6], g2m[6];
//   double g3p[6], g3m[6];
//   double L[eq+1], F[eq+1];
//   vec_ v;
//   lim_ l;
//
//   for(i = gc; i <= Nx1-gc; i++)
//   {
//      for(j = gc; j <= Nx2-gc; j++)
//      {
//         for(k = gc; k <= Nx3-gc; k++)
//         {
//            I[0] = i;
//            I[1] = j;
//            I[2] = k;
//
//            Dx1 = grid.X1p[i] - grid.X1m[i];
//            Dx2 = grid.X2p[j] - grid.X2m[j];
//            Dx3 = grid.X2p[k] - grid.X2m[k];
//
//            Reconst3D(u,&l,I);
//            Flux3D(&v,&l,I);
//
//            for(n = 0; n < eq; n++)
//            {
//               F[n] = (S1p(i,j,k)*v.Fp[n] - S1m(i,j,k)*v.Fm[n])/(Dx1) + \
//                      (S2p(i,j,k)*v.Gp[n] - S2m(i,j,k)*v.Gm[n])/(Dx2) - \
//                      (S3p(i,j,k)*v.Hp[n] - S3m(i,j,k)*v.Hm[n])/(Dx3) - \
//                      v.S[n];
//            }
//
//            switch(order)
//            {
//               case 1:
//                  for(n = 0; n < eq; n++)
//                  {
//                     q1(n,i,j,k) = q(n,i,j,k) - (Dt)*L[n];
//                  }
//               break;
//
//               case 2:
//                  for(n = 0; n < eq; n++)
//                  {
//                     q2(n,i,j,k) = 0.5*(q1(n,i,j,k) + \
//                     q(n,i,j,k) - (Dt)*L[n]);
//                  }
//               break;
//            }
//         }
//      }
//   }
//
//   return 0;
//}
//
//#endif
