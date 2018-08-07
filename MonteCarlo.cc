
#include <cstdlib>
#include <iostream>
#include <cmath>
#include <sstream>
#include <string>

struct Point {
    Point(double xx=0., double yy=0., double zz=0.) {
        fX = xx;
        fY = yy;
        fZ = zz;
    }

    double operator*(const Point& a) {
        return (fX*a.fX + fY*a.fY + fZ*a.fZ);
    }
    Point operator+(const Point& a) {
        return Point(fX+a.fX, fY+a.fY, fZ+a.fZ);
    }
    Point operator-(const Point& a) {
        return Point(fX-a.fX, fY-a.fY, fZ-a.fZ);
    }

    double fX; 
    double fY; 
    double fZ; 
};

struct Line {
    Line(Point origin, Point unitvector) {
        fOrigin = origin;
        fUnitVector = unitvector;
    }

    Point fOrigin;
    Point fUnitVector;
};

double Magnitude(Point a)
{
    return sqrt(a*a);
}

struct Sphere {
    Sphere(Point origin, double radius) {
        fOrigin = origin;
        fRadius = radius;
    }
    
    Point fOrigin;
    double fRadius;
};

Line GenerateLineInSphere(double radius)
{
    double xx, yy, zz;
    double calculated_radius = 2.*radius; 
    
    // calculate point within the sphere
    while(calculated_radius >= radius) {
        xx = ( 2.0*((double)rand()/(RAND_MAX)) - 1.0 ) * radius; 
        yy = ( 2.0*((double)rand()/(RAND_MAX)) - 1.0 ) * radius; 
        zz = ( 2.0*((double)rand()/(RAND_MAX)) - 1.0 ) * radius; 
        calculated_radius = sqrt( xx*xx + yy*yy + zz*zz );
    }
    Point origin(xx, yy, zz);

    // calculate a random direction
    xx = ( 2.0*((double)rand()/(RAND_MAX)) - 1.0 ); 
    yy = ( 2.0*((double)rand()/(RAND_MAX)) - 1.0 ); 
    zz = ( 2.0*((double)rand()/(RAND_MAX)) - 1.0 ); 
    calculated_radius = sqrt( xx*xx + yy*yy + zz*zz );
    Point unitvector(xx/calculated_radius, yy/calculated_radius, zz/calculated_radius);

    return Line(origin, unitvector);
}

void TestGenerateLineInSphere()
{
    double radius = 1.0;
    Line test = GenerateLineInSphere(radius);     
    std::cout << " ---> Testing the GenerateLineInSphere function w/ radius " << radius << std::endl;
    std::cout << " ---> Origin      = ( " << test.fOrigin.fX << " , " << test.fOrigin.fY << " , " << test.fOrigin.fZ << " ), radius = " << Magnitude(test.fOrigin) << std::endl;
    std::cout << " ---> Unit Vector = ( " << test.fUnitVector.fX << " , " << test.fUnitVector.fY << " , " << test.fUnitVector.fZ << " ), radius = " << Magnitude(test.fUnitVector) << std::endl;
}

bool LineSphereIntersection(Line ln, Sphere sph)
{
    // from wikipedia: Line-sphere intersection
    double value_under_sqrt = ( pow((ln.fUnitVector*(ln.fOrigin-sph.fOrigin)),2.) - pow(Magnitude(ln.fOrigin-sph.fOrigin),2.) + pow(sph.fRadius,2.) );
    
    if(value_under_sqrt < 0) return false;
    else return true;
}

int main(int argc, char *argv[])
{
    srand(time(NULL));

    double earth_radius = 6371e3; // in meters

    double volume = 1.0; // in meters^3
    double detector_radius = std::pow( (3.*volume)/(4.*M_PI) , 1./3. ); // in meters

    long int thou = 1000;
    long int mil  = 1000000;
    long int bil  = 1000000000;
    long int nevents = 1000000000;
    
    if((argc-1)%2!=0) {
        std::cout << " Incorrect arguments; Usage: \n"
                  << "  -n: number of events to simulate (default " << nevents << ")\n"
                  << "  -r: radius of the detector (default " << detector_radius << "m)\n";
        return 0;
    }

    for(int i=1; i<argc; i+=2) {
        if(std::string(argv[i]) == "-r") {
            std::stringstream ss;
            ss << argv[i+1];
            ss >> detector_radius;
        }
        if(std::string(argv[i]) == "-n") {
            std::stringstream ss;
            ss << argv[i+1];
            ss >> nevents;
        }
    }

    std::cout << " Earth radius = " << earth_radius/double(thou) << " km " << std::endl;
    std::cout << " Detector radius = " << detector_radius << " m " << std::endl;
    std::cout << " Events to simulate = " << double(nevents)/double(bil) << "b " << std::endl;
    
    Point detector_origin(earth_radius+detector_radius,0.,0.);
    Sphere detector(detector_origin,detector_radius);

    long int nhits = 0;
    for(long int i=0; i<nevents; i+=2) {
        Line test = GenerateLineInSphere(earth_radius);
        if(LineSphereIntersection(test,detector)) nhits++;
    
        if(i%mil==0) {
            std::cout << " ---> " << nhits << " / " << double(i)/double(bil) << "b ( " << 100.*double(nhits)/double(i) << " % ) seen             \r" << std::flush;
        }
    }
    std::cout << " ---> " << nhits << " / " << double(nevents)/double(bil) << "b ( " << 100.*double(nhits)/double(nevents) << " % ) seen              " << std::endl;

    return 0;
}

