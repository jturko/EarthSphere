
#include <iostream>
#include <cmath>
#include <sstream>
#include <string>
#include <fstream>

double func(double * xx, double r, double R)
{   
    // x integral
    double val = 2.*M_PI*r*r*atan(sqrt(R*R-xx[0]*xx[0]-xx[1]*xx[1])/sqrt(xx[0]*xx[0]-2.*xx[0]*R+R*R+xx[1]*xx[1]))/sqrt(xx[0]*xx[0]-2.*xx[0]*R+R*R+xx[1]*xx[1]);
    
    // z integral
    //double val = M_PI*r*r*(atan((sqrt(R*R-xx[0]*xx[0]-xx[1]*xx[1])+R)/sqrt(xx[0]*xx[0]))+atan((sqrt(R*R-xx[0]*xx[0]-xx[1]*xx[1])-R)/sqrt(xx[0]*xx[0])))/sqrt(xx[0]*xx[0]+xx[1]*xx[1]);
    
    //std::cout << " func return = " << val << std::endl;
    
    if(!std::isnan(val)) return val;
    else return 0;
}

double integrate(double r, double R, double step, bool faster=false, bool verbose=true)
{
    double xlo = -R;
    //double xlo = 0.;
    double xhi = +R;
    double ylo, yhi;
    
    if(verbose) {
        std::cout << " Earth radius = " << R << std::endl;
        std::cout << " Detector radius = " << r << std::endl;
        std::cout << " Step size = " << step << std::endl;
        std::cout << " Faster (?) = " << faster << std::endl;
    }

    double integral = 0.;
    
    for(double xiter=xlo; xiter<=xhi; xiter+=step) {
        if(faster) { 
            if(xiter<0) continue;
        }
        //ylo = -sqrt(R*R-xiter*xiter);
        ylo = 0.;
        yhi = +sqrt(R*R-xiter*xiter);
        for(double yiter=ylo; yiter<=yhi; yiter+=step) {
            double point[2] = { xiter, yiter };
            double value = func(point,r,R);
            integral += step*step*value;
        }
    }
    
    double factor = ( 4./3.*M_PI*R*R*R * 4.*M_PI );
    integral *= 2.*100./factor;
    std::cout << " detector radius = " << r << "\t integral = " << integral << "% " << std::endl;

    return integral;

}

int main(int argc, char * argv[])
{
    double r = 0.62035;
    double R = 6371000;
    double step = 1000;
    bool faster = false; 
    bool verbose = true;
    
    if((argc-1)%2==0 && argc>1) {
        for(int i=1; i<argc; i+=2) {
            if(std::string(argv[i])=="-r") {
                std::stringstream ss;
                ss << argv[i+1];
                ss >> r;
            }
            if(std::string(argv[i])=="-s") {
                std::stringstream ss;
                ss << argv[i+1];
                ss >> step;
            }
            if(std::string(argv[i])=="-v") {
                std::stringstream ss;
                ss << argv[i+1];
                ss >> verbose;
            }
        }
    }
    
    integrate(r,R,step,faster,verbose);
    
    //double value;
    //std::ofstream outfile;
    //outfile.open("Integration2D.csv");
    //double radius[9] = { 900000., 800000., 700000., 600000., 500000., 400000., 300000., 200000., 100000. }; 
    //for(int div=0; div<6.; div++) {
    //    for(int i=0; i<9; i++) {
    //        radius[i] /= 10.;
    //        value = integrate(radius[i],R,step,faster,verbose);
    //        outfile << radius[i] << "," << value << std::endl;
    //    }
    //}
    //outfile.close();

    return 0;
}


