
#include <iostream>
#include <cmath>
#include <sstream>
#include <string>
#include <fstream>

double func(double * xx, double r, double R)
{   
    return ( (M_PI*r*r)/(pow(xx[0]-R,2.)+pow(xx[1],2.)+pow(xx[2],2.)) );
}

double integrate(double r, double R, double step, bool faster=false, bool verbose=true)
{
    double xlo = -R;
    double xhi = +R;
    double ylo, yhi, zlo, zhi;
    
    if(verbose) {
        std::cout << " Earth radius = " << R << std::endl;
        std::cout << " Detector radius = " << r << std::endl;
        std::cout << " Step size = " << step << std::endl;
        std::cout << " Faster (?) = " << faster << std::endl;
    }

    double integral = 0;
    
    for(double xiter=xlo; xiter<=xhi; xiter+=step) {
        if(faster) { 
            if(xiter<0) continue;
        }
        ylo = -sqrt(R*R-xiter*xiter);
        yhi = +sqrt(R*R-xiter*xiter);
        for(double yiter=ylo; yiter<=yhi; yiter+=step) {
            zlo = -sqrt(R*R-xiter*xiter-yiter*yiter);
            zhi = +sqrt(R*R-xiter*xiter-yiter*yiter);
            for(double ziter=zlo; ziter<=zhi; ziter+=step) {
                double point[3] = { xiter, yiter, ziter };
                double value = func(point,r,R);
                //std::cout << " point = ( " << xiter << " , " << yiter << " , " << ziter << " ), radius = " 
                //          << sqrt(xiter*xiter+yiter*yiter+ziter*ziter) << "\t value = " << value << std::endl;
                integral += step*step*step*value;
            }
        }
    }
    
    double factor = ( 4./3.*M_PI*R*R*R * 4.*M_PI );
    std::cout << " detector radius = " << r << "\t integral = " << integral/factor*100. << "% " << std::endl;

    return (integral/factor*100.);

}

int main(int argc, char * argv[])
{
    double r = 0.62035;
    double R = 6371000;
    double step = 10000;
    bool faster = false; 
    bool verbose = false;
    
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
        }
    }
    
    //integrate(r,R,step,faster);
    
    double value;
    std::ofstream outfile;
    outfile.open("Integrate.csv");
    double radius[9] = { 900000., 800000., 700000., 600000., 500000., 400000., 300000., 200000., 100000. }; 
    for(int div=0; div<6.; div++) {
        for(int i=0; i<9; i++) {
            radius[i] /= 10.;
            value = integrate(radius[i],R,step,faster,verbose);
            outfile << radius[i] << "," << value << std::endl;
        }
    }
    outfile.close();

    return 0;
}


