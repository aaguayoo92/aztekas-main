#include<stdio.h>
#include<math.h>
#include"../Headers/vector.h"
#include"../Headers/main.h"

int funct_Dm(double *a, double *uu)
{
   int i;
   double n, p, u=0, v=0, w=0;
   double R, V;
   double yt11, Bt1, Vt1, c;
   n = uu[0];
   p = uu[1];
   u = uu[2];
   if(dim >= 2){v = uu[3];}
   if(dim == 3){w = uu[4];}

   R    = x1;
   V    = (pow(w,2.0)+pow(sin(x2),2.0)*pow(v,2.0)+(pow(x1,2.0)-2*MM*x1)*pow(sin(x2),2.0)*pow(u,2.0))/(pow(x1,2.0)*pow(sin(x2),2.0));
   yt11 = (x1-2*MM)/x1;
   Bt1  = 0;
   Vt1  = ((x1-2*MM)*u)/x1;
   c    = sqrt(((pow(K,2.0)-K)*p)/(K*p+(K-1)*n));

   a[0] = -((c)*sqrt(x1-2*MM)*sqrt(((pow(V,2.0)-V)*pow(c,2.0)-V+1)*yt11+(1-V)*pow(Vt1,2.0)*pow(c,2.0)+(V-1)*pow(Vt1,2.0))+(Vt1-Vt1*pow(c,2.0))*sqrt(x1-2*MM)+(Bt1*V*pow(c,2.0)-Bt1)*sqrt(x1))/((V*pow(c,2.0)-1)*sqrt(x1));
   a[1] = ((c)*sqrt(x1-2*MM)*sqrt(((pow(V,2.0)-V)*pow(c,2.0)-V+1)*yt11+(1-V)*pow(Vt1,2.0)*pow(c,2.0)+(V-1)*pow(Vt1,2.0))+(Vt1*pow(c,2.0)-Vt1)*sqrt(x1-2*MM)+(Bt1-Bt1*V*pow(c,2.0))*sqrt(x1))/((V*pow(c,2.0)-1)*sqrt(x1));
   a[2] = (Vt1*sqrt(x1-2*MM)-Bt1*sqrt(x1))/sqrt(x1);

   return 0;
}
